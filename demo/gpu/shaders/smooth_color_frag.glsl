#version 330 core

noperspective in vec4 finalColor;
out vec4 fragColor;

void main()
{
	fragColor = finalColor;
}
