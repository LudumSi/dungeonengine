#include "physics.h"
#include "transform.h"

//Constant defining how many pixels per simulation meter
#define PIXELS_PER_METER 64.f
#define GRAVITY 9.8f

PhysicsSystem::PhysicsSystem(World* world): System(world){
	world->subscribe_event(this, &PhysicsSystem::move_entity);
};

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

void PhysicsSystem::move_entity(MoveEntityEvent* e){

	PhysicsComp* physics = world->get_component<PhysicsComp>(e->target);
	physics->velocity += e->delta;
}
