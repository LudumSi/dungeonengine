#pragma once

#include "entity.h"
#include "entity_interface.h"

//Entity handle, for making working with entities easier
class EntityHandle {

public:
	Entity entity;
	EntityInterface* inter;
	EntityHandle(Entity, EntityInterface*);

	void destroy() {
		inter->delete_entity(entity);
	};

	template <typename CompType>
	void add(CompType c) {
		inter->add_component<CompType>(entity, c);
	}

	template <typename CompType>
	void remove() {
		inter->remove_component<CompType>(entity);
	}

	template <typename CompType>
	bool has() {
		return inter->has_component<CompType>(entity);
	}

	template <typename CompType>
	CompType* get() {
		return inter->get_component<CompType>(entity);
	}
};