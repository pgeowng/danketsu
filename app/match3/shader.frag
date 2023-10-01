#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

void main() {
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diffuse_magnitude = max(dot(norm, lightDir), 0.0);

    // // specular
    // vec3 viewDir = normalize(-FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);
    // float specular_magnitude = pow(max(dot(viewDir, reflectDir), 0.0), max(material.shininess, 0.1));
    // // float specularStrength = clamp(0.5 * diff, 0.0, 1.0);

    // vec3 ambient = light.ambient * material.ambient;
    // vec3 diffuse = light.diffuse * material.diffuse * diffuse_magnitude;
    // vec3 specular = light.specular * material.specular  * specular_magnitude;

    // vec3 result = diffuse + ambient + specular;
    // FragColor = vec4(result, 1.0);

    // vec3 result = material.diffuse;//material.ambient + material.diffuse; //* diffuse_magnitude;
    // vec3 result = normalize(light.position - FragPos);
    vec3 result = material.diffuse * diffuse_magnitude * 2;
    // vec3 result = material.ambient +  material.diffuse * diffuse_magnitude;


    FragColor = vec4(result,1.0);
}