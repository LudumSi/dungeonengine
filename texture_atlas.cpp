
#include "texture_atlas.h"
#include <stb_image.h>
#include <iostream>

TextureAtlas::TextureAtlas(int chunk, int size) {
	resolution = chunk;
	side_len = size;

	next_x = 0;
	next_y = 0;

	/*
	//Create assignment grid and fill it with falses to indicate nothing has been moved
	assignment_grid = new bool*[side_len];

	for (int i = 0; i < side_len; i++) {
		assignment_grid[i] = new bool[side_len];
		for (int j = 0; j < side_len; j++) {
			assignment_grid[i][j] = false;
		}
	}
	*/

	//Create buffer and fill it with empty data
	//At some point there should be a default missing texture which should be loaded in here as well

	scanline_size = resolution * side_len;

	//side_len x side_len chunks, each of which are resolution x resolution
	texture = new Texture(scanline_size,scanline_size);
}

TextureAtlas::~TextureAtlas() {

	/*
	if (assignment_grid) {
		for (int i = 0; i < side_len; i++) {
			delete[] assignment_grid[i];
		}

		delete[] assignment_grid;
	}
	*/

	if (texture) {
		delete texture;
	}
}

void TextureAtlas::add_image(const char* path) {

	//Check to make sure there are available coordinates
	if (next_y >= side_len) {
		std::cout << "Cannot add " << path << " to texture atlas: Atlas full" << std::endl;
		return;
	}

	//Create a new texture based on the path
	Texture image = Texture(path);

	//Catch failure to load image
	//Texture loader will already have printed an error
	if (!image.buffer) return;

	//Check that the image height and width are equal to atlas resolution
	//At some point should handle integer multiples but we'll cross that bridge when we get to it
	if (image.height != resolution) {
		std::cerr << "Image " << path << " with a height of " << image.height << " is longer than " << resolution << std::endl;
		return;
	}

	if (image.width != resolution) {
		std::cerr << "Image " << path << " with a width of " << image.width << " is longer than " << resolution << std::endl;
		return;
	}

	//Actually copy the new texture to the next available coordinates
	texture->copy_image(&image, next_x * resolution, next_y * resolution);

	//Save the texture coords in the dictionary along with the texture name
	atlas[std::string(path)] = glm::vec2(next_x * resolution, next_y * resolution);

	//Update the next available coords
	next_x++;
	if (next_x >= side_len) {
		next_x = 0;
		next_y++;
	}

	//Texture is deleted when it goes out of scope
}

glm::vec2 TextureAtlas::get_coords(const char* name) {
	return atlas[name];
}
