
#include <iostream>
#include <fstream>
#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <GL/GL.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "client/graphics/texture_atlas.h"
#include "client/graphics/sprite.h"
#include "client/graphics/sprite_render.h"
#include "common/physics.h"
#include "client/playercontrol.h"
#include "client/control.h"

#include "client/Client.h"
#include <mutex>
#include <thread>

#include <queue>

//TODO: Cross compatability
#include <windows.h>

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

std::queue<MoveCommand> actionqueue;

//TODO: Intermediate layer to allow setting keybinds
//Keyboard input handler
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key) {

		case GLFW_KEY_W:
			if (action == GLFW_PRESS) {
				actionqueue.push(MoveCommand::UpStart);
			}
			else if (action == GLFW_RELEASE) {
				actionqueue.push(MoveCommand::UpStop);
			}
			break;

		case GLFW_KEY_A:
			if (action == GLFW_PRESS) {
				actionqueue.push(MoveCommand::LeftStart);
			}
			else if (action == GLFW_RELEASE) {
				actionqueue.push(MoveCommand::LeftStop);
			}
			break;

		case GLFW_KEY_S:
			if (action == GLFW_PRESS) {
				actionqueue.push(MoveCommand::DownStart);
			}
			else if (action == GLFW_RELEASE) {
				actionqueue.push(MoveCommand::DownStop);
			}
			break;

		case GLFW_KEY_D:
			if (action == GLFW_PRESS) {
				actionqueue.push(MoveCommand::RightStart);
			}
			else if (action == GLFW_RELEASE) {
				actionqueue.push(MoveCommand::RightStop);
			}
			break;

		default: return;
	}
}

//Returns a timestamp in msecs
//TODO: Cross compatability
double get_time_ms() {

	LARGE_INTEGER t, freq;

	//Ticks
	QueryPerformanceCounter(&t);
	//Ticks per second
	QueryPerformanceFrequency(&freq);
	return (double)(t.QuadPart / (double)freq.QuadPart);
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

	//Set up keyboard control callback
	glfwSetKeyCallback(window, key_callback);

	glClearColor(0.f, 0.f, 0.f, 1.f);

	stbi_set_flip_vertically_on_load(true);
	TextureAtlas atlas = TextureAtlas(256, 3, "assets/debug/daddy.png");
	atlas.add_image("assets/debug/gradient.png");
	atlas.add_image("assets/entities/wiz.png");

	World world;
	
	world.add_manager<Sprite>();
	world.add_manager<Transform>();
	world.add_manager<PhysicsComp>();
	world.add_manager<PlayerControl>();

	SpriteRenderSystem renderer = SpriteRenderSystem(&world, window, &atlas);
	world.subscribe_system<Transform>(&renderer);
	world.subscribe_system<Sprite>(&renderer);
	
	PhysicsSystem physics = PhysicsSystem(&world);
	world.subscribe_system<Transform>(&physics);
	world.subscribe_system<PhysicsComp>(&physics);
	
	ControlSystem control = ControlSystem(&world, &actionqueue);
	world.subscribe_system<PhysicsComp>(&control);
	world.subscribe_system<PlayerControl>(&control);

	EntityHandle test = world.create_entity();
	test.add<Sprite>(new Sprite(&atlas, "entities/wiz"));
	test.add<Transform>(new Transform(100.f,100.f));
	test.add<PhysicsComp>(new PhysicsComp{glm::vec2(0.0f,0.f),glm::vec2(0.0f,0.0f) });
	test.add<PlayerControl>(new PlayerControl{ 50.f, 0.2f, glm::vec2(0.f,0.f) });

	EntityHandle ptest = world.create_entity();
	ptest.add<Sprite>(new Sprite(&atlas, "debug/gradient"));
	ptest.add<Transform>(new Transform(0.f, 0.f));

	EntityHandle ptest1 = world.create_entity();
	ptest1.add<Sprite>(new Sprite(&atlas, "debug/gradient"));
	ptest1.add<Transform>(new Transform(300.f, 0.f));

	EntityHandle ptest2 = world.create_entity();
	ptest2.add<Sprite>(new Sprite(&atlas, "FUCK"));
	ptest2.add<Transform>(new Transform(0.f, 300.f));

	renderer.set_camera(100.f,100.f);
	renderer.focus_entity = &test;

	double t = 0.0;
	double dt = 1.0 / 60.0;

	double current_time = get_time_ms();
	double accumulator = 0.0;

	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		
		//TODO: Linear interpolation of frames based on accumulator
		//Should make things move smoother
		// https://www.gafferongames.com/post/fix_your_timestep/

		double new_time = get_time_ms();
		double frame_time = (new_time - current_time);
		current_time = new_time;

		accumulator += frame_time;

		while (accumulator >= dt) {
			
			control.update(dt);
			physics.update(dt);
			
			accumulator -= dt;
			t += dt;
		}

		//Render
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