#pragma once

#include "ecs.h"
#include "glm/glm.hpp"

struct PhysicsComp {
	glm::vec2 velocity;
	glm::vec2 acceleration;
};

struct MoveEntityEvent: public Event{
	MoveEntityEvent(Entity target, glm::vec2 delta): target{target}, delta{delta}{};
	Entity target;
	glm::vec2 delta;
};

class PhysicsSystem : public System {

	public:

		PhysicsSystem(ECSHandle* handle);
		void update(float delta);

		void move_entity(MoveEntityEvent*);
};
