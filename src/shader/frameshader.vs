#version 420 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
uniform mat4 model;
void main()
{
    TexCoords = aTexCoords;
    gl_Position = model*vec4(position, 0.0, 1.0);
}