#pragma once

#include "comp_registry.h"
#include "system_registry.h"

class EntityHandle;

class EntityInterface {

	private:
		EntityIDGenerator eid;
		ComponentRegistry* comp_reg;
		SystemRegistry* sys_reg;

	public:

        EntityInterface(ComponentRegistry* c, SystemRegistry* s){
            comp_reg = c;
            sys_reg = s;
        }

		//Create an entity handle
		EntityHandle create_entity();

		//Delete an entity entirely
		void delete_entity(Entity e) {
			comp_reg->delete_entity(e);
			sys_reg->delete_entity(e);
		}

		//Add a component to an entity
		//Also updates systems entity lists
		template <typename CompType>
		void add_component(Entity e, CompType comp) {
			comp_reg->add_component(e,comp);
			sys_reg->add_component(e,comp);
		}

		//Remove a given component from an entity
		//Also updates systems entity lists
		template <typename CompType>
		void remove_component(Entity e) {
			comp_reg->remove_component<CompType>(e);
			sys_reg->remove_component<CompType>(e);
		}

		//Sees if the entity has the given component
		template <typename CompType>
		bool has_component(Entity e) {
			return comp_reg->has_component<CompType>(e);
		}

		//Returns a pointer to a component for a given entity
		template <typename CompType>
		CompType* get_component(Entity e) {
			return comp_reg->get_component<CompType>(e);
		}
};