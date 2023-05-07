
#include "sprite.h"
#include "glad\glad.h"
#include <iostream>

Sprite::Sprite(TextureAtlas* atlas, const char* texture) {

	this->atlas = atlas;

	this->tex_coords = atlas->get_coords(texture);

	//Generate a VAO for the sprite
	generate_VAO();
}

Sprite::Sprite(const Sprite& old_sprite) {

	this->atlas = old_sprite.atlas;
	this->tex_coords = old_sprite.tex_coords;
	
	generate_VAO();
}

void Sprite::generate_VAO() {

	//Set up a quad

	float side_size = (float)atlas->resolution / (float)atlas->scanline_size;
	float half_texel_offset = (1.f / (float)atlas->scanline_size) * 0.5f;

	//Quad
	float vertices[] = {
		// positions        // texture coords
		-50.f, -50.f, 0.f,		tex_coords[0]+half_texel_offset, tex_coords[1]+side_size-half_texel_offset,   // top left
		50.f, -50.f, 0.f,    tex_coords[0]+side_size-half_texel_offset, tex_coords[1]+side_size-half_texel_offset,   // top right
		50.f, 50.f, 0.f,  tex_coords[0]+side_size-half_texel_offset, tex_coords[1]+half_texel_offset,   // bottom right
		-50.f, 50.f, 0.f,    tex_coords[0]+half_texel_offset, tex_coords[1]+half_texel_offset,   // Bottom left
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