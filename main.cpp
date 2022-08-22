#include <iostream>
#include <fstream>
#include "glad\glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW\glfw3.h"
#include <GL\GL.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture_atlas.h"
#include "ecstest.h"

#include "Client.h"
#include <mutex>
#include <thread>

using namespace std;

//Hello triangle triangle
float vertices[] = {
	// positions          // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
};

//Indices for triangles
unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
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

	string vertexSource = get_file("shaders/basic.vert");
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

	string fragSource = get_file("shaders/basic.frag");

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

//Sets up sexture
unsigned int setup_texture() {
	
	TextureAtlas atlas = TextureAtlas(256,3,"assets/debug/daddy.png");

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	atlas.add_image("assets/debug/gradient.png");
	atlas.add_image("assets/entities/wiz.png");
	atlas.add_image("assets/entities/wiz.png");
	atlas.add_image("assets/entities/wiz.png");
	atlas.add_image("assets/entities/wiz.png");
	atlas.add_image("assets/debug/gradient.png");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas.scanline_size, atlas.scanline_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas.texture->buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	//stbi_image_free(data);
	return texture;
}

//GLFW error callback function
void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void send_func(Client * c) {
	std::string msg;
	while (true) {
		std::cout << "Input Message. Q to quit: ";
		getline(std::cin, msg);
		if (msg == "Q") return;
		strcpy_s(c->out_buf, msg.c_str());
		c->send();
	}
}

void recv_func(Client* c) {
	while (true) {
		c->recv();
	}
}

int main() {

	//ecstest();
	std::string ip;
	int port;
	std::cout << "Enter IP: ";
	std::cin >> ip;

	std::cout << "Enter Port: ";
	std::cin >> port;

	Client c(port);

	c.connect(ip.c_str());
	
	std::thread th_snd(send_func, &c);
	std::thread th_rcv(recv_func, &c);

	th_snd.join();
	th_rcv.join();



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

	//Will only be a static image without this being updated
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
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

	//Set up shaders
	unsigned int shaderProgram = get_shaders();

	//Set up textures
	stbi_set_flip_vertically_on_load(true);
	unsigned int texture = setup_texture();

	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Destroy the window
	glfwDestroyWindow(window);

	//Shut down GLFW
	glfwTerminate();

	return 0;
}