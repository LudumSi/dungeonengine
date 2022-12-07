#pragma once

#include "../common/ecs.h"
#define GLFW_INCLUDE_NONE
#include "GLFW\glfw3.h"
#include "sprite.h"
#include "../common/transform.h"

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

		//Position of the camera
		glm::vec3 camera_position;

	public:
		//Position of the entity we're following
		EntityHandle* focus_entity;
		
		RenderSystem(GLFWwindow* window, TextureAtlas* atlas, ComponentManager<Sprite>* spriteman, ComponentManager<Transform>* positions);

		void update(float delta);
		void render();

		void set_camera(float, float);
};
