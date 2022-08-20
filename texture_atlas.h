
#ifndef TEXTURE_ATLAS
#define TEXTURE_ATLAS

#include <map>
#include <string>
#include "glm/glm.hpp"

typedef uint32_t pixel;

class TextureAtlas {

	private:

		//2D array showing which chunks have already been assigned
		bool** assignment_grid;

		//Dictionary of texture names to atlas coordinates
		std::map<std::string, glm::vec2> atlas;

	public:
		
		//Function to add an image file to the atlas

		//Function to retrieve texture coordinates from the atlas based on texture name
		//Eventually should spit out a missing texture otherwise, and a 

		TextureAtlas(int, int);
		~TextureAtlas();

		//!!!Should be private after testing is done

		//Size in pixels of each square "chunk" of the atlas
		int resolution;

		//Total size in chunks of the atlas
		int side_len;

		//Length of a single scanline
		int scanline_size;

		//Total buffer size
		int buffer_size;

		//2D buffer of image data
		//Should be deleted once sent to the GPU
		pixel* buffer;

		//Function to add image data to a the buffer at arbitary texture coordinates
		//Frees the image data after
		void copy_image(pixel*, int, int, int, int);

		//Debug function to print out all data
		void print_image();
};

#endif
