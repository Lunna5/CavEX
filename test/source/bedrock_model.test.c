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

#include <assert.h>
#include <graphics/model/bedrock_geometry.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file_to_buffer(const char* filename, size_t* size_out) {
	/* Open the file in binary read mode */
	FILE* file = fopen(filename, "rb");
	if(file == NULL) {
		perror("Error opening file");
		return NULL;
	}

	/* Get file size */
	fseek(file, 0, SEEK_END); /* Move position indicator to the end */
	const size_t file_size
		= ftell(file);		  /* Get current position (which is the size) */
	fseek(file, 0, SEEK_SET); /* Rewind to start of file */

	/* Allocate memory for the entire file content */
	const auto buffer
		= (char*)malloc(file_size + 1); /* +1 for null terminator */

	if(buffer == NULL) {
		perror("Memory allocation failed");
		fclose(file);
		return nullptr;
	}

	/* Read file contents into buffer */
	size_t read_size = fread(buffer, 1, file_size, file);

	/* Check if we read the expected number of bytes */
	if(read_size != file_size) {
		perror("Error reading file");
		free(buffer);
		fclose(file);
		return nullptr;
	}

	/* Null-terminate the buffer (useful for text files) */
	buffer[file_size] = '\0';

	/* Close the file */
	fclose(file);

	/* Output the size if requested */
	if(size_out != NULL) {
		*size_out = file_size;
	}

	return buffer;
}

int main() {
	const char* filename = "resources/player.geo.json";
	size_t file_size;

	/* Read the entire file into a buffer */
	char* file_buffer = read_file_to_buffer(filename, &file_size);

	assert(file_buffer != NULL);

	bedrock_geometry_root* geometry = bedrock_geometry_parse(file_buffer);

	assert(geometry != NULL);

	assert(geometry->geometries_count == 3);

	assert(strcmp(geometry->geometries[1].description.identifier,
				  "geometry.npc.steve")
		   == 0);

	/* Always free allocated memory when done */
	free(file_buffer);
}