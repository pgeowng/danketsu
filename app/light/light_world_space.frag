#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
    float ambientStrength = 0.5;
    float ambient = ambientStrength;


    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float lightAngle = dot(norm, lightDir);
    float diffuse = clamp(lightAngle, 0.1, 1);
    // vec3 diffuse = vec3(1.0) ;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    float specularStrength = 0.5;
    vec3 specular =  specularStrength * spec * lightColor;

    // vec3 result = max(diffuse, ambient) * lightColor * objectColor; // iteration 1: allow ambient light minimum
    vec3 result = (diffuse + ambient + specular) * lightColor * objectColor; // iteration 2: allow always above ambient light
    FragColor = vec4(result, 1.0);
}