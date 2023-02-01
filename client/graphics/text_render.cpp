#include "text_render.h"
#include "render_helpers.h"
#include "glad\glad.h"
#include "glm\gtc\type_ptr.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H  

FT_Face initialize_fonts(){
    
    //Initialize FreeType
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return 1;
	}

    //Generate font
	FT_Face face;
	if (FT_New_Face(ft, "assets/fonst/ComicNeue-Regular.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
		return -1;
	}

    return face;
}

Font generate_font(){

    const int height = 48;
    FT_Face face = initialize_fonts();
    FT_Set_Pixel_Sizes(face, 0, height);

    TextureAtlas atlas = TextureAtlas(2*width, 10, "assets/debug/daddy.png");

    Font new_font;

    //Start with space, go from there
    for(char c = 32; c < 128; c++){

        if (FT_Load_Char(face, c), FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            return 1;
        }

        //Add to texture atlas
        atlas.add_bitmap(face->glyph->bitmap, 2*height, 2*height, std::string(c));

        //Store character data
        Character character;
        character.texture_coordinates = atlas.get_coords(c);
        character.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        character.bearing= glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        character.advance = face->glyph->advance.x;

        font.characters[c] = character;
    }

    //Turn the atlas into a texture
    //TODO: Connect this up to OpenGL
}

TextRenderSystem::TextRenderSystem(World* world, Camera* camera): System(world) {

	this->camera = camera;

	//Set up shaders
	this->shader = get_shaders("sprite");

	//Set up textures
	if(!atlas->flushed){
		this->texture = texture_from_atlas(atlas);
	}else{
		printf("Failed to create render system; atlas has already been consumed\n");
	}
}

void TextRenderSystem::render() {

	//Clear previous frame
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(shader);

	//Set projection matrix uniform
	int project_loc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(camera->projection));

	//Set up camera view matrix
	glm::mat4 camera_matrix = glm::mat3(1.0);

	int camera_loc = glGetUniformLocation(shader, "camera_view");
	glUniformMatrix4fv(camera_loc, 1, GL_FALSE, glm::value_ptr(camera->camera_view));
	
	//Draw all sprites in the sprites
	for (auto & entity : entities) {

		Sprite* sprite = world->get_component<Sprite>(entity);
		Transform* position = world->get_component<Transform>(entity);

		if(!sprite || !position) continue;

		//Set transform matrix
		int transform_loc = glGetUniformLocation(shader, "transform");
		glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(position->transform));

		//Bind the vertex array and draw the sprite
		glBindVertexArray(sprite->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}