#version 330 core

#define N_LIGHTS 9

struct Material {
    vec3 ambient;
    sampler2D diffuseMap;
    vec3 specular;
    float shininess;
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

uniform PointLight light[N_LIGHTS];
uniform Material material;
uniform vec3 viewPos;

void main()
{
    vec3 result = vec3(0.0);
    for(int i = 0; i < N_LIGHTS; i++) {
        //Ambient
        vec3 ambient = light[i].ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoord));

        //Diffuse
        vec3 normal = normalize(Normal);
        vec3 lightDir = normalize(light[i].position - FragPos);
        float NDotL = max(dot(normal, lightDir), 0.0f);
        vec3 diffuse = light[i].diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL;

        //Specular (Blinn-Phong)
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 halfDir = normalize(viewDir + lightDir);
        float NDotH = max(dot(halfDir, normal), 0.0f);
        vec3 specular = light[i].specular * material.specular * pow(NDotH, material.shininess);

        float d = length(light[i].position - FragPos);
        float attenuation = 1.0f / (light[i].constant + light[i].linear * d + light[i].exponent * (d*d));

        diffuse *= attenuation;
        specular *= attenuation;

        result += (ambient + diffuse + specular);
    }

    frag_color = vec4(result, 1.0f);
};