#include "scene.h"

Scene::Scene(const char *path, Shader *shader,
	     glm::mat4 *view_matrix,
	     glm::mat4 *projection_matrix) {

	unsigned int flags = aiProcess_GenSmoothNormals |
			     aiProcess_Triangulate |
			     aiProcess_CalcTangentSpace |
			     aiProcess_FlipUVs;
	scene = importer.ReadFile(path, flags);

	if(!scene) {
		std::cerr << "Error importing " << path << ": "
			  << importer.GetErrorString() << std::endl;
		return;
	}

	this->shader = shader;
	this->model_view_matrix_name = model_view_matrix_name;
	this->model_view_projection_matrix_name =
		model_view_projection_matrix_name;
	this->normal_matrix_name = normal_matrix_name;

	this->view_matrix = view_matrix;
	this->projection_matrix = projection_matrix;

	traverse_nodes(scene->mRootNode, NULL);
}

Scene::~Scene() {
	meshes.clear();
}

void Scene::traverse_nodes(aiNode *start_node, aiMatrix4x4 *parent_trafo) {
	aiMatrix4x4 trafo = start_node->mTransformation;;
	if(parent_trafo)
		trafo = *parent_trafo * trafo;

	for(unsigned int i = 0; i < start_node->mNumMeshes; i++) {
		create_mesh(scene->mMeshes[i], &trafo);
	}

	for(unsigned int i = 0; i < start_node->mNumChildren; i++) {
		traverse_nodes(start_node->mChildren[i], &trafo);
	}
}

void Scene::create_mesh(aiMesh *mesh, aiMatrix4x4 *trafo) {
	unsigned int num_uv = mesh->GetNumUVChannels();
	std::vector<sgltk::Vertex> vertices;
	std::vector<unsigned short> indices;
	//glm::vec3 tex_coord[(num_uv - 1)][mesh->mNumVertices];

	//************************************
	// Vertices
	//************************************
	for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
		sgltk::Vertex vert_tmp;

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
		//for(unsigned int j = 0; j < num_uv; j++) {
			if(mesh->HasTextureCoords(0)) {
		//		continue;
		//	if(j == 0) {
				vert_tmp.tex_coord[0] =
					mesh->mTextureCoords[0][i].x;
				vert_tmp.tex_coord[1] =
					mesh->mTextureCoords[0][i].y;
				vert_tmp.tex_coord[2] =
					mesh->mTextureCoords[0][i].z;
			/*} else {
				tex_coord[j].push_back(glm::vec3());
				vert_tmp.tex_coord[0] =
					mesh->mTextureCoords[j][i].x;
				vert_tmp.tex_coord[1] =
					mesh->mTextureCoords[j][i].y;
				vert_tmp.tex_coord[2] =
					mesh->mTextureCoords[j][i].z;
			}*/
		}

		//tangents
		if(mesh->HasTangentsAndBitangents()) {
			vert_tmp.tangent[0] = mesh->mTangents[i].x;
			vert_tmp.tangent[1] = mesh->mTangents[i].y;
			vert_tmp.tangent[2] = mesh->mTangents[i].z;
			vert_tmp.tangent[3] = 1;
		}

		//color
		if(mesh->HasVertexColors(0)) {
			vert_tmp.color[0] = mesh->mColors[0][i].r;
			vert_tmp.color[1] = mesh->mColors[0][i].g;
			vert_tmp.color[2] = mesh->mColors[0][i].b;
			vert_tmp.color[3] = mesh->mColors[0][i].a;
		}

		vertices.push_back(vert_tmp);
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
	// Materials
	//************************************
	aiString str;
	Texture *texture;
	unsigned int num_textures;
	aiColor4D color(0.0f, 0.0f, 0.0f, 0.0f);
	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

	Mesh<> *mesh_tmp = new Mesh<>();
	mesh_tmp->attach_vertex_buffer(&vertices);
	mesh_tmp->attach_index_buffer(&indices);
	ai_to_glm_mat4(trafo, mesh_tmp->model_matrix);

	mesh_tmp->setup_shader(shader);
	mesh_tmp->setup_camera(view_matrix, projection_matrix);

	//wireframe or solid?
	mesh_tmp->wireframe = false;
	mat->Get(AI_MATKEY_ENABLE_WIREFRAME, mesh_tmp->wireframe);

	//can we use back face culling?
	mesh_tmp->twosided = true;
	mat->Get(AI_MATKEY_TWOSIDED, mesh_tmp->twosided);

	//ambient material color
	mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	mesh_tmp->material.color_ambient.x = color[0];
	mesh_tmp->material.color_ambient.y = color[1];
	mesh_tmp->material.color_ambient.z = color[2];
	mesh_tmp->material.color_ambient.w = color[3];

	//diffuse material color
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	mesh_tmp->material.color_diffuse.x = color[0];
	mesh_tmp->material.color_diffuse.y = color[1];
	mesh_tmp->material.color_diffuse.z = color[2];
	mesh_tmp->material.color_diffuse.w = color[3];

	//specular material color
	mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	mesh_tmp->material.color_specular.x = color[0];
	mesh_tmp->material.color_specular.y = color[1];
	mesh_tmp->material.color_specular.z = color[2];
	mesh_tmp->material.color_specular.w = color[3];
	mesh_tmp->material.shininess = 0.0;
	mat->Get(AI_MATKEY_SHININESS, mesh_tmp->material.shininess);
	mesh_tmp->material.shininess_strength = 1.0;
	mat->Get(AI_MATKEY_SHININESS_STRENGTH,
		mesh_tmp->material.shininess_strength);

	//ambient textures
	num_textures = mat->GetTextureCount(aiTextureType_AMBIENT);
	for(unsigned int i = 0; i < num_textures; i++) {
		mat->GetTexture(aiTextureType_AMBIENT, i, &str);
		texture = Texture::find_texture(str.C_Str());
		if(!texture) {
			texture = new Texture(str.C_Str());
			Texture::store_texture(str.C_Str(), texture);
		}
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
	}

	meshes.push_back(mesh_tmp);
}

void Scene::set_vertex_attribute(const char *attrib_name,
				 unsigned int buffer_index,
				 GLint size,
				 GLenum type,
				 GLsizei stride,
				 const GLvoid *pointer) {
	for(unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->set_vertex_attribute(attrib_name,
						buffer_index,
						size, type,
						stride, pointer);
	}
}

void Scene::draw() {
	for(unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->draw(GL_TRIANGLES);
	}
}

void Scene::draw(glm::mat4 *model_matrix) {
	for(unsigned int i = 0; i < meshes.size(); i++) {
		glm::mat4 matrix_tmp = meshes[i]->model_matrix;
		if(model_matrix)
			matrix_tmp = *model_matrix * matrix_tmp;
		meshes[i]->draw(GL_TRIANGLES, &matrix_tmp);
	}
}

void Scene::ai_to_glm_mat4(aiMatrix4x4 *in, glm::mat4 &out) {
	out[0][0] = in->a1;
	out[0][1] = in->b1;
	out[0][2] = in->c1;
	out[0][3] = in->d1;

	out[1][0] = in->a2;
	out[1][1] = in->b2;
	out[1][2] = in->c2;
	out[1][3] = in->d2;

	out[2][0] = in->a3;
	out[2][1] = in->b3;
	out[2][2] = in->c3;
	out[2][3] = in->d3;

	out[3][0] = in->a4;
	out[3][1] = in->b4;
	out[3][2] = in->c4;
	out[3][3] = in->d4;
}
