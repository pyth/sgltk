#include "scene.h"

using namespace sgltk;

std::vector<std::string> Scene::paths = {"./"};

Scene::Scene() {
	scene = NULL;
	shader = NULL;
	view_matrix = NULL;
	projection_matrix = NULL;

	position_name = "pos_in";
	normal_name = "norm_in";
	tangent_name = "tang_in";
	color_name = "col_in";
	texture_coordinates_name = "tex_coord_in";
	bone_ids_name = "bone_ids_in";
	bone_weights_name = "bone_weights_in";

	num_bones = 0;
	bounding_box.push_back(glm::vec3(0, 0, 0));
	bounding_box.push_back(glm::vec3(0, 0, 0));
}

Scene::~Scene() {
	meshes.clear();
	bones.clear();
	bone_map.clear();
}

bool Scene::load(std::string filename) {
	unsigned int flags = aiProcess_GenSmoothNormals |
			     aiProcess_Triangulate |
			     aiProcess_CalcTangentSpace |
			     aiProcess_FlipUVs;

	if((filename.length() > 1 && filename[0] == '/') ||
			(filename.length() > 2 && filename[1] == ':')) {
		scene = importer.ReadFile(filename.c_str(), flags);
	} else {
		for(unsigned int i = 0; i < Scene::paths.size(); i++) {
			scene = importer.ReadFile(filename.c_str(), flags);
			if(scene)
				break;
		}
	}

	if(!scene) {
		std::cerr << "Error importing " << filename << ": "
			  << importer.GetErrorString() << std::endl;
		return false;
	}

	traverse_scene_nodes(scene->mRootNode, NULL);
	compute_bounding_box();
	set_animation_speed(1.0);
	aiMatrix4x4 inv_transformation = scene->mRootNode->mTransformation.Inverse();
	glob_inv_transf = ai_to_glm_mat4(&inv_transformation);

}

void Scene::compute_bounding_box() {
	for(unsigned int i = 0; i < meshes.size(); i++) {
		glm::vec3 min = meshes[i]->bounding_box[0];
		glm::vec3 max = meshes[i]->bounding_box[1];
		if(min.x < bounding_box[0].x)
			bounding_box[0].x = min.x;
		if(min.y < bounding_box[0].y)
			bounding_box[0].y = min.y;
		if(min.z < bounding_box[0].z)
			bounding_box[0].z = min.z;
		if(max.x > bounding_box[1].x)
			bounding_box[1].x = max.x;
		if(max.y > bounding_box[1].y)
			bounding_box[1].y = max.y;
		if(max.z > bounding_box[1].z)
			bounding_box[1].z = max.z;
	}
}

void Scene::setup_camera(glm::mat4 *view_matrix,
			 glm::mat4 *projection_matrix) {
	this->view_matrix = view_matrix;
	this->projection_matrix = projection_matrix;
}

void Scene::setup_shader(Shader *shader) {
	this->shader = shader;
}

glm::mat4 Scene::interpolate_scaling(float time, aiNodeAnim *node) {

	if(node->mNumScalingKeys == 1) {
		aiVector3D s_vec = node->mScalingKeys[0].mValue;
		aiMatrix4x4 ret;
		aiMatrix4x4::Scaling(s_vec, ret);
		return ai_to_glm_mat4(&ret);
	}

	unsigned int index = 0;
	for(unsigned int i = 0; i < node->mNumScalingKeys - 1; i++) {
		if(time < node->mScalingKeys[i + 1].mTime)
			index = i;
	}
	float dt = node->mScalingKeys[index + 1].mTime -
		node->mScalingKeys[index].mTime;
	float factor = (time - node->mScalingKeys[index].mTime) / dt;
	aiVector3D start = node->mScalingKeys[index].mValue;
	aiVector3D end = node->mScalingKeys[index + 1].mValue;
	aiVector3D s_vec = start + factor * (end - start);

	aiMatrix4x4 ret;
	aiMatrix4x4::Scaling(s_vec, ret);
	return ai_to_glm_mat4(&ret);
}

