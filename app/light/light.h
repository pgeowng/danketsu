#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "flycamera.h"
#include "debug.h"

struct app_s {
		// glm::vec3 position;
		// glm::vec3 color;
		// glm::vec3 ambient;
		// glm::vec3 diffuse;
		// glm::vec3 specular;

	GLuint vao;
	GLuint light_vao;
	GLuint vbo;
	GLuint ebo;

	flycamera_s camera = {};
	shader_s lighting_shader = {};
	shader_s lamp_shader = {};

};

bool app_init(app_s *app) {
	bool ok = false;
	flycamera_init(&app->camera, false);
	ok = shader_New(&app->lighting_shader, "./app/light/light.vert", "./app/light/light.frag");
	if (!ok) {
		printf("lighting shader new failed");
		return ok;
	}

	ok = shader_New(&app->lamp_shader, "./app/light/light.vert", "./app/light/light_src.frag");
	if (!ok) {
		printf("lamp shader new failed");
		return ok;
	}

	float cube[] = {
		// front
		-0.5f, -0.5f, -0.5f,
		0.0f, 0.0f, -0.5f,

		-0.5f, 0.5f, -0.5f, // Front Left Top
		0.0f, 0.0f, -0.5f,

		0.5f, 0.5f, -0.5f, // Front Right Top
		0.0f, 0.0f, -0.5f,


		0.5f, 0.5f, -0.5f, // Front Right Top
		0.0f, 0.0f, -0.5f,

		0.5f, -0.5f, -0.5f, // Front Right Bottom
		0.0f, 0.0f, -0.5f,

		-0.5f, -0.5f, -0.5f,
		0.0f, 0.0f, -0.5f,


		// right
		0.5f, -0.5f, -0.5f,
		1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, -0.5f,
		1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, 0.5f,
		1.0f, 0.0f, 0.0f,


		0.5f, 0.5f, 0.5f,
		1.0f, 0.0f, 0.0f,

		0.5f, -0.5f, 0.5f,
		1.0f, 0.0f, 0.0f,

		0.5f, -0.5f, -0.5f,
		1.0f, 0.0f, 0.0f,


		// back
		0.5f, -0.5f, 0.5f,
		0.0f, 0.0f, 1.0f,

		0.5f, 0.5f, 0.5f,
		0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f,
		0.0f, 0.0f, 1.0f,


		-0.5f, 0.5f, 0.5f,
		0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f, 0.5f,
		0.0f, 0.0f, 1.0f,

		0.5f, -0.5f, 0.5f,
		0.0f, 0.0f, 1.0f,


		// left
		-0.5f, -0.5f, 0.5f,
		-1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f,
		-1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, -0.5f,
		-1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, -0.5f,
		-1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,
		-1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,
		-1.0f, 0.0f, 0.0f,


		// top
		-0.5f, 0.5f, -0.5f,
		0.0f, 1.0f, 0.0f,

		-0.5, 0.5f, 0.5f,
		0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f,
		0.0f, 1.0f, 0.0f,


		0.5f, 0.5f, 0.5f,
		0.0f, 1.0f, 0.0f,

		0.5, 0.5f, -0.5f,
		0.0f, 1.0f, 0.0f,

		-0.5f, 0.5f, -0.5f,
		0.0f, 1.0f, 0.0f,


		// bottom
		-0.5f, -0.5f, -0.5f,
		0.0f, -1.0f, 0.0f,

		-0.5, -0.5f, 0.5f,
		0.0f, -1.0f, 0.0f,

		0.5f, -0.5f, 0.5f,
		0.0f, -1.0f, 0.0f,


		0.5f, -0.5f, 0.5f,
		0.0f, -1.0f, 0.0f,

		0.5, -0.5f, -0.5f,
		0.0f, -1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,
		0.0f, -1.0f, 0.0f

	};

	glGenVertexArrays(1, &app->vao);
	glBindVertexArray(app->vao);
	glGenBuffers(1, &app->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &app->light_vao);
	glBindVertexArray(app->light_vao);

	glBindBuffer(GL_ARRAY_BUFFER, app->vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	return ok;
}

void app_update(app_s *app, float dt) {
	flycamera_update(&app->camera, dt);

	 float time = SDL_GetTicks() / 1000.0f;

	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 light_pos = glm::vec3(1.2f + cos(time), 1.0f, 2.0f + sin(time));


        glm::mat4 view = flycamera_get_view_matrix(&app->camera);
        glm::mat4 proj = flycamera_get_projection_matrix(&app->camera);


 {
	model = glm::translate(model, light_pos);
	model = glm::scale(model, glm::vec3(0.2f));

	shader_Use(&app->lamp_shader);
	shader_SetMatrix4fv(&app->lamp_shader, "model", glm::value_ptr(model));
	shader_SetMatrix4fv(&app->lamp_shader, "view", glm::value_ptr(view));
	shader_SetMatrix4fv(&app->lamp_shader, "projection", glm::value_ptr(proj));

	glBindVertexArray(app->light_vao);
	glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
	glDrawArrays(GL_TRIANGLES, 0, 72);
 }

 {
	model = glm::mat4(1.0f);
	shader_Use(&app->lighting_shader);
	shader_Set3f(&app->lighting_shader, "objectColor", 1.0f, 0.5f, 0.31f);
	shader_Set3f(&app->lighting_shader, "lightColor", 1.0f, 1.0f, 1.0f);
	shader_Set3f(&app->lighting_shader, "lightPos", light_pos.x, light_pos.y, light_pos.z);

	shader_SetMatrix4fv(&app->lighting_shader, "model", glm::value_ptr(model));
	shader_SetMatrix4fv(&app->lighting_shader, "view", glm::value_ptr(view));
	shader_SetMatrix4fv(&app->lighting_shader, "projection", glm::value_ptr(proj));

	glBindVertexArray(app->vao);
	glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
	glDrawArrays(GL_TRIANGLES, 0, 72);
 }
}

#endif