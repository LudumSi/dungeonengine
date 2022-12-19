#include "control.h"

ControlSystem::ControlSystem(World* world, std::queue<MoveCommand>* actions): System(world) {

	this->actions = actions;
	this->up = 0;
	this->left = 0;
}

void ControlSystem::update(float dt) {
	
	if (!entities.size()) return;

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

	for (auto & entity : entities) {

		PhysicsComp* physics = world->get_component<PhysicsComp>(entity);
		PlayerControl* player = world->get_component<PlayerControl>(entity);

		if (!physics || !player) continue;

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