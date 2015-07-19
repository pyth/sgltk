#ifndef __GUI_H__
#define __GUI_H__

#include <sgltk/app.h>
#include <sgltk/mesh.h>
#include <sgltk/scene.h>
#include <sgltk/camera.h>
#include <sgltk/texture.h>
#include <sgltk/shader.h>

typedef struct Vertex {
	GLfloat position[4];
	GLfloat normal[3];
	GLfloat tex_coord[3];
	Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec3 tc) {
		position[0] = pos.x;
		position[1] = pos.y;
		position[2] = pos.z;
		position[3] = 1;

		normal[0] = norm.x;
		normal[1] = norm.y;
		normal[2] = norm.z;

		tex_coord[0] = tc.x;
		tex_coord[1] = tc.y;
		tex_coord[2] = tc.z;
	}
} Vertex;

extern Shader *shader;
extern Texture *tex;
extern Mesh<> *mesh;
extern Scene *scene;
extern Camera *camera;

class GUI : public App {
	bool wireframe;
	bool wireframe_change;
public:
	GUI(const char *title, int res_x, int res_y, int offset_x,
	    int offset_y, int gl_maj, int gl_min,
	    unsigned int flags);

	void handle_resize();
	void handle_keyboard();
	void handle_mouse_motion(int x, int y);
	void handle_mouse_wheel(int x, int y);
	void handle_mouse_button(int x, int y, MOUSE_BUTTON button, bool state, int clicks);
	void display();
};

#endif

