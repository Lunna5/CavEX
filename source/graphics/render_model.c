/*
	Copyright (c) 2023 ByteBit/xtreme8000

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

#include <assert.h>

#include "render_model.h"

#include "model/bedrock_geometry.h"

#include "../util.h"
#include "model/bedrock_uv_mapper.h"

#include <string.h>

typedef struct {
	uint16_t top[4];
	uint16_t bottom[4];
	uint16_t front[4];
	uint16_t back[4];
	uint16_t right[4];
	uint16_t left[4];
} PartUVs;

static bedrock_geometry_root* player_geometry = NULL;

void init_bedrock_geometry() {
	char* json_data = read_file_to_buffer("assets/model/player.geo.json", NULL);
	assert(json_data != NULL);
	player_geometry = bedrock_geometry_parse(json_data);
	free(json_data);
	assert(player_geometry != NULL);
}

static void adjust_from_and_to(vec3 from, vec3 to, float inflate, vec3 stretch,
							   vec3 adjusted_from, vec3 adjusted_to) {
	// Calculate half size
	vec3 half_size;
	half_size[0] = (to[0] - from[0]) / 2.0f;
	half_size[1] = (to[1] - from[1]) / 2.0f;
	half_size[2] = (to[2] - from[2]) / 2.0f;

	// Calculate center
	vec3 center;
	center[0] = from[0] + half_size[0];
	center[1] = from[1] + half_size[1];
	center[2] = from[2] + half_size[2];

	// Apply inflation and stretch
	for(int i = 0; i < 3; i++) {
		adjusted_from[i] = center[i] - (half_size[i] + inflate) * stretch[i];
		adjusted_to[i] = center[i] + (half_size[i] + inflate) * stretch[i];
	}
}

static void generate_cube_vertices(vec3 from, vec3 to, vec3 origin,
                               float inflate, vec3 stretch,
                               int16_t* vertices) {
    // Adjust from and to considering inflation and stretching
    vec3 adjusted_from, adjusted_to;
    adjust_from_and_to(from, to, inflate, stretch, adjusted_from, adjusted_to);

    // Compute vertices relative to origin
    // Order: front, back, left, right, top, bottom

    // Front face (North in JS code)
    vertices[0] = adjusted_from[0] - origin[0];   // x
    vertices[1] = adjusted_to[1] - origin[1];     // y
    vertices[2] = adjusted_from[2] - origin[2];   // z
    vertices[3] = adjusted_to[0] - origin[0];     // x
    vertices[4] = adjusted_to[1] - origin[1];     // y
    vertices[5] = adjusted_from[2] - origin[2];   // z
    vertices[6] = adjusted_to[0] - origin[0];     // x
    vertices[7] = adjusted_from[1] - origin[1];   // y
    vertices[8] = adjusted_from[2] - origin[2];   // z
    vertices[9] = adjusted_from[0] - origin[0];   // x
    vertices[10] = adjusted_from[1] - origin[1];  // y
    vertices[11] = adjusted_from[2] - origin[2];  // z

    // Back face (South in JS code)
    vertices[12] = adjusted_to[0] - origin[0];    // x
    vertices[13] = adjusted_to[1] - origin[1];    // y
    vertices[14] = adjusted_to[2] - origin[2];    // z
    vertices[15] = adjusted_from[0] - origin[0];  // x
    vertices[16] = adjusted_to[1] - origin[1];    // y
    vertices[17] = adjusted_to[2] - origin[2];    // z
    vertices[18] = adjusted_from[0] - origin[0];  // x
    vertices[19] = adjusted_from[1] - origin[1];  // y
    vertices[20] = adjusted_to[2] - origin[2];    // z
    vertices[21] = adjusted_to[0] - origin[0];    // x
    vertices[22] = adjusted_from[1] - origin[1];  // y
    vertices[23] = adjusted_to[2] - origin[2];    // z

    // Left face (West in JS code)
    vertices[24] = adjusted_from[0] - origin[0];  // x
    vertices[25] = adjusted_to[1] - origin[1];    // y
    vertices[26] = adjusted_to[2] - origin[2];    // z
    vertices[27] = adjusted_from[0] - origin[0];  // x
    vertices[28] = adjusted_to[1] - origin[1];    // y
    vertices[29] = adjusted_from[2] - origin[2];  // z
    vertices[30] = adjusted_from[0] - origin[0];  // x
    vertices[31] = adjusted_from[1] - origin[1];  // y
    vertices[32] = adjusted_from[2] - origin[2];  // z
    vertices[33] = adjusted_from[0] - origin[0];  // x
    vertices[34] = adjusted_from[1] - origin[1];  // y
    vertices[35] = adjusted_to[2] - origin[2];    // z

    // Right face (East in JS code)
    vertices[36] = adjusted_to[0] - origin[0];    // x
    vertices[37] = adjusted_to[1] - origin[1];    // y
    vertices[38] = adjusted_from[2] - origin[2];  // z
    vertices[39] = adjusted_to[0] - origin[0];    // x
    vertices[40] = adjusted_to[1] - origin[1];    // y
    vertices[41] = adjusted_to[2] - origin[2];    // z
    vertices[42] = adjusted_to[0] - origin[0];    // x
    vertices[43] = adjusted_from[1] - origin[1];  // y
    vertices[44] = adjusted_to[2] - origin[2];    // z
    vertices[45] = adjusted_to[0] - origin[0];    // x
    vertices[46] = adjusted_from[1] - origin[1];  // y
    vertices[47] = adjusted_from[2] - origin[2];  // z

    // Top face (Up in JS code)
    vertices[48] = adjusted_from[0] - origin[0];  // x
    vertices[49] = adjusted_to[1] - origin[1];    // y
    vertices[50] = adjusted_from[2] - origin[2];  // z
    vertices[51] = adjusted_from[0] - origin[0];  // x
    vertices[52] = adjusted_to[1] - origin[1];    // y
    vertices[53] = adjusted_to[2] - origin[2];    // z
    vertices[54] = adjusted_to[0] - origin[0];    // x
    vertices[55] = adjusted_to[1] - origin[1];    // y
    vertices[56] = adjusted_to[2] - origin[2];    // z
    vertices[57] = adjusted_to[0] - origin[0];    // x
    vertices[58] = adjusted_to[1] - origin[1];    // y
    vertices[59] = adjusted_from[2] - origin[2];  // z

    // Bottom face (Down in JS code)
    vertices[60] = adjusted_from[0] - origin[0];  // x
    vertices[61] = adjusted_from[1] - origin[1];  // y
    vertices[62] = adjusted_from[2] - origin[2];  // z
    vertices[63] = adjusted_to[0] - origin[0];    // x
    vertices[64] = adjusted_from[1] - origin[1];  // y
    vertices[65] = adjusted_from[2] - origin[2];  // z
    vertices[66] = adjusted_to[0] - origin[0];    // x
    vertices[67] = adjusted_from[1] - origin[1];  // y
    vertices[68] = adjusted_to[2] - origin[2];    // z
    vertices[69] = adjusted_from[0] - origin[0];  // x
    vertices[70] = adjusted_from[1] - origin[1];  // y
    vertices[71] = adjusted_to[2] - origin[2];    // z
}

void create_cube_at_origin_with_view(const mat4 view) {
	// Position at origin
	vec3 position = {-4.0F, 24.0F, -4.0F};
	// No pivot offset
	vec3 pivot = {0.0F, 24.0F, 0.0F};
	// No rotation
	vec3 rotation = {0.0F, 0.0F, 0.0F};
	// UV origin (can be 0,0 for default texture coordinates)
	ivec2 uv_origin = {0, 0};
	// Cube size: width=1, depth=1, height=1
	ivec3 box = {10, 10, 10};
	// No padding, no mirroring, full brightness
	float padding = 0.0F;
	bool mirror = false;
	float brightness = 1.0F;

	// Render the cube with the provided view matrix
	render_model_box(view, position, pivot, rotation, uv_origin, box, padding,
					 mirror, brightness);
}

void render_model_box(mat4 view, vec3 position, vec3 pivot, vec3 rotation,
                     ivec2 origin, ivec3 box, float padding, bool mirror,
                     float brightness) {
    // Only check valid ranges for float parameters
    assert(padding >= -GLM_FLT_EPSILON &&
           brightness >= -GLM_FLT_EPSILON &&
           brightness <= 1.0F + GLM_FLT_EPSILON);

    // Create model matrix with transformations applied in correct order
    mat4 model;
    glm_translate_make(model, position);

    // Apply rotations in Z-Y-X order (standard Euler angles)
    glm_rotate_z(model, glm_rad(rotation[2]), model);
    glm_rotate_y(model, glm_rad(rotation[1]), model);
    glm_rotate_x(model, glm_rad(rotation[0]), model);

    // Apply pivot offset and padding
    glm_translate(model, (vec3){
        -pivot[0] - padding,
        -pivot[1] - padding,
        -pivot[2] - padding
    });

    // Scale according to box dimensions and padding
    // Note: The order of dimensions is intentionally [0,2,1] to match Minecraft's
    // coordinate system where Y is height
    glm_scale(model, (vec3){
        padding * 2.0F / box[0] + 1.0F,
        padding * 2.0F / box[2] + 1.0F,
        padding * 2.0F / box[1] + 1.0F
    });

    // Combine view and model matrices
    mat4 modelview;
    glm_mat4_mul(view, model, modelview);
    gfx_matrix_modelview(modelview);

    // Convert brightness to 8-bit color component
    uint8_t light = roundf(brightness * 0xFF);

    // Calculate UV coordinates for the cube faces
    uint16_t uvCoords[48];
    bedrock_calculate_cube_uv_map(
        (uint16_t[]){box[0], box[2], box[1]},
        (uint16_t[]){origin[0], origin[1]},
        uvCoords,
        mirror
    );

    // Generate cube vertices
    vec3 from = {0, 0, 0};
    vec3 to = {box[0], box[2], box[1]};
    vec3 cube_origin = {0, 0, 0};
    vec3 stretch = {1.0f, 1.0f, 1.0f};
    float inflate = 0.0f;
    int16_t vertices[72];
    generate_cube_vertices(from, to, cube_origin, inflate, stretch, vertices);

    // Create color array more efficiently
    uint8_t colors[96]; // 24 vertices × 4 components (RGBA)
    for (int i = 0; i < 24; i++) {
        colors[i*4+0] = light; // R
        colors[i*4+1] = light; // G
        colors[i*4+2] = light; // B
        colors[i*4+3] = 0xFF;  // A (always fully opaque)
    }

    // Draw the cube (top, bottom, back, front, right, left faces)
    gfx_draw_quads_64(24, vertices, colors, uvCoords);
}

void render_bedrock_model(const mat4 mv, const bedrock_geometry_root root,
                         struct tex_gfx tex_gfx, int geometry_index) {
    assert(mv);
    assert(root.geometries_count > 0);
    assert(geometry_index >= 0 && geometry_index < root.geometries_count);

    // Use the provided texture instead of hardcoded texture_mob_char
    gfx_bind_texture(&tex_gfx);

    const bedrock_geometry_entry* geometry = &root.geometries[geometry_index];
    if (!geometry) return;

    // Process bones in hierarchical order
    for (int i = 0; i < geometry->bones_count; i++) {
        const bedrock_geometry_bone* bone = &geometry->bones[i];
        if (!bone) continue;

        // TODO: Apply bone hierarchy transformations here
        // If bone->parent is defined, we should apply parent transforms first

        // Store bone transformation matrix for child bones
        mat4 bone_transform;
        glm_mat4_identity(bone_transform);

        // Render all cubes in this bone
        for (int j = 0; j < bone->cubes_count; j++) {
            const bedrock_geometry_cube* cube = &bone->cubes[j];
            if (!cube) continue;

            // Position from the cube definition
            vec3 position = {cube->origin.x, cube->origin.y, cube->origin.z};

            // Use bone pivot if available, otherwise no pivot
            vec3 pivot = {0.0F, 0.0F, 0.0F};
            // TODO: If bone has pivot data, use it: pivot = bone->pivot

            // Apply rotation from the cube or bone
            vec3 rotation = {0.0F, 0.0F, 0.0F};
            // TODO: If cube has rotation, use it: rotation = cube->rotation

            // UV origin from the cube
            ivec2 uv_origin = {cube->uv.u, cube->uv.v};

            // Cube size in the proper order for rendering
            ivec3 box = {cube->size.x, cube->size.z, cube->size.y};

            // Apply model parameters
            float padding = 0.0F;
            bool mirror = false; // Use mirror setting from the cube definition
            float brightness = 1.0F;

            // Create a combined matrix that includes bone transformations
            mat4 combined_mv;
            glm_mat4_copy(mv, combined_mv);
            // TODO: Apply bone_transform to combined_mv if using bone hierarchy

            // Render the cube with all transformations
            render_model_box(combined_mv, position, pivot, rotation,
                            uv_origin, box, padding, mirror, brightness);
        }
    }
}

void render_model_player(mat4 mv, float head_pitch, float head_yaw,
						 float foot_angle, float arm_angle,
						 struct item_data* held_item, struct item_data* helmet,
						 struct item_data* chestplate,
						 struct item_data* leggings, struct item_data* boots) {
	assert(mv);

	struct item* held_item_it = held_item ? item_get(held_item) : NULL;
	struct item* helmet_it = helmet ? item_get(helmet) : NULL;
	struct item* chestplate_it = chestplate ? item_get(chestplate) : NULL;
	struct item* leggings_it = leggings ? item_get(leggings) : NULL;
	struct item* boots_it = boots ? item_get(boots) : NULL;

	gfx_bind_texture(&texture_test);
	/*
	create_cube_at_origin_with_view(mv);
	*/

	gfx_bind_texture(&texture_mob_char);
	/*render_bedrock_model_complete(
		mv, player_geometry, (vec3) {0.0F, -4.0F, 0.0F}, &texture_mob_char,
		(vec3) {0.0F, 0.0F, 0.0F}, 1.0F, 1.0F);*/

	render_bedrock_model(mv, *player_geometry, texture_mob_char, 0);

	/*render_model_box(mv, (vec3) {-4.0F, -4.0F, 24}, // Z position adjusted
					 (vec3) {-4.0F, -4.0F, 24},
					 (vec3) {0.0F, 0.0F, 0.0F}, // Using clamped pitch
					 (ivec2) {0, 0}, (ivec3) {8, 8, 8}, 0.0F, false, 1.0F);
	render_model_box(mv, (vec3) {-4.0F, -24.0F, 12}, // Z position adjusted
					 (vec3) {-4.0F, -2.0F, 12},
					 (vec3) {0.0F, 0.0F, 0.0F}, // Using clamped pitch
					 (ivec2) {16, 16}, (ivec3) {8, 4, 12}, 0.0F, false, 1.0F);*/
	// head

	/*float head_pitch_clamped
		= glm_clamp(head_pitch, -60.0f, 60.0f); // Limit extreme angles
	float head_forward_offset = 0.0f;

	if(head_pitch < 0) {
		// When looking down, move head forward proportionally to the angle
		// This prevents intersection with the body
		head_forward_offset
			= -head_pitch * 0.04f; // Adjust this multiplier as needed
	}

	render_model_box(
		mv,
		(vec3) {0.0F, -4.0F, 0.0F + head_forward_offset}, // Z position
	adjusted (vec3) {4.0F, 0.0F, 4.0F}, (vec3) {head_pitch_clamped,
	head_yaw, 0.0F}, // Using clamped pitch (ivec2) {0, 0}, (ivec3) {8, 8,
	8}, 0.0F, false, 1.0F);


	// left foot
	render_model_box(mv, (vec3) {2.0F, -16.0F, 0.0F},
					 (vec3) {2.0F, 12.0F, 2.0F},
					 (vec3) {foot_angle, 0.0F, 0.0F}, (ivec2) {0, 16},
					 (ivec3) {4, 4, 12}, 0.0F, true, 1.0F);
	// right foot
	render_model_box(mv, (vec3) {-2.0F, -16.0F, 0.0F},
					 (vec3) {2.0F, 12.0F, 2.0F},
					 (vec3) {-foot_angle, 0.0F, 0.0F}, (ivec2) {0, 16},
					 (ivec3) {4, 4, 12}, 0.0F, false, 1.0F);
	// left arm
	render_model_box(mv, (vec3) {4.0F, -4.0F, 0.0F}, (vec3)
	{0.0F, 12.0F, 2.0F}, (vec3) {arm_angle, 0.0F, 4.0F}, (ivec2) {40, 16},
					 (ivec3) {4, 4, 12}, 0.0F, true, 1.0F);

	// head overlay
	render_model_box(mv, (vec3) {0.0F, -4.0F, 0.0F}, (vec3) {4.0F,
	0.0F, 4.0F}, (vec3) {head_pitch, head_yaw, 0.0F}, (ivec2) {32, 0},
					 (ivec3) {8, 8, 8}, 0.5F, false, 1.0F);
	// body
	render_model_box(mv, (vec3) {0.0F, -4.0F, 0.0F}, (vec3) {4.0F, 12.0F,
	0.0F}, (vec3) {0.0F, 0.0F, 0.0F}, (ivec2) {16, 16}, (ivec3) {8, 4, 12},
	0.0F, false, 1.0F);*/
	gfx_cull_func(MODE_BACK);

	// head
	/*render_model_box(mv, (vec3) {0.0F, -4.0F, 0.0F}, (vec3) {4.0F,
	0.0F, 4.0F}, (vec3) {head_pitch, head_yaw, 0.0F}, (ivec2) {8, 8},
	(ivec3) {8, 8, 8}, 0.0F, false, 1.0F); gfx_cull_func(MODE_BACK);*/
	/*
	// head overlay
	render_model_box(mv, (vec3) {0.0F, -4.0F, 0.0F}, (vec3) {4.0F,
	0.0F, 4.0F}, (vec3) {head_pitch, head_yaw, 0.0F}, (ivec2) {40, 8},
					 (ivec3) {8, 8, 8}, 0.5F, false, 1.0F);
	// body
	render_model_box(mv, (vec3) {0.0F, -4.0F, 0.0F}, (vec3)
	{4.0F, 12.0F, 2.0F}, (vec3) {0.0F, 0.0F, 0.0F}, (ivec2) {20, 20},
					 (ivec3) {8, 4, 12}, 0.0F, false, 1.0F);
	// left foot
	render_model_box(mv, (vec3) {2.0F, -16.0F, 0.0F},
					 (vec3) {2.0F, 12.0F, 2.0F},
					 (vec3) {foot_angle, 0.0F, 0.0F}, (ivec2) {4, 20},
					 (ivec3) {4, 4, 12}, 0.0F, true, 1.0F);
	// right foot
	render_model_box(mv, (vec3) {-2.0F, -16.0F, 0.0F},
					 (vec3) {2.0F, 12.0F, 2.0F},
					 (vec3) {-foot_angle, 0.0F, 0.0F}, (ivec2) {4, 20},
					 (ivec3) {4, 4, 12}, 0.0F, false, 1.0F);
	// left arm
	render_model_box(mv, (vec3) {4.0F, -4.0F, 0.0F}, (vec3)
	{0.0F, 12.0F, 2.0F}, (vec3) {arm_angle, 0.0F, 4.0F}, (ivec2) {44, 20},
					 (ivec3) {4, 4, 12}, 0.0F, true, 1.0F);
	// right arm
	render_model_box(
		mv, (vec3) {-4.0F, -4.0F, 0.0F}, (vec3) {4.0F, 12.0F, 2.0F},
		(vec3) {(held_item_it ? -22.5F : 0.0F) - arm_angle, 0.0F, -4.0F},
		(ivec2) {44, 20}, (ivec3) {4, 4, 12}, 0.0F, false, 1.0F);

	if(held_item_it) {
		mat4 model;
		glm_translate_make(model, (vec3) {-6.0F, -4.0F, 0.0F});
		glm_rotate_z(model, glm_rad(-4.0F), model);
		glm_rotate_x(model, glm_rad(-22.5F - arm_angle), model);

		mat4 mv_item;
		glm_mat4_mul(mv, model, mv_item);
		held_item_it->renderItem(held_item_it, held_item, mv_item, true,
								 R_ITEM_ENV_THIRDPERSON);
	}

	struct tex_gfx* armor_texture[][2] = {
		[ARMOR_TIER_BLOCK] = {NULL, NULL},
		[ARMOR_TIER_CHAIN] = {&texture_armor_chain1, &texture_armor_chain2},
		[ARMOR_TIER_CLOTH] = {&texture_armor_cloth1, &texture_armor_cloth2},
		[ARMOR_TIER_GOLD] = {&texture_armor_gold1, &texture_armor_gold2},
		[ARMOR_TIER_IRON] = {&texture_armor_iron1, &texture_armor_iron2},
		[ARMOR_TIER_DIAMOND]
		= {&texture_armor_diamond1, &texture_armor_diamond2},
	};

	gfx_cull_func(MODE_NONE);

	if(helmet_it && helmet_it->armor.is_armor
	   && helmet_it->armor.type == ARMOR_TYPE_HELMET) {
		if(helmet_it->armor.tier == ARMOR_TIER_BLOCK) {
			mat4 model;
			glm_translate_make(model, (vec3) {0.0F, -4.0F, 0.0F});
			glm_rotate_y(model, glm_rad(head_yaw), model);
			glm_rotate_x(model, glm_rad(head_pitch), model);
			glm_translate(model, (vec3) {4.5F, -0.5F, 4.5F});
			glm_rotate_y(model, glm_rad(180.0F), model);
			glm_scale_uni(model, 9.0F);

			mat4 mv_item;
			glm_mat4_mul(mv, model, mv_item);

			// pumpkin etc.
			helmet_it->renderItem(helmet_it, helmet, mv_item, true,
								  R_ITEM_ENV_ENTITY);
		} else {
			gfx_bind_texture(armor_texture[helmet_it->armor.tier][0]);

			// head
			render_model_box(
				mv, (vec3) {0.0F, -4.0F, 0.0F}, (vec3) {4.0F, 0.0F, 4.0F},
				(vec3) {head_pitch, head_yaw, 0.0F}, (ivec2) {8, 8},
				(ivec3) {8, 8, 8}, 1.0F, false, 1.0F);
		}
	}

	if(chestplate_it && chestplate_it->armor.is_armor
	   && chestplate_it->armor.type == ARMOR_TYPE_CHESTPLATE) {
		gfx_bind_texture(armor_texture[chestplate_it->armor.tier][0]);

		// body
		render_model_box(mv, (vec3) {0.0F, -4.0F, 0.0F},
						 (vec3) {4.0F, 12.0F, 2.0F}, (vec3) {0.0F, 0.0F,
	0.0F}, (ivec2) {20, 20}, (ivec3) {8, 4, 12}, 1.0F, false, 1.0F);
		// left arm
		render_model_box(mv, (vec3) {4.0F, -4.0F, 0.0F},
						 (vec3) {0.0F, 12.0F, 2.0F},
						 (vec3) {arm_angle, 0.0F, 4.0F}, (ivec2) {44, 20},
						 (ivec3) {4, 4, 12}, 1.0F, true, 1.0F);
		// right arm
		render_model_box(
			mv, (vec3) {-4.0F, -4.0F, 0.0F}, (vec3) {4.0F, 12.0F, 2.0F},
			(vec3) {(held_item_it ? -22.5F : 0.0F) - arm_angle, 0.0F,
	-4.0F}, (ivec2) {44, 20}, (ivec3) {4, 4, 12}, 1.0F, false, 1.0F);
	}

	if(leggings_it && leggings_it->armor.is_armor
	   && leggings_it->armor.type == ARMOR_TYPE_LEGGINGS) {
		gfx_bind_texture(armor_texture[leggings_it->armor.tier][1]);
		// body
		render_model_box(mv, (vec3) {0.0F, -4.0F, 0.0F},
						 (vec3) {4.0F, 12.0F, 2.0F}, (vec3) {0.0F, 0.0F,
	0.0F}, (ivec2) {20, 20}, (ivec3) {8, 4, 12}, 0.5F, false, 1.0F);
		// left leg
		render_model_box(mv, (vec3) {2.0F, -16.0F, 0.0F},
						 (vec3) {2.0F, 12.0F, 2.0F},
						 (vec3) {foot_angle, 0.0F, 0.0F}, (ivec2) {4, 20},
						 (ivec3) {4, 4, 12}, 0.5F, true, 1.0F);
		// right leg
		render_model_box(mv, (vec3) {-2.0F, -16.0F, 0.0F},
						 (vec3) {2.0F, 12.0F, 2.0F},
						 (vec3) {-foot_angle, 0.0F, 0.0F}, (ivec2) {4, 20},
						 (ivec3) {4, 4, 12}, 0.5F, false, 1.0F);
	}

	if(boots_it && boots_it->armor.is_armor
	   && boots_it->armor.type == ARMOR_TYPE_BOOTS) {
		gfx_bind_texture(armor_texture[boots_it->armor.tier][0]);
		// left leg
		render_model_box(mv, (vec3) {2.0F, -16.0F, 0.0F},
						 (vec3) {2.0F, 12.0F, 2.0F},
						 (vec3) {foot_angle, 0.0F, 0.0F}, (ivec2) {4, 20},
						 (ivec3) {4, 4, 12}, 1.0F, true, 1.0F);
		// right leg
		render_model_box(mv, (vec3) {-2.0F, -16.0F, 0.0F},
						 (vec3) {2.0F, 12.0F, 2.0F},
						 (vec3) {-foot_angle, 0.0F, 0.0F}, (ivec2) {4, 20},
						 (ivec3) {4, 4, 12}, 1.0F, false, 1.0F);
	}

	gfx_cull_func(MODE_BACK);*/
}