#include "world.h"
#include "ecs.h"

EntityHandle World::create_entity() {
	return EntityHandle(e_manager.create_entity(),this);
}