#include "gui.h"

GUI::GUI(const char *title, int res_x, int res_y, int offset_x,
	 int offset_y, int gl_maj, int gl_min, unsigned int flags)
	 :App(title, res_x, res_y, offset_x, offset_y, gl_maj, gl_min, flags) {

	fps_trafo = glm::scale(glm::vec3(50.0, 30.0, 0.0));
	fps_trafo = glm::translate(fps_trafo, glm::vec3(11.5, 15.0, 0.0));
	for(int i = -4; i < 4; i+=1) {
		for(int j = -4; j < 4; j+=1) {
			trafos.push_back(glm::translate(glm::vec3((float)i*4, 0.0f, (float)j*4)));
		}
	}

	fps = 0;
	wireframe = false;
	set_relative_mode(true);
	enable_vsync(true);
}

void GUI::display() {
	glClearColor(0,0,0,1);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	time_cnt += delta_time;
	frame_cnt++;
	if(time_cnt >= 1000) {
		fps = frame_cnt;
		frame_cnt = 0;
		time_cnt = 0;
	}
	Image fps_text;
	fps_text.create_text("FPS: " + std::to_string(fps),
			     "data/Oswald-Medium.ttf", 40, 0, 0, 255, 255);
	Texture fps_tex = Texture(&fps_text);

	fps_shader->bind();
	int texture_loc = glGetUniformLocation(fps_shader->shader,
						    "Texture");
	glUniform1i(texture_loc, 0);

	shader->bind();
	int light_loc = glGetUniformLocation(shader->shader,
					  "light_dir");
	texture_loc = glGetUniformLocation(shader->shader,
						    "Texture");
	//int time_loc = glGetUniformLocation(shader->shader, "time");

	glm::vec3 light_dir = glm::vec3(camera->view_matrix *
				glm::vec4(1.0, -1.0, -1.0, 0.0));
	glUniform3fv(light_loc, 1, &light_dir[0]);
	//glUniform1f(time_loc, 0);
	glUniform1i(texture_loc, 0);
	shader->unbind();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	fps_tex.bind();
	//tex->bind();
	fps_display->draw(GL_TRIANGLE_STRIP, &fps_trafo);
	fps_tex.unbind();
	/*for(int i = -2; i < 3; i+=2) {
		glm::mat4 trafo = glm::translate(glm::vec3((float)i,0.0f,0.0f));
		mesh->draw(GL_TRIANGLE_STRIP, &trafo);
	}*/
	if(wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for(int i = 0; i < trafos.size(); i++) {
		scene->draw(&trafos[i]);
	}
	check_gl_error("display");
}

void GUI::handle_resize() {
	glViewport(0, 0, width, height);
	fps_camera->update_projection_matrix(width, height);
	camera->update_projection_matrix(width, height);
}

void GUI::handle_keyboard() {
	if(key_pressed("Escape")) {
		exit(0);
	}
	if(key_pressed("A")) {
		camera->move_right(-0.01 * delta_time);
	}
	if(key_pressed("D")) {
		camera->move_right(0.01 * delta_time);
	}
	if(key_pressed("W")) {
		camera->move_forward(0.01 * delta_time);
	}
	if(key_pressed("S")) {
		camera->move_forward(-0.01 * delta_time);
	}
	if(key_pressed("R")) {
		camera->move_up(0.01 * delta_time);
	}
	if(key_pressed("F")) {
		camera->move_up(-0.01 * delta_time);
	}
	if(key_pressed("Q")) {
		camera->roll(-0.005 * delta_time);
	}
	if(key_pressed("E")) {
		camera->roll(0.005 * delta_time);
	}
	if(key_pressed("P")) {
		shader->recompile();
		fps_shader->recompile();
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
	camera->yaw(-glm::atan(x)/500);
	camera->pitch(-glm::atan(y)/500);
	camera->update_view_matrix();
}

void GUI::handle_mouse_wheel(int x, int y) {
}

void GUI::handle_mouse_button(int x, int y, MOUSE_BUTTON button, bool state, int clicks) {
	printf("%i %i\t%i %i %i\n", x, y, button, state, clicks);
	fflush(stdout);
}
