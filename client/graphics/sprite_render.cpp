#include "sprite_render.h"
#include "render_helpers.h"
#include "glad\glad.h"
#include "glm\gtc\type_ptr.hpp"

SpriteRenderSystem::SpriteRenderSystem(ECSHandle* handle, Camera* camera, TextureAtlas* atlas): System(handle) {

	handle->subscribe_system<Transform>(this);
	handle->subscribe_system<Sprite>(this);

	this->camera = camera;

	//Set up shaders
	this->shader = get_shaders("sprite");

	//Set up textures
	if(!atlas->flushed){
		this->texture = texture_from_atlas(atlas);
	}else{
		printf("Failed to create render system; atlas has already been consumed\n");
	}
}

void SpriteRenderSystem::render() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(shader);

	//Set projection matrix uniform
	int project_loc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(project_loc, 1, GL_FALSE, glm::value_ptr(camera->projection));

	//Set up camera matrix uniform
	int camera_loc = glGetUniformLocation(shader, "camera_view");
	glUniformMatrix4fv(camera_loc, 1, GL_FALSE, glm::value_ptr(camera->camera_view));
	
	//Draw all sprites in the sprites
	for (auto & entity : entities) {

		Sprite* sprite = handle->get_component<Sprite>(entity);
		Transform* position = handle->get_component<Transform>(entity);

		if(!sprite || !position) continue;

		//Set transform matrix
		int transform_loc = glGetUniformLocation(shader, "transform");
		glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(position->transform));

		//Bind the vertex array and draw the sprite
		glBindVertexArray(sprite->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}
