#pragma once

#include "render.h"
#include <iostream>
#include <fstream>
#include "glad\glad.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stb_image.h>

//Function to get shader file data
std::string get_file(const char* filename) {

	std::ifstream file;
	file.open(filename, std::ifstream::in);

	std::string output;

	//Catch failed file open
	if (file.fail()) return output;

	//Get everything from the file
	char c = file.get();
	while (file.good()) {
		output.push_back(c);
		c = file.get();
	}

	return output;
}

//Function to make shader program
unsigned int get_shaders() {

	//Set up shaders

	//Get and compile vertex shader from source
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::string vertexSource = get_file("shaders/basic.vert");
	if (vertexSource.length() <= 0) {
		std::cout << "Vertex shader could not be loaded!" << std::endl;
		exit(1);
	}
	const char* vertexShaderSource = vertexSource.c_str();

	int  err;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &err);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	if (err)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		exit(1);
	}

	//Get an compile fragment shader from source
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string fragSource = get_file("shaders/basic.frag");

	if (fragSource.length() <= 0) {
		std::cout << "Fragment shader could not be loaded!" << std::endl;
		exit(1);
	}

	const char* fragmentShaderSource = fragSource.c_str();

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &err);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	if (err)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		exit(1);
	}

	//Put the final program together
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Clean up
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

//Sets up texture
unsigned int setup_texture(TextureAtlas* atlas) {

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas->scanline_size, atlas->scanline_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas->texture->buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	//stbi_image_free(data);
	return texture;
}

RenderSystem::RenderSystem(GLFWwindow* window, TextureAtlas* atlas, ComponentManager<Sprite>* sprites, ComponentManager<Transform>* positions) {

	//Set up height and width
	this->window = window;
	glfwGetFramebufferSize(window, &win_width, &win_height);

	//Set up shaders
	this->shader = get_shaders();

	//Set up textures
	this->texture = setup_texture(atlas);

	//View matrix
	this->projection = glm::ortho(0.0f, (float)win_width, (float)win_height, 0.0f, -1.0f, 1.0f);

	this->sprites = sprites;
	this->positions = positions;
}

void RenderSystem::update(float delta_t) {
	return;
}

void RenderSystem::render() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(shader);

	//Set projection matrix uniform
	int project_loc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(projection));
	
	//Draw all sprites in the sprites
	for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it) {

		Sprite* sprite = sprites->get_component(*it);
		Transform* position = positions->get_component(*it);

		if(sprite && position){

			//Set transform matrix
			int transform_loc = glGetUniformLocation(shader, "transform");
			glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(position->transform));

			//Bind the vertex array and draw the sprite
			glBindVertexArray(sprite->VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		}
	}

}