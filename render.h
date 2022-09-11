#pragma once

#ifndef RENDER_SYS
#define RENDER_SYS

#include "ecs.h"
#define GLFW_INCLUDE_NONE
#include "GLFW\glfw3.h"
#include "sprite.h"

class RenderSystem : public System {

	private:
		GLFWwindow* window;
		unsigned int shader;
		unsigned int texture;
		glm::mat4 projection;
		Sprite test_sprite;

	public:
		RenderSystem(GLFWwindow* window);

		void update();
		void render();

		int win_height;
		int win_width;
};


#endif
