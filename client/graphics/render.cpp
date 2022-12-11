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

	atlas->flush();
	return texture;
}

RenderSystem::RenderSystem(GLFWwindow* window, TextureAtlas* atlas, ComponentManager<Sprite>* sprites, ComponentManager<Transform>* positions) {

	//Set up height and width
	this->window = window;
	glfwGetFramebufferSize(window, &win_width, &win_height);

	//Set up shaders
	this->shader = get_shaders();

	//Set up textures
	if(!atlas->flushed){
		this->texture = setup_texture(atlas);
	}else{
		printf("Failed to create render system; atlas has already been consumed\n");
	}

	//View matrix
	//TODO: Fix things so that the Y-axis works as traditionally expected
	this->projection = glm::ortho(-1.f * (float)win_width/2, (float)win_width/2, (float)win_height/2, -1.f * (float)win_height/2, -1.0f, 1.0f);

	this->sprites = sprites;
	this->positions = positions;

	this->focus_entity = NULL;
	this->camera_position = glm::vec3(0.f,0.f,0.f);
}

void RenderSystem::set_camera(float x, float y){
	this->camera_position = glm::vec3(x,y,0.f);
}

void RenderSystem::update(float delta_t) {
	return;
}

void RenderSystem::render() {

	//Clear previous frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(shader);

	//Set projection matrix uniform
	int project_loc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(projection));

	//Set up camera view matrix
	glm::mat4 camera_matrix = glm::mat3(1.0);

	if(focus_entity){
		if(focus_entity->has<Transform>()){
			camera_position = focus_entity->get<Transform>()->get_position();
		}
	}

	camera_matrix = glm::translate(camera_matrix, (-1.f * camera_position));
	int camera_loc = glGetUniformLocation(shader, "camera_view");
	glUniformMatrix4fv(camera_loc, 1, GL_FALSE, glm::value_ptr(camera_matrix));
	
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
