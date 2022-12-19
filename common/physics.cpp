#include "physics.h"

//Constant defining how many pixels per simulation meter
#define PIXELS_PER_METER 64.f
#define GRAVITY 9.8f

void PhysicsSystem::update(float delta_t) {

	for (auto & entity : entities) {

		//Ensure we have a physics and position vector
		PhysicsComp* physics = world->get_component<PhysicsComp>(entity);
		Transform* position = world->get_component<Transform>(entity);

		if (!physics || !position) continue;

		//Update velocity
		physics->velocity += delta_t * physics->acceleration;

		//Update position
		position->translate(physics->velocity * PIXELS_PER_METER * delta_t);
	}

}
