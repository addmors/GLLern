#version 400  core


in vec2 coords0;

out vec4 fColor;

uniform sampler2D texUnit;

void main()
{
     vec4 texcolor = texture2D(texUnit,coords0);
 
     fColor = texcolor;
}