#include "system.h"

System::System(World* world){
    this->world = world;
}

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