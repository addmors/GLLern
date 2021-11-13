#version 450 core
layout (location = 0) in vec2 position;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec4 ClipSpace;
out vec2 texCoord;
out vec3 toCameraVector;
uniform mat4 model;

uniform vec3 cameraPosition;
const float tiling = 20.0;
void main()
{
    vec4 worldPosition = model*vec4(position.x, 0.0, position.y, 1.0);
    ClipSpace = projection* view * worldPosition;
    gl_Position = ClipSpace; 
    texCoord = vec2(position.x/2.0+0.5 ,position.y/2.0+0.5)*tiling;
    toCameraVector = cameraPosition - worldPosition.xyz;
}  