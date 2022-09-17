
#include "texture.h"
#include <iostream>
#include <stb_image.h>

void Texture::generate_buffer() {

	int buffer_size = height * width;

	//side_len x side_len chunks, each of which are resolution x resolution
	buffer = new pixel[buffer_size];

	for (int i = 0; i < buffer_size; i++) {
		buffer[i] = 0;
	}

}

Texture::Texture(int width, int height) {

	this->height = height;
	this->width = width;
	generate_buffer();
}

Texture::Texture(Texture* tex) {
	
	height = tex->height;
	width = tex->width;
	generate_buffer();

	copy_image(tex,0,0);
}

Texture::Texture(const char* path) {

	int nrChannels;

	pixel* data = (pixel*)stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);

	generate_buffer();

	if (data) {
		copy_image_raw(data,width,height,0,0);
	}
	else {
		std::cerr << "Failed to load " << path << std::endl;
	}
}

Texture::~Texture() {

	if (buffer) {
		stbi_image_free(buffer);
	}
}

void Texture::copy_image_raw(pixel* input, int in_width, int in_height, int x, int y) {

	int buffer_x = x;
	int buffer_y = y;

	int scanline_size = this->width;

	//std::cout << buffer_x << "," << buffer_y << "," << scanline_size << std::endl;

	for (int input_y = 0; input_y < in_height; input_y++) {
		//Copy a scanline of the image into the buffer

		for (int input_x = 0; input_x < in_width; input_x++) {

			//std::cout << "Moving " << (width * input_y) + input_x << " to " << (scanline_size * buffer_y) + buffer_x << std::endl;

			buffer[(scanline_size * buffer_y) + buffer_x] = input[(in_width * input_y) + input_x];

			//std::cout << (int)buffer[(scanline_size * buffer_y) + buffer_x] - (int)input[(width * input_y) + input_x] << ",";

			//Check to ensure we're not going off the right side of the buffer
			buffer_x++;
			if (buffer_x >= scanline_size) {
				break;
				//std::cout << "Went off right side" << std::endl;
			}
		}

		//Check to ensure that we're not trying to go off the bottom of the buffer
		buffer_y++;
		if (buffer_y >= scanline_size) {
			break;
		}

		buffer_x = x;
	}
}

void Texture::copy_image(Texture* tex, int x, int y) {
	copy_image_raw(tex->buffer, tex->width, tex->height, x, y);
}

void Texture::print() {

	for (int i = 0; i < width; i++) {

		for (int j = 0; j < width; j++) {

			std::cout << std::hex << buffer[(width * i) + j];
		}

		std::cout << std::endl;
	}
}