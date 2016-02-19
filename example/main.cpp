#include <sgltk/app.h>
#include <sgltk/scene.h>
#include <sgltk/camera.h>
#include <sgltk/texture.h>
#include <unistd.h>
#include <string.h>

#include "gui.h"

using namespace std;

Shader *shader;
Shader *fps_shader;
Texture *tex;
Mesh<> *fps_display;
Scene *scene;
Camera *fps_camera;
Camera *camera;
glm::mat4 V, P;

GUI *app;

static void init(void);

int main(int argc, char** argv) {
	//change the current working directory to the one containing the executable
	std::string path(argv[0]);
	path = path.substr(0, path.find_last_of("\\/"));
	chdir(path.c_str());

	Image::add_path("data");

	app = new GUI("Test", 1024, 768, 100, 100, 3, 0, 0);
	//GL calls should be used after the App class initializes GLEW
	init();
	app->run(60);
	return 0;
}

void init() {
	//enable textures and blending
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT, GL_FILL);

	//load an image
	Image img1;
	img1.load("earth.jpg");
	//create an image from text
	Image img2;
	img2.create_text("TTF fonts are cool!", "data/Oswald-Medium.ttf",
			 64, 255, 0, 0, 255);
	//copy one image into the other
	img1.copy_from(&img2, img1.width/2-img2.width/2,
		       img1.height-img2.height);
	//make a texture out of the image
	tex = new Texture(&img1);

	//set up the cameras
	fps_camera = new Camera(sgltk::ORTHOGRAPHIC);
	camera = new Camera(glm::vec3(0,0,10), glm::vec3(0,0,-1),
			    glm::vec3(0,1,0), 70.0f, app->width,
			    app->height, 0.1f, 800.0f);

	//load the shaders
	shader = new Shader();
	shader->attach_file("data/example_vs.glsl", GL_VERTEX_SHADER);
	shader->attach_file("data/example_fs.glsl", GL_FRAGMENT_SHADER);
	shader->link();

	fps_shader = new Shader();
	fps_shader->attach_file("data/fps_vs.glsl", GL_VERTEX_SHADER);
	fps_shader->attach_file("data/fps_fs.glsl", GL_FRAGMENT_SHADER);
	fps_shader->link();

	//arrange a few vertices into a square
	vector<sgltk::Vertex> vert;
	vert.push_back(sgltk::Vertex(glm::vec3(0.0,0.0,0.0),
				     glm::vec3(0.0,0.0,1.0),
				     glm::vec3(0.0,1.0,0.0)));
	vert.push_back(sgltk::Vertex(glm::vec3(0.0,1.0,0.0),
				     glm::vec3(0.0,0.0,1.0),
				     glm::vec3(0.0,0.0,0.0)));
	vert.push_back(sgltk::Vertex(glm::vec3(1.0,0.0,0.0),
				     glm::vec3(0.0,0.0,1.0),
				     glm::vec3(1.0,1.0,0.0)));
	vert.push_back(sgltk::Vertex(glm::vec3(1.0,1.0,0.0),
				     glm::vec3(0.0,0.0,1.0),
				     glm::vec3(1.0,0.0,0.0)));
	vector<unsigned short> ind;
	ind.push_back(0);
	ind.push_back(1);
	ind.push_back(2);
	ind.push_back(3);

	//create a mesh out of the vertices
	fps_display = new Mesh<>();
	fps_display->attach_vertex_buffer(&vert);
	fps_display->attach_index_buffer(&ind);
	fps_display->setup_shader(fps_shader);
	fps_display->setup_camera(&fps_camera->view_matrix,
			   &fps_camera->projection_matrix_ortho);
	fps_display->set_vertex_attribute("pos_in", 0, 4, GL_FLOAT, sizeof(sgltk::Vertex),
				   (void*)offsetof(sgltk::Vertex, position));
	fps_display->set_vertex_attribute("tex_coord_in0", 0, 3, GL_FLOAT, sizeof(sgltk::Vertex),
				   (void*)offsetof(sgltk::Vertex, tex_coord));

	scene = new Scene();
	scene->setup_shader(shader);
	scene->setup_camera(&camera->view_matrix, &camera->projection_matrix_persp);
	scene->load("data/Spikey.dae");
	//scene->load("data/boblampclean.md5mesh");
}
