#version 420 core

layout (location = 0) in vec3 position;

layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec4 s_vWeights;
layout (location = 6) in ivec4 s_vIDs;


out vec2 TexCoord;
out vec3 FragPos; 
out vec3 Normal;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec4 plane;

const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];
out vec4 we;

void main()
{

	mat4  BMatrix = gBones[s_vIDs[0]] * s_vWeights[0];
	for (int i =  1; i < 4; i++){
	BMatrix += gBones[s_vIDs[i]] * s_vWeights[i];
	}
	vec4 worldPosition  = model*BMatrix*vec4(position,1.0);
    
    gl_ClipDistance[0] = dot(worldPosition, plane); 
    
	vec3 T = normalize(mat3(transpose(inverse(view*model*BMatrix))) * aTangent);	
    vec3 N = normalize(mat3(transpose(inverse(view*model*BMatrix))) * aNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	TBN = mat3(T, B, N);
	
	we = vec4(s_vWeights[0] ,s_vWeights[1], s_vWeights[2], s_vWeights[3]);	  
	Normal = normalize( mat3(transpose(inverse(view*model*BMatrix))) * aNormal);
    TexCoord = vec2(texCoord.x, texCoord.y); 
    gl_Position = projection * view *worldPosition;
	FragPos = vec3(view * worldPosition);
}