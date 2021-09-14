#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {

    vec3 direction;

    vec3 position;
    float cutOff;
    float outerCutOff;


    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);


    float theta = dot(lightDir, normalize(-light.direction));
    //ako je ugao manji - cosinus je veci i zato oduzimamo od manjeg ugla veci
    float epsilon = (light.cutOff - light.outerCutOff);
    //ovde svasta moze da se dobije pa se koristi clamp
    float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);


    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //ambijentalna komponenta ne utice na svetlo jer dolazi iz svih pravaca
    diffuse   *= attenuation * intensity;
    specular *= attenuation * intensity;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

}