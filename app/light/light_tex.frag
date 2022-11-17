#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
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

in vec4 gl_FragCoord;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform float time;

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


    vec3 specular = light.specular * specular_magnitude * vec3(texture(material.specular, TexCoords));

    // ambient = vec3(0.0f);
    // diffuse = vec3(0.0f);
    // specular = light.specular;
    // specular = specular * gl_FragCoord.xyz / gl_FragCoord.w ;

    /* invert specular light */
    vec3 specularMap = (vec3(1.0-texture(material.specular, TexCoords)) * 4.0f - 3.0f);
    specularMap = clamp(specularMap, 0.0, 1.0);
    float specularGray = (specularMap.r + specularMap.g + specularMap.b) / 3.0f;
    // vec3 specular = light.specular * specular_magnitude * (specularGray * vec3(texture(material.diffuse, TexCoords))) ;
    vec3 emission = specularGray * vec3(texture(material.emission, TexCoords + vec2(0.0, time/4.0f)));



    vec3 result = diffuse +  ambient + specular + emission;
    // vec3 result = vec3(max(diffuse.x, max(ambient.x, specular.x)), max(diffuse.y, max(ambient.y, specular.y)), max(diffuse.z, max(ambient.z, specular.z)));
    FragColor = vec4(result, 1.0);
}