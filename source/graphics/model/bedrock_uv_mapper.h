/*
	Copyright (c) 2025 Lunna5

	This file is part of CavEX.

	CavEX is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	CavEX is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with CavEX.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CAVEX_BEDROCK_UV_MAPPER_H
#define CAVEX_BEDROCK_UV_MAPPER_H
#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint16_t u1, v1; // Top-left UV coordinates
	uint16_t u2, v2; // Bottom-right UV coordinates
} bedrock_uv_face;

typedef struct {
	uint16_t x, y;			// Position in texture atlas
	uint16_t width, height; // Size of the texture atlas region
} bedrock_uv_texture_region;

bedrock_uv_face bedrock_calculate_uv_face(bedrock_uv_texture_region region,
										  uint16_t face_width,
										  uint16_t face_height);

void bedrock_calculate_cube_uv_map(uint16_t cube_size[3],
								   uint16_t texture_region[2],
								   uint16_t* uv_coords, bool mirror);

#endif // CAVEX_BEDROCK_UV_MAPPER_H
