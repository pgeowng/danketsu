#ifndef SHADER_H
#define SHADER_H

#include "unity.h"

struct shader_s {
  // char name[MAX_QPATH];
  // int index;
  GLuint program;
  // GLuint vertexShader;
  // GLuint fragmentShader;
  // GLint attribs[ATTR_INDEX_MAX];
  // GLint uniforms[UNIFORM_MAX];
  // struct shader_s *next;
};

void printProgramLog(GLuint program) {
  if (!glIsProgram(program)) {
    printf("print program log failed: %d isn't program\n", program);
    return;
  }

  int infoLogLength = 0;
  int maxLength = infoLogLength;

  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

  char *infoLog = new char[maxLength];
  glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
  if (infoLogLength > 0) {
    printf("%s\n", infoLog);
  }

  delete[] infoLog;
}

void printShaderLog(GLuint shader) {
  if (!glIsShader(shader)) {
    printf("print shader log failed: %d isn't shader\n", shader);
    return;
  }

  int infoLogLength = 0;
  int maxLength = infoLogLength;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

  char *infoLog = new char[maxLength];
  glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
  if (infoLogLength > 0) {
    printf("%s\n", infoLog);
  }

  delete[] infoLog;
}

bool shader_init(shader_s *shader_s, const char *vertexFilename,
                 const char *fragmentFilename) {
  GLint ok = GL_FALSE;
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

  {
    void *vertexFile = SDL_LoadFile(vertexFilename, NULL);
    if (vertexFile == NULL) {
      printf("failed to read vertex shader file\n");
      return false;
    }
    {
      glShaderSource(vertexShader, 1, (const char *const *)&vertexFile, NULL);
      glCompileShader(vertexShader);

      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ok);
      if (ok != GL_TRUE) {
        printf("compile vertex shader failed: %d\n", vertexShader);
        printShaderLog(vertexShader);

        // error defer
        glDeleteShader(vertexShader);

        // defer
        SDL_free(vertexFile);

        return false;
      }
    }

    SDL_free(vertexFile); // defer
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  {
    void *fragmentFile = SDL_LoadFile(fragmentFilename, NULL);
    {
      glShaderSource(fragmentShader, 1, (const char *const *)&fragmentFile,
                     NULL);
      glCompileShader(fragmentShader);

      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ok);
      if (ok != GL_TRUE) {
        printf("%s:%d: compile fragment shader failed %s: %d\n", __FILE__,
               __LINE__, fragmentFilename, fragmentShader);
        printShaderLog(fragmentShader);

        // error defer
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // defer
        SDL_free(fragmentFile);

        return false;
      }
    }

    SDL_free(fragmentFile); // defer
  }

  GLuint program = glCreateProgram();

  {
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (ok != GL_TRUE) {
      printf("link shader program failed: %d\n", program);
      printProgramLog(program);

      // error defer
      glDeleteProgram(program);
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      return false;
    }
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  shader_s->program = program;

  return true;
}

void shader_clean(shader_s *shader_s) { glDeleteProgram(shader_s->program); }

void shader_use(shader_s *shader) { glUseProgram(shader->program); }

void shader_4f(shader_s *shader_s, const char *name, float x, float y, float z,
               float w) {
  GLint location = glGetUniformLocation(shader_s->program, name);
  glUniform4f(location, x, y, z, w);
}

void shader_3f(shader_s *shader_s, const char *name, float x, float y,
               float z) {
  GLint location = glGetUniformLocation(shader_s->program, name);
  glUniform3f(location, x, y, z);
}

void shader_1i(shader_s *shader_s, const char *name, int x) {
  GLint location = glGetUniformLocation(shader_s->program, name);
  glUniform1i(location, x);
}

void shader_1f(shader_s *shader_s, const char *name, float x) {
  GLint location = glGetUniformLocation(shader_s->program, name);
  glUniform1f(location, x);
}

void shader_mat4fv(shader_s *shader_s, const char *name, const float *matrix) {
  GLint location = glGetUniformLocation(shader_s->program, name);
  glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
}

#endif