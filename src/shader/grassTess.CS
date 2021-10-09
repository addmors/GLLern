#version 420

layout(vertices = 1) out;

in vec3 Normal_CS_in[];
out vec3 Normal_ES_in[];


uniform vec3 eyePosition;
uniform vec3 lookDirection;

int lod()
{
    float dist = distance(gl_in[gl_InvocationID].gl_Position.xyz, eyePosition);
    if (dist < 10.0f)
    {
        return 48;
    }
    if (dist < 20.0f)
    {
        return 24;
    }
    if (dist < 40.0f)
    {
        return 12;
    }
    if (dist < 80.0f)
    {
        return 6;
    }
    return 4;
}

/*
TODO: replace half-space culling with frustum culling?
*/

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    gl_TessLevelOuter[0] = gl_in[gl_InvocationID].gl_Position.w;
    float halfspaceCull =
        step(dot(eyePosition - gl_in[gl_InvocationID].gl_Position.xyz, lookDirection), 0);
    gl_TessLevelOuter[1] = lod() * halfspaceCull;
    Normal_ES_in[gl_InvocationID] = Normal_CS_in[gl_InvocationID];
}