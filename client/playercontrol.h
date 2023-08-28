#pragma once

#include "glm/glm.hpp"

enum class MoveCommand {
	UpStart,
	UpStop,
	LeftStart,
	LeftStop,
	RightStart,
	RightStop,
	DownStart,
	DownStop
};

struct PlayerControl {
		float max_speed;
		float max_delta;
		glm::vec2 player_velocity;
};