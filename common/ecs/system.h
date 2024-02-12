#pragma once

#include <vector>
#include "entity.h"
#include "comp_registry.h"
#include "ecshandle.h"

class World;
class System {

	protected:
		//Array of entities the system cares about
		std::vector<Entity> entities;
		//World the system belongs to
		ECSHandle* handle;
	public:
		
		System(ECSHandle* handle): handle(handle){};

		//Register entity
		void register_entity(Entity);
		//Unregister entity
		void deregister_entity(Entity);

		//susbscribe to a component
		template <typename CompType>
		void subscribe(){
			handle->subscribe_system<CompType>(this);
		}
};