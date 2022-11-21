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

		//printf("Control Action: %i\n", (int)action);

		switch (action) {

		case MoveCommand::DownStart:
		case MoveCommand::UpStop:
			new_up += 1;
			break;

		case MoveCommand::DownStop:
		case MoveCommand::UpStart:
			new_up -= 1;
			break;

		case MoveCommand::LeftStart:
		case MoveCommand::RightStop:
			new_left -= 1;
			break;

		case MoveCommand::RightStart:
		case MoveCommand::LeftStop:
			new_left += 1;
			break;

		}
	}

	for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it) {

		PhysicsComp* physics = deltas->get_component(*it);
		PlayerControl* player = players->get_component(*it);

		if (physics && player) {

			//Remove previous acceleration
			//printf("Prev accel: %f %f\n", physics->acceleration.x, physics->acceleration.y);
			physics->acceleration -= player->prev_movement;

			//Calculate new acceleration
			
			//Direction
			glm::vec2 new_accel = glm::vec2(new_left,new_up);

			if(glm::length(new_accel) > 0.f){
				
				//Magnitude
				new_accel = glm::normalize(new_accel);

				float current_speed = glm::length(physics->velocity);
				float multiplier = fmin(player->max_acceleration,fmax(player->max_speed-current_speed,0.f));

				//printf("Current speed: %f Multiplier: %f\n", current_speed, multiplier);

				new_accel *= multiplier;
				//printf("New accel: %f %f\n", new_accel.x, new_accel.y);
			}

			//Apply new acceleration
			physics->acceleration += new_accel;
			player->prev_movement = new_accel;
		}
	}

	left = new_left;
	up = new_up;

}