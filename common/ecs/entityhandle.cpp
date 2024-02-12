#include "entityhandle.h"

//Entity handle class
EntityHandle::EntityHandle(Entity e, EntityInterface* r) {
	entity = e;
	inter = r;
}