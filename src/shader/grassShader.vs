#version 450 core

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;

out vec3 Normal_CS_in;
void main(void) {

    gl_Position = position;
	Normal_CS_in = normalize(normal);

}