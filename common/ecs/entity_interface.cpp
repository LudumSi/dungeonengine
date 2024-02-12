#include "entity_interface.h"
#include "entityhandle.h"

EntityHandle EntityInterface::create_entity(){
    return EntityHandle(eid.create_entity(), this);
}