
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

	//side_len x side_len chunks, each of which are resolution x resolution
	buffer = new Texture(scanline_size,scanline_size);
}

TextureAtlas::~TextureAtlas() {

	if (assignment_grid) {
		for (int i = 0; i < side_len; i++) {
			delete[] assignment_grid[i];
		}

		delete[] assignment_grid;
	}

	delete buffer;
}
