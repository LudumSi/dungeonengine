#pragma once

#ifndef RENDER_SYS
#define RENDER_SYS

#include "ecs.h"
#define GLFW_INCLUDE_NONE
#include "GLFW\glfw3.h"
#include "sprite.h"
#include "transform.h"

class RenderSystem : public System {

	private:
		GLFWwindow* window;
		unsigned int shader;
		unsigned int texture;
		glm::mat4 projection;
		ComponentManager<Sprite>* sprites;
		ComponentManager<Transform>* positions;

		int win_height;
		int win_width;

	public:
		RenderSystem(GLFWwindow* window, TextureAtlas* atlas, ComponentManager<Sprite>* spriteman, ComponentManager<Transform>* positions);

		void update();
		void render();
};


#endif
