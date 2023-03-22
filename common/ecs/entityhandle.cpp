#include "entityhandle.h"

//Entity handle class
EntityHandle::EntityHandle(Entity e, World* w) {
	entity = e;
	world = w;
}