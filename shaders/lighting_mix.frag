#version 330 core

#define N_LIGHTS 11

struct Material {
    vec3 ambient;
    sampler2D diffuseMap;
    vec3 specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float exponent;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 frag_color;

uniform DirectionalLight dirLight;
uniform PointLight pointLights[N_LIGHTS];

uniform Material material;
uniform vec3 viewPos;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, normal, viewDir);

    for(int i = 0; i < N_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
    }

    frag_color = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    // Ambient
    vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoord));

    // Diffuse
    vec3 lightDir = normalize(-light.direction);
    float NDotL = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL;

    // Specular (Blinn-Phong)
    vec3 halfDir = normalize(viewDir + lightDir);
    float NDotH = max(dot(halfDir, normal), 0.0f);
    vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Ambient
    vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoord));

    // Diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float NDotL = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL;

    // Specular (Blinn-Phong)
    vec3 halfDir = normalize(viewDir + lightDir);
    float NDotH = max(dot(halfDir, normal), 0.0f);
    vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

    // Attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d*d));

    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}