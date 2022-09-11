#include <iostream>
#include <fstream>
#include "glad\glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW\glfw3.h"
#include <GL\GL.h>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "texture_atlas.h"
#include "ecstest.h"
#include "sprite.h"
#include "render.h"

#include "Client.h"
#include <mutex>
#include <thread>

int win_width = 640;
int win_height = 480;

using namespace std;

//GLFW error callback function
void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

//Window resize handler
void window_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	//glfwGetFramebufferSize(window, &win_width, &win_height);
	//Will have to figure out how to scale while preseserving aspect ratios
}

int main() {

	//ecstest();
	/*
	std::string ip;
	int port;
	std::cout << "Enter IP: ";
	std::cin >> ip;

	std::cout << "Enter Port: ";
	std::cin >> port;

	Client c(port);

	c.connect(ip.c_str());
	c.bind_listener();
	
	std::thread th_snd(send_func, &c);
	std::thread th_rcv(recv_func, &c);

	th_snd.join();
	th_rcv.join();
	*/

	//Initialize GLFW
	if (!glfwInit())
	{
		cout << "GLFW Initialization failed!" << endl;
		return 1;
	}

	//Set up error callback for GLFW
	glfwSetErrorCallback(error_callback);

	//Create a window
	GLFWwindow* window = glfwCreateWindow(win_width, win_height, "Hey, Ben. I made a window.", NULL, NULL);
	if (!window)
	{
		cout << "Window initialization failed" << endl;
	}

	//Make the window the current context
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSwapInterval(1);

	//Set up viewport size
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	//Set up resize callback
	glfwSetWindowSizeCallback(window, window_resize_callback);

	World world;
	EntityManager emanager;
	ComponentManager<Sprite> spritemanager;
	RenderSystem renderer = RenderSystem(window);

	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		renderer.render();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Destroy the window
	glfwDestroyWindow(window);

	//Shut down GLFW
	glfwTerminate();

	return 0;
}