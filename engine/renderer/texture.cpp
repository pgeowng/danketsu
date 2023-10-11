void RenderTextureLoad(Texture *tx, const char *path) {
  i32 width, height, channels;
  u8 *data = stbi_load(path, &width, &height, &channels, 0);
  if (!data) {
    LogError("LoadTexture: failed to read filepath: %s\n", path);
    return;
  }

  tx->width = (u32)width;
  tx->height = (u32)height;
  tx->channels = (u32)channels;

  glGenTextures(1, &tx->texture);
  glBindTexture(GL_TEXTURE_2D, tx->texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  channels == 4 ? GL_CLAMP_TO_EDGE : GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                  channels == 4 ? GL_CLAMP_TO_EDGE : GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (channels == 3) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
  } else if (channels == 4) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  return;
}

void RenderTextureFree(u32 handle) { glDeleteTextures(1, &handle); }