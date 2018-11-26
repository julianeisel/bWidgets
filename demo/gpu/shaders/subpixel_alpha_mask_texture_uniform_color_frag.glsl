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
	float ofs_fac = subpixel_offset / 0.333;
	bool is_first_texel = (texCoord_interp.x * textureSize(glyph, 0).x) < 1.0;

	if (subpixel_offset <= 0.333) {
		alpha_mask.r = mix(alpha_mask.r, is_first_texel ? 0.0 : alpha_mask_prev.b, ofs_fac);
		alpha_mask.g = mix(alpha_mask.g, alpha_mask.r, ofs_fac);
		alpha_mask.b = mix(alpha_mask.b, alpha_mask.g, ofs_fac);
	}
	else if (subpixel_offset <= 0.666) {
		ofs_fac -= 1;
		alpha_mask.r = is_first_texel ? 0.0 : mix(alpha_mask_prev.b, alpha_mask_prev.g, ofs_fac);
		alpha_mask.g = mix(alpha_mask.r, is_first_texel ? 0.0 : alpha_mask_prev.b, ofs_fac);
		alpha_mask.b = mix(alpha_mask.g, alpha_mask.r, ofs_fac);
	}
	else if (subpixel_offset < 1.0) {
		ofs_fac -= 2;
		alpha_mask.r = is_first_texel ? 0.0 : mix(alpha_mask_prev.g, alpha_mask_prev.r, ofs_fac);
		alpha_mask.g = is_first_texel ? 0.0 : mix(alpha_mask_prev.b, alpha_mask_prev.g, ofs_fac);
		alpha_mask.b = mix(alpha_mask.r, is_first_texel ? 0.0 : alpha_mask_prev.b, ofs_fac);
	}

	fragColor = color.a * alpha_mask;
}
