#version 330 core
out vec4 FragColor;

in vec2 FragPos;
in vec2 TexCoords;
uniform sampler2D tex;

void main() {
    FragColor = vec4(texture(tex, TexCoords));
    // FragColor = vec4(1.0f);
}
