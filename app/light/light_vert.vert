#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

out vec3 FragPos;
out vec3 Normal;
out vec3 ourColor;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    FragPos = vec3(view * model * vec4(aPos, 1.0));
    Normal = vec3(transpose(inverse(view)) * transpose(inverse(model)) * vec4(aNormal, 0.0));

    float ambientStrength = 0.5;
    float ambient = ambientStrength;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - aPos);
    float lightAngle = dot(norm, lightDir);
    float diffuse = clamp(lightAngle, 0.1, 1.0);

    vec3 viewDir = normalize(viewPos - aPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    float specularStrength = clamp(0.5 * lightAngle, 0.0, 1.0);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specularStrength * spec) * lightColor * objectColor;

    ourColor = result;
}