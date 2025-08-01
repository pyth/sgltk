#include <sgltk/model.h>

#ifdef HAVE_ASSIMP_H

using namespace sgltk;

std::vector<std::string> Model::paths = {"./"};

Model::Model() {
	scene = nullptr;
	shader = nullptr;

	view_matrix = nullptr;
	projection_matrix = nullptr;

	position_name = "pos_in";
	normal_name = "norm_in";
	tangent_name = "tang_in";
	color_name = "col_in";
	texture_coordinates_name = "tex_coord_in";
	bone_ids_name = "bone_ids_in";
	bone_weights_name = "bone_weights_in";
	bone_array_name = "bone_array";

	model_matrix_buf = -1;
	normal_matrix_buf = -1;

	bounding_box = {glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)};
}

Model::~Model() {
	bounding_box.clear();
	bone_offsets.clear();
	bones.clear();
	bone_map.clear();
	importer.FreeScene();
}

bool Model::load(const std::string& filename) {
	unsigned int flags = aiProcess_GenSmoothNormals |
			     aiProcess_Triangulate |
			     aiProcess_CalcTangentSpace |
			     aiProcess_FlipUVs;

	if((filename.length() > 1 && filename[0] == '/') ||
			(filename.length() > 2 && filename[1] == ':')) {
		scene = importer.ReadFile(filename.c_str(), flags);
	} else {
		for(unsigned int i = 0; i < paths.size(); i++) {
			scene = importer.ReadFile((paths[i] + filename).c_str(), flags);
			if(scene)
				break;
		}
	}

	if(!shader) {
		std::string error = std::string("No shader specified before"
			"loading a scene");
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}

	if (!scene)
		App::error_string.push_back(std::string("Error importing ") + filename);

	if(scene->mFlags==AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		App::error_string.push_back(std::string("Error importing ") +
				filename + std::string(": ") +
				importer.GetErrorString());
		return false;
	}

	glob_inv_transf = glm::inverse(ai_to_glm_mat4(scene->mRootNode->mTransformation));

	traverse_scene_nodes(scene->mRootNode, nullptr);
	compute_bounding_box();
	set_animation_speed(1.0);
	animate(0.0f);
	return true;
}

