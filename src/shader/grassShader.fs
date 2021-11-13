#version 450
#extension GL_ARB_shading_language_include : require

#include</src/shader/inc/classicnoise2D.glsl>
#include</src/shader/inc/classicnoise3D.glsl>
#include</src/shader/inc/shared.glsl>
#include</src/shader/inc/worley.glsl>


in vec3 fragPosition;
in vec3 fragNormal;
in float fragAlong; // Surface coordinate along the stem.
in float fragAround; // Surface coordinate around the stem.
flat in float stemIdxFrag; // We know which stem the fragment belongs to!
uniform vec3 eyePosition;

out vec4 outColor;

float cap(const in float x) {
    return -abs(fma(x, 2.0f, -1.0f)) + 1.0f;
}

void main(void) {
    const vec4 ambient = vec4(0.1f, 0.3f, 0.05f, 1.0f);
    const vec4 diffuseColor = vec4(0.3f, 0.7f, 0.1f, 1.0f);
    const vec4 specularColor = vec4(0.001f, 0.001f, 0.001f, 1.0f);

    // Phong lighting.
    const float specPow = 32.0f;
    const vec3 lightPosition = vec3(0.0f, 0.5f, 0.0f);
    vec3 L = normalize(lightPosition - fragPosition);
    vec3 E = normalize(eyePosition - fragPosition);
    vec3 R = normalize(reflect(-L, fragNormal));
    float diffuseCoeff = max(dot(fragNormal, L), 0.0f);
    float specularCoeff = pow(max(dot(R, E), 0.0f), specPow);
    vec4 diffuse = clamp(diffuseColor * diffuseCoeff, 0.0f, 1.0f) * (fragAlong * 0.5f + 0.5f);
    vec4 specular = clamp(specularColor * specularCoeff, 0.0f, 1.0f);

    // Cellular texture.
    float cell = cellular2x2(vec2(fma(sfn, 100, rand(stemIdxFrag) + fragAlong * 3.0f),
        cap(fragAround)) * 10.0f).x * 0.3f;

    outColor = ambient + diffuse + specular + vec4(0.0f, cell, 0.0f, 0.0f);
    outColor.a = 1.0;
}