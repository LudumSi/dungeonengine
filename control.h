#ifndef DUNGEON_CONTROL_SYS
#define DUNGEON_CONTROL_SYS

#include "common/ecs.h"
#include "playercontrol.h"
#include "common/transform.h"
#include <queue>

class ControlSystem : public System {

	private:
		ComponentManager<PhysicsComp>* deltas;
		ComponentManager<PlayerControl>* players;
		std::queue<MoveCommand>* actions;

		int left;
		int up;

	public:

		ControlSystem(std::queue<MoveCommand>* actions, ComponentManager<PhysicsComp>* deltas, ComponentManager<PlayerControl>* players);
		void update(float delta);
		void render();
};

#endif