#ifndef DUNGEON_PLAYER
#define DUNGEON_PLAYER

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

#endif