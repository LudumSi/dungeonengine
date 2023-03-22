
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

#include "graphics/texture_atlas.h"
#include "graphics/sprite.h"
#include "graphics/render.h"
#include "physics.h"
#include "playercontrol.h"
#include "control.h"

#include "network/network_manager.h"
#include <mutex>
#include <thread>

#include <queue>

//TODO: Cross compatability
#include <windows.h>

int win_width = 640;
int win_height = 480;

std::mutex outbox_mutex;
std::mutex inbox_mutex;

std::queue<std::string> outbox;
std::queue<std::string> inbox;

using namespace std;

/* 
void process_inbox() {
    std::string message;
    do {
        message = "";
        
        inbox_mutex.lock();
        
        if(!inbox.empty()) {
            message = inbox.front();
            inbox.pop();
            std::cout << message << std::endl;
        }
                
        inbox_mutex.unlock();
        
    } while(message != "logout");
}



void recv_clients(SOCKET* server_socket, sockaddr_in* client) {
	std::string message;
	int client_length = 0;
	char buf[1024];

	do {
		// Wait for messages from clients
		client_length = sizeof(*client);
		ZeroMemory(client, client_length);
		
		ZeroMemory(buf, 1024);

		int bytes_in = recvfrom(*server_socket, buf, 1024, 0, (sockaddr*)client, &client_length);

		if(bytes_in == SOCKET_ERROR) {
			// Error bad
			continue;
		}

		// WE SHOULD GET INFORMATION FROM THE CLIENT HERE
		// can verify things like IP and specific ids
		// Turn data into packeted info?

		if(bytes_in > 0) {
			message = buf;
			std::cout << "Received data from someone" << std::endl;
			inbox_mutex.lock();
			inbox.push(buf);
			inbox_mutex.unlock();
		}
	} while(message != "logout");
}

void send_clients(SOCKET* server_socket, sockaddr_in* client) {
    //ZeroMemory(&client, client_length);
    std::string message;
    // Serialize data here?

    // Send out data from outbox
	do {
		outbox_mutex.lock();
		if(!outbox.empty()) {
			message = outbox.front();
			std::cout << "Sending " << message << " to someone" << std::endl;

			int bytes_out = sendto(*server_socket, message.c_str(), message.length() + 1, 0, (sockaddr*)client, sizeof(*client));
			if(bytes_out == SOCKET_ERROR) {
				std::cout << "UHHHHHHHH" << std::endl;
				outbox.pop();
			} else {
				std::cout << "CLIENT: Successful send" << std::endl;
				outbox.pop();
			}
		}
		outbox_mutex.unlock();
	} while(message != "logout");
    
}
 */

void process_outbox(ConnectionManager * manager) {
    std::string message;
    do {
        printf("Input CLIENT message: ");
        getline(std::cin, message, '\n');
        manager->add_message(message, 0);

    } while(message != "logout");
}

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

	// Init Network Here
    SOCKET server_socket;
    sockaddr_in server_hints;
    int port = 7777;
	const char * ip = "192.168.254.11";


    ConnectionManager c;
	c.start(port, ip, 1);
	c.add_message("Fuck you Preston", 0);
	c.add_message("Eat my ass", 0);
	c.add_message("close", 0);
	c.add_message("logout", 0);
	c.run();

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
	EntityManager emanager;
	ComponentManager<Sprite> spritemanager;
	ComponentManager<Transform> tmanager;
	ComponentManager<PhysicsComp> physmanager;
	ComponentManager<PlayerControl> controlmanager;
	RenderSystem renderer = RenderSystem(window, &atlas, &spritemanager, &tmanager);
	PhysicsSystem physics = PhysicsSystem(&tmanager, &physmanager);
	ControlSystem control = ControlSystem(&actionqueue, &physmanager, &controlmanager);

	world.add_manager<Sprite>(&spritemanager);
	world.add_manager<Transform>(&tmanager);
	world.add_manager<PhysicsComp>(&physmanager);
	world.add_manager<PlayerControl>(&controlmanager);
	world.subscribe_system<Sprite>(&renderer);
	world.subscribe_system<Transform>(&renderer);
	world.subscribe_system<Transform>(&physics);
	world.subscribe_system<PhysicsComp>(&physics);
	world.subscribe_system<PhysicsComp>(&control);
	world.subscribe_system<PlayerControl>(&control);

	EntityHandle test = world.create_entity();
	test.add<Sprite>(new Sprite(&atlas, "assets/entities/wiz.png"));
	test.add<Transform>(new Transform(100.f,100.f));
	test.add<PhysicsComp>(new PhysicsComp{glm::vec2(0.0f,0.f),glm::vec2(0.0f,0.0f) });
	test.add<PlayerControl>(new PlayerControl{ 20.f, 0.02f, glm::vec2(0.f,0.f) });

	EntityHandle ptest = world.create_entity();
	ptest.add<Sprite>(new Sprite(&atlas, "assets/fart/wiz.png"));
	ptest.add<Transform>(new Transform(0.f, 0.f));
	ptest.add<PhysicsComp>(new PhysicsComp{glm::vec2(0.0f,0.f),glm::vec2(0.1f,0.0f) });

	EntityHandle ptest1 = world.create_entity();
	ptest1.add<Sprite>(new Sprite(&atlas, "assets/fart/wiz.png"));
	ptest1.add<Transform>(new Transform(0.f, 0.f));
	ptest1.add<PhysicsComp>(new PhysicsComp{glm::vec2(0.0f,0.0f),glm::vec2(0.1f,0.1f) });

	EntityHandle ptest2 = world.create_entity();
	ptest2.add<Sprite>(new Sprite(&atlas, "assets/fart/wiz.png"));
	ptest2.add<Transform>(new Transform(0.f, 0.f));
	ptest2.add<PhysicsComp>(new PhysicsComp{glm::vec2(0.f,5.f),glm::vec2(0.0f,0.0f) });

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.render();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Destroy the window
	glfwDestroyWindow(window);

	//Shut down GLFW
	glfwTerminate();

	//network_listener.join();
    //network_poster.join();
    //inboxer.join();
    //outboxer.join();


	return 0;
}