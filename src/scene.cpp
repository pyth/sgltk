#include "scene.h"

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
}

Scene::~Scene() {
	meshes.clear();
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

	traverse_nodes(scene->mRootNode, NULL);
}

void Scene::setup_camera(glm::mat4 *view_matrix,
			 glm::mat4 *projection_matrix) {
	this->view_matrix = view_matrix;
	this->projection_matrix = projection_matrix;
}

void Scene::setup_shader(Shader *shader) {
	this->shader = shader;
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
	// Mesh
	//************************************
	Mesh<Scene_vertex> *mesh_tmp = new Mesh<Scene_vertex>();
	mesh_tmp->attach_vertex_buffer(&vertices);
	mesh_tmp->attach_vertex_buffer(sizeof(glm::vec3) *
				       mesh->mNumVertices * num_uv,
				       (void *)tex_coord);
	mesh_tmp->attach_vertex_buffer(sizeof(glm::vec4) *
				       mesh->mNumVertices * num_col,
				       (void *)col);
	mesh_tmp->attach_index_buffer(&indices);
	ai_to_glm_mat4(trafo, mesh_tmp->model_matrix);

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

	//set attribute pointers
	mesh_tmp->set_vertex_attribute(position_name, 0, 4, GL_FLOAT,
				       sizeof(Scene_vertex),
				       (void *)offsetof(Scene_vertex, position));
	mesh_tmp->set_vertex_attribute(normal_name, 0, 3, GL_FLOAT,
				       sizeof(Scene_vertex),
				       (void *)offsetof(Scene_vertex, normal));
	mesh_tmp->set_vertex_attribute(tangent_name, 0, 4, GL_FLOAT,
				       sizeof(Scene_vertex),
				       (void *)offsetof(Scene_vertex, tangent));

	for(unsigned int i = 0; i < num_uv; i++) {
		mesh_tmp->set_vertex_attribute(texture_coordinates_name+std::to_string(i), 1, 3, GL_FLOAT, 0,
					       (void *)(long)(i * mesh->mNumVertices));
	}
	for(unsigned int i = 0; i < num_col; i++) {
		mesh_tmp->set_vertex_attribute(color_name+std::to_string(i), 2, 4, GL_FLOAT, 0,
					       (void *)(long)(i * mesh->mNumVertices));
	}

	meshes.push_back(mesh_tmp);
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

void Scene::add_path(std::string path) {
	if(path[path.length() - 1] != '/')
		path += '/';

	if(std::find(Scene::paths.begin(), Scene::paths.end(), path) == Scene::paths.end())
		Scene::paths.push_back(path);
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
