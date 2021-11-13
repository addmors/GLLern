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

out vec2 TexCoord;
out vec3 FragPos; 
out vec3 Normal;
out mat3 TBN;


uniform vec3 lightPos;
uniform vec4 plane;


void main()
{
	vec4 worldPosition  = instanceMatrix*vec4(position,1.0);
    
    gl_ClipDistance[0] = dot(worldPosition, plane); 

	vec3 T = normalize(mat3(transpose(inverse(instanceMatrix))) * aTangent);	
    vec3 N = normalize(mat3(transpose(inverse(instanceMatrix))) * aNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	TBN = mat3(T, B, N); 
	Normal = normalize( mat3(transpose(inverse(instanceMatrix))) * aNormal);
    TexCoord = vec2(texCoord.x, texCoord.y); 
    gl_Position = projection * view * worldPosition;
	FragPos = worldPosition.xyz;	
}