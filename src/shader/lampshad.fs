#version 330 core
out vec4 color;
in vec2 TexCoord;
in vec3 FColor;
void main()
{

    color = vec4(FColor, 1.0f); // ������������� ��� 4 ���������� ������� ������� 1.0f
}