#ifndef SPRITE
#define SPRITE

#include "texture_atlas.h"

class Sprite {

	private:
		//Texture atlas to pull texture coords from
		TextureAtlas* atlas;

		//Texture coordinates of top left of sprite
		glm::vec2 tex_coords;

		//Transformation matrix for the sprite (Top left)
		glm::mat4 transform;

		//VAO for the sprite
		unsigned int VAO;

		//Contains texture information
		void generate_VAO();

	public:
		Sprite();
		Sprite(TextureAtlas* atlas, const char* texture);

		void set_texture(const char* texture);
		//Translate the sprite to another location
		void translate(float x, float y);
		//Rotate the sprite around a point
		void rotate_about_deg(float theta, float x, float y);
		void rotate_deg(float theta);
		void scale(float factor);

		void draw(unsigned int shaderProgram);
};

#endif