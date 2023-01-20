#include "sprite_render.h"
#include "render_helpers.h"
#include "glad\glad.h"
#include "glm\gtc\type_ptr.hpp"

SpriteRenderSystem::SpriteRenderSystem(World* world, GLFWwindow* window, TextureAtlas* atlas): System(world) {

	//Set up height and width
	this->window = window;
	glfwGetFramebufferSize(window, &win_width, &win_height);

	//Set up shaders
	this->shader = get_shaders("sprite");

	//Set up textures
	if(!atlas->flushed){
		this->texture = texture_from_atlas(atlas);
	}else{
		printf("Failed to create render system; atlas has already been consumed\n");
	}

	//View matrix
	//TODO: Fix things so that the Y-axis works as traditionally expected
	this->projection = glm::ortho(-1.f * (float)win_width/2, (float)win_width/2, (float)win_height/2, -1.f * (float)win_height/2, -1.0f, 1.0f);

	this->focus_entity = NULL;
	this->camera_position = glm::vec3(0.f,0.f,0.f);
}

void SpriteRenderSystem::set_camera(float x, float y){
	this->camera_position = glm::vec3(x,y,0.f);
}

void SpriteRenderSystem::render() {

	//Clear previous frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(shader);

	//Set projection matrix uniform
	int project_loc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(projection));

	//Set up camera view matrix
	glm::mat4 camera_matrix = glm::mat3(1.0);

	if(focus_entity){
		if(focus_entity->has<Transform>()){
			camera_position = focus_entity->get<Transform>()->get_position();
		}
	}

	camera_matrix = glm::translate(camera_matrix, (-1.f * camera_position));
	int camera_loc = glGetUniformLocation(shader, "camera_view");
	glUniformMatrix4fv(camera_loc, 1, GL_FALSE, glm::value_ptr(camera_matrix));
	
	//Draw all sprites in the sprites
	for (auto & entity : entities) {

		Sprite* sprite = world->get_component<Sprite>(entity);
		Transform* position = world->get_component<Transform>(entity);

		//printf("%x\n", world);
		//world->unpack(entity, sprite, position);

		if(!sprite || !position) continue;

		//Set transform matrix
		int transform_loc = glGetUniformLocation(shader, "transform");
		glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(position->transform));

		//Bind the vertex array and draw the sprite
		glBindVertexArray(sprite->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}
