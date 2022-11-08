#version 330 core

in vec2 texCoord;
in vec4 vertexColor;
out vec4 FragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
    // FragColor = mix(texture(tex1, texCoord), texture(tex2, texCoord), 0.5) * vertexColor;
    FragColor = mix(texture(tex1, texCoord), texture(tex2, texCoord), 0.2) ;
    // FragColor = texture(tex1, texCoord);
}