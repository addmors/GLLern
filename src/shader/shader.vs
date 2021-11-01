#version 420 core
#extension GL_ARB_shading_language_include : require


layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;
layout (location = 1) in vec3 normal;
out vec2 TexCoord;

out vec3 FragPos; 
out vec3 Normal;
out vec3 FColor;

uniform vec3 lightPos;
uniform vec3 color;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoord = vec2(texCoord.x, texCoord.y);
	Normal = mat3(transpose(inverse(view*model))) * normal;
	FragPos = vec3(view* model * vec4(position, 1.0f));
	FColor = color;
} 