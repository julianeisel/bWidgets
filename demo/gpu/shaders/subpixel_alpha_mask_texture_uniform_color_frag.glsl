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
 * Original work Copyright (c) 2018 Julian Eisel
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#version 330 core

uniform vec4 color;
uniform float subpixel_offset;
uniform sampler2D glyph;

in vec2 texCoord_interp;
out vec4 fragColor;

void main()
{
	vec4 alpha_mask = texture(glyph, texCoord_interp);
	vec4 alpha_mask_prev = textureOffset(glyph, texCoord_interp, ivec2(-1, 0));
	bool is_first_texel = (texCoord_interp.x * textureSize(glyph, 0).x) < 1.0;

	if (subpixel_offset <= 0.333) {
		/* pass */
	}
	else if (subpixel_offset <= 0.666) {
		/* offset by one subpixel */
		alpha_mask.b = alpha_mask.g;
		alpha_mask.g = alpha_mask.r;
		alpha_mask.r = is_first_texel ? 0 : alpha_mask_prev.b;
	}
	else if (subpixel_offset < 1.0) {
		/* offset by two subpixel */
		alpha_mask.b = alpha_mask.r;
		alpha_mask.g = is_first_texel ? 0 : alpha_mask_prev.b;
		alpha_mask.r = is_first_texel ? 0 : alpha_mask_prev.g;
	}

	fragColor = color.a * alpha_mask;
}
