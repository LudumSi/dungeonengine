#pragma once

#include "ecs.h"
#include "playercontrol.h"
#include "transform.h"
#include "graphics/camera.h"
#include <queue>

class ControlSystem : public System {

	private:
		Camera* camera;
		std::queue<MoveCommand>* actions;

		int left;
		int up;

		glm::vec2 player_velocity;

	public:

		ControlSystem(World* world, std::queue<MoveCommand>* actions, Camera* camera);
		void update(float delta);
};