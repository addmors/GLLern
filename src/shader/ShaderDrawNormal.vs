#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 instanceMatrix;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VS_OUT {
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
} vs_out;

void main()
{
    gl_Position = projection * view * instanceMatrix * vec4(position, 1.0); 
    mat3 normalMatrix = mat3(transpose(inverse(view * instanceMatrix)));
    vs_out.normal = normalize(vec3(projection * vec4(normalMatrix * aNormal, 0.0)));
    vs_out.tangent = normalize(vec3(projection * vec4(normalMatrix * aTangent, 0.0)));
    vs_out.bitangent = normalize(vec3(projection * vec4(normalMatrix * cross(aNormal,aTangent), 0.0)));

}