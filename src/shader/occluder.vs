#version 420
layout (location = 0) in vec3 inPosition;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

void main()
{
	vec4 vEyeSpacePosVertex = view*model*vec4(inPosition, 1.0);
	gl_Position = projection*vEyeSpacePosVertex;
}