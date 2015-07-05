#include "gui.h"

GUI::GUI(const char *title, int res_x, int res_y, int offset_x,
	 int offset_y, int gl_maj, int gl_min, unsigned int flags)
	 :App(title, res_x, res_y, offset_x, offset_y, gl_maj, gl_min, flags) {

	wireframe = false;
	set_relative_mode(true);
	enable_vsync(true);
}

void GUI::display() {
	glClearColor(0,0,0,1);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->bind();

	int light_loc = glGetUniformLocation(shader->shader,
					  "light_pos");
	int texture_loc = glGetUniformLocation(shader->shader,
						    "Texture");
	//int time_loc = glGetUniformLocation(shader->shader, "time");

	glm::vec3 light_pos = glm::vec3(1.0, 1.0, 1.0);
	glUniform3fv(light_loc, 1, glm::value_ptr(light_pos));
	//glUniform1f(time_loc, 0);
	glUniform1i(texture_loc, 0);

	tex->bind();

	/*for(int i = -2; i < 3; i+=2) {
		glm::mat4 trafo = glm::translate(glm::vec3((float)i,0.0f,0.0f));
		mesh->draw(GL_TRIANGLE_STRIP, &trafo);
	}*/
	//scene->model_matrix = glm::mat4(1.0);
	scene->draw();
	shader->unbind();
}

void GUI::handle_keyboard() {
	if(key_pressed("Escape")) {
		exit(0);
	}
	if(key_pressed("A")) {
		camera->move_right(-1.0/10);
	}
	if(key_pressed("D")) {
		camera->move_right(1.0/10);
	}
	if(key_pressed("W")) {
		camera->move_forward(1.0/10);
	}
	if(key_pressed("S")) {
		camera->move_forward(-1.0/10);
	}
	if(key_pressed("R")) {
		camera->move_up(1.0/10);
	}
	if(key_pressed("F")) {
		camera->move_up(-1.0/10);
	}
	if(key_pressed("Q")) {
		camera->roll(-0.1);
	}
	if(key_pressed("E")) {
		camera->roll(0.1);
	}
	if(key_pressed("P")) {
		shader->recompile();
	}
	if(key_pressed("L")) {
		if(wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframe = false;
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframe = true;
		}
	}
	camera->update_view_matrix();
}

void GUI::handle_mouse_motion(int x, int y) {
	camera->yaw(-glm::atan(x)/100);
	camera->pitch(-glm::atan(y)/100);
	camera->update_view_matrix();
}

void GUI::handle_mouse_wheel(int x, int y) {
}

void GUI::handle_mouse_button(int x, int y, MOUSE_BUTTON button, bool state, int clicks) {
	printf("%i %i\t%i %i %i\n", x, y, button, state, clicks);
	fflush(stdout);
}
