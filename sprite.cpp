
#include "sprite.h"
#include "glad\glad.h"
#include "glm/gtc/type_ptr.hpp"

Sprite::Sprite() {

}

Sprite::Sprite(TextureAtlas* atlas, const char* texture) {

	this->atlas = atlas;

	this->tex_coords = atlas->get_coords(texture);

	this->transform = glm::mat4(1.0);

	//Generate a VAO for the sprite
	generate_VAO();
}

void Sprite::generate_VAO() {

	//Set up a quad

	float side_size = (float)atlas->resolution / (float)atlas->scanline_size;

	//Quad
	float vertices[] = {
		// positions        // texture coords
		0.f, 0.f, 0.f,		tex_coords[0], tex_coords[1]+side_size,   // top left
		100.f, 0.f, 0.f,    tex_coords[0]+side_size, tex_coords[1]+side_size,   // top right
		100.f, 100.f, 0.f,  tex_coords[0]+side_size, tex_coords[1],   // bottom right
		0.f, 100.f, 0.f,    tex_coords[0], tex_coords[1],   // Bottom left
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

void Sprite::draw(unsigned int shaderProgram) {

	//Set transform matrix
	int transform_loc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(this->transform));

	//Bind the vertex array and draw the sprite
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	return;
}

void Sprite::translate(float x, float y) {

	this->transform = glm::translate(this->transform,glm::vec3(x,y,0.f));

}

void Sprite::rotate_about_deg(float theta, float x, float y) {

	this->transform = glm::translate(this->transform, glm::vec3(x,y,0.f));
	float rads = 3.141592 / 180.f * theta;
	this->transform = glm::rotate(this->transform,rads,glm::vec3(0,0,1));
	this->transform = glm::translate(this->transform, glm::vec3(-1.f*x, -1.f*y, 0.f));
}