#version 330 core
out vec4 FragColor;


struct Material {


   sampler2D diffuse;
   sampler2D specular;
   float shininess;

};

struct Light{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


uniform vec3 viewPosition;
uniform Light light;
uniform Material material;

void main(){
    //ambijentalno:
    //boja * koliko objekat reflektuje boju
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    //difuzno:
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    //difuzna komp. svetlosti * jacina difuzne boje u tom fragmentu * isto ko gore
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;


    //spekularno:
    vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 reflDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflDir),0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    vec3 result =  ambient + diffuse + specular;


    FragColor = vec4(result, 1.0);


}


