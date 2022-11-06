#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform vec3 ourPosition;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aPos.xyz + ourPosition.xyz, 1.0);
    vertexColor = vec4((aPos.xyz + ourPosition.xyz + vec3(1.0f, 1.0f, 0.0f))/2.0f + vec3(0, 0, aPos.x + aPos.y + ourPosition.x + ourPosition.y), 1.0);
    // vertexColor = vec4(aColor, 1.0);
}