#include "entity.h"
#include "entityhandle.h"

//Returns a unique integer id
Entity EntityIDGenerator::create_entity() {
	this->id_index++;
	return this->id_index;
}