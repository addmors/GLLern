#version 330 core
out vec4 color;
in vec2 TexCoord;
in vec3 FColor;
void main()
{

    color = vec4(FColor, 1.0f); // Устанавливает все 4 компоненты вектора равными 1.0f
}