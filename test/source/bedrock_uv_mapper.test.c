#include <stdint.h>
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
#include "log/log.h"

#include <assert.h>
#include <graphics/model/bedrock_uv_mapper.h>

// clang-format off
const uint16_t head_uvs[48] = {
	// Front face (8,8) - (16,16)
	8, 8, 16, 8, 16, 16, 8, 16,

	// Back face (24,8) - (32,16)
	24, 8, 32, 8, 32, 16, 24, 16,

	// Left face (0,8) - (8,16)
	0, 8, 8, 8, 8, 16, 0, 16,

	// Right face (16,8) - (24,16)
	16, 8, 24, 8, 24, 16, 16, 16,

	// Top face (8,0) - (16,8)
	8, 0, 16, 0, 16, 8, 8, 8,

	// Bottom face (16,0) - (24,8)
	16, 8, 24, 8, 24, 0, 16, 0,
};
// clang-format on

int main() {
	uint16_t uv_map[48];

	bedrock_calculate_cube_uv_map((uint16_t[]) {8, 8, 8}, // Cube size
								  (uint16_t[]) {0, 0},	  // Texture region
								  uv_map, // Output UV coordinates
								  false	  // No mirroring
	);

	for(int i = 0; i < 48; i += 2) {
		log_debug("(%u, %u) == (%u, %u)", uv_map[i], uv_map[i + 1], head_uvs[i],
				  head_uvs[i + 1]);
		assert(uv_map[i] == head_uvs[i] && uv_map[i + 1] == head_uvs[i + 1]);
	}
}