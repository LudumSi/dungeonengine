#pragma once

#include "ecs.h"
#include "camera.h"
#include "sprite.h"
#include "transform.h"

class SpriteRenderSystem : public System {

	private:
		
		unsigned int shader;
		unsigned int texture;
		Camera* camera;

	public:
	
		SpriteRenderSystem(ECSHandle* handle, Camera* camera, TextureAtlas* atlas);
		void render();
};
