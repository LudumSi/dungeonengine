#include "control.h"

ControlSystem::ControlSystem(std::queue<MoveCommand>* actions, ComponentManager<PhysicsComp>* deltas, ComponentManager<PlayerControl>* players) {

	this->actions = actions;
	this->deltas = deltas;
	this->players = players;
}

void ControlSystem::render() {
	return;
}

void ControlSystem::update(float dt) {
	
	if (!entities.size()) {
		return;
	}
	
	//Calculate movement vector
	glm::vec2 movement = glm::vec2(0.f,0.f);
	
	while (!actions->empty()) {
		MoveCommand action = actions->front();
		actions->pop();

		switch (action) {

		case DownStart:
		case UpStop:
			movement += glm::vec2(0.f, 1.f);
			break;

		case DownStop:
		case UpStart:
			movement += glm::vec2(0.f, -1.f);
			break;

		case LeftStart:
		case RightStop:
			movement += glm::vec2(-1.f, 0.f);
			break;

		case RightStart:
		case LeftStop:
			movement += glm::vec2(1.f, 0.f);
			break;

		}
	}

	if (movement.x || movement.y) {
		movement = glm::normalize(movement);
	}

	for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it) {

		PhysicsComp* delta = deltas->get_component(*it);
		PlayerControl* player = players->get_component(*it);

		if (delta && player) {

			//Update velocity
			delta->velocity = movement * player->speed;
		}
	}

}