#version 420 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float epsilon;
void main()
{ 
    const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;


    vec3 mapped = vec3(1.0) - exp(-hdrColor * epsilon);
    // гамма-коррекция
    mapped = pow(mapped, vec3(1.0 / gamma));


    FragColor = vec4(mapped, 1.0);
}