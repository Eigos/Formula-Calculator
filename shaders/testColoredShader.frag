#version 330 core
out vec4 FragColor;

in vec4 aCol;

void main()
{

	FragColor = normalize(abs(aCol));
}