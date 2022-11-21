#include "control.h"

ControlSystem::ControlSystem(std::queue<MoveCommand>* actions, ComponentManager<PhysicsComp>* deltas, ComponentManager<PlayerControl>* players) {

	this->actions = actions;
	this->deltas = deltas;
	this->players = players;
	this->up = 0;
	this->left = 0;
}

void ControlSystem::render() {
	return;
}

void ControlSystem::update(float dt) {
	
	if (!entities.size()) {
		return;
	}
	
	//Calculate movement changes
	int new_up = up;
	int new_left = left; //fuckin hippies
	
	while (!actions->empty()) {
		MoveCommand action = actions->front();
		actions->pop();

		switch (action) {

		case DownStart:
		case UpStop:
			new_up += 1;
			break;

		case DownStop:
		case UpStart:
			new_up -= 1;
			break;

		case LeftStart:
		case RightStop:
			new_left -= 1;
			break;

		case RightStart:
		case LeftStop:
			new_left += 1;
			break;

		}
	}

	if (new_left == left && new_up == up) {
		return;
	}

	//if (movement.x || movement.y) {
		//movement = glm::normalize(movement);
	//}

	for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it) {

		PhysicsComp* delta = deltas->get_component(*it);
		PlayerControl* player = players->get_component(*it);

		if (delta && player) {

			//Remove previous velocity
			glm::vec2 old_movement = glm::vec2(left * 1.f, up * 1.f);

			//Update velocity
			if (old_movement.x || old_movement.y) {
				delta->acceleration -= glm::normalize(old_movement) * player->speed;
			}

			//Add new velocity
			glm::vec2 new_movement = glm::vec2(new_left * 1.f, new_up * 1.f);

			//Add new velocity
			if (new_movement.x || new_movement.y) {
				delta->acceleration += glm::normalize(new_movement) * player->speed;
			}

			left = new_left;
			up = new_up;
		}
	}

}