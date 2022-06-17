#pragma once
#ifndef ECS_TEST
#define ECS_TEST

#include "ecs.h"
#include <iostream>

class NameComp {
	public:
		std::string name;
		NameComp(std::string);
};

class NameSystem : public System {
	public:
		ComponentManager<NameComp>* names;
		void update();
		void init();
};

void ecstest();

#endif