
#include "texture_atlas.h"
#include <stb_image.h>
#include <iostream>

TextureAtlas::TextureAtlas(int chunk, int size) {
	resolution = chunk;
	side_len = size;

	//Create assignment grid and fill it with falses to indicate nothing has been moved
	assignment_grid = new bool*[side_len];

	for (int i = 0; i < side_len; i++) {
		assignment_grid[i] = new bool[side_len];
		for (int j = 0; j < side_len; j++) {
			assignment_grid[i][j] = false;
		}
	}

	//Create buffer and fill it with empty data
	//At some point there should be a default missing texture which should be loaded in here as well

	scanline_size = resolution * side_len;

	buffer_size = scanline_size * scanline_size;

	//side_len x side_len chunks, each of which are resolution x resolution
	buffer = new pixel[buffer_size];

	for (int i = 0; i < buffer_size; i++) {
		buffer[i] = 0;
	}

}

TextureAtlas::~TextureAtlas() {

	if (assignment_grid) {
		for (int i = 0; i < side_len; i++) {
			delete[] assignment_grid[i];
		}

		delete[] assignment_grid;
	}

	if (buffer) {
		stbi_image_free(buffer);
	}

}

void TextureAtlas::copy_image(pixel* input,int width,int height,int x, int y) {

	int buffer_x = x;
	int buffer_y = y;

	//std::cout << buffer_x << "," << buffer_y << "," << scanline_size << std::endl;

	for (int input_y = 0; input_y < height; input_y++) {
		//Copy a scanline of the image into the buffer

		for (int input_x = 0; input_x < width; input_x++) {

			//std::cout << "Moving " << (width * input_y) + input_x << " to " << (scanline_size * buffer_y) + buffer_x << std::endl;

			buffer[(scanline_size * buffer_y) + buffer_x] = input[(width * input_y) + input_x];

			//std::cout << (int)buffer[(scanline_size * buffer_y) + buffer_x] - (int)input[(width * input_y) + input_x] << ",";

			//Check to ensure we're not going off the right side of the buffer
			buffer_x++;
			if (buffer_x >= scanline_size) {
				break;
				std::cout << "Went off right side" << std::endl;
			}
		}

		//Check to ensure that we're not trying to go off the bottom of the buffer
		buffer_y++;
		if (buffer_y >= scanline_size) {
			break;
		}

		buffer_x = x;
	}

	stbi_image_free(input);
}

void TextureAtlas::print_image() {

	std::cout << "Printing image!" << std::endl;

	for (int i = 0; i < scanline_size; i++) {
		
		for (int j = 0; j < scanline_size; j++) {

			std::cout << std::hex << buffer[(scanline_size * i) + j];
		}

		std::cout << std::endl;
	}
}
