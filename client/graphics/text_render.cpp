#include "text_render.h"
#include "render_helpers.h"
#include "glad\glad.h"
#include "glm\gtc\type_ptr.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

//Convert 1bpp bitmap output to RGBA
unsigned char* bitmap_convert_1bpp_rgba(unsigned char* old_map, int px_width, int px_height){
 
	unsigned char* new_map = new unsigned char[4*px_height*px_width];

	for(int i = 0; i < px_height*px_width;i++){
		new_map[4*i] = old_map[i];
		new_map[(4*i)+1] = old_map[i];
		new_map[(4*i)+2] = old_map[i];
		new_map[(4*i)+3] = (char)1.f;
	}

	return new_map;
}

//Reverses a 4bpp bitmap row by row
unsigned char* bitmap_reverse_rgba(unsigned char* old_map, int px_width, int px_height){
	
	unsigned char* new_map = new unsigned char[px_height*px_width*4];

	int max = (px_height - 1) * px_width * 4;

	for(int i = 0; i < px_height;i++){
		int offset = i * px_width * 4;
		memcpy(new_map + offset, old_map + max - offset, px_width * 4);
	}

	return new_map;
}

FT_Face initialize_fonts(FT_Library lib){

    //Generate font
	FT_Face face;
	if (FT_New_Face(lib, "assets/fonts/ComicNeue-Regular.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

    return face;
}

Font generate_font(FT_Library lib){

    const int height = 256;
    FT_Face face = initialize_fonts(lib);
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

		int glyph_width = face->glyph->bitmap.width;
		int glyph_height = face->glyph->bitmap.rows;
		unsigned char* buffer = face->glyph->bitmap.buffer;

		//Need to change image to a four-byte representation rather than the default 1-byte
		buffer = bitmap_convert_1bpp_rgba(buffer, glyph_width, glyph_height);

		//OpenGL uses images flipped on the Y axis for some reason
		buffer = bitmap_reverse_rgba(buffer, glyph_width, glyph_height);

		printf("String: %s\n", str);
        atlas.add_image(buffer, glyph_width, glyph_height, str);

        //Store character data
        Character character;
        character.texture_coords = atlas.get_coords(&c);
        character.size = glm::ivec2(glyph_width, glyph_height);
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

	//Initialize FreeType
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

    this->font = generate_font(ft);

	generate_VAO();
}

void TextRenderSystem::generate_VAO() {

	//Quad
	float vertices[] = {
		// positions        // texture coords
		-500.f, -500.f, 0.f, 0.f, 1.f,   // top left
		500.f, -500.f, 0.f, 1.f, 1.f, // top right
		500.f, 500.f, 0.f, 1.f, 0.f,// bottom right
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