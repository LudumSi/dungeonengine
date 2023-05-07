#pragma once

#include <map>
#include <vector>
#include <cstddef>
#include <cstdint>
#include "entity.h"

typedef uint64_t CompID;

CompID generate_comp_id();

template <class CompType> 
CompID get_comp_id(){
	static bool initialized = false;
	static CompID id;

	if(!initialized){
		id = generate_comp_id();
		initialized = true;
	}

	return id;
}

//Component base class
class CompManagerBase {

	public:
		virtual void remove_component(Entity)=0;
		virtual bool has_component(Entity)=0;
		virtual void add_component_raw(Entity, void*)=0;
		virtual char* get_component_raw(Entity)=0;
};

template <class CompType>
class ComponentManager: public CompManagerBase {

	private:
		//Map from entities to index in vector of components
		//Allows both random access and iteration
		std::map<Entity, int> entity_to_comp;
		std::map<int, Entity> comp_to_entity;

	public:
		//Add a component to an entity
		void add_component(Entity, CompType);
		//Remove a component from an entity
		void remove_component(Entity);
		//Check to see if an entity has a component
		bool has_component(Entity);
		//Get a pointer to a component
		CompType* get_component(Entity);
		//Add a component from raw data
		void add_component_raw(Entity, void*) override;
		//Get raw data of a component
		char* get_component_raw(Entity) override; 

		//Actual vector of components
		//Look into returning an iterator on this
		std::vector<CompType> components;
};

//C++ Templates don't seem to get along with .cpp files

//Component manager class
template <class CompType>
void ComponentManager<CompType>::add_component(Entity e, CompType c) {

	//Map entity to new index
	entity_to_comp[e] = components.size();
	comp_to_entity[components.size()] = e;
	//Add component to index
	components.push_back(c);
}

template <class CompType>
void ComponentManager<CompType>::remove_component(Entity e) {

	//Find the old index and length and store it
	int idx = entity_to_comp[e];
	int len = components.size() - 1;

	//In order to keep things packed in, swap with the last component in the vector
	CompType comp = components[len];
	components[idx] = comp;
	components.pop_back();

	//Update the map
	//Whatever was pointing to len now points to idx
	//Idx now points back to that entity
	entity_to_comp[comp_to_entity[len]] = idx;
	comp_to_entity[idx] = comp_to_entity[len];

	//Remove e
	entity_to_comp.erase(e);
	comp_to_entity.erase(len);
}

template <class CompType>
bool ComponentManager<CompType>::has_component(Entity e) {
	return (bool)entity_to_comp.count(e);
}

//Returns the component if it exists, otherwise returns NULL
template <class CompType>
CompType* ComponentManager<CompType>::get_component(Entity e) {

	if (this->has_component(e)) {
		return &components[entity_to_comp[e]];
	}
	else {
		return NULL;
	}
}

//Returns true if the component can be added, otherwise returns false
//Better be damn sure what you're putting in is this component
//Intended to be used for networking only
template <class CompType>
void ComponentManager<CompType>::add_component_raw(Entity e, void* data){

	CompType* cast_data = (CompType*)data;

	//Null pointer check
	if(!cast_data) return;

	CompType new_comp = *cast_data;

	//Some kind of check or exception should be made here to ensure the data is good

	add_component(e, new_comp);
}

template <class CompType>
char* ComponentManager<CompType>::get_component_raw(Entity e){

	CompType* comp = get_component(e);
	return (char*)comp;
}
