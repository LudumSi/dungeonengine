#pragma once

#include "ecs.h"
#include "transform.h"

class PhysicsSystem : public System {

	public:

		PhysicsSystem(World* world): System(world){};
		void update(float delta);
};
