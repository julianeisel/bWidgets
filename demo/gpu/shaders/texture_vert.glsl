#version 330 core

uniform mat4 ModelViewProjectionMatrix;

uniform vec4 color;

in vec2 pos;
in vec2 texCoord;
noperspective out vec2 texCoord_interp;

void main()
{
	gl_Position = ModelViewProjectionMatrix * vec4(pos, 0.0, 1.0);

	texCoord_interp = texCoord;
}
