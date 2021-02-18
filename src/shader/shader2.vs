#version 400

uniform mat4 MMatrix;
uniform mat4 VPMatrix;

in vec3 s_vPosition;
in vec2 s_vCoords0;

in vec3 s_vNormals;
in vec3 s_vTangents;

out vec2 coords0;

const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];

in vec4 s_vWeights;
in ivec4 s_vIDs;

void main ()
{
     mat4 BMatrix = gBones[s_vIDs[0]] * s_vWeights[0];
          BMatrix += gBones[s_vIDs[1]] * s_vWeights[1];
          BMatrix += gBones[s_vIDs[2]] * s_vWeights[2];
          BMatrix += gBones[s_vIDs[3]] * s_vWeights[3];
 
     gl_Position = VPMatrix * MMatrix * BMatrix * (vec4(s_vPosition,1.0));
 
     coords0 = s_vCoords0.xy;
}