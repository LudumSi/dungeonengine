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

	while (!actions->empty()) {
		MoveCommand action = actions->front();
		actions->pop();

		//printf("Control Action: %i\n", (int)action);

		switch (action) {

		case MoveCommand::DownStart:
		case MoveCommand::UpStop:
			up += 1;
			break;

		case MoveCommand::DownStop:
		case MoveCommand::UpStart:
			up -= 1;
			break;

		case MoveCommand::LeftStart:
		case MoveCommand::RightStop:
			left -= 1;
			break;

		case MoveCommand::RightStart:
		case MoveCommand::LeftStop:
			left += 1;
			break;

		}
	}

	for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it) {

		PhysicsComp* physics = deltas->get_component(*it);
		PlayerControl* player = players->get_component(*it);

		if (physics && player) {

			//Remove previous velocity
			//printf("Prev accel: %f %f\n", physics->acceleration.x, physics->acceleration.y);
			physics->velocity -= player->prev_movement;

			//Calculate target_velocity
			
			//Direction
			glm::vec2 target_velocity = glm::vec2(left,up);

			if(glm::length(target_velocity) > 0.f){
				//Magnitude
				target_velocity = glm::normalize(target_velocity);
			}

			//Cap change to the target by max acceleration
			glm::vec2 delta_v = target_velocity - player->prev_movement;
			if(glm::length(delta_v) > player->max_acceleration){
				delta_v = glm::normalize(delta_v);
				delta_v *= player->max_acceleration;
			}

			glm::vec2 new_velocity = player->prev_movement + delta_v;

			//Apply new velocity
			physics->velocity += new_velocity;
			player->prev_movement = new_velocity;
		}
	}
}