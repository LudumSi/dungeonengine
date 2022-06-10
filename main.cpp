#include <iostream>
#include <fstream>
#include "glad\glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW\glfw3.h"
#include <GL\GL.h>

using namespace std;

//Hello triangle triangle
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

//Function to get shader file data
string get_file(const char* filename) {
	
	ifstream file;
	file.open(filename,ifstream::in);

	string output;

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

	string vertexSource = get_file("basic.vert");
	if (vertexSource.length() <= 0) { 
		cout << "Vertex shader could not be loaded!" << endl; 
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

	string fragSource = get_file("basic.frag");

	if (fragSource.length() <= 0) {
		cout << "Fragment shader could not be loaded!" << endl;
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

//GLFW error callback function
void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main() {

	//Initialize GLFW
	if (!glfwInit())
	{
		cout << "GLFW Initialization failed!" << endl;
		return 1;
	}

	//Set up error callback for GLFW
	glfwSetErrorCallback(error_callback);

	//Create a window
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hey, Ben. I made a window.", NULL, NULL);
	if (!window)
	{
		cout << "Window initialization failed" << endl;
	}

	//Make the context the current context
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSwapInterval(1);

	//Set up viewport size
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	//TODO: Setup resize callback

	//Set up VBO/VAO object

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Will only be a static image without this being updated

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Set up shaders
	unsigned int shaderProgram = get_shaders();

	//Main loop
	while (!glfwWindowShouldClose(window))
	{

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Destroy the window
	glfwDestroyWindow(window);

	//Shut down GLFW
	glfwTerminate();

	return 0;
}