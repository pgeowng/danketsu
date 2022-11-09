#ifndef HELLO_H
#define HELLO_H

#include <GL/glew.h>
#include "../engine/shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct cubes
{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    flycamera_s camera;
    shader_s shader;
};

void cubes_init(cubes *demo)
{
    flycamera_init(&demo->camera);
    shader_New(&demo->shader, "shaders/cubes.vert", "shaders/cubes.frag");
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

    glGenVertexArrays(1, &demo->vao);
    glBindVertexArray(demo->vao);
    glGenBuffers(1, &demo->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, demo->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &demo->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, demo->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
}

void cubes_render(cubes *demo, float delta)
{
    static float timeValue = 0.0f;
    timeValue += delta;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(20.0f * timeValue), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    flycamera_update(&demo->camera, delta);

    shader_Use(&demo->shader);
    shader_Set1i(&demo->shader, "tex1", 0);
    shader_Set1i(&demo->shader, "tex2", 1);
    shader_SetMatrix4fv(&demo->shader, "model", glm::value_ptr(model));
    shader_SetMatrix4fv(&demo->shader, "view", glm::value_ptr(flycamera_get_view_matrix(&demo->camera)));
    shader_SetMatrix4fv(&demo->shader, "projection", glm::value_ptr(flycamera_get_projection_matrix(&demo->camera)));

    glBindVertexArray(demo->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, demo->ebo);
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

        shader_SetMatrix4fv(&demo->shader, "model", glm::value_ptr(model));
        glBindVertexArray(demo->vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, demo->ebo);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}
#endif