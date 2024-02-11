#pragma once

#include <map>
#include <unordered_map>
#include <iostream>

#include "entity.h"
#include "component.h"
#include "system.h"
#include "event.h"

class EntityHandle;

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

class SystemRegistry{

	private:
		//Vector of systems
		std::vector<System*> systems;

		//Maps of comp types to systems
		std::unordered_multimap<CompID, System*> comp_type_to_system;
		std::unordered_multimap<System*, CompID> system_to_comp_type;

		//Check to see whether an entity has all the components a given system cares about
		bool has_system_components(Entity e, System* sys, ComponentRegistry* comp_reg){

			bool has_all = true;

			//See if the entity now has all the components the system cares about
			auto range = system_to_comp_type.equal_range(sys);
			for(auto it = range.first; it != range.second; ++it){
				
				if(!comp_reg->has_component_by_id(e, it->second)){
					has_all = false;
					break;
				}
			}
			return has_all;
		}

	public:

		void delete_entity(Entity e){
			//Remove the entity from all systems
			for (int i = 0; i < systems.size(); i++) {
				systems[i]->deregister_entity(e);
			}
		}

		template <typename CompType>
		void add_component(Entity e, CompType comp, ComponentRegistry* comp_reg){
			//Iterate through all systems which use this component and add it if it has the full suite of components
			auto range = comp_type_to_system.equal_range(get_comp_id<CompType>());
			for(auto it = range.first; it != range.second; ++it){
				//If so, add the entity
				if(has_system_components(e, it->second, comp_reg)){
					it->second->register_entity(e);
				}
			}
		}

		template <typename CompType>
		void remove_component(Entity e) {
			//Iterate through all systems which use this component and deregister the entity
			auto range = comp_type_to_system.equal_range(get_comp_id<CompType>);
			for(auto it = range.first; it != range.second; ++it){
				it->second->deregister_entity(e);
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
				systems.push_back(sys);
			}

			CompID comp_id = get_comp_id<CompType>();

			comp_type_to_system.insert(std::pair<CompID, System*>(comp_id,sys));
			system_to_comp_type.insert(std::pair<System*, CompID>(sys,comp_id));
		}
};

//Interface class for everything which makes the ECS system work
class World {

	public:

		//Create an entity handle
		EntityHandle create_entity();

		//Delete an entity entirely
		void delete_entity(Entity e) {

			comp_reg.delete_entity(e);
			sys_reg.delete_entity(e);
		}

		//Add a component to an entity
		//Also updates systems entity lists
		template <typename CompType>
		void add_component(Entity e, CompType comp) {
			comp_reg.add_component(e,comp);
			sys_reg.add_component(e,comp,&comp_reg);
		}

		//Remove a given component from an entity
		//Also updates systems entity lists
		template <typename CompType>
		void remove_component(Entity e) {
			comp_reg.remove_component<CompType>(e);
			sys_reg.remove_component<CompType>(e);
		}

		//Sees if the entity has the given component
		template <typename CompType>
		bool has_component(Entity e) {
			return comp_reg.has_component<CompType>(e);
		}

		//Returns a pointer to a component for a given entity
		template <typename CompType>
		CompType* get_component(Entity e) {
			return comp_reg.get_component<CompType>(e);
		}

		//Add a component manager
		template <typename CompType>
		void add_manager() {
			comp_reg.add_manager<CompType>();
		}

		//Subscribe a system to changes in a given component type
		template <typename CompType>
		void subscribe_system(System* sys) {
			sys_reg.subscribe_system<CompType>(sys);
		}

		template<class T, class EventType>
        void subscribe_event(T* system, void(T::*handler)(EventType*)){
			event_reg.subscribe(system, handler);
		}

		template<class EventType>
        void broadcast(EventType* event){
			event_reg.broadcast(event);
		}

	private:

		//Entity manager
		EntityManager e_manager;

		//Stores component managers, interface to them
		ComponentRegistry comp_reg;

		//Maintains system component subscriptions; does not store systems
		SystemRegistry sys_reg;

		//Event handler for system events
		EventRegistry event_reg;
};