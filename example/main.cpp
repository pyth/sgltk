#include <sgltk/core.h>
#include "gui.h"

using namespace std;

Shader *shader;
Texture *tex;
Mesh *mesh;
Camera *camera;
glm::mat4 V, P;

GUI *app;

static void init(void);

int main(int argc, char** argv) {
	app = new GUI("Test", 1024, 768, 100, 100, 4, 0, 0);
	//GL calls should be used after the App class initializes GLEW
	init();
	V = glm::lookAt(glm::vec3(camera->pos), glm::vec3(camera->dir),
			glm::vec3(camera->up));
	P = glm::perspective(70.0f, (GLfloat)app->width/(GLfloat)app->height,
			     1.0f, 800.0f);
	app->run(60);
	return 0;
}

void init() {
	//enable textures and blending
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//load an image
	Image img1;
	img1.load("data/earth.jpg");
	//create an image from text
	Image img2;
	img2.create_text("TTF fonts are cool!", "data/Oswald-Medium.ttf",
			 64, 255, 0, 0, 255);
	//copy one image into the other
	img1.copy_from(&img2, img1.width/2-img2.width/2,
		       img1.height-img2.height);
	//make a texture out of the image
	tex = new Texture();
	tex->load_texture(&img1);

	//set up the camera
	camera = new Camera(glm::vec3(0,0,5), glm::vec3(0,0,-1), glm::vec3(0,1,0));

	//load the shaders
	shader = new Shader();
	shader->attach("data/example_vs.glsl", GL_VERTEX_SHADER);
	shader->attach("data/example_fs.glsl", GL_FRAGMENT_SHADER);
	shader->link();

	//arrange a few vertices into a square
	vector<Vertex> vert;
	vert.push_back(Vertex(glm::vec3(-0.5,-0.5,-1.0), glm::vec3(0.0,0.0,1.0),
		       glm::vec2(0.0,1.0)));
	vert.push_back(Vertex(glm::vec3(-0.5,0.5,-1.0), glm::vec3(0.0,0.0,1.0),
		       glm::vec2(0.0,0.0)));
	vert.push_back(Vertex(glm::vec3(0.5,-0.5,-1.0), glm::vec3(0.0,0.0,1.0),
		       glm::vec2(1.0,1.0)));
	vert.push_back(Vertex(glm::vec3(0.5,0.5,-1.0), glm::vec3(0.0,0.0,1.0),
		       glm::vec2(1.0,0.0)));
	vector<unsigned short> ind;
	ind.push_back(0);
	ind.push_back(1);
	ind.push_back(2);
	ind.push_back(3);

	//create a mesh out of the vertices
	mesh = new Mesh();
	mesh->attach_vertex_array(&vert);
	mesh->attach_index_array(&ind);
	mesh->use_shader(shader);
	mesh->set_vertex_attribute("pos", 4, GL_FLOAT, sizeof(Vertex),
				   (void*)offsetof(Vertex, position));
	mesh->set_vertex_attribute("norm", 4, GL_FLOAT, sizeof(Vertex),
				   (void*)offsetof(Vertex, normal));
	mesh->set_vertex_attribute("tc_in", 2, GL_FLOAT, sizeof(Vertex),
				   (void*)offsetof(Vertex, texcoord));
}
