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

#include "bedrock_geometry.h"
#include <stdlib.h>
#include <string.h>
#include "bedrock_geometry.h"
#include <parson.h>
#include <stdlib.h>
#include <string.h>

// Helper function to parse a Vector3 from a JSON array
static void parse_vector3(JSON_Array* array, bedrock_geometry_vector3* vector) {
    if (array && json_array_get_count(array) >= 3) {
        vector->x = (float)json_array_get_number(array, 0);
        vector->y = (float)json_array_get_number(array, 1);
        vector->z = (float)json_array_get_number(array, 2);
    } else {
        vector->x = 0.0f;
        vector->y = 0.0f;
        vector->z = 0.0f;
    }
}

// Helper function to parse a Vector3 from a JSON object with x, y, z properties
static void parse_vector3_from_object(JSON_Object* object, bedrock_geometry_vector3* vector) {
    if (object) {
        vector->x = (float)json_object_get_number(object, "x");
        vector->y = (float)json_object_get_number(object, "y");
        vector->z = (float)json_object_get_number(object, "z");
    } else {
        vector->x = 0.0f;
        vector->y = 0.0f;
        vector->z = 0.0f;
    }
}

// Helper function to parse a Vector2 from a JSON object with u, v properties
static void parse_vector2(JSON_Object* object, bedrock_geometry_vector2* vector) {
    if (object) {
        vector->u = (float)json_object_get_number(object, "u");
        vector->v = (float)json_object_get_number(object, "v");
    } else {
        vector->u = 0.0f;
        vector->v = 0.0f;
    }
}

// Parse a cube from JSON object
static void parse_cube(JSON_Object* object, bedrock_geometry_cube* cube) {
    // Parse origin as array [x, y, z]
    JSON_Array* origin_array = json_object_get_array(object, "origin");
    parse_vector3(origin_array, &cube->origin);

    // Parse size as array [x, y, z]
    JSON_Array* size_array = json_object_get_array(object, "size");
    parse_vector3(size_array, &cube->size);

    // Parse UV coordinates
    JSON_Array* uv_array = json_object_get_array(object, "uv");
    if (uv_array && json_array_get_count(uv_array) >= 2) {
        cube->uv.u = (float)json_array_get_number(uv_array, 0);
        cube->uv.v = (float)json_array_get_number(uv_array, 1);
    } else {
        // If uv is provided as an object
        JSON_Value* uv_value = json_object_get_value(object, "uv");
        if (json_value_get_type(uv_value) == JSONObject) {
            JSON_Object* uv_object = json_value_get_object(uv_value);
            parse_vector2(uv_object, &cube->uv);
        }
    }

    // Parse inflate if available
    if (json_object_has_value(object, "inflate")) {
        cube->inflate = (float)json_object_get_number(object, "inflate");
        cube->has_inflate = true;
    } else {
        cube->inflate = 0.0f;
        cube->has_inflate = false;
    }
}

// Parse locators from JSON object
static void parse_locators(JSON_Object* locators_obj, bedrock_geometry_locators* locators) {
    if (!locators_obj) {
        locators->entries = NULL;
        locators->count = 0;
        return;
    }

    // Get the number of locator entries
    size_t locator_count = json_object_get_count(locators_obj);
    locators->count = (int)locator_count;

    // Allocate memory for locator entries
    locators->entries = (bedrock_geometry_locator_entry*)malloc(locator_count * sizeof(bedrock_geometry_locator_entry));

    // Iterate through all locator properties
    for (size_t i = 0; i < locator_count; i++) {
        const char* locator_name = json_object_get_name(locators_obj, i);
        JSON_Value* locator_value = json_object_get_value_at(locators_obj, i);
        JSON_Array* pos_array = json_value_get_array(locator_value);

        // Store the locator name and position
        locators->entries[i].name = strdup(locator_name);
        parse_vector3(pos_array, &locators->entries[i].position);
    }
}

// Parse a bone from JSON object
static void parse_bone(JSON_Object* bone_obj, bedrock_geometry_bone* bone) {
    // Parse name
    bone->name = strdup(json_object_get_string(bone_obj, "name"));

    // Parse pivot
    JSON_Array* pivot_array = json_object_get_array(bone_obj, "pivot");
    parse_vector3(pivot_array, &bone->pivot);

    // Parse parent if available
    if (json_object_has_value(bone_obj, "parent")) {
        bone->parent = strdup(json_object_get_string(bone_obj, "parent"));
    } else {
        bone->parent = NULL;
    }

    // Parse rotation if available
    if (json_object_has_value(bone_obj, "rotation")) {
        JSON_Array* rotation_array = json_object_get_array(bone_obj, "rotation");
        parse_vector3(rotation_array, &bone->rotation);
        bone->has_rotation = true;
    } else {
        bone->rotation.x = 0.0f;
        bone->rotation.y = 0.0f;
        bone->rotation.z = 0.0f;
        bone->has_rotation = false;
    }

    // Parse cubes if available
    if (json_object_has_value(bone_obj, "cubes")) {
        JSON_Array* cubes_array = json_object_get_array(bone_obj, "cubes");
        size_t cubes_count = json_array_get_count(cubes_array);
        bone->cubes_count = (int)cubes_count;
        bone->cubes = (bedrock_geometry_cube*)malloc(cubes_count * sizeof(bedrock_geometry_cube));

        for (size_t i = 0; i < cubes_count; i++) {
            JSON_Object* cube_obj = json_array_get_object(cubes_array, i);
            parse_cube(cube_obj, &bone->cubes[i]);
        }
    } else {
        bone->cubes = NULL;
        bone->cubes_count = 0;
    }

    // Parse locators if available
    if (json_object_has_value(bone_obj, "locators")) {
        JSON_Object* locators_obj = json_object_get_object(bone_obj, "locators");
        bone->has_locators = true;
        parse_locators(locators_obj, &bone->locators);
    } else {
        bone->has_locators = false;
        bone->locators.entries = NULL;
        bone->locators.count = 0;
    }

    // Parse mirror if available
    if (json_object_has_value(bone_obj, "mirror")) {
        bone->mirror = json_object_get_boolean(bone_obj, "mirror");
        bone->has_mirror = true;
    } else {
        bone->mirror = false;
        bone->has_mirror = false;
    }
}

