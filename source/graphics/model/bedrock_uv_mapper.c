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

#include "bedrock_uv_mapper.h"
bedrock_uv_face bedrock_calculate_uv_face(bedrock_uv_texture_region region,
										  uint16_t face_width,
										  uint16_t face_height) {
	bedrock_uv_face uv;

	uv.u1 = region.x;
	uv.v1 = region.y;
	uv.u2 = region.x + face_width;
	uv.v2 = region.y + face_height;

	return uv;
}

void bedrock_calculate_cube_uv_map(uint16_t cube_size[3],
								   uint16_t texture_region[2],
								   uint16_t* uv_coords, bool mirror) {
	// +--------+--------+--------+--------+
	// |		|        |        |		   |
	// |    	| TOP    | BOTTOM |		   |
	// |		|        |        |		   |
	// +--------+--------+--------+--------+
	// |		|        |        |        |
	// | LEFT	| FRONT  | RIGHT  | BACK   |
	// |		|        |        |        |
	// +--------+--------+--------+--------+

	uint16_t face_width[3] = {
		cube_size[0], // Width for top/bottom
		cube_size[0], // Width for front/back
		cube_size[2]  // Width for left/right (depth)
	};

	uint16_t face_height[3] = {
		cube_size[2], // Height for top/bottom (depth)
		cube_size[1], // Height for front/back
		cube_size[1]  // Height for left/right
	};

	bedrock_uv_texture_region regions[6];

	// Front face
	regions[0].x = texture_region[0] + cube_size[2];
	regions[0].y = texture_region[1] + cube_size[2];
	regions[0].width = face_width[1];
	regions[0].height = face_height[1];

	// Back face
	regions[1].x = texture_region[0] + 2 * cube_size[2] + cube_size[0];
	regions[1].y = texture_region[1] + cube_size[2];
	regions[1].width = face_width[1];
	regions[1].height = face_height[1];

	// Left face
	regions[2].x = texture_region[0];
	regions[2].y = texture_region[1] + cube_size[2];
	regions[2].width = face_width[2];
	regions[2].height = face_height[2];

	// Right face
	regions[3].x = texture_region[0] + cube_size[2] + cube_size[0];
	regions[3].y = texture_region[1] + cube_size[2];
	regions[3].width = face_width[2];
	regions[3].height = face_height[2];

	// Top Face
	regions[4].x = texture_region[0] + cube_size[2];
	regions[4].y = texture_region[1];
	regions[4].width = face_width[0];
	regions[4].height = face_height[0];

	// Bottom face
	regions[5].x = texture_region[0] + cube_size[2] + cube_size[0];
	regions[5].y = texture_region[1];
	regions[5].width = face_width[0];
	regions[5].height = face_height[0];

	for(int face = 0; face < 5; face++) {
		bedrock_uv_face uv = bedrock_calculate_uv_face(
			regions[face], regions[face].width, regions[face].height);

		// Store UV coordinates in the output buffer
		// Each face has 4 vertices, each with 2 UV coordinates
		int offset = face * 8;

		if(mirror) {
			// Apply mirroring if needed
			uv_coords[offset + 0] = uv.u2;
			uv_coords[offset + 1] = uv.v1;
			uv_coords[offset + 2] = uv.u1;
			uv_coords[offset + 3] = uv.v1;
			uv_coords[offset + 4] = uv.u1;
			uv_coords[offset + 5] = uv.v2;
			uv_coords[offset + 6] = uv.u2;
			uv_coords[offset + 7] = uv.v2;
		} else {
			// Standard UV mapping
			uv_coords[offset + 0] = uv.u1;
			uv_coords[offset + 1] = uv.v1;
			uv_coords[offset + 2] = uv.u2;
			uv_coords[offset + 3] = uv.v1;
			uv_coords[offset + 4] = uv.u2;
			uv_coords[offset + 5] = uv.v2;
			uv_coords[offset + 6] = uv.u1;
			uv_coords[offset + 7] = uv.v2;
		}
	}

	// Rotate the bottom face
	int offset = 5 * 8; // Bottom face is the last one
	bedrock_uv_face uv = bedrock_calculate_uv_face(regions[5], regions[5].width,
												   regions[5].height);

	uv_coords[offset + 0] = uv.u1;
	uv_coords[offset + 1] = uv.v2; // Top-left
	uv_coords[offset + 2] = uv.u2;
	uv_coords[offset + 3] = uv.v2; // Top-right
	uv_coords[offset + 4] = uv.u2;
	uv_coords[offset + 5] = uv.v1; // Bottom-right
	uv_coords[offset + 6] = uv.u1;
	uv_coords[offset + 7] = uv.v1; // Bottom-left
}
