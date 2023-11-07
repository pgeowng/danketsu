#ifndef RENDERER_3D_H
#define RENDERER_3D_H

typedef struct Renderer3D {
  Camera camera ={};
  shader_s shader = {};

} Renderer3D;

static b8 r3Init(Renderer3D * r3d) {
  cameraInit(&r3d->camera, 45.0f, 1.0f, 0.1f, 100.0f, 1);

  b8 ok = shader_init(&r3d->shader, "./app/hello/hello.vert",
                   "./app/hello/hello.frag");
  if (!ok) {
    printf("shader new failed");
    return ok;
  }

  shader_use(&r3d->shader);

  return 1;
}

static void r3DrawMesh(Renderer3D *r3, mesh_s *m, m4 transform) {

  shader_use(&r3->shader);
  shader_1i(&r3->shader, "tex1", 0);
  shader_1i(&r3->shader, "tex2", 1);
  shader_mat4fv(&r3->shader, "model", transform);
  shader_mat4fv(&r3->shader, "view", glm::value_ptr(camViewMat(&r3->camera)));
  // shader_SetMatrix4fv(&r3->shader, "view",
  // glm::value_ptr(flycamera_get_weird_view_matrix(&r3->camera)));
  shader_mat4fv(&r3->shader, "projection",
                glm::value_ptr(camProjMat(&r3->camera)));

  int diffuse_nr = 1;
  int specular_nr = 1;

  for (int i = 0; i < m->textures_size; i++) {

    glActiveTexture(GL_TEXTURE0 + i);

    int slot = 0;
    if (strcmp(m->textures[i].type, "material.diffuse") == 0) {
      slot = diffuse_nr++;
    } else if (strcmp(m->textures[i].type, "material.specular") == 0) {
      slot = specular_nr++;
    }

    char name[50];
    mysprintf(name, "%s%d", m->textures[i].type, slot);
    shader_1i(&r3->shader, name, i);
    glBindTexture(GL_TEXTURE_2D, m->textures[i].id);
  }

  glBindVertexArray(m->vao);
  // printf("MeshDraw: indices_size: %d\n", m->indices_size);
  // printf("MeshDraw: verts_size: %d\n", m->verts_size);
  assert(m->verts_size != 0);
  if (m->indices_size > 0) {
    glDrawElements(GL_TRIANGLES, m->indices_size, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, m->verts_size);
  }
  glBindVertexArray(0);

  glActiveTexture(GL_TEXTURE0);
}

static void r3Update(Renderer3D *r3, f32 delta) {
  flycamera_update(&r3->camera, delta);
}

#endif