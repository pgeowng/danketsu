void RenderInit(Renderer *r) {
  glGenVertexArrays(1, &r->vao);
  glBindVertexArray(r->vao);

  glGenBuffers(1, &r->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
  glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(RenderVertex), nullptr,
               GL_DYNAMIC_DRAW);

  r->projection = r->_pj;
  r->projection =
      m4Ortho(r->projection, 0.0f, 1080.0f, 720.0f, 0.0f, -0.01f, 1.0f);

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

  glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, r->projection);
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

void RenderBeginFrame(Renderer *r) { r->triangleLen = 0; }
void RenderEndFrame(Renderer *r) {
  glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, r->triangleLen * 3 * sizeof(RenderVertex),
                  r->triangleBuffer);
  glUseProgram(r->shader);

  glBindVertexArray(r->vao);
  glDrawArrays(GL_TRIANGLES, 0, r->triangleLen * 3);
}

void RenderPushTriangle(Renderer *r, v2 a, v2 b, v2 c, v4 aColor, v4 bColor,
                        v4 cColor) {
  // Flush the batch if it is full. We don't like segfaults on this channel.
  if (r->triangleLen == MAX_TRIANGLES) {
    RenderEndFrame(r);
    RenderBeginFrame(r);
  }

  r->triangleBuffer[r->triangleLen * 3 + 0].pos[0] = a[0];
  r->triangleBuffer[r->triangleLen * 3 + 0].pos[1] = a[1];
  r->triangleBuffer[r->triangleLen * 3 + 0].color[0] = aColor[0];
  r->triangleBuffer[r->triangleLen * 3 + 0].color[1] = aColor[1];
  r->triangleBuffer[r->triangleLen * 3 + 0].color[2] = aColor[2];
  r->triangleBuffer[r->triangleLen * 3 + 0].color[3] = aColor[3];
  r->triangleBuffer[r->triangleLen * 3 + 1].pos[0] = b[0];
  r->triangleBuffer[r->triangleLen * 3 + 1].pos[1] = b[1];
  r->triangleBuffer[r->triangleLen * 3 + 1].color[0] = bColor[0];
  r->triangleBuffer[r->triangleLen * 3 + 1].color[1] = bColor[1];
  r->triangleBuffer[r->triangleLen * 3 + 1].color[2] = bColor[2];
  r->triangleBuffer[r->triangleLen * 3 + 1].color[3] = bColor[3];
  r->triangleBuffer[r->triangleLen * 3 + 2].pos[0] = c[0];
  r->triangleBuffer[r->triangleLen * 3 + 2].pos[1] = c[1];
  r->triangleBuffer[r->triangleLen * 3 + 2].color[0] = cColor[0];
  r->triangleBuffer[r->triangleLen * 3 + 2].color[1] = cColor[1];
  r->triangleBuffer[r->triangleLen * 3 + 2].color[2] = cColor[2];
  r->triangleBuffer[r->triangleLen * 3 + 2].color[3] = cColor[3];
  r->triangleLen++;
}