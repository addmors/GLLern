#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    float visibility;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;


const float density = 0.0035;
const float gradient = 5;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vec4 positionRelativeToCam = view*model*vec4(aPos, 1.0); 
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * positionRelativeToCam;

    float distance = length(positionRelativeToCam.xyz);
    vs_out.visibility = exp(-pow((distance*density),gradient));
    vs_out.visibility = clamp(vs_out.visibility,0.0,1.0);
}   