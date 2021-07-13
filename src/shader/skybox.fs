#version 420 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox1;
uniform samplerCube skybox2;

uniform vec3 fogColor;
uniform float blendFactor; 

const float lowerLimit = -1.0;
const float upperLimit = 7.0;

void main()
{    
    vec4 texture1 = texture(skybox1, TexCoords);
    vec4 texture2 = texture(skybox2, TexCoords);
    vec4 finalColor = mix(texture1,texture2, blendFactor);    
    float factor = (TexCoords.y - lowerLimit)/(upperLimit - lowerLimit);
    factor = clamp(factor,0.0,1.0);
    FragColor = mix(vec4(fogColor,1.0),finalColor, factor);
}