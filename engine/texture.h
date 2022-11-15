#ifndef TEXTURE_H
#define TEXTURE_H

#include "unity.h"

#include "texture.h"

bool tex_load(GLuint* tex_unit, const char* path, GLenum tex_unit_enum,
              GLenum format) {

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
  if (data == NULL) {
    printf("failed to load texture %s", path);
    return 0;
  }

  glGenTextures(1, tex_unit);
  glBindTexture(GL_TEXTURE_2D, *tex_unit);

  // set the texture wrapping/filtering options (on the currently bound
  // texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // float borderColor[] = { 1.0f, 0.2f, 0.45f, 0.5f };
  // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  glActiveTexture(tex_unit_enum);
  glBindTexture(GL_TEXTURE_2D, *tex_unit);

  return 1;
}

#endif