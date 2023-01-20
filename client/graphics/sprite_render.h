#pragma once

#include "ecs.h"
#define GLFW_INCLUDE_NONE
#include "GLFW\glfw3.h"
#include "sprite.h"
#include "transform.h"

class SpriteRenderSystem : public System {

	private:
		GLFWwindow* window;
		unsigned int shader;
		unsigned int texture;
		glm::mat4 projection;

		int win_height;
		int win_width;

		//Position of the camera
		glm::vec3 camera_position;

	public:
		//Position of the entity we're following
		EntityHandle* focus_entity;
		
		SpriteRenderSystem(World* world, GLFWwindow* window, TextureAtlas* atlas);

		void render();

		void set_camera(float, float);
};
