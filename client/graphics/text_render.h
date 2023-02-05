#pragma once

#include "ecs.h"
#include "camera.h"
#include "sprite.h"
#include "transform.h"

struct Character{
    glm::vec2 texture_coords;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

struct Font{
    unsigned int texture;
    std::map<char, Character> characters;
};

class TextRenderSystem : public System {

	private:
		
		unsigned int shader;
		unsigned int texture;
		Camera* camera;
        Font font;
        unsigned int VAO;

	public:
	
		TextRenderSystem(World* world, Camera* camera);
		void render();
        void generate_VAO();
};