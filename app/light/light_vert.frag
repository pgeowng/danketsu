#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 ourColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
    FragColor = vec4(ourColor, 1.0);
}