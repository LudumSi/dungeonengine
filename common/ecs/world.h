#pragma once

#include <map>
#include <unordered_map>
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
		//Also updates systems entity lists
		template <typename CompType>
		void add_component(Entity e, CompType* comp) {
			
			//Actually add the component to the entity
			ComponentManager<CompType>* manager = get_manager<CompType>();
			manager->add_component(e,comp);

			//Update systems about this change
			//Iterate through all systems which use this component
			auto range = comp_type_to_system.equal_range(get_manager_id<CompType>());
			for(auto it = range.first; it != range.second; ++it){

				//If so, add the entity
				if(has_system_components(e, it->second)){
					it->second->register_entity(e);
				}
			}
		}

		//Remove a given component from an entity
		//Also updates systems
		template <typename CompType>
		void remove_component(Entity e) {
			ComponentManager<CompType>* manager = get_manager<CompType>();
			manager->remove_component(e);

			//Update systems about this change
			//Iterate through all systems which use this component
			auto range = comp_type_to_system.equal_range(get_manager_id<CompType>());
			for(auto it = range.first; it != range.second; ++it){

				//If so, add the entity
				if(has_system_components(e, it->second)){
					it->second->deregister_entity(e);
				}
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
		void add_manager() {
			ComponentManager<CompType>* man = new ComponentManager<CompType>;
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

			bool already_added = false;
			for(int i = 0; i < systems.size(); i++){
				if(systems[i] == sys){
					already_added = true;
					break;
				}
			}

			if(!already_added){
				add_system(sys);
			}

			const char* comp_id = get_manager_id<CompType>();

			comp_type_to_system.insert(std::pair<const char*, System*>(comp_id,sys));
			system_to_comp_type.insert(std::pair<System*, const char*>(sys, comp_id));
		}
		
		//Add a system
		void add_system(System* sys) {
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

		//Sees if the entity has the given component
		bool has_component_by_id(Entity e, const char* id) {
			CompManagerBase* manager = managers[id];
			return manager->has_component(e);
		}

		//Check to see whether an entity has all the components a given system cares about
		bool has_system_components(Entity e, System* sys){

			bool has_all = true;

			//See if the entity now has all the components the system cares about
				auto range = system_to_comp_type.equal_range(sys);
				for(auto it = range.first; it != range.second; ++it){
					
					if(!has_component_by_id(e, it->second)){
						has_all = false;
						break;
					}
				}

			return has_all;
		}

		//Entity manager
		EntityManager e_manager;

		//Map of component ids to component managers
		std::map<const char*,CompManagerBase*> managers;

		//Vector of systems
		std::vector<System*> systems;

		//Map of manager pointers to vectors of indices of systems which need to be updated
		std::unordered_multimap<const char*, System*> comp_type_to_system;
		std::unordered_multimap<System*, const char*> system_to_comp_type;
		
};