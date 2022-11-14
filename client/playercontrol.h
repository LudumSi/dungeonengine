#pragma once

enum MoveCommand {
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
		float speed;
};