// Parse a geometry description
static void parse_description(JSON_Object* desc_obj, bedrock_geometry_description* desc) {
    desc->identifier = strdup(json_object_get_string(desc_obj, "identifier"));
    desc->texture_width = (int)json_object_get_number(desc_obj, "texture_width");
    desc->texture_height = (int)json_object_get_number(desc_obj, "texture_height");

    // Parse visible bounds if available
    if (json_object_has_value(desc_obj, "visible_bounds_width") &&
        json_object_has_value(desc_obj, "visible_bounds_height")) {
        desc->visible_bounds_width = (float)json_object_get_number(desc_obj, "visible_bounds_width");
        desc->visible_bounds_height = (float)json_object_get_number(desc_obj, "visible_bounds_height");

        if (json_object_has_value(desc_obj, "visible_bounds_offset")) {
            JSON_Array* offset_array = json_object_get_array(desc_obj, "visible_bounds_offset");
            parse_vector3(offset_array, &desc->visible_bounds_offset);
        } else {
            desc->visible_bounds_offset.x = 0.0f;
            desc->visible_bounds_offset.y = 0.0f;
            desc->visible_bounds_offset.z = 0.0f;
        }
        desc->has_visible_bounds = true;
    } else {
        desc->has_visible_bounds = false;
    }
}

bedrock_geometry_root* bedrock_geometry_parse(const char* json_string) {
    // Initialize Parson JSON values
    JSON_Value* root_value = json_parse_string(json_string);
    if (!root_value) {
        return NULL; // Failed to parse JSON
    }

    JSON_Object* root_object = json_value_get_object(root_value);
    if (!root_object) {
        json_value_free(root_value);
        return NULL;
    }

    // Allocate the root structure
    bedrock_geometry_root* result = (bedrock_geometry_root*)malloc(sizeof(bedrock_geometry_root));
    if (!result) {
        json_value_free(root_value);
        return NULL;
    }

    // Parse format version
    const char* format_version = json_object_get_string(root_object, "format_version");
    strncpy(result->format_version, format_version ? format_version : "", 7);
    result->format_version[7] = '\0';

    // Parse geometries
    JSON_Array* geometries_array = json_object_get_array(root_object, "minecraft:geometry");
    if (!geometries_array) {
        free(result);
        json_value_free(root_value);
        return NULL;
    }

    size_t geometries_count = json_array_get_count(geometries_array);
    result->geometries_count = (int)geometries_count;
    result->geometries = (bedrock_geometry_entry*)malloc(geometries_count * sizeof(bedrock_geometry_entry));

    for (size_t i = 0; i < geometries_count; i++) {
        JSON_Object* geometry_obj = json_array_get_object(geometries_array, i);

        // Parse description
        JSON_Object* desc_obj = json_object_get_object(geometry_obj, "description");
        parse_description(desc_obj, &result->geometries[i].description);

        // Parse bones
        JSON_Array* bones_array = json_object_get_array(geometry_obj, "bones");
        size_t bones_count = json_array_get_count(bones_array);
        result->geometries[i].bones_count = (int)bones_count;
        result->geometries[i].bones = (bedrock_geometry_bone*)malloc(bones_count * sizeof(bedrock_geometry_bone));

        for (size_t j = 0; j < bones_count; j++) {
            JSON_Object* bone_obj = json_array_get_object(bones_array, j);
            parse_bone(bone_obj, &result->geometries[i].bones[j]);
        }
    }

    // Free the JSON value as we've extracted all data
    json_value_free(root_value);

    return result;
}

void bedrock_geometry_free(bedrock_geometry_root* root) {
    if (!root) return;

    // Free all allocated memory
    for (int i = 0; i < root->geometries_count; i++) {
        bedrock_geometry_entry* geometry = &root->geometries[i];

        // Free description
        free(geometry->description.identifier);

        // Free bones
        for (int j = 0; j < geometry->bones_count; j++) {
            bedrock_geometry_bone* bone = &geometry->bones[j];

            free(bone->name);
            if (bone->parent) free(bone->parent);

            // Free cubes
            if (bone->cubes) free(bone->cubes);

            // Free locators
            if (bone->has_locators) {
                for (int k = 0; k < bone->locators.count; k++) {
                    free(bone->locators.entries[k].name);
                }
                free(bone->locators.entries);
            }
        }

        free(geometry->bones);
    }

    free(root->geometries);
    free(root);
}