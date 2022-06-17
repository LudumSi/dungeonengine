
#include "ecstest.h"

NameComp::NameComp(std::string str) {
	name = str;
}

void NameSystem::update() {

	for (int i = 0; i < entities.size(); i++) {
		std::cout << names->get_component(entities[i])->name << std::endl;
	}
}

void NameSystem::init() {
	return;
}

void ecstest() {

	World world;
	ComponentManager<NameComp> namemanager;
	EntityManager emanager;
	NameSystem system;

	world.add_manager<NameComp>(&namemanager);

	world.subscribe_system<NameComp>(&system);
	system.names = world.get_manager<NameComp>();;

	EntityHandle e1 = world.create_entity();
	EntityHandle e2 = world.create_entity();
	EntityHandle e3 = world.create_entity();
	EntityHandle e4 = world.create_entity();

	NameComp name1 = NameComp(std::string("Alice"));
	NameComp name2 = NameComp(std::string("Bob"));
	NameComp name3 = NameComp(std::string("Carol"));
	NameComp name4 = NameComp(std::string("Dan"));

	e1.add<NameComp>(name1);
	e2.add<NameComp>(name2);
	e3.add<NameComp>(name3);
	e4.add<NameComp>(name4);

	system.update();

	e2.destroy();

	system.update();

	if (e2.has<NameComp>()) {
		std::cout << "Has component (Not good)!" << std::endl;
	}
	else {
		std::cout << "Doesn't have component (good)!" << std::endl;
	}

	e1.remove<NameComp>();
	e3.remove<NameComp>();
	e4.remove<NameComp>();

	system.update();

	e2.add<NameComp>(name1);

	system.update();
}

