#pragma once

#include <map>
#include <iostream>

#include "entity.h"
#include "component.h"
#include "system.h"

class EntityHandle;
class World {

	public:
		//Create an entity handle
		EntityHandle create_entity();

		//Delete an entity entirely
		//Probably has some fairly large overhead
		void delete_entity(Entity e) {

			//Remove the entity and all its components from all component managers
			for (std::map<const char*, CompManagerBase*>::iterator it = managers.begin(); it != managers.end(); ++it) {
				it->second->remove_component(e);
			}

			//Remove the entity from all systems
			for (int i = 0; i < systems.size(); i++) {
				systems[i]->deregister_entity(e);
			}
		}

		//Add a component to an entity
		//Also updates systems
		template <typename CompType>
		void add_component(Entity e, CompType* comp) {
			ComponentManager<CompType>* manager = get_manager<CompType>();
			manager->add_component(e,comp);

			std::vector<int> indices = subscribers[get_manager_id<CompType>()];
			for (int i = 0; i < indices.size(); i++) {
				systems[indices[i]]->register_entity(e);
			}
		}

		//Remove a given component from an entity
		//Also updates systems
		template <typename CompType>
		void remove_component(Entity e) {
			ComponentManager<CompType>* manager = get_manager<CompType>();
			manager->remove_component(e);

			std::vector<int> indices = subscribers[get_manager_id<CompType>()];
			for (int i = 0; i < indices.size(); i++) {
				systems[indices[i]]->deregister_entity(e);
			}
		}

		//Sees if the entity has the given component
		template <typename CompType>
		bool has_component(Entity e) {
			ComponentManager<CompType>* manager = get_manager<CompType>();
			return manager->has_component(e);
		}

		//Returns a pointer to a component for a given entity
		template <typename CompType>
		CompType* get_component(Entity e) {
			ComponentManager<CompType>* manager = this->get_manager<CompType>();
			return manager->get_component(e);
		}

		//Add a component manager
		template <typename CompType>
		void add_manager(ComponentManager<CompType>* man) {
			const char* id = typeid(CompType).name();
			managers[id] = (CompManagerBase*)man;
		}

		//Get a component manager
		template <typename CompType>
		ComponentManager<CompType>* get_manager() {

			CompManagerBase* candidate = managers[get_manager_id<CompType>()];
			//This line of code is horrifically unsafe
			//...but it works :)
			ComponentManager<CompType>* manager = static_cast<ComponentManager<CompType>*>(candidate);
			if (manager) {
				return manager;
			}
			else {
				//More than likely this code will never be called due to static cast always "working"
				std::cout << "Could not cast manager for type " << typeid(CompType).name() << std::endl;
				exit(1);
			}
		}

		//Subscribe a system to changes in a given component type
		//Adds the system if it hasn't been already
		template <typename CompType>
		void subscribe_system(System* sys) {

			if (!system_indices.count(sys)) {
				add_system(sys);
			}
			subscribers[get_manager_id<CompType>()].push_back(system_indices[sys]);;
		}
		
		//Add a system
		void add_system(System* sys) {
			system_indices[sys] = (int)systems.size();
			systems.push_back(sys);
		}

	private:

		//Check to ensure that the given type exists in the managers array
		template <typename CompType>
		const char* get_manager_id() {

			const char* id = typeid(CompType).name();
			
			if (managers.count(id)) {
				return id;
			}
			else {
				std::cout << "Could not find manager for type " << typeid(CompType).name() << std::endl;
				exit(1);
			}
		}

		//Entity manager
		EntityManager e_manager;

		//Map of component ids to component managers
		std::map<const char*,CompManagerBase*> managers;

		//Vector of systems
		std::vector<System*> systems;
		//Map of systems back to indices
		std::map<System*, int> system_indices;

		//Map of manager pointers to vectors of indices of systems which need to be updated
		std::map<const char*, std::vector<int>> subscribers;
};