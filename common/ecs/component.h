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
};

//Custom datatype for storing data in a contigous array while being easily acessible
template <class KeyType, class DataType>
class HashVector{

	private:
		std::map<KeyType,int> key_to_data;
		std::map<int,KeyType> data_to_key;
		std::vector<DataType> data;

	public:
		void add_data(KeyType, DataType);
		void remove_data(KeyType);
		bool has_data(KeyType);
		DataType* get_data(KeyType);
};

//C++ Templates don't seem to get along with .cpp files

template <class KeyType, class DataType>
void HashVector<KeyType, DataType>::add_data(KeyType k , DataType d){
	//Map data to key and vice versa
	key_to_data[k] = data.size();
	data_to_key[data.size()] = k;
	//Add data
	data.push_back(d);
}

template <class KeyType, class DataType>
void HashVector<KeyType, DataType>::remove_data(KeyType k){
	
	//Find the old index and length and store it
	int idx = key_to_data[k];
	int len = data.size() - 1;

	//In order to keep things packed in, swap with the last component in the vector
	DataType data_inst = data[len];
	data[idx] = data_inst;
	data.pop_back();

	//Update the map
	//Whatever was pointing to len now points to idx
	//Idx now points back to that entity
	key_to_data[data_to_key[len]] = idx;
	data_to_key[idx] = data_to_key[len];

	//Remove key
	key_to_data.erase(k);
	data_to_key.erase(len);
}

template <class KeyType, class DataType>
bool HashVector<KeyType, DataType>::has_data(KeyType k){
	return (bool)key_to_data.count(k);
}

template <class KeyType, class DataType>
DataType* HashVector<KeyType, DataType>::get_data(KeyType k){
	if (this->has_data(k)) {
		return &data[key_to_data[k]];
	}
	else {
		return NULL;
	}
}

template <class CompType>
class ComponentManager: public CompManagerBase {

	private:
		//Map from entities to index in vector of components
		//Allows both random access and iteration
		HashVector<Entity, CompType> component_data;

	public:
		//Add a component to an entity
		void add_component(Entity, CompType);
		//Remove a component from an entity
		void remove_component(Entity);
		//Check to see if an entity has a component
		bool has_component(Entity);
		//Get a pointer to a component
		CompType* get_component(Entity);
};

//Component manager class
template <class CompType>
void ComponentManager<CompType>::add_component(Entity e, CompType c) {
	component_data.add_data(e,c);
}

template <class CompType>
void ComponentManager<CompType>::remove_component(Entity e) {
	component_data.remove_data(e);
}

template <class CompType>
bool ComponentManager<CompType>::has_component(Entity e) {
	return component_data.has_data(e);
}

//Returns the component if it exists, otherwise returns NULL
template <class CompType>
CompType* ComponentManager<CompType>::get_component(Entity e) {
	return component_data.get_data(e);
}
