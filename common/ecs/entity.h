#pragma once
#include <cstdint>

//Entity. Literally just an id.
typedef uint64_t Entity;

//Entity manager. Ensures that each entity has a unique id
//TODO: System for re-using ids?
class EntityManager {

	private:
		Entity id_index = 0;

	public:
		Entity create_entity();
		//TODO: delete_entity()?
};