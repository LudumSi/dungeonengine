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
		new_map[(4*i)+3] = old_map[i];
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

    const int height = 64;
    FT_Face face = initialize_fonts(lib);
    FT_Set_Pixel_Sizes(face, 0, height);

    TextureAtlas* atlas = new TextureAtlas(256, 10, "assets/debug/daddy.png");

    Font new_font = {0, atlas, std::map<char, Character>()};

    //Start with space, go from there
    for(char c = 32; c < 127; c++){

        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        }

        //Add to texture atlas

        std::string str(1, c);

		int glyph_width = face->glyph->bitmap.width;
		int glyph_height = face->glyph->bitmap.rows;
		unsigned char* buffer = face->glyph->bitmap.buffer;

		//Need to change image to a four-byte representation rather than the default 1-byte
		buffer = bitmap_convert_1bpp_rgba(buffer, glyph_width, glyph_height);

		//OpenGL uses images flipped on the Y axis for some reason
		buffer = bitmap_reverse_rgba(buffer, glyph_width, glyph_height);

        atlas->add_image(buffer, glyph_width, glyph_height, str);

        //Store character data
        Character character;
        character.texture_coords = atlas->get_coords(str.c_str());
        character.size = glm::ivec2(glyph_width, glyph_height);
        character.bearing= glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        character.advance = face->glyph->advance.x;

        new_font.characters[c] = character;
    }

    //Turn the atlas into a texture
    new_font.texture = texture_from_atlas(atlas);
	new_font.atlas = atlas;

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
}

void TextRenderSystem::generate_VAO(TextComp* comp){

    //Generate arrays
	std::vector<float> vertices;
	std::vector<int> indices;

	float advance = 0.f;

	for(int i = 0; i < comp->str.length(); i++){
		
		char c = comp->str[i];

		//Get texture coordinates
		Character glyph = font.characters[c];
		glm::vec2 tex_coords = glyph.texture_coords;
		float side_size = font.atlas->scanline_size;
		glm::vec2 tex_size = glm::vec2(glyph.size[0]/side_size,glyph.size[1]/side_size);
		glm::ivec2 bearing = glyph.bearing;

		float origin_x = advance+(float)bearing[0];
		float origin_y = (float)(glyph.size[1]-bearing[1]);
		float offset_x = (float)glyph.size[0];
		float offset_y = -1.f*(float)glyph.size[1];

		//Quad generation
		vertices.insert(vertices.end(),
			{
			// positions                                         // texture coords
			origin_x,          origin_y,          0.f, tex_coords[0],             tex_coords[1],   // bottom left
			origin_x+offset_x, origin_y,          0.f, tex_coords[0]+tex_size[0], tex_coords[1],   // bottom right
			origin_x+offset_x, origin_y+offset_y, 0.f, tex_coords[0]+tex_size[0], tex_coords[1]+tex_size[1], // top right
			origin_x,          origin_y+offset_y, 0.f, tex_coords[0],             tex_coords[1]+tex_size[1], // top left
			}
		);

		advance += (float)glyph.advance/64.f;

		//Indices for triangles
		int vertex_offset = i*4;
		indices.insert(indices.end(),
			{
			vertex_offset,   vertex_offset+1, vertex_offset+3, // first triangle
			vertex_offset+1, vertex_offset+2, vertex_offset+3  // second triangle
			}
		);
	}

	//Generate VAO from the arrays
    unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &comp->VAO);

	glBindVertexArray(comp->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);

	//Set up the attribute pointers for the shaders
	// X, Y, Z, S, T
	//Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Textures
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	comp->VAO_generated = true;
}

void TextRenderSystem::render() {

	glActiveTexture(GL_TEXTURE0);
	//Only one font for now, but might look into sorting batches of chars by font
	glBindTexture(GL_TEXTURE_2D, font.texture);
	glUseProgram(shader);

	//Set projection matrix uniform
	int project_loc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(camera->projection));

	//Set up camera matrix uniform
	int camera_loc = glGetUniformLocation(shader, "camera_view");
	glUniformMatrix4fv(camera_loc, 1, GL_FALSE, glm::value_ptr(camera->camera_view));
	
	//Draw all sprites in the sprites
	for (auto & entity : entities) {

		TextComp* text = world->get_component<TextComp>(entity);
		Transform* position = world->get_component<Transform>(entity);

		if(!text->VAO_generated){
			generate_VAO(text);
		}

		//Set transform matrix
		int transform_loc = glGetUniformLocation(shader, "transform");
		glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(position->transform));

		//Bind the vertex array and draw the sprite
		glBindVertexArray(text->VAO);
		glDrawElements(GL_TRIANGLES, text->str.length() * 6, GL_UNSIGNED_INT, 0);
	}
}