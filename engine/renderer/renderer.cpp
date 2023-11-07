
void RenderInit(Renderer *r) {
  glGenVertexArrays(1, &r->vao);
  glGenBuffers(1, &r->vbo);

  glBindVertexArray(r->vao);
  glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
  glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(RenderVertex), nullptr,
               GL_DYNAMIC_DRAW);

  r->projection = r->_pj;
  // TODO: adopt to different resolutions
  r->projection =
      m4Ortho(r->projection, 0.0f, 1000.0f, 1000.0f, 0.0f, -0.01f, 1.0f);

  r->shader = glCreateProgram();
  u32 vertModule = glCreateShader(GL_VERTEX_SHADER);
  u32 fragModule = glCreateShader(GL_FRAGMENT_SHADER);

  u32 vertModuleFileLen = 0;
  void *vertModuleFile = SDL_LoadFile("assets/vertexshader.vert.glsl",
                                      (size_t *)(&vertModuleFileLen));
  if (vertModuleFile == nullptr) {
    LogError("RenderInit: failed to load vertModuleFile");
    return;
  }

  u32 fragModuleFileLen = 0;
  void *fragModuleFile = SDL_LoadFile("assets/fragmentshader.frag.glsl",
                                      (size_t *)&fragModuleFileLen);
  if (fragModuleFile == nullptr) {
    LogError("RenderInit: failed to load fragModuleFile");
    return;
  }

  glShaderSource(vertModule, 1, (const GLchar *const *)&vertModuleFile,
                 (GLint *)&vertModuleFileLen);
  glShaderSource(fragModule, 1, (const GLchar *const *)&fragModuleFile,
                 (GLint *)&fragModuleFileLen);

  glCompileShader(vertModule);
  glCompileShader(fragModule);

  i32 ok = GL_FALSE;

  glGetShaderiv(vertModule, GL_COMPILE_STATUS, &ok);
  if (ok != GL_TRUE) {
    LogError("RenderInit: Vertex Shader Compilation failed");
    printShaderLog(vertModule);
  }

  glGetShaderiv(fragModule, GL_COMPILE_STATUS, &ok);
  if (ok != GL_TRUE) {
    LogError("RenderInit: Fragment Shader Compilation failed");
    printShaderLog(fragModule);
  }

  glAttachShader(r->shader, vertModule);
  glAttachShader(r->shader, fragModule);

  glLinkProgram(r->shader);
  glGetProgramiv(r->shader, GL_LINK_STATUS, &ok);
  if (ok != GL_TRUE) {
    LogError("RenderInit: Program Linking failed");
    printProgramLog(r->shader);
  }

  glDetachShader(r->shader, vertModule);
  glDetachShader(r->shader, fragModule);
  glDeleteShader(vertModule);
  glDeleteShader(fragModule);

  SDL_free(vertModuleFile);
  SDL_free(fragModuleFile);

  glUseProgram(r->shader);
  u32 projectionLocation = glGetUniformLocation(r->shader, "u_proj");
  r->projectionLocation = projectionLocation;
  glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, r->projection);

  u32 textureLocation = glGetUniformLocation(r->shader, "u_tex");
  i32 textures[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  glUniform1iv(textureLocation, 8, textures);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(RenderVertex),
                        (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(RenderVertex),
                        (void *)offsetof(RenderVertex, color));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RenderVertex),
                        (void *)offsetof(RenderVertex, uv));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(RenderVertex),
                        (void *)offsetof(RenderVertex, texIndex));
  glBindVertexArray(0);
}

void printShaderLog(u32 shader) {
  if (!glIsShader(shader)) {
    LogError("shaderInfoLog: %d isn't shader\n", shader);
    return;
  }

  // TODO: make use of string allocator
  // glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

  const i32 bufferCap = 512;
  i32 bufferLen = 0;
  i8 buffer[bufferCap];

  glGetShaderInfoLog(shader, bufferCap, &bufferLen, (GLchar *)buffer);
  if (bufferLen > 0) {
    LogError("shaderInfoLog: %s\n", buffer);
  }
}

void printProgramLog(u32 program) {
  if (!glIsProgram(program)) {
    LogError("programInfoLog: %d isn't program\n", program);
    return;
  }

  // TODO: make use of string allocator
  // glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

  const i32 bufferCap = 512;
  i32 bufferLen = 0;
  i8 buffer[bufferCap];

  glGetProgramInfoLog(program, bufferCap, &bufferLen, (GLchar *)buffer);
  if (bufferLen > 0) {
    LogError("programInfoLog: %s\n", buffer);
  }
}

void RenderFree(Renderer *r) {
  glDeleteBuffers(1, &r->vbo);
  glDeleteVertexArrays(1, &r->vao);
  glDeleteProgram(r->shader);
}

void RenderBeginFrame(Renderer *r) {
  glClear(GL_COLOR_BUFFER_BIT);

  r->triangleLen = 0;
  r->textureLen = 0;
}

