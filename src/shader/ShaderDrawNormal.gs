 #version 420 core
layout (triangles) in;
layout (line_strip, max_vertices = 18) out;

in VS_OUT {
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
} gs_in[];

out vec3 fColor;

const float MAGNITUDE = 0.4;

void GenerateLine(int index)
{   fColor = vec3(1,0,0);
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}
void GenerateLineT(int index)
{
    fColor = vec3(0,1,0);
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].tangent, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}
void GenerateLineB(int index)
{
    fColor = vec3(0,0,1);
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].bitangent, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}


void main()
{
    GenerateLine(0); // вектор нормали для первой вершины
    GenerateLine(1); // ... для второй
    GenerateLine(2); // ... для третьей
    GenerateLineT(0); // вектор нормали для первой вершины
    GenerateLineT(1); // ... для второй
    GenerateLineT(2); // ... для третьей
    GenerateLineB(0); // вектор нормали для первой вершины
    GenerateLineB(1); // ... для второй
    GenerateLineB(2); // ... для третьей

}