glm::mat4 Scene::interpolate_translation(float time, aiNodeAnim *node) {
	if(node->mNumPositionKeys == 1) {
		aiVector3D t_vec = node->mPositionKeys[0].mValue;
		aiMatrix4x4 ret;
		aiMatrix4x4::Translation(t_vec, ret);
		return ai_to_glm_mat4(&ret);
	}

	unsigned int index = 0;
	for(unsigned int i = 0; i < node->mNumPositionKeys - 1; i++) {
		if(time < node->mPositionKeys[i + 1].mTime)
			index = i;
	}
	float dt = node->mPositionKeys[index + 1].mTime -
		node->mPositionKeys[index].mTime;
	float factor = (time - node->mPositionKeys[index].mTime) / dt;
	aiVector3D start = node->mPositionKeys[index].mValue;
	aiVector3D end = node->mPositionKeys[index + 1].mValue;
	aiVector3D t_vec = start + factor * (end - start);

	aiMatrix4x4 ret;
	aiMatrix4x4::Translation(t_vec, ret);
	return ai_to_glm_mat4(&ret);
}

glm::mat4 Scene::interpolate_rotation(float time, aiNodeAnim *node) {
	if(node->mNumRotationKeys == 1) {
		aiQuaternion rot = node->mRotationKeys[0].mValue;
		aiMatrix4x4 ret = aiMatrix4x4(rot.GetMatrix());
		return ai_to_glm_mat4(&ret);
	}

	unsigned int index = 0;
	aiQuaternion rot;
	for(unsigned int i = 0; i < node->mNumRotationKeys - 1; i++) {
		if(time < node->mRotationKeys[i + 1].mTime)
			index = i;
	}
	float dt = node->mRotationKeys[index + 1].mTime -
		node->mRotationKeys[index].mTime;
	float factor = (time - node->mRotationKeys[index].mTime) / dt;
	aiQuaternion start = node->mRotationKeys[index].mValue;
	aiQuaternion end = node->mRotationKeys[index + 1].mValue;
	aiQuaternion::Interpolate(rot, start, end, factor);
	aiMatrix4x4 ret = aiMatrix4x4(rot.GetMatrix());

	return ai_to_glm_mat4(&ret);
}

void Scene::traverse_scene_nodes(aiNode *start_node, aiMatrix4x4 *parent_trafo) {
	aiMatrix4x4 trafo = start_node->mTransformation;;
	if(parent_trafo)
		trafo = *parent_trafo * trafo;

	for(unsigned int i = 0; i < start_node->mNumMeshes; i++) {
		create_mesh(scene->mMeshes[i], &trafo);
	}

	for(unsigned int i = 0; i < start_node->mNumChildren; i++) {
		traverse_scene_nodes(start_node->mChildren[i], &trafo);
	}
}

