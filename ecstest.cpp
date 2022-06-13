
#include "ecstest.h"

NameComp::NameComp(std::string str) {
	name = str;
}

void NameSystem::update() {

	for (int i = 0; i < names->components.size(); i++) {
		std::cout << names->components[i].name << std::endl;
	}
}

void ecstest() {

	ComponentManager<NameComp> namemanager;
	EntityManager emanager;
	NameSystem system;
	system.names = &namemanager;

	Entity e1 = emanager.create_entity();
	Entity e2 = emanager.create_entity();
	Entity e3 = emanager.create_entity();
	Entity e4 = emanager.create_entity();

	NameComp name1 = NameComp(std::string("Alice"));
	NameComp name2 = NameComp(std::string("Bob"));
	NameComp name3 = NameComp(std::string("Carol"));
	NameComp name4 = NameComp(std::string("Dan"));

	namemanager.add_component(e1, name1);
	namemanager.add_component(e2, name2);
	namemanager.add_component(e3, name3);
	namemanager.add_component(e4, name4);

	system.update();

	namemanager.remove_component(e2);

	system.update();

	if (namemanager.has_component(e2)) {
		std::cout << "Has component (Not good)!" << std::endl;
	}
	else {
		std::cout << "Doesn't have component (good)!" << std::endl;
	}

	namemanager.remove_component(e1);
	namemanager.remove_component(e3);
	namemanager.remove_component(e4);

	system.update();

	namemanager.add_component(e2, name1);

	system.update();
}

