#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
    float ambientStrength = 0.1;
    float ambient = ambientStrength;


    vec3 lightDir = normalize(lightPos - FragPos);
    float lightAngle = dot(normalize(Normal), lightDir);
    float specular = clamp(lightAngle, 0, 1);
    // vec3 specular = vec3(1.0) ;


    // vec3 result = max(specular, ambient) * lightColor * objectColor; // iteration 1: allow ambient light minimum
    vec3 result = (specular + ambient) * lightColor * objectColor; // iteration 2: allow always above ambient light
    FragColor = vec4(result, 1.0);
}