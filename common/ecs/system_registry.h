#pragma once

#include <map>
#include <unordered_map>
#include <iostream>

#include "comp_registry.h"

class System;

class SystemRegistry{

	private:
        ComponentRegistry* comp_reg;

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

        SystemRegistry(ComponentRegistry* reg);

		void delete_entity(Entity e);

		template <typename CompType>
		void add_component(Entity e, CompType comp){
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