#pragma once

#include "ecs.h"
#include "playercontrol.h"
#include "transform.h"
#include <queue>

class ControlSystem : public System {

	private:
		std::queue<MoveCommand>* actions;

		int left;
		int up;

	public:

		ControlSystem(World* world, std::queue<MoveCommand>* actions);
		void update(float delta);
};