void RenderSetProjection(Renderer *r, m4 newProjection) {
  m4Copy(r->projection, newProjection);
}

void RenderEndFrame(Renderer *r) {
  glUseProgram(r->shader);
  glUniformMatrix4fv(r->projectionLocation, 1, GL_FALSE, r->projection);

  for (u32 i = 0; i < r->textureLen; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, r->texture[i]);
  }

  glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, r->triangleLen * 3 * sizeof(RenderVertex),
                  r->triangleBuffer);
  glUseProgram(r->shader);

  glBindVertexArray(r->vao);
  glDrawArrays(GL_TRIANGLES, 0, r->triangleLen * 3);
}

void RenderPushTriangle(Renderer *r, v2 a, v2 b, v2 c, v4 aColor, v4 bColor,
                        v4 cColor, v2 aUV, v2 bUV, v2 cUV, u32 texture) {
  f32 a3[3];
  v2Copy(a3, a);
  a3[2] = 0;
  f32 b3[3];
  v2Copy(b3, a);
  b3[2] = 0;
  f32 c3[3];
  v2Copy(c3, a);
  c3[2] = 0;

  RenderPushTriangle3D(r, a3, b3, c3, aColor, bColor, cColor, aUV, bUV, cUV,
                       texture);
}

void RenderPushTriangle3D(Renderer *r, v3 a, v3 b, v3 c, v4 aColor, v4 bColor,
                          v4 cColor, v2 aUV, v2 bUV, v2 cUV, u32 texture) {
  u32 texIndex = 1248;
  for (u32 i = 0; i < r->textureLen; i++) {
    if (r->texture[i] == texture) {
      texIndex = i;
      break;
    }
  }

  if (texIndex == 1248 && r->textureLen < 8) {
    r->texture[r->textureLen] = texture;
    texIndex = r->textureLen;
    r->textureLen += 1;
  }

  // Flush the batch if it is full. We don't like segfaults on this channel.
  if (r->triangleLen == MAX_TRIANGLES || texIndex == 1248) {
    RenderEndFrame(r);
    RenderBeginFrame(r);
  }

  r->triangleBuffer[r->triangleLen * 3 + 0].pos[0] = a[0];
  r->triangleBuffer[r->triangleLen * 3 + 0].pos[1] = a[1];
  r->triangleBuffer[r->triangleLen * 3 + 0].pos[2] = a[2];
  r->triangleBuffer[r->triangleLen * 3 + 0].color[0] = aColor[0];
  r->triangleBuffer[r->triangleLen * 3 + 0].color[1] = aColor[1];
  r->triangleBuffer[r->triangleLen * 3 + 0].color[2] = aColor[2];
  r->triangleBuffer[r->triangleLen * 3 + 0].color[3] = aColor[3];
  r->triangleBuffer[r->triangleLen * 3 + 0].uv[0] = aUV[0];
  r->triangleBuffer[r->triangleLen * 3 + 0].uv[1] = aUV[1];
  r->triangleBuffer[r->triangleLen * 3 + 0].texIndex = (f32)texIndex;
  r->triangleBuffer[r->triangleLen * 3 + 1].pos[0] = b[0];
  r->triangleBuffer[r->triangleLen * 3 + 1].pos[1] = b[1];
  r->triangleBuffer[r->triangleLen * 3 + 1].pos[2] = b[2];
  r->triangleBuffer[r->triangleLen * 3 + 1].color[0] = bColor[0];
  r->triangleBuffer[r->triangleLen * 3 + 1].color[1] = bColor[1];
  r->triangleBuffer[r->triangleLen * 3 + 1].color[2] = bColor[2];
  r->triangleBuffer[r->triangleLen * 3 + 1].color[3] = bColor[3];
  r->triangleBuffer[r->triangleLen * 3 + 1].uv[0] = bUV[0];
  r->triangleBuffer[r->triangleLen * 3 + 1].uv[1] = bUV[1];
  r->triangleBuffer[r->triangleLen * 3 + 1].texIndex = (f32)texIndex;
  r->triangleBuffer[r->triangleLen * 3 + 2].pos[0] = c[0];
  r->triangleBuffer[r->triangleLen * 3 + 2].pos[1] = c[1];
  r->triangleBuffer[r->triangleLen * 3 + 2].pos[2] = c[2];
  r->triangleBuffer[r->triangleLen * 3 + 2].color[0] = cColor[0];
  r->triangleBuffer[r->triangleLen * 3 + 2].color[1] = cColor[1];
  r->triangleBuffer[r->triangleLen * 3 + 2].color[2] = cColor[2];
  r->triangleBuffer[r->triangleLen * 3 + 2].color[3] = cColor[3];
  r->triangleBuffer[r->triangleLen * 3 + 2].uv[0] = cUV[0];
  r->triangleBuffer[r->triangleLen * 3 + 2].uv[1] = cUV[1];
  r->triangleBuffer[r->triangleLen * 3 + 2].texIndex = (f32)texIndex;
  r->triangleLen++;
}