void Model::compute_bounding_box() {
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

bool Model::setup_camera(glm::mat4 *view_matrix,
			 glm::mat4 *projection_matrix) {
	bool ret;
	for(auto& mesh : meshes) {
		ret = mesh->setup_camera(view_matrix, projection_matrix);
		if(!ret)
			return false;
	}
	this->view_matrix = view_matrix;
	this->projection_matrix = projection_matrix;
	return true;
}

bool Model::setup_camera(Camera *camera) {
	bool ret;
	for(auto& mesh : meshes) {
		ret = mesh->setup_camera(camera);
		if(!ret)
			return false;
	}
	view_matrix = &camera->view_matrix;
	projection_matrix = &camera->projection_matrix;
	return true;
}

void Model::set_position_name(const std::string& name) {
	if(name.length() == 0)
		position_name = "pos_in";

	position_name = name;
}

void Model::set_normal_name(const std::string& name) {
	if(name.length() == 0)
		normal_name = "norm_in";

	normal_name = name;
}

void Model::set_tangent_name(const std::string& name) {
	if(name.length() == 0)
		tangent_name = "tang_in";

	tangent_name = name;
}

void Model::set_color_name(const std::string& name) {
	if(name.length() == 0)
		color_name = "col_in";

	color_name = name;
}

void Model::set_texture_coordinates_name(const std::string& name) {
	if(name.length() == 0)
		texture_coordinates_name = "tex_coord_in";

	texture_coordinates_name = name;
}

void Model::set_bone_ids_name(const std::string& name) {
	if(name.length() == 0)
		bone_ids_name = "bone_ids_in";

	bone_ids_name = name;
}

void Model::set_bone_weights_name(const std::string& name) {
	if(name.length() == 0)
		bone_weights_name = "bone_weights_in";

	bone_weights_name = name;
}

void sgltk::Model::set_bone_array_name(const std::string& name) {
	if(name.length() == 0)
		bone_array_name = "bone_array";

	bone_array_name = name;
}

void Model::setup_shader(Shader *shader) {
	this->shader = shader;
	for(std::unique_ptr<Mesh>& mesh : meshes) {
		mesh->setup_shader(shader);
		set_vertex_attribute(mesh);
	}
}

void Model::set_vertex_attribute(std::unique_ptr<Mesh>& mesh) {
	unsigned int buf = 0;
	mesh->set_vertex_attribute(position_name, buf++, 4, GL_FLOAT, 0, 0);
	mesh->set_vertex_attribute(normal_name, buf++, 3, GL_FLOAT, 0, 0);
	mesh->set_vertex_attribute(tangent_name, buf++, 4, GL_FLOAT, 0, 0);
	mesh->set_vertex_attribute(bone_ids_name, buf++, BONES_PER_VERTEX, GL_INT, 0, 0);
	mesh->set_vertex_attribute(bone_weights_name, buf++, BONES_PER_VERTEX, GL_FLOAT, 0, 0);

	if(mesh->num_uv) {
		for(unsigned int i = 0; i < mesh->num_uv; i++) {
			mesh->set_vertex_attribute(
				texture_coordinates_name + std::to_string(i),
				buf++, 3, GL_FLOAT, 0,
				(void *)(long)(i * mesh->num_vertices));
		}
	}
	if(mesh->num_col) {
		for(unsigned int i = 0; i < mesh->num_col; i++) {
			mesh->set_vertex_attribute(
				color_name + std::to_string(i), buf++, 4, GL_FLOAT, 0,
				(void *)(long)(i * mesh->num_vertices));
		}
	}
}

void Model::traverse_scene_nodes(aiNode *start_node, aiMatrix4x4 *parent_trafo) {
	aiMatrix4x4 trafo = start_node->mTransformation;;
	if(parent_trafo)
		trafo = *parent_trafo * trafo;

	for(unsigned int i = 0; i < start_node->mNumMeshes; i++) {
		auto mesh_tmp = create_mesh(start_node->mMeshes[i]);
		mesh_tmp->model_matrix = ai_to_glm_mat4(trafo);

		//if the mesh has no name, name it
		std::string mesh_name = scene->mMeshes[start_node->mMeshes[i]]->mName.C_Str();
		if(mesh_name.length() == 0) {
			mesh_name = "sgltk_mesh_" + std::to_string(i);
		}
		mesh_map[mesh_name.c_str()] = meshes.size();
		meshes.push_back(std::move(mesh_tmp));
	}

	for(unsigned int i = 0; i < start_node->mNumChildren; i++) {
		traverse_scene_nodes(start_node->mChildren[i], &trafo);
	}
}

std::unique_ptr<Mesh> Model::create_mesh(unsigned int index) {
	aiMesh *mesh = scene->mMeshes[index];
	unsigned int num_uv = mesh->GetNumUVChannels();
	unsigned int num_col = mesh->GetNumColorChannels();
	std::vector<glm::vec4> position(mesh->mNumVertices);
	std::vector<glm::vec3> normal(mesh->mNumVertices);
	std::vector<glm::vec4> tangent(mesh->mNumVertices);
	std::vector<int> bone_ids(mesh->mNumVertices * BONES_PER_VERTEX);
	std::vector<float> bone_weights(mesh->mNumVertices * BONES_PER_VERTEX, 0.0);
	std::vector<std::vector<glm::vec3> > tex_coord(num_uv,
			std::vector<glm::vec3>(mesh->mNumVertices));
	std::vector<std::vector<glm::vec4> > col(num_col,
			std::vector<glm::vec4>(mesh->mNumVertices));
	std::vector<unsigned int> indices;

	// Vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
		//position
		if(mesh->HasPositions()) {
			position[i][0] = mesh->mVertices[i].x;
			position[i][1] = mesh->mVertices[i].y;
			position[i][2] = mesh->mVertices[i].z;
			position[i][3] = 1;
		}

		//normals
		if(mesh->HasNormals()) {
			normal[i][0] = mesh->mNormals[i].x;
			normal[i][1] = mesh->mNormals[i].y;
			normal[i][2] = mesh->mNormals[i].z;
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
			tangent[i][0] = mesh->mTangents[i].x;
			tangent[i][1] = mesh->mTangents[i].y;
			tangent[i][2] = mesh->mTangents[i].z;
			tangent[i][3] = 1;
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

	}

	// Bones
	for(unsigned int i = 0; i < mesh->mNumBones; i++) {
		unsigned int index = 0;
		std::string bone_name(mesh->mBones[i]->mName.data);
		if(bone_name.length() == 0) {
			bone_name = "sgltk_bone_" + std::to_string(i);
		}

		if(bone_map.find(bone_name) == bone_map.end()) {
			index = bones.size();
			bone_map[bone_name] = index;
			bone_offsets.push_back(ai_to_glm_mat4(mesh->mBones[i]->mOffsetMatrix));
			bones.push_back(glm::mat4(1));
		} else {
			index = bone_map[bone_name];
		}

		for(unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			unsigned int vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			for(unsigned int k = 0; k < BONES_PER_VERTEX; k++) {
				if(bone_weights[vertex_id * BONES_PER_VERTEX + k] == 0.0) {
					bone_ids[vertex_id * BONES_PER_VERTEX + k] = index;
					bone_weights[vertex_id * BONES_PER_VERTEX + k] = weight;
					break;
				}
			}
		}
	}

	// Faces
	for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// Mesh
	std::unique_ptr<Mesh> mesh_tmp = std::make_unique<Mesh>();
	mesh_tmp->num_uv = num_uv;
	mesh_tmp->num_col = num_col;
	mesh_tmp->attach_vertex_buffer(position);
	mesh_tmp->attach_vertex_buffer(normal);
	mesh_tmp->attach_vertex_buffer(tangent);

	mesh_tmp->attach_vertex_buffer(bone_ids.data(),
					bone_ids.size());
	mesh_tmp->attach_vertex_buffer(bone_weights.data(),
					bone_weights.size());

	if(num_uv) {
		mesh_tmp->attach_vertex_buffer(tex_coord[0].data(), mesh->mNumVertices * num_uv);
	}
	if(num_col) {
		mesh_tmp->attach_vertex_buffer(col[0].data(), mesh->mNumVertices * num_col);
	}
	mesh_tmp->compute_bounding_box(position, 0);
	mesh_tmp->attach_index_buffer(indices);
	if(shader) {
		mesh_tmp->setup_shader(shader);
		set_vertex_attribute(mesh_tmp);
	}
	if(view_matrix && projection_matrix)
		mesh_tmp->setup_camera(view_matrix, projection_matrix);

	// Materials
	aiString str;
	std::shared_ptr<Texture> texture;
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
			texture = std::make_shared<Texture_2d>(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->auto_textures.push_back({"texture_ambient", *texture, i});
	}

	//diffuse textures
	num_textures = mat->GetTextureCount(aiTextureType_DIFFUSE);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = std::make_shared<Texture_2d>(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->auto_textures.push_back({"texture_diffuse", *texture, i});
	}

	//specular textures
	num_textures = mat->GetTextureCount(aiTextureType_SPECULAR);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_SPECULAR, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = std::make_shared<Texture_2d>(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->auto_textures.push_back({"texture_specular", *texture, i});
	}

	//shininess textures
	num_textures = mat->GetTextureCount(aiTextureType_SHININESS);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_SHININESS, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = std::make_shared<Texture_2d>(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->auto_textures.push_back({"texture_shininess", *texture, i});
	}

	//emissive textures
	num_textures = mat->GetTextureCount(aiTextureType_EMISSIVE);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_EMISSIVE, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = std::make_shared<Texture_2d>(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->auto_textures.push_back({"texture_emissive", *texture, i});
	}

	//normals textures
	num_textures = mat->GetTextureCount(aiTextureType_NORMALS);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_NORMALS, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = std::make_shared<Texture_2d>(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->auto_textures.push_back({"texture_normals", *texture, i});
	}

	//displacement textures
	num_textures = mat->GetTextureCount(aiTextureType_DISPLACEMENT);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_DISPLACEMENT, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = std::make_shared<Texture_2d>(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->auto_textures.push_back({"texture_displacement", *texture, i});
	}

	//opacity textures
	num_textures = mat->GetTextureCount(aiTextureType_OPACITY);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_OPACITY, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = std::make_shared<Texture_2d>(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->auto_textures.push_back({"texture_opacity", *texture, i});
	}

	//lightmap textures
	num_textures = mat->GetTextureCount(aiTextureType_LIGHTMAP);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_LIGHTMAP, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = std::make_shared<Texture_2d>(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
		mesh_tmp->auto_textures.push_back({"texture_lightmap", *texture, i});
	}

	return mesh_tmp;
}

void Model::traverse_animation_nodes(float time,
			      aiNode *node,
			      glm::mat4 parent_transformation) {

	std::string node_name(node->mName.data);
	glm::mat4 node_transformation = ai_to_glm_mat4(node->mTransformation);
	aiNodeAnim *node_animation = nullptr;

	for(unsigned int i = 0; i < scene->mAnimations[0]->mNumChannels; i++) {
		aiNodeAnim *node_anim = scene->mAnimations[0]->mChannels[i];
		if(std::string(node_anim->mNodeName.data) == node_name) {
			node_animation = node_anim;
			break;
		}
	}

	if(node_animation) {
		aiMatrix4x4 scaling;
		aiVector3D s_vec = interpolate_scaling(time, node_animation);
		aiMatrix4x4::Scaling(s_vec, scaling);

		aiMatrix4x4 translation;
		aiVector3D t_vec = interpolate_translation(time, node_animation);
		aiMatrix4x4::Translation(t_vec, translation);

		aiQuaternion rot = interpolate_rotation(time, node_animation);
		aiMatrix4x4 rotation = aiMatrix4x4(rot.GetMatrix());

		node_transformation = ai_to_glm_mat4(translation * rotation * scaling);
	}

	glm::mat4 glob_transf = parent_transformation * node_transformation;
	if(bone_map.find(node_name) != bone_map.end()) {
		unsigned int index = bone_map[node_name];
		bones[index] = glob_inv_transf * glob_transf * bone_offsets[index];
	}
	for(unsigned int i = 0; i < node->mNumChildren; i++)
		traverse_animation_nodes(time, node->mChildren[i], glob_transf);
}

void Model::set_animation_speed(double speed) {
	if(!scene)
		return;

	if(!scene->HasAnimations())
		return;

	if(scene->mAnimations[0]->mTicksPerSecond == 0)
		ticks_per_second = 25.0 * speed;
	else
		ticks_per_second = scene->mAnimations[0]->mTicksPerSecond *
								speed;
}

aiVector3D Model::interpolate_scaling(float time, aiNodeAnim *node) {

	if(node->mNumScalingKeys == 1)
		return node->mScalingKeys[0].mValue;

	unsigned int index = 0;
	for(unsigned int i = 0; i < node->mNumScalingKeys - 1; i++)
		if(time < (float)node->mScalingKeys[i + 1].mTime) {
			index = i;
			break;
		}

	float dt = (float)(node->mScalingKeys[index + 1].mTime -
		node->mScalingKeys[index].mTime);
	float factor = (float)(time - node->mScalingKeys[index].mTime) / dt;
	aiVector3D start = node->mScalingKeys[index].mValue;
	aiVector3D end = node->mScalingKeys[index + 1].mValue;
	return start + factor * (end - start);
}

aiVector3D Model::interpolate_translation(float time, aiNodeAnim *node) {
	if(node->mNumPositionKeys == 1)
		return node->mPositionKeys[0].mValue;

	unsigned int index = 0;
	for(unsigned int i = 0; i < node->mNumPositionKeys - 1; i++)
		if(time < node->mPositionKeys[i + 1].mTime) {
			index = i;
			break;
		}

	float dt = (float)(node->mPositionKeys[index + 1].mTime -
		node->mPositionKeys[index].mTime);
	float factor = (float)(time - node->mPositionKeys[index].mTime) / dt;
	aiVector3D start = node->mPositionKeys[index].mValue;
	aiVector3D end = node->mPositionKeys[index + 1].mValue;
	return start + factor * (end - start);
}

aiQuaternion Model::interpolate_rotation(float time, aiNodeAnim *node) {
	if(node->mNumRotationKeys == 1)
		return node->mRotationKeys[0].mValue;

	unsigned int index = 0;
	for(unsigned int i = 0; i < node->mNumRotationKeys - 1; i++)
		if(time < node->mRotationKeys[i + 1].mTime) {
			index = i;
			break;
		}

	aiQuaternion rot;
	float dt = (float)(node->mRotationKeys[index + 1].mTime -
		node->mRotationKeys[index].mTime);
	float factor = (float)(time - node->mRotationKeys[index].mTime) / dt;
	aiQuaternion start = node->mRotationKeys[index].mValue;
	aiQuaternion end = node->mRotationKeys[index + 1].mValue;
	aiQuaternion::Interpolate(rot, start, end, factor);
	return rot.Normalize();
}

void Model::attach_texture(const std::string& name,
			   const Texture& texture,
			   unsigned int index) {

	for(const auto& mesh : meshes) {
		mesh->attach_texture(name, texture, index);
	}
}

void Model::set_texture_parameter(GLenum name, int parameter) {
	for(const auto& mesh : meshes) {
		for(const auto& tex : mesh->auto_textures) {
			const_cast<Texture&>(std::get<1>(tex)).set_parameter(name, parameter);
		}
	}
}

void Model::set_texture_parameter(GLenum name, float parameter) {
	for(const auto& mesh : meshes) {
		for(const auto& tex : mesh->auto_textures) {
			const_cast<Texture&>(std::get<1>(tex)).set_parameter(name, parameter);
		}
	}
}

bool Model::animate(float time) {
	if(!scene)
		return false;

	if(!scene->HasAnimations())
		return false;

	glm::mat4 mat = glm::mat4(1);

	double animation_time = fmod(time * ticks_per_second,
					scene->mAnimations[0]->mDuration);
	traverse_animation_nodes((float)animation_time, scene->mRootNode, mat);
	int loc = shader->get_uniform_location(bone_array_name);
	if(loc >= 0) {
		shader->set_uniform(loc, false, bones);
	} else {
		return false;
	}
	return true;
}

void Model::setup_instanced_matrix(const std::vector<glm::mat4>& model_matrix,
								GLenum usage) {
	if(!shader) {
		std::string error = std::string("No shader specified before a"
			"call to the setup_instanced_matrix function");
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}
	for(const auto& mesh : meshes) {
		std::vector<glm::mat3> normal_matrix(model_matrix.size());
		for(unsigned int i = 0; i < normal_matrix.size(); i++) {
			normal_matrix[i] = glm::mat3(glm::transpose(glm::inverse(model_matrix[i])));
		}
		model_matrix_buf = mesh->attach_vertex_buffer(model_matrix, usage);
		normal_matrix_buf = mesh->attach_vertex_buffer(normal_matrix, usage);
		int model_loc = mesh->shader->get_attribute_location(mesh->model_matrix_name);
		int normal_loc = mesh->shader->get_attribute_location(mesh->normal_matrix_name);
		if(model_loc >= 0) {
			for(int i = 0; i < 4; i++) {
				mesh->set_vertex_attribute(model_loc + i,
								model_matrix_buf,
								4, GL_FLOAT,
								sizeof(glm::mat4),
								(GLvoid *)(i * sizeof(glm::vec4)), 1);
			}
		}
		if(normal_loc >= 0) {
			for(int i = 0; i < 3; i++) {
				mesh->set_vertex_attribute(normal_loc + i,
								normal_matrix_buf,
								3, GL_FLOAT,
								sizeof(glm::mat3),
								(GLvoid *)(i * sizeof(glm::vec3)), 1);
			}
		}
	}
}

void Model::set_instanced_matrix_attributes() {
	if(!shader) {
		std::string error = std::string("No shader specified before a"
			"call to the setup_instanced_matrix function");
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}
	for(const auto& mesh : meshes) {
		int model_loc = mesh->shader->get_attribute_location(mesh->model_matrix_name);
		int normal_loc = mesh->shader->get_attribute_location(mesh->normal_matrix_name);
		if(model_loc >= 0) {
			for(int i = 0; i < 4; i++) {
				mesh->set_vertex_attribute(model_loc + i,
								model_matrix_buf,
								4, GL_FLOAT,
								sizeof(glm::mat4),
								(GLvoid *)(i * sizeof(glm::vec4)), 1);
			}
		}
		if(normal_loc >= 0) {
			for(int i = 0; i < 3; i++) {
				mesh->set_vertex_attribute(normal_loc + i,
								normal_matrix_buf,
								3, GL_FLOAT,
								sizeof(glm::mat3),
								(GLvoid *)(i * sizeof(glm::vec3)), 1);
			}
		}
	}
}

void Model::draw(const glm::mat4 *model_matrix) {
	for(const auto& mesh : meshes) {
		if(model_matrix) {
			glm::mat4 matrix_tmp = *model_matrix * mesh->model_matrix;
			mesh->draw(GL_TRIANGLES, &matrix_tmp);
		}
		else
			mesh->draw(GL_TRIANGLES);
	}
}

void Model::draw_instanced(unsigned int num_instances) {
	if(num_instances == 0)
		return;

	for(const auto& mesh : meshes) {
		mesh->draw_instanced(GL_TRIANGLES, 0, num_instances);
	}
}

void Model::add_path(std::string path) {
	if(path[path.length() - 1] != '/')
		path += '/';

	if(std::find(Model::paths.begin(), Model::paths.end(), path) == Model::paths.end())
		Model::paths.push_back(path);
}

glm::mat4 Model::ai_to_glm_mat4(const aiMatrix4x4& in) {
	glm::mat4 ret;

	ret[0][0] = in.a1;
	ret[0][1] = in.b1;
	ret[0][2] = in.c1;
	ret[0][3] = in.d1;

	ret[1][0] = in.a2;
	ret[1][1] = in.b2;
	ret[1][2] = in.c2;
	ret[1][3] = in.d2;

	ret[2][0] = in.a3;
	ret[2][1] = in.b3;
	ret[2][2] = in.c3;
	ret[2][3] = in.d3;

	ret[3][0] = in.a4;
	ret[3][1] = in.b4;
	ret[3][2] = in.c4;
	ret[3][3] = in.d4;

	return ret;
}

#endif //HAVE_ASSIMP_H
