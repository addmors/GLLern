#version 330 core
out vec4 FragColor;
  
in vec4 ClipSpace;
in vec2 texCoord;
in vec3 toCameraVector;
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform float moveFactor;

const float waveStrength = 0.02;
void main()
{ 
    vec2 ndc = (ClipSpace.xy/ClipSpace.w)/2+0.5;
    vec2 refractionTextureCoords = ndc+vec2(waveStrength*1.25,waveStrength*1.25);
    vec2 reflectionTextureCoords = vec2(ndc.x, -ndc.y)+vec2(waveStrength*1.25,waveStrength*1.25);

    vec2 distortion1 = (texture(dudvMap,vec2(texCoord.x+moveFactor,texCoord.y)).rg*2.0-1)*waveStrength;
    vec2 distortion2 = (texture(dudvMap,vec2(-texCoord.x+moveFactor,texCoord.y)).rg*2.0-1)*waveStrength;
    vec2 totalDistortion = distortion1+distortion2;

    reflectionTextureCoords += totalDistortion;
    reflectionTextureCoords.x = clamp(reflectionTextureCoords.x, 0.001, 0.999);
    reflectionTextureCoords.y = clamp(reflectionTextureCoords.y, -0.999, -0.001);

    refractionTextureCoords += totalDistortion;
    refractionTextureCoords = clamp(refractionTextureCoords, 0.001, 0.999);


    vec4 reflectionColor = texture(reflectionTexture, reflectionTextureCoords);
    vec4 refractionColor = texture(refractionTexture, refractionTextureCoords);

    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot(viewVector, vec3(0.0,1.0,0.0));
    //refractiveFactor = pow(refractiveFactor,2);

    FragColor =mix(reflectionColor,refractionColor,refractiveFactor);
    //FragColor = mix(FragColor,vec4(0.0,0.3,0.5,1.0),0.2);
}