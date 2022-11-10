#version 330 core

in vec2 texCoord;
in vec4 vertexColor;
out vec4 FragColor;

uniform vec4 ourColor;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
    FragColor = mix(texture(tex1, texCoord), texture(tex2, vec2(1.0f - texCoord.x, texCoord.y)), 0.2);
}