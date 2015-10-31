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

	if(wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	shader->bind();

	int light_loc = glGetUniformLocation(shader->shader,
					  "light_dir");
	int texture_loc = glGetUniformLocation(shader->shader,
						    "Texture");
	//int time_loc = glGetUniformLocation(shader->shader, "time");

	glm::vec3 light_dir = glm::vec3(camera->view_matrix *
				glm::vec4(1.0, -1.0, -1.0, 0.0));
	glUniform3fv(light_loc, 1, &light_dir[0]);
	//glUniform1f(time_loc, 0);
	glUniform1i(texture_loc, 0);
	shader->unbind();

	tex->bind();

	//mesh->draw(GL_TRIANGLE_STRIP);
	/*for(int i = -2; i < 3; i+=2) {
		glm::mat4 trafo = glm::translate(glm::vec3((float)i,0.0f,0.0f));
		mesh->draw(GL_TRIANGLE_STRIP, &trafo);
	}*/
	//scene->model_matrix = glm::mat4(1.0);
	/*for(int i = -2; i < 3; i+=2) {
		glm::mat4 trafo = glm::translate(glm::vec3((float)i * 2, 0.0f, 0.0f));
		scene->draw(&trafo);
	}*/
	scene->draw();
	check_gl_error("display");
}

void GUI::handle_resize() {
	glViewport(0, 0, width, height);
	camera->update_projection_matrix(width, height);
}

void GUI::handle_keyboard() {
	if(key_pressed("Escape")) {
		exit(0);
	}
	if(key_pressed("A")) {
		camera->move_right(-0.01 * frame_time);
	}
	if(key_pressed("D")) {
		camera->move_right(0.01 * frame_time);
	}
	if(key_pressed("W")) {
		camera->move_forward(0.01 * frame_time);
	}
	if(key_pressed("S")) {
		camera->move_forward(-0.01 * frame_time);
	}
	if(key_pressed("R")) {
		camera->move_up(0.01 * frame_time);
	}
	if(key_pressed("F")) {
		camera->move_up(-0.01 * frame_time);
	}
	if(key_pressed("Q")) {
		camera->roll(-0.01 * frame_time);
	}
	if(key_pressed("E")) {
		camera->roll(0.01 * frame_time);
	}
	if(key_pressed("P")) {
		shader->recompile();
	}
	if(key_pressed("L") && !wireframe_change) {
		wireframe_change = true;
		wireframe = !wireframe;
	} else if(!key_pressed("L")) {
		wireframe_change = false;
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
