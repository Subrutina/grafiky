#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
     // obtain normal from normal map in range [0,1]
    vec4 normal = texture(normalMap, fs_in.TexCoords);
    if(normal.a < 0.1){
        discard;
    }
    // transform normal vector to range [-1,1]

    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    // get diffuse color
    vec4 color = texture(diffuseMap, fs_in.TexCoords);
    if(color.a < 0.1){
        discard;
    }
    // ambient
    vec3 ambient = 0.1 * color.rgb;
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal.rgb), 0.0);
    vec3 diffuse = diff * color.rgb;
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal.rgb);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal.rgb, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    vec3 res = ambient + diffuse + specular;
    FragColor = vec4(res, 1.0);
}