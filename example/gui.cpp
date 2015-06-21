#include "gui.h"

GUI::GUI(const char *title, int res_x, int res_y, int offset_x,
	 int offset_y, int gl_maj, int gl_min, unsigned int flags):App(title,
	 res_x, res_y, offset_x, offset_y, gl_maj, gl_min, flags) {

	set_relative_mode(true);
	enable_vsync(true);
}

void GUI::update_cam() {
	V = glm::lookAt(glm::vec3(camera->pos),
			glm::vec3(camera->pos + camera->dir),
			glm::vec3(camera->up));
}

void GUI::display() {
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);

	shader->bind();

	int MVP_loc = glGetUniformLocation(shader->shader, "MVP");
	int MV_loc = glGetUniformLocation(shader->shader, "MV");
	int NM_loc = glGetUniformLocation(shader->shader,
					  "NormalMatrix");
	int LS_loc = glGetUniformLocation(shader->shader,
					  "LightSource");
	int location_Texture = glGetUniformLocation(shader->shader,
						    "Texture");
	//int time_loc = glGetUniformLocation(shader->shader, "time");

	glm::mat4 MV = V * mesh->trafo;
	glm::mat4 MVP = P * MV;
	glm::mat3 NM = glm::transpose(glm::inverse(glm::mat3(MV)));
	glm::vec4 LS = glm::vec4(1, 1, 1, 1);
	glUniformMatrix4fv(MVP_loc, 1, false, glm::value_ptr(MVP));
	glUniformMatrix4fv(MV_loc, 1, false, glm::value_ptr(MV));
	glUniformMatrix3fv(NM_loc, 1, false, glm::value_ptr(NM));
	glUniform4fv(LS_loc, 1, glm::value_ptr(LS));
	//glUniform1f(time_loc, 0);
	glUniform1i(location_Texture, 0);

	glActiveTexture(GL_TEXTURE0);
	tex->bind();

	mesh->draw(GL_TRIANGLE_STRIP);
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
		camera->roll(-0.01);
	}
	if(key_pressed("E")) {
		camera->roll(0.01);
	}
	if(key_pressed("P")) {
		shader->recompile();
	}
	update_cam();
}

void GUI::handle_mouse_motion(int x, int y) {
	camera->yaw(-glm::atan(x)/100);
	camera->pitch(-glm::atan(y)/100);
	update_cam();
}

void GUI::handle_mouse_wheel(int x, int y) {
}

void GUI::handle_mouse_button(int x, int y, MOUSE_BUTTON button, bool state, int clicks) {
	printf("%i %i\t%i %i %i\n", x, y, button, state, clicks);
	fflush(stdout);
}
