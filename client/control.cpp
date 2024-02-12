#include "control.h"
#include "physics.h"

ControlSystem::ControlSystem(ECSHandle* handle, std::queue<MoveCommand>* actions, Camera* camera): System(handle) {

	handle->subscribe_system<PhysicsComp>(this);
	handle->subscribe_system<Transform>(this);

	this->camera = camera;
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

		PlayerControl* player = handle->get_component<PlayerControl>(entity);
		Transform* transform = handle->get_component<Transform>(entity);

		if (!player || !transform) continue;

		//Calculate target_velocity
		
		//Direction
		glm::vec2 target_velocity = glm::vec2(left,up);

		//Magnitude
		if(glm::length(target_velocity) > 0.f){
			target_velocity = glm::normalize(target_velocity);
		}

		//Cap change to the target by max acceleration
		glm::vec2 delta_v = target_velocity - player->player_velocity;
		if(glm::length(delta_v) > player->max_delta){
			delta_v = glm::normalize(delta_v);
			delta_v *= player->max_delta;
		}

		player->player_velocity += delta_v;

		//Apply new velocity
		handle->broadcast(new MoveEntityEvent(entity, delta_v));

		//Update camera position
		camera->set_camera(glm::vec2(transform->get_position()));
	}
}