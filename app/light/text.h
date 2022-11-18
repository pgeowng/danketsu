#ifndef TEXT_H
#define TEXT_H

#include "unity.h"

#include "shader.h"
#include "texture.h"

struct text_s {
  shader_s shader;
  GLuint tex, vao, vbo;
};

internal bool text_init(text_s* t) {
  bool ok = false;

  ok = shader_init(&t->shader, "app/text/text.vert", "app/text/text.frag");
  if (!ok) {
    printf("text: failed to load text shaders\n");
    return ok;
  }

  ok = tex_load(&t->tex, "assets/ascii.png");
  if (!ok) {
    printf("text: failed to load font texture\n");
    return ok;
  }

  float text[] = {
    0.0f,
    0.0f,
    0.0f,
    0.0f,
  };

  glGenVertexArrays(1, &t->vao);
  glBindVertexArray(t->vao);
  glGenBuffers(1, &t->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, t->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(text), text, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  return ok;
}

internal void text_draw(text_s* t, int x, int y, const char* str) {
  // 24 floats per character
  const int chars_size = 24 * 20;
  float chars[chars_size] = {};
  int size = 0;

  int idx = 0;

  float screen_w = 16.0f;
  float screen_h = 16.0f;
  float char_w = 16.0f;
  float char_h = 28.0f;
  while (str[idx] != '\0' && size < chars_size) {
    char str_i = str[idx];
    // float tex_x = str_i % 16 * char_width;
    // float tex_y = str_i / 16 * 16;

    float screen_x = idx * screen_w;
    float screen_y = 0;
    float char_x = str_i % 16 * char_w;
    float char_y = str_i / 16 * char_h;

    {
      // bottom left
      chars[size++] = screen_x;
      chars[size++] = screen_y;
      chars[size++] = char_x;
      chars[size++] = char_y;
    }
    for (int i = 0; i < 2; i++) {
      { // top left
        chars[size++] = screen_x;
        chars[size++] = screen_y + screen_h;
        chars[size++] = char_x;
        chars[size++] = char_y + char_h;
      }
      {
        // bottom right
        chars[size++] = screen_x + screen_w;
        chars[size++] = screen_y;
        chars[size++] = char_x + char_w;
        chars[size++] = char_y;
      }
    }
    {
      // top right
      chars[size++] = screen_x + screen_w;
      chars[size++] = screen_y + screen_h;
      chars[size++] = char_x + char_w;
      chars[size++] = char_y + char_h;
    }
  }

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, t->tex);

  glBindVertexArray(t->vao);
  glBindBuffer(GL_ARRAY_BUFFER, t->vbo);
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), chars, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  mat4 model(1.0f);
  // model = glm::scale(model, vec3(1.0f / 800.0f, 1.0f / 600.0f, 1.0f));
  shader_use(&t->shader);
  shader_mat4fv(&t->shader, "model", glm::value_ptr(model));
  shader_1i(&t->shader, "tex", 1);

  glBindVertexArray(t->vao);
  glBindBuffer(GL_ARRAY_BUFFER, t->vbo);
  glDrawArrays(GL_TRIANGLES, 0, 24);
}

#endif