void Scene::create_mesh(aiMesh *mesh, aiMatrix4x4 *trafo) {
	unsigned int num_uv = mesh->GetNumUVChannels();
	unsigned int num_col = mesh->GetNumColorChannels();
	std::vector<Scene_vertex> vertices;
	std::vector<unsigned short> indices;
	glm::vec4 col[num_col][mesh->mNumVertices];
	glm::vec3 tex_coord[num_uv][mesh->mNumVertices];

	//************************************
	// Vertices
	//************************************
	for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Scene_vertex vert_tmp;

		//position
		if(mesh->HasPositions()) {
			vert_tmp.position[0] = mesh->mVertices[i].x;
			vert_tmp.position[1] = mesh->mVertices[i].y;
			vert_tmp.position[2] = mesh->mVertices[i].z;
			vert_tmp.position[3] = 1;
		}

		//normals
		if(mesh->HasNormals()) {
			vert_tmp.normal[0] = mesh->mNormals[i].x;
			vert_tmp.normal[1] = mesh->mNormals[i].y;
			vert_tmp.normal[2] = mesh->mNormals[i].z;
		}

		//texture coordinates
		for(unsigned int j = 0; j < num_uv; j++) {
			if(!mesh->HasTextureCoords(j))
				continue;
			tex_coord[j][i][0] = mesh->mTextureCoords[j][i].x;
			tex_coord[j][i][1] = mesh->mTextureCoords[j][i].y;
			tex_coord[j][i][2] = mesh->mTextureCoords[j][i].z;
		}

		//tangents
		if(mesh->HasTangentsAndBitangents()) {
			vert_tmp.tangent[0] = mesh->mTangents[i].x;
			vert_tmp.tangent[1] = mesh->mTangents[i].y;
			vert_tmp.tangent[2] = mesh->mTangents[i].z;
			vert_tmp.tangent[3] = 1;
		}

		//color
		for(unsigned int j = 0; j < num_col; j++) {
			if(!mesh->HasVertexColors(j))
				continue;
			col[j][i][0] = mesh->mColors[j][i].r;
			col[j][i][1] = mesh->mColors[j][i].g;
			col[j][i][2] = mesh->mColors[j][i].b;
			col[j][i][3] = mesh->mColors[j][i].a;
		}

		vert_tmp.bones = 0;
		vert_tmp.bone_weights[0] = 0.f;
		vert_tmp.bone_weights[1] = 0.f;
		vert_tmp.bone_weights[2] = 0.f;
		vert_tmp.bone_weights[3] = 0.f;

		vertices.push_back(vert_tmp);
	}

	//************************************
	// Bones
	//************************************
	for(int i=0; i<mesh->mNumBones; i++) {
		unsigned int index;
		std::string bone_name(mesh->mBones[i]->mName.C_Str());
		if(bone_map.find(bone_name) == bone_map.end()) {
			index = num_bones;
			num_bones++;
			Bone bone;
			bone.offset = ai_to_glm_mat4(&mesh->mBones[i]->mOffsetMatrix);
			bones.push_back(bone);
			bone_map[bone_name] = index;
		} else {
			index = bone_map[bone_name];
		}
		for(int j=0; j < mesh->mBones[i]->mNumWeights; j++) {
			unsigned int vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			unsigned int bone_index = vertices[vertex_id].bones;
			vertices[vertex_id].bone_ids[bone_index] = index;
			vertices[vertex_id].bone_weights[bone_index] = weight;
			vertices[vertex_id].bones++;
		}
	}

	//************************************
	// Faces
	//************************************
	for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//************************************
	// Mesh
	//************************************
	Mesh<Scene_vertex> *mesh_tmp = new Mesh<Scene_vertex>();
	mesh_tmp->attach_vertex_buffer(&vertices, true);
	mesh_tmp->attach_vertex_buffer<glm::vec3>((void *)tex_coord,
				       sizeof(glm::vec3) *
				       mesh->mNumVertices * num_uv);
	mesh_tmp->attach_vertex_buffer<glm::vec4>((void *)col, sizeof(glm::vec4) *
				       mesh->mNumVertices * num_col);
	mesh_tmp->compute_bounding_box(offsetof(Scene_vertex, position));
	mesh_tmp->attach_index_buffer(&indices);
	mesh_tmp->model_matrix = ai_to_glm_mat4(trafo);

	mesh_tmp->setup_shader(shader);
	mesh_tmp->setup_camera(view_matrix, projection_matrix);

	//************************************
	// Materials
	//************************************
	aiString str;
	Texture *texture;
	unsigned int num_textures;
	aiColor4D color(0.0f, 0.0f, 0.0f, 0.0f);
	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

	//wireframe or solid?
	mesh_tmp->wireframe = false;
	mat->Get(AI_MATKEY_ENABLE_WIREFRAME, mesh_tmp->wireframe);

	//can we use back face culling?
	mesh_tmp->twosided = true;
	mat->Get(AI_MATKEY_TWOSIDED, mesh_tmp->twosided);

	//ambient color
	mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	mesh_tmp->color_ambient.x = color[0];
	mesh_tmp->color_ambient.y = color[1];
	mesh_tmp->color_ambient.z = color[2];
	mesh_tmp->color_ambient.w = color[3];

	//diffuse color
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	mesh_tmp->color_diffuse.x = color[0];
	mesh_tmp->color_diffuse.y = color[1];
	mesh_tmp->color_diffuse.z = color[2];
	mesh_tmp->color_diffuse.w = color[3];

	//specular color
	mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	mesh_tmp->color_specular.x = color[0];
	mesh_tmp->color_specular.y = color[1];
	mesh_tmp->color_specular.z = color[2];
	mesh_tmp->color_specular.w = color[3];

	mesh_tmp->shininess = 0.0;
	mat->Get(AI_MATKEY_SHININESS, mesh_tmp->shininess);
	mesh_tmp->shininess_strength = 1.0;
	mat->Get(AI_MATKEY_SHININESS_STRENGTH,
		mesh_tmp->shininess_strength);

	//ambient textures
	num_textures = mat->GetTextureCount(aiTextureType_AMBIENT);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_AMBIENT, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = new Texture(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->textures_ambient.push_back(texture);
	}

	//diffuse textures
	num_textures = mat->GetTextureCount(aiTextureType_DIFFUSE);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = new Texture(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->textures_diffuse.push_back(texture);
	}

	//specular textures
	num_textures = mat->GetTextureCount(aiTextureType_SPECULAR);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_SPECULAR, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = new Texture(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->textures_specular.push_back(texture);
	}

	//shininess textures
	num_textures = mat->GetTextureCount(aiTextureType_SHININESS);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_SHININESS, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = new Texture(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->textures_shininess.push_back(texture);
	}

	//emmisive textures
	num_textures = mat->GetTextureCount(aiTextureType_EMISSIVE);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_EMISSIVE, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = new Texture(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->textures_emmisive.push_back(texture);
	}

	//normals textures
	num_textures = mat->GetTextureCount(aiTextureType_NORMALS);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_NORMALS, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = new Texture(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->textures_normals.push_back(texture);
	}

	//displacement textures
	num_textures = mat->GetTextureCount(aiTextureType_DISPLACEMENT);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_DISPLACEMENT, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = new Texture(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->textures_displacement.push_back(texture);
	}

	//opacity textures
	num_textures = mat->GetTextureCount(aiTextureType_OPACITY);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_OPACITY, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = new Texture(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->textures_opacity.push_back(texture);
	}

	//lightmap textures
	num_textures = mat->GetTextureCount(aiTextureType_LIGHTMAP);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_LIGHTMAP, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = new Texture(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->textures_lightmap.push_back(texture);
	}

	//************************************
	// Set attribute pointers
	//************************************
	mesh_tmp->set_vertex_attribute(position_name, 0, 4, GL_FLOAT,
				sizeof(Scene_vertex),
				(void *)offsetof(Scene_vertex, position));
	mesh_tmp->set_vertex_attribute(normal_name, 0, 3, GL_FLOAT,
				sizeof(Scene_vertex),
				(void *)offsetof(Scene_vertex, normal));
	mesh_tmp->set_vertex_attribute(tangent_name, 0, 4, GL_FLOAT,
				sizeof(Scene_vertex),
				(void *)offsetof(Scene_vertex, tangent));
	mesh_tmp->set_vertex_attribute(bone_ids_name, 0, BONES_PER_VERTEX,
				GL_INT, sizeof(Scene_vertex),
				(void *)offsetof(Scene_vertex, bone_ids));
	mesh_tmp->set_vertex_attribute(bone_weights_name, 0, BONES_PER_VERTEX,
				GL_FLOAT, sizeof(Scene_vertex),
				(void *)offsetof(Scene_vertex, bone_weights));

	for(unsigned int i = 0; i < num_uv; i++) {
		mesh_tmp->set_vertex_attribute(
			texture_coordinates_name + std::to_string(i),
			1, 3, GL_FLOAT, 0,
			(void *)(long)(i * mesh->mNumVertices));
	}
	for(unsigned int i = 0; i < num_col; i++) {
		mesh_tmp->set_vertex_attribute(
			color_name + std::to_string(i), 2, 4, GL_FLOAT, 0,
			(void *)(long)(i * mesh->mNumVertices));
	}

	meshes.push_back(mesh_tmp);
}

