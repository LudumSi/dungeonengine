#ifndef DUNGEON_SPRITE
#define DUNGEON_SPRITE

#include "texture_atlas.h"

class Sprite {

	private:
		//Generates the Vertex Array Object for the sprite
		void generate_VAO();

	public:
		Sprite(TextureAtlas* atlas, const char* texture);
		//Copy constructor
		Sprite(const Sprite& sprite);

		//Texture atlas to pull texture coords from
		TextureAtlas* atlas;
		//Texture coordinates of top left of sprite
		glm::vec2 tex_coords;
		//VAO for the sprite
		unsigned int VAO;

		void set_texture(const char* texture);
};

#endif