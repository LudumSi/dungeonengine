#ifndef DUNGEON_CONTROL_SYS
#define DUNGEON_CONTROL_SYS

#include "ecs.h"
#include "playercontrol.h"
#include "transform.h"
#include <queue>

class ControlSystem : public System {

	private:
		ComponentManager<PhysicsComp>* deltas;
		ComponentManager<PlayerControl>* players;
		std::queue<MoveCommand>* actions;

	public:

		ControlSystem(std::queue<MoveCommand>* actions, ComponentManager<PhysicsComp>* deltas, ComponentManager<PlayerControl>* players);
		void update(float delta);
		void render();
};

#endif