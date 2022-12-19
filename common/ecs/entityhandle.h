#pragma once

#include "entity.h"
#include "world.h"

//Entity handle, for making working with entities easier
class EntityHandle {

public:
	Entity entity;
	World* world;
	EntityHandle(Entity, World*);

	void destroy() {
		world->delete_entity(entity);
	};

	template <typename CompType>
	void add(CompType* c) {
		world->add_component<CompType>(entity, c);
	}

	template <typename CompType>
	void remove() {
		world->remove_component<CompType>(entity);
	}

	template <typename CompType>
	bool has() {
		return world->has_component<CompType>(entity);
	}

	template <typename CompType>
	CompType* get() {
		return world->get_component<CompType>(entity);
	}
};