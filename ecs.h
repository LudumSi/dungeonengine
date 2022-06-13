#ifndef ECS_H
#define ECS_H

#include <vector>
#include <map>
#include <iterator>

//Entity. Literally just an id.
typedef unsigned int Entity;

//Entity manager. Ensures that each entity has a unique id
//System for re-using ids?
class EntityManager {

	private:
		Entity id_index = 0;

	public:
		Entity create_entity();
		//Might be needed in the future
		//void delete_entity();
};

//Base component class for components to inherit from
class Component;

template <class CompType>
class ComponentManager {

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

		//Actual vector of components
		//NEVER just add or remove stuff from it
		//Look into returning an iterator instead
		std::vector<CompType> components;
};

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

	if (has_component(e)) {
		return &(components[entity_to_comp[e]]);
	}
	else {
		return NULL;
	}
}


class System {

	private:
		//Array of entities the system cares about
		std::vector<Entity> entities;

		//TODO: Needs some way of showing which components it cares about

	public:
		
		//Register entity
		//Unregister entity

		//Update function
		virtual void update() = 0;

		//Initialize function
		virtual void init() = 0;
};

#endif
