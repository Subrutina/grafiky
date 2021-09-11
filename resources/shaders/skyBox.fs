#version 330 core
out vec4 FragColor;

//mesto gde direkcioni vektor prolazi kroz teksturu je teksel koji uzimamo za semplovanje
in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoords);
}