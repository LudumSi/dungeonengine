#ifndef DUNGEON_TEXTURE_H
#define DUNGEON_TEXTURE_H

#include <stdint.h>

typedef uint32_t pixel;

class Texture {

	private:
		//Generates an empty pixel buffer to work on
		void generate_buffer();

		void copy_image_raw(pixel*,int,int,int,int);
		
	public:
		pixel* buffer;
		int width;
		int height;
		int channels;

		//Construct a blank texture with the given height and width
		Texture(int,int);
		//Texture copy constructor
		Texture(Texture*);
		//Construct a texture from a loaded file
		//Not guranteed to work
		Texture(const char*);
		
		~Texture();

		//Copy a texture onto this texture, at the given pixel coordinates
		void copy_image(Texture*, int, int);

		//Debug function to print out a texture as hexadecimal
		void print();
};

#endif