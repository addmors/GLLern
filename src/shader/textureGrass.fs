#version 420 core
out vec4 FragColor;

in vec2 TexCoords;
in float alphaScale;
uniform sampler2D grass;

void main()
{             
    vec4 texColor = texture(grass, TexCoords);
    if(texColor.a < 0.7 ||alphaScale < 0.1)
        discard;

    FragColor = texColor;
}