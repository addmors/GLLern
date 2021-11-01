#version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    float visibility;
    float visibilityToPlane;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};


uniform mat4 model;

uniform vec4 plane;
uniform mat4 lightSpaceMatrix;


const float density = 0.0035;
const float gradient = 5;

const float densityPlane = 100;
const float gradientPlane = 0.2;


void main()
{
    vec4 worldPosition  = model*vec4(position,1.0);
    
    gl_ClipDistance[0] = dot(worldPosition, plane); 

    vs_out.FragPos = vec3(model * vec4(position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vec4 positionRelativeToCam = view*worldPosition; 
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * positionRelativeToCam;

    float distance = length(positionRelativeToCam.xyz);
    float distanceToPlane = abs(plane.x*worldPosition.x+plane.y*worldPosition.y+plane.z*worldPosition.z + plane.w/length(plane.xyz));
    vs_out.visibility = exp(-pow((distance*density),gradient));
    vs_out.visibilityToPlane = exp(-pow((distanceToPlane*densityPlane),gradientPlane));
    vs_out.visibility = clamp(vs_out.visibility,0.0,1.0);
    
    vs_out.visibilityToPlane = clamp( vs_out.visibilityToPlane,0.0,1.0);
}   