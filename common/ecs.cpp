#include "ecs.h"

//Entity manager class

Entity EntityManager::create_entity() {
	this->id_index++;
	return this->id_index;
}

//Entity handle class
EntityHandle::EntityHandle(Entity e, World* w) {
	entity = e;
	world = w;
}

//System class
void System::register_entity(Entity e) {
	entities.push_back(e);
}

void System::deregister_entity(Entity e) {
	
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i] == e) {
			entities.erase(entities.begin() + i);
			return;
		}
	}
}

//World entity handle creator
EntityHandle World::create_entity() {
	return EntityHandle(e_manager.create_entity(),this);
}