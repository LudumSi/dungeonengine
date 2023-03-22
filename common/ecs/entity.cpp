#include "entity.h"

//Returns a unique integer id
Entity EntityManager::create_entity() {
	this->id_index++;
	return this->id_index;
}