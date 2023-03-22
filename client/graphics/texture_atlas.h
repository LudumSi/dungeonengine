#pragma once

#include <map>
#include <string>
#include "glm/glm.hpp"
#include "texture.h"

class TextureAtlas {

	private:

		//2D array showing which chunks have already been assigned
		// Will be useful when we're doing larger textures
		//bool** assignment_grid;

		//Dictionary of texture names to atlas coordinates
		std::map<std::string, glm::vec2> atlas;

		//Next available texture location
		int next_x;
		int next_y;

		//Total size in chunks of the atlas
		int side_len;

		void add_image(Texture texture, std::string name);

	public:

		TextureAtlas(int resolution, int side_len, const char* missing_tex_path);
		~TextureAtlas();

		//Whether or not the texture atlas has been flushed
		bool flushed;

		//Length of a single scanline
		int scanline_size;

		//Texture buffer
		Texture* texture;

		//Size in pixels of each square "chunk" of the atlas
		int resolution;
		
		//Function to add an image file to the atlas
		void add_image(const char* path, std::string name);
		void add_image(const char* path);

		void add_image(unsigned char* buffer, int width, int height, std::string name);

		//Function to retrieve texture coordinates from the atlas based on texture name
		//Eventually should spit out a missing texture if not recognizes
		glm::vec2 get_coords(const char* name);

		//Function to flush the texture data from the atlas, to reduce memory usage
		void flush();
};
