#pragma once

#include "ecs.h"
#include "transform.h"

class PhysicsSystem : public System {

	private:
		ComponentManager<PhysicsComp>* deltas;
		ComponentManager<Transform>* positions;

	public:

		PhysicsSystem(ComponentManager<Transform>* positions, ComponentManager<PhysicsComp>* deltas);

		void update(float delta);
		void render();
};
