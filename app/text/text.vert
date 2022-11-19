#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;

out vec2 FragPos;
out vec2 TexCoords;

void main() {
    gl_Position = model * vec4(aPos, 0.0f, 1.0f);
    FragPos = vec2(model * vec4(aPos, 0.0f, 1.0f));
    TexCoords = vec2(model * vec4(aTexCoords, 0.0f, 1.0f));
}