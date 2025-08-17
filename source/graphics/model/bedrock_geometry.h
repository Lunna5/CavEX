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

#ifndef CAVEX_BEDROCK_GEOMETRY_H
#define CAVEX_BEDROCK_GEOMETRY_H
#include <stdint.h>
#include <stdbool.h>

// Vector types
typedef struct {
    float x;
    float y;
    float z;
} bedrock_geometry_vector3;

typedef struct {
    float u;
    float v;
} bedrock_geometry_vector2;

// Structure for cube in bones
typedef struct {
    bedrock_geometry_vector3 origin;
    bedrock_geometry_vector3 size;
    bedrock_geometry_vector2 uv;
    float inflate;  // Optional, 0.0 if not present
    bool has_inflate;  // Flag to check if inflate was provided
} bedrock_geometry_cube;

// Structure for locator entry
typedef struct {
    char* name;
    bedrock_geometry_vector3 position;
} bedrock_geometry_locator_entry;

// Structure for locators
typedef struct {
    bedrock_geometry_locator_entry* entries;
    int count;
} bedrock_geometry_locators;

// Structure for bone
typedef struct bedrock_geometry_bone {
    char* name;
    bedrock_geometry_vector3 pivot;
    char* parent;  // Optional, NULL if not present
    bedrock_geometry_vector3 rotation;  // Optional, {0,0,0} if not present
    bool has_rotation;  // Flag to check if rotation was provided

    bedrock_geometry_cube* cubes;  // Optional array of cubes
    int cubes_count;

    bedrock_geometry_locators locators;  // Optional
    bool has_locators;  // Flag to check if locators were provided

    bool mirror;  // Optional, false if not present
    bool has_mirror;  // Flag to check if mirror was provided
} bedrock_geometry_bone;

// Structure for description
typedef struct {
    char* identifier;
    int texture_width;
    int texture_height;
    float visible_bounds_width;  // Optional
    float visible_bounds_height;  // Optional
    bedrock_geometry_vector3 visible_bounds_offset;  // Optional
    bool has_visible_bounds;  // Flag to check if visible bounds were provided
} bedrock_geometry_description;

// Structure for a single geometry entry
typedef struct {
    bedrock_geometry_description description;
    bedrock_geometry_bone* bones;
    int bones_count;
} bedrock_geometry_entry;

// Root structure containing the whole file
typedef struct {
    char format_version[8];
    bedrock_geometry_entry* geometries;
    int geometries_count;
} bedrock_geometry_root;

bedrock_geometry_root* bedrock_geometry_parse(const char* json_string);
void bedrock_geometry_free(bedrock_geometry_root* root);

#endif // CAVEX_BEDROCK_GEOMETRY_H