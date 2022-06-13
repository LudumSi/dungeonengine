#include "ecs.h"

//Entity manager class

Entity EntityManager::create_entity() {
	this->id_index++;
	return this->id_index;
}

