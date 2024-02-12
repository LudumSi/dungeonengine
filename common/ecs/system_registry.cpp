#include "system_registry.h"
#include "system.h"

void SystemRegistry::delete_entity(Entity e){
    //Remove the entity from all systems
    for (int i = 0; i < systems.size(); i++) {
        systems[i]->deregister_entity(e);
    }
}