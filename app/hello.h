#ifndef HELLO_H
#define HELLO_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "flycamera.h"

struct app_s
{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    flycamera_s camera = {};
    shader_s shader = {};
};

bool app_init(app_s *app)
{
    bool ok = false;
    flycamera_init(&app->camera);
    ok = shader_New(&app->shader, "./engine/shaders/hello.vert", "./engine/shaders/hello.frag");
    if (!ok)
    {
        printf("shader new failed");
        return ok;
    }

    float vertices[] = {
        // front
        -0.5f, -0.5f, -0.5f,
        0.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, // Front Left Top
        0.0f, 1.0f,         // Texture

        0.5f, 0.5f, -0.5f, // Front Right Top
        1.0f, 1.0f,        // Texture

        0.5f, -0.5f, -0.5f, // Front Right Bottom
        1.0f, 0.0f,         // Texture

        // right
        0.5f, -0.5f, -0.5f,
        0.0f, 0.0f,

        0.5f, 0.5f, -0.5f,
        1.0f, 0.0f,

        0.5f, 0.5f, 0.5f,
        1.0f, 1.0f,

        0.5f, -0.5f, 0.5f,
        0.0f, 1.0f,

        // back
        0.5f, -0.5f, 0.5f,
        0.0f, 0.0f,

        0.5f, 0.5f, 0.5f,
        0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f,
        1.0f, 1.0f,

        -0.5f, -0.5f, 0.5f,
        1.0f, 0.0f,

        // left
        -0.5f, -0.5f, 0.5f,
        0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f,
        0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f,
        1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,
        1.0f, 0.0f,

        // top

        -0.5f, 0.5f, -0.5f,
        0.0f, 0.0f,

        -0.5, 0.5f, 0.5f,
        0.0f, 1.0f,

        0.5f, 0.5f, 0.5f,
        1.0f, 1.01f,

        0.5, 0.5f, -0.5f,
        1.0f, 0.0f,

        // bottom
        -0.5f, -0.5f, -0.5f,
        0.0f, 0.0f,

        -0.5, -0.5f, 0.5f,
        0.0f, 1.0f,

        0.5f, -0.5f, 0.5f,
        1.0f, 1.0f,

        0.5, -0.5f, -0.5f,
        1.0f, 0.0f

        // -0.5f, -0.5f, 0.5f, // Back Left Bottom
        // 1.0f, 1.0f, 1.0f,   // Color
        // 1.0f, 1.0f,         // Texture

        // -0.5f, 0.5f, 0.5f, // Back Left Top
        // 1.0f, 1.0f, 1.0f,  // Color
        // 1.0f, 0.0f,        // Texture

        // 0.5f, 0.5f, 0.5f, // Back Right Top
        // 1.0f, 1.0f, 1.0f, // Color
        // 0.0f, 0.0f,       // Texture

        // 0.5f, -0.5f, 0.5f, // Back Right Bottom
        // 1.0f, 1.0f, 1.0f, // Color
        // 0.0f, 1.0f,       // Texture
    };

    unsigned int faces[] = {
        0, 1, 2, // Front

        2, 3, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20,

        //     // 4, 5, 6, // Back
        //     // 6, 7,4,

        //     // 0, 1, 5, // Left
        //     // 5, 4, 0,

        //     // 3, 2, 6, // Right
        //     // 6, 7, 3,

        //     // 1, 2, 6, // Top
        //     // 6, 5, 1,

        //     // 0, 3, 7, // Bottom
        //     // 7, 4, 0,
    };

    glGenVertexArrays(1, &app->vao);
    glBindVertexArray(app->vao);
    glGenBuffers(1, &app->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &app->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

    return ok;
}


void app_update(app_s *app, float delta)
{
    static float timeValue = 0.0f;
    timeValue += delta;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(20.0f * timeValue), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    flycamera_update(&app->camera, delta);

    shader_Use(&app->shader);
    shader_Set1i(&app->shader, "tex1", 0);
    shader_Set1i(&app->shader, "tex2", 1);
    shader_SetMatrix4fv(&app->shader, "model", glm::value_ptr(model));
    shader_SetMatrix4fv(&app->shader, "view", glm::value_ptr(flycamera_get_view_matrix(&app->camera)));
    // shader_SetMatrix4fv(&app->shader, "view", glm::value_ptr(flycamera_get_weird_view_matrix(&app->camera)));
    shader_SetMatrix4fv(&app->shader, "projection", glm::value_ptr(flycamera_get_projection_matrix(&app->camera)));

    glBindVertexArray(app->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->ebo);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glm::vec3 positions[] = {
        glm::vec3(0.2f, -0.2f, 1.0f),
        glm::vec3(-0.4f, 0.4f, 0.5f),
        glm::vec3(0.0f, 0.0f, 1.5f),
        glm::vec3(0.89f, 0.5f, 2.0f),
    };

    for (int i = 0; i < 3; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, positions[i]);
        model = glm::rotate(model, glm::radians(timeValue * 25 * (i + 1)), positions[i]);

        shader_SetMatrix4fv(&app->shader, "model", glm::value_ptr(model));
        glBindVertexArray(app->vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->ebo);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}
#endif