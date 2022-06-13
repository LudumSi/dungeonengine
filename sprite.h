#ifndef SPRITE_H
#define SPRITE_H

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <vector>
#include <map>

//Everything in this file will eventually be broken up into components and a rendering system
//This is a test to play around with effeciently rendering multiple sprites

//Resolution of sprites
#define SPRITE_SIZE 64

//Sprite class.
class Sprite {

	public:
		//Position, rotation, etc. of the sprite in world coordinates
		glm::mat3 position;
		
		//Size of the sprite
		float height, width;

		//Texture file of the sprite
		std::string filename;
};

//Sprite manager class
class SpriteManager {

	public:
		//Vector of sprite objects
		std::vector<Sprite> sprites;

		//Hash table of sprite names to texture coordinates
		std::map<std::string, glm::vec2> atlas;

		//Goes through all sprite objects and builds a new texture atlas, binding it to the 
		void build_atlas();
};

#endif
