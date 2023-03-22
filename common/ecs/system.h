#pragma once

#include <vector>
#include "entity.h"

class World;
class System {

	protected:
		//Array of entities the system cares about
		std::vector<Entity> entities;
		//World the system belongs to
		World* world;
	public:
		
		System(World* world);

		//Register entity
		void register_entity(Entity);
		//Unregister entity
		void deregister_entity(Entity);
};