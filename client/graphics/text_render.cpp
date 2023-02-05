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
	}

    //Generate font
	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/ComicNeue-Regular.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

    return face;
}

Font generate_font(){

    const int height = 48;
    FT_Face face = initialize_fonts();
    FT_Set_Pixel_Sizes(face, 0, height);

    TextureAtlas atlas = TextureAtlas(256, 10, "assets/debug/daddy.png");

    Font new_font = {0, std::map<char, Character>()};

    //Start with space, go from there
    for(char c = 32; c < 127; c++){

        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        }

        //Add to texture atlas
        std::string str = std::string(1,c);
		//printf("Buffer in text_renderer: %x\n", face->glyph->bitmap.buffer);
        atlas.add_image(face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, str);

        //Store character data
        Character character;
        character.texture_coords = atlas.get_coords(&c);
        character.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        character.bearing= glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        character.advance = face->glyph->advance.x;

        new_font.characters[c] = character;
    }

    //Turn the atlas into a texture
    new_font.texture = texture_from_atlas(&atlas);

    return new_font;
}

TextRenderSystem::TextRenderSystem(World* world, Camera* camera): System(world) {

	this->camera = camera;

	//Set up shaders
	this->shader = get_shaders("sprite");

    this->font = generate_font();

	generate_VAO();
}

void TextRenderSystem::generate_VAO() {

	//Quad
	float vertices[] = {
		// positions        // texture coords
		-500.f, -500.f, 0.f, 0.f, 1.f,   // top left
		500.f, -500.f, 0.f, 1.f, 1.f, // top right
		500.f, 500.f, 0.f, 1.f, 0.f// bottom right
		-500.f, 500.f, 0.f, 0.f, 0.f// Bottom left
	};

	//Indices for triangles
	unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//Will only be a static image without this being updated
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &this->VAO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Set up the attribute pointers for the shaders
	// X, Y, Z, S, T
	//Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Textures
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void TextRenderSystem::render() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font.texture);
	glUseProgram(shader);

	//Set projection matrix uniform
	int project_loc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(camera->projection));

	//Set up camera matrix
	int camera_loc = glGetUniformLocation(shader, "camera_view");
	glUniformMatrix4fv(camera_loc, 1, GL_FALSE, glm::value_ptr(camera->camera_view));

	//Set transform matrix
	int transform_loc = glGetUniformLocation(shader, "transform");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));

	//Bind the vertex array and draw the sprite
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}