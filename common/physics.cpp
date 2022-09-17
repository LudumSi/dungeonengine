#pragma once

#include "physics.h"

PhysicsSystem::PhysicsSystem(ComponentManager<Transform>* positions, ComponentManager<PhysicsComp>* deltas) {

	this->positions = positions;
	this->deltas = deltas;
}

void PhysicsSystem::render() {
	return;
}

void PhysicsSystem::update(float delta_t) {

	for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it) {

		PhysicsComp* delta = deltas->get_component(*it);
		Transform* position = positions->get_component(*it);

		if (delta && position) {

			//Update velocity
			delta->velocity += delta_t * delta->acceleration;

			//Update position
			position->translate(delta->velocity * delta_t);
		}
	}

}
