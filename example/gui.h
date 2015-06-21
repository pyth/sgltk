#ifndef __GUI_H__
#define __GUI_H__

#include <sgltk/app.h>
#include <sgltk/mesh.h>
#include <sgltk/camera.h>
#include <sgltk/texture.h>
#include <sgltk/shader.h>

extern Shader *shader;
extern Texture *tex;
extern Mesh *mesh;
extern Camera *camera;
extern glm::mat4 V, P;

class GUI : public App {
public:
	GUI(const char *title, int res_x, int res_y, int offset_x,
	    int offset_y, int gl_maj, int gl_min,
	    unsigned int flags);

	void update_cam(void);
	void handle_keyboard();
	void handle_mouse_motion(int x, int y);
	void handle_mouse_wheel(int x, int y);
	void handle_mouse_button(int x, int y, MOUSE_BUTTON button, bool state, int clicks);
	void display();
};

#endif

