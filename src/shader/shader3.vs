#version 420 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 aNormal;
layout (location = 1) in vec2 texCoord;
in vec4 s_vWeights;
in ivec4 s_vIDs;


out vec2 TexCoord;
out vec3 FragPos; 
out vec3 Normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];
out vec4 we;

void main()
{
	mat4  BMatrix = gBones[s_vIDs[0]] * s_vWeights[0];
	for (int i =  1; i < 4; i++){
	BMatrix += gBones[s_vIDs[i]] * s_vWeights[i];
	}
	we = vec4(s_vWeights[0] ,s_vWeights[1], s_vWeights[2], s_vWeights[3]);	  
	Normal = mat3(transpose(inverse(view*model*BMatrix))) * aNormal;
    TexCoord = vec2(texCoord.x, texCoord.y); 
    gl_Position = projection * view * model * BMatrix*vec4(position, 1.0f);
	FragPos = vec3(view* model  * BMatrix * vec4(position, 1.0f));
}