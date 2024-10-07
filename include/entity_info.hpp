#pragma once

enum class entity_cmd
{
	MV_RIGHT,
	ST_RIGHT,
	MV_LEFT,
	ST_LEFT,
	MV_JUMP,
	ST_JUMP,
	MV_ROLL,
	TR_FIRE
};

enum class anim_type
{
	RUNNING,
	IDLE,
	RISING,
	FALLING,
	WALL_SLIDE,
	FIRE,
	DEATH,
	ROLL,
	DOUBLE_JUMP
};

typedef void(*tr_func)(void*);

struct anim_data
{
	int row;
	int start;
	int nframes;
	int delay;
	int next;
	tr_func tr;
};

namespace entity_state
{
	constexpr unsigned int GROUNDED	= 0x0001;
	constexpr unsigned int MV_RIGHT = 0x0002;
	constexpr unsigned int MV_LEFT	= 0x0004;
	constexpr unsigned int ILOCK	= 0x0008;
	constexpr unsigned int JMP		= 0x0010;
	constexpr unsigned int DBJUMP	= 0x0020;
	constexpr unsigned int COLLX	= 0x0040;
	constexpr unsigned int WLJMP	= 0x0080;
}

struct entity_hitbox
{
	float off_x;
	float off_y;
	float w;
	float h;
};

struct transform
{
	float vel_x;
	float vel_y;
	float acc_x;
	float acc_y;
	int dir;
};

struct entity_info
{
	float x;
	float y;
	float vel_x;
	float vel_y;
	float acc_x;
	float acc_y;
	float max_y;
	int dir;
	anim_type anim;
	unsigned int state;
	entity_hitbox hit;
};