void Scene::traverse_animation_nodes(float time,
			      aiNode *node,
			      glm::mat4 parent_transformation) {
	std::string node_name(node->mName.C_Str());
	glm::mat4 node_transformation = ai_to_glm_mat4(&node->mTransformation);
	aiNodeAnim *node_animation = NULL;
	for(unsigned int i = 0; i < scene->mAnimations[0]->mNumChannels; i++) {
		aiNodeAnim *node_anim = scene->mAnimations[0]->mChannels[i];
		if(std::string(node_anim->mNodeName.C_Str()) == node_name)
			node_animation = node_anim;
	}

	if(node_animation) {
		glm::mat4 scaling = interpolate_scaling(time, node_animation);
		glm::mat4 rotation = interpolate_rotation(time,
								node_animation);
		glm::mat4 translation = interpolate_translation(time,
								node_animation);

		node_transformation = translation * rotation * scaling;
	}

	glm::mat4 glob_transf = parent_transformation * node_transformation;
	if(bone_map.find(node_name) != bone_map.end()) {
		unsigned int index = bone_map[node_name];
		bones[index].transformation = glob_inv_transf * glob_transf *
						bones[index].offset;
	}
	for(unsigned int i = 0; i < node->mNumChildren; i++)
		traverse_animation_nodes(time, node->mChildren[i], glob_transf);
}

