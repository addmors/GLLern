#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 offset;


out vec2 TexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 up;
uniform vec3 right;

void main()
{
    
    TexCoords = aTexCoords;
    
    gl_Position = projection *view*vec4(aPos.x*right+aPos.y*up+offset, 1.0);
}