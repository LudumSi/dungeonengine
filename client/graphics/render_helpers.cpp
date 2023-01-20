#include "render_helpers.h"

#include "glad\glad.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

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
//shader_name is prefix to the .vert and .frag shaders
unsigned int get_shaders(const char* shader_name) {

	//Set up shaders

    //Set up shader names
    std::string vert_name = "shaders/" + std::string(shader_name) + ".vert";
    std::string frag_name = "shaders/" + std::string(shader_name) + ".frag";

	//Get and compile vertex shader from source
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::string vertexSource = get_file(vert_name.c_str());
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

	std::string fragSource = get_file(frag_name.c_str());

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
unsigned int texture_from_atlas(TextureAtlas* atlas) {

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