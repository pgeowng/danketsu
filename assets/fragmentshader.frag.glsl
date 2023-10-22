#version 330 core

in vec4 v_color;
in vec2 v_uv;
in float v_tex_index;

layout (location = 0) out vec4 f_color;

uniform sampler2D u_tex[8];

void main() {
  vec4 a_color = v_color;
  switch (int(v_tex_index)) {
    case 0: {a_color = v_color * texture(u_tex[0], v_uv); break;}
    case 1: {a_color = v_color * texture(u_tex[1], v_uv); break;}
    case 2: {a_color = v_color * texture(u_tex[2], v_uv); break;}
    case 3: {a_color = v_color * texture(u_tex[3], v_uv); break;}
    case 4: {a_color = v_color * texture(u_tex[4], v_uv); break;}
    case 5: {a_color = v_color * texture(u_tex[5], v_uv); break;}
    case 6: {a_color = v_color * texture(u_tex[6], v_uv); break;}
    case 7: {a_color = v_color * texture(u_tex[7], v_uv); break;}
    default: discard;
  }


  if (a_color.a < 0.05) discard;
  f_color = a_color;
}