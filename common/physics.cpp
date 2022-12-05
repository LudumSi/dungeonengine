#pragma once

#include "physics.h"

//Constant defining how many pixels per simulation meter
#define PIXELS_PER_METER 64.f
#define GRAVITY 9.8f

PhysicsSystem::PhysicsSystem(ComponentManager<Transform>* positions, ComponentManager<PhysicsComp>* deltas) {

	this->positions = positions;
	this->deltas = deltas;
}

void PhysicsSystem::render() {
	return;
}

void PhysicsSystem::update(float delta_t) {

	for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it) {

		//Ensure we have a physics and position vector
		PhysicsComp* physics = deltas->get_component(*it);
		Transform* position = positions->get_component(*it);

		if (physics && position) {

			//Update velocity
			physics->velocity += delta_t * physics->acceleration;

			//Update position
			position->translate(physics->velocity * PIXELS_PER_METER * delta_t);

			//Reset acceleration to zero
			//Consider doing something to allow forces to persist
			//physics->acceleration = glm::vec2(0.f, 0.f);
		}
	}

}