void Scene::set_animation_speed(double speed) {
	if(scene->mAnimations[0]->mTicksPerSecond == 0)
		ticks_per_second = 0.0001;
	else
		ticks_per_second = scene->mAnimations[0]->mTicksPerSecond *
								speed;
}

bool Scene::animate(float time) {
	if(!scene->HasAnimations())
		return false;
	
	glm::mat4 mat;

	double animation_time = fmod(time * ticks_per_second,
					scene->mAnimations[0]->mDuration);
	traverse_animation_nodes(animation_time, scene->mRootNode, mat);
	trafos.resize(bones.size());
	for(unsigned int i = 0; i < bones.size(); i++)
		trafos[i] = bones[i].transformation;
	return true;
}

void Scene::draw() {
	draw(NULL);
}

void Scene::draw(glm::mat4 *model_matrix) {
	for(unsigned int i = 0; i < meshes.size(); i++) {
		glm::mat4 matrix_tmp = meshes[i]->model_matrix;
		if(model_matrix)
			matrix_tmp = *model_matrix * matrix_tmp;
		meshes[i]->draw(GL_TRIANGLES, &matrix_tmp);
	}
}

void Scene::add_path(std::string path) {
	if(path[path.length() - 1] != '/')
		path += '/';

	if(std::find(Scene::paths.begin(), Scene::paths.end(), path) == Scene::paths.end())
		Scene::paths.push_back(path);
}

glm::mat4 Scene::ai_to_glm_mat4(aiMatrix4x4 *in) {
	glm::mat4 ret;

	ret[0][0] = in->a1;
	ret[0][1] = in->b1;
	ret[0][2] = in->c1;
	ret[0][3] = in->d1;

	ret[1][0] = in->a2;
	ret[1][1] = in->b2;
	ret[1][2] = in->c2;
	ret[1][3] = in->d2;

	ret[2][0] = in->a3;
	ret[2][1] = in->b3;
	ret[2][2] = in->c3;
	ret[2][3] = in->d3;

	ret[3][0] = in->a4;
	ret[3][1] = in->b4;
	ret[3][2] = in->c4;
	ret[3][3] = in->d4;

	return ret;
}
