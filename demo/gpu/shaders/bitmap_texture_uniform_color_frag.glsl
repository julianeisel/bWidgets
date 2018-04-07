/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Original work Copyright (c) 2017, 2018 Julian Eisel, Mike Erwin
 *
 * ***** END GPL LICENSE BLOCK *****
 */

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
