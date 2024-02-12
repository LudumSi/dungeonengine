#pragma once

#include <map>
#include <unordered_map>
#include <iostream>

#include "entity.h"
#include "component.h"

//Stores and manages all component managers
class ComponentRegistry{

	private:
		//Map of component ids to component managers
		std::map<CompID,CompManagerBase*> managers;

		//Check to ensure that the given type exists in the managers array
		template <typename CompType>
		CompID get_manager_id() {

			CompID id = get_comp_id<CompType>();
			
			if (managers.count(id)) {
				return id;
			}
			else {
				std::cout << "Could not find manager for type " << typeid(CompType).name() << std::endl;
				exit(1);
			}
		}
	
	public:

		//Delete entity from all component managers
		void delete_entity(Entity e) {
			//Remove the entity and all its components from all component managers
			for (std::map<CompID, CompManagerBase*>::iterator it = managers.begin(); it != managers.end(); ++it) {
				it->second->remove_component(e);
			}
		}

		template <typename CompType>
		void add_component(Entity e, CompType comp){
			//Actually add the component to the entity
			ComponentManager<CompType>* manager = get_manager<CompType>();
			manager->add_component(e,comp);
		}

		template <typename CompType>
		void remove_component(Entity e){
			ComponentManager<CompType>* manager = get_manager<CompType>();
			manager->remove_component(e);
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
			CompID id = get_comp_id<CompType>();
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

		//Sees if the entity has the given component
		bool has_component_by_id(Entity e, CompID id) {
			CompManagerBase* manager = managers[id];
			return manager->has_component(e);
		}
};