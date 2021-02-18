#version  core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 we;
out mat4 G;
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 
uniform Material material;

uniform sampler2D texture_diffuse1;

void main()
{   
	vec4 texcolor = texture2D(material.texture_diffuse1,TexCoords);
    FragColor = texcolor;
}