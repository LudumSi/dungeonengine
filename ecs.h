#ifndef ECS_H
#define ECS_H

#include <vector>
#include <map>
#include <iterator>
#include <typeinfo>
#include <iostream>

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

//Component base class
class CompManagerBase {
	public:
		virtual void remove_component(Entity)=0;
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

		//Actual vector of components
		//Look into returning an iterator on this
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

	protected:
		//Array of entities the system cares about
		std::vector<Entity> entities;

		//Array of component managers. Can be used to show the component types

	public:
		
		//Register entity
		void register_entity(Entity);
		//Unregister entity
		void deregister_entity(Entity);

		//Update function
		virtual void update() = 0;

		//Initialize function
		virtual void init() = 0;
};

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
		void add_component(Entity e, CompType comp) {
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
			ComponentManager<CompType>* manager = get_manager<CompType>();
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
				std::cout << "Could not cast manager for type " << *typeid(CompType).name() << std::endl;
				exit(1);
			}
		}

		//Subscribe a system to a component manager
		//Adds the system if it hasn't been already
		template <typename CompType>
		void subscribe_system(System* sys) {

			if (!system_indices.count(sys)) {
				add_system(sys);
			}
			subscribers[get_manager_id<CompType>()].push_back(system_indices[sys]);
		}
		
		//Add a system
		void add_system(System* sys) {
			system_indices[sys] = systems.size();
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
				std::cout << "Could not find manager for type " << *typeid(CompType).name() << std::endl;
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

//Entity handle, for making working with entities easier
class EntityHandle {

public:
	Entity entity;
	World* world;
	EntityHandle(Entity, World*);

	void destroy() {
		world->delete_entity(entity);
	};

	template <typename CompType>
	void add(CompType c) {
		world->add_component<CompType>(entity, c);
	}

	template <typename CompType>
	void remove() {
		world->remove_component<CompType>(entity);
	}

	template <typename CompType>
	bool has() {
		return world->has_component<CompType>(entity);
	}

	template <typename CompType>
	CompType* get() {
		return world->get_component<CompType>(entity);
	}
};

#endif
