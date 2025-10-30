#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform bool u_useTexture;
uniform vec3 u_objectColor;
uniform vec3 u_lightColor;

void main()
{
    if(u_useTexture)
    {
        FragColor = texture(texture1, TexCoord);
    }
    else
    {
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * u_lightColor;
        FragColor = vec4(ambient * u_objectColor, 1.0);
    }
}