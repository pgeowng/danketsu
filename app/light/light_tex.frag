#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

void main() {
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diffuse_magnitude = max(dot(norm, lightDir), 0.0);

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specular_magnitude = pow(max(dot(viewDir, reflectDir), 0.0), max(material.shininess, 0.1));
    // float specularStrength = clamp(0.5 * diff, 0.0, 1.0);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diffuse_magnitude * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular *  specular_magnitude * vec3(texture(material.specular, TexCoords));

    vec3 result = diffuse + ambient + specular;
    FragColor = vec4(result, 1.0);
}