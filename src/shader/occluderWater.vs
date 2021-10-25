#version 420
layout (location = 0) in vec2 inPosition;
 
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
	vec4 vEyeSpacePosVertex = view*model*vec4(inPosition.x,0.0, inPosition.y, 1.0);
	gl_Position = projection*vEyeSpacePosVertex;
}