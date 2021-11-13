#version 450 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
	sampler2D texture_normal1;
	sampler2D texture_opacity1;
    float shininess;
};
uniform Material material;

void main()
{    
    if(texture(material.texture_diffuse1, TexCoord).a<0.1) discard;
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(TBN*(texture(material.texture_normal1, TexCoord).rgb*2.0-1.0));
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = (texture(material.texture_diffuse1, TexCoord).rgb);
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(material.texture_specular1, TexCoord).r;
}
