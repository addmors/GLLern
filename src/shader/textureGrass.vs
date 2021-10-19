#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 offset;
layout(location = 3) in vec3 normal;


out vec2 TexCoords;
out float alphaScale;

uniform float time;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform sampler1D urandom01;
uniform vec3 up;
uniform vec3 right;

float rand(const in float seed) {
    return texture(urandom01, seed).r;
}


void main()
{
    

    TexCoords = aTexCoords;
    alphaScale = 16/ ( 1 + distance(cameraPos, offset));
    vec3 B = cross(normal, right);
    gl_Position = projection *view*vec4(aPos+offset, 1.0);
    
    if ( aPos.y > 0.0 )
    {   
        float t =  time+texture(urandom01, mod((offset.x-offset.z)/(offset.z+offset.z),1.0)).r*100;
        float angle1 = t + 0.5077 * aPos.x - 0.421 * aPos.z;
        float angle2 = 1.31415 * t + 0.3 * aPos.y - 0.6 * aPos.z;
        gl_Position += 0.05*vec4 ( sin ( angle1 ), cos ( angle2 ), 0, 0 );
    }
}