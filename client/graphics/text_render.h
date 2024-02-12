#pragma once

#include "ecs.h"
#include "camera.h"
#include "sprite.h"
#include "transform.h"

struct TextComp {
    
    std::string str;
    unsigned int VAO = 0;
    bool VAO_generated = false;
};

struct Character{
    glm::vec2 texture_coords;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

struct Font{
    unsigned int texture;
    TextureAtlas* atlas;
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
	
		TextRenderSystem(ECSHandle* handle, Camera* camera);
		void render();
        void generate_VAO(TextComp* comp);
};