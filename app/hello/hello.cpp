#include "hello.h"

bool app_init(Scene *app) {
  b8 ok = r3Init(&app->r3);
  if (!ok) {
    printf("init renderer failed");
    return ok;
  }

  MeshZero(&app->cube_mesh);
  MeshSetCube(&app->cube_mesh);
  MeshInitialize(&app->cube_mesh);


  ok = tex_load(&app->texWall, "assets/wall.jpg");
  if (!ok) {
    return false;
  }
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, app->texWall);

  ok = tex_load(&app->texAwesome, "assets/awesomeface.png");
  if (!ok) {
    return false;
  }
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, app->texAwesome);

  return ok;
}

void app_update(Scene *app, f32 delta) {
  appUpd(app, delta);
  appDraw(app);
}

static f32 timeValue = 0.0f;
static f32 transforms[16 * 5];
static f32 positions[3 * 4];

static void appUpd(Scene*app, f32 delta) {
  r3Update(&app->r3, delta);
  timeValue += delta;

  f32 *ttransform = (f32*)&transforms;
  m4One(ttransform);
  m4Rotate(ttransform, mRadians(20.0f * timeValue), 0.0f, 1.0f, 0.0f);
  m4Rotate(ttransform,mRadians(-55.0f), 1.0f, 0.0f, 0.0f);

  f32 *pposition = (f32*)&positions;
  v3Set(pposition, 0.2f, -0.2f, 1.0f);
  pposition+=3;
  v3Set(pposition,-0.4f, 0.4f, 0.5f),
  pposition+=3;
  v3Set(pposition,0.0f, 0.0f, 1.5f),
  pposition+=3;
  v3Set(pposition,0.89f, 0.5f, 2.0f);

  for (int i = 0; i < 4; i++) {
    ttransform += 16;
    m4One(ttransform);
    m4Translate(ttransform, &positions[i]);
    m4Rotate(ttransform,  mRadians(timeValue * 25 * (i + 1)),
                        positions[i*3],positions[i*3+1],positions[i*3+2]);

  }
}

static void appDraw(Scene *app) {
  Renderer3D *r3 = &app->r3;

  f32 *ttransform = (f32*)&transforms;
  r3DrawMesh(r3, &app->cube_mesh, ttransform);

  for (int i = 0; i < 4; i++) {
    ttransform += 16;
    r3DrawMesh(r3, &app->cube_mesh, transforms);
  }
}