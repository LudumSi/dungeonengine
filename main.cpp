#include <iostream>
#include <fstream>
#include "glad\glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW\glfw3.h"
#include <GL\GL.h>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture_atlas.h"
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

	glClearColor(0.f, 0.f, 0.f, 1.f);

	stbi_set_flip_vertically_on_load(true);
	TextureAtlas atlas = TextureAtlas(256, 3, "assets/debug/daddy.png");
	atlas.add_image("assets/debug/gradient.png");
	atlas.add_image("assets/entities/wiz.png");

	World world;
	EntityManager emanager;
	ComponentManager<Sprite> spritemanager;
	ComponentManager<Transform> tmanager;
	RenderSystem renderer = RenderSystem(window, &atlas, &spritemanager, &tmanager);

	world.add_manager<Sprite>(&spritemanager);
	world.add_manager<Transform>(&tmanager);
	world.subscribe_system<Sprite>(&renderer);
	world.subscribe_system<Transform>(&renderer);

	Sprite test_sprite = Sprite(&atlas, "assets/entities/wiz.png");

	EntityHandle test = world.create_entity();
	test.add<Sprite>(&test_sprite);
	test.add<Transform>(new Transform(100.f,100.f));

	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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