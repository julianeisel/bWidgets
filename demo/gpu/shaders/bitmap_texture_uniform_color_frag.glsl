#version 330 core

uniform vec4 color;

noperspective in vec2 texCoord_interp;
out vec4 fragColor;

uniform sampler2D glyph;

void main()
{
	fragColor.rgb = color.rgb;
	fragColor.a = color.a * texture(glyph, texCoord_interp).r;
}
