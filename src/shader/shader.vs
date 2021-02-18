#version 420 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;
layout (location = 1) in vec3 normal;
out vec2 TexCoord;

out vec3 FragPos; 
out vec3 Normal;

uniform vec3 lightPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoord = vec2(texCoord.x, texCoord.y);
	Normal = mat3(transpose(inverse(view*model))) * normal;
	FragPos = vec3(view* model * vec4(position, 1.0f));
} 