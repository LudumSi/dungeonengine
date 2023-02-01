#pragma once

#include <stdint.h>

typedef uint32_t pixel;

class Texture {

	private:
		//Generates an empty pixel buffer to work on
		void generate_buffer();

		void copy_image_raw(pixel* buffer,int width,int height,int x, int y);
		
	public:
		pixel* buffer;
		int width;
		int height;
		int channels;

		//Construct a blank texture with the given height and width
		Texture(int width,int height);
		//Texture copy constructor
		Texture(Texture* tex);
		//Construct a texture from a loaded file
		//Not guranteed to work, check loaded
		Texture(const char* path);
		//Construct a texture from a bitmap buffer
		Texture(unsigned char* bitmap, int width, int height);
		
		~Texture();

		//Copy a texture onto this texture, at the given pixel coordinates
		void copy_image(Texture* tex, int x, int y);

		//Debug function to print out a texture as hexadecimal
		void print();
};