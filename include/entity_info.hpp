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
	constexpr unsigned int MV_RIGHT = 0;
	constexpr unsigned int MV_LEFT	= 1;
	constexpr unsigned int X_DIR	= 2;
}

struct entity_base
{
	unsigned int state;
};

struct entity_pos
{
	float x;
	float y;
};

struct entity_trans
{
	float vx;
	float vy;
	float ax;
	float ay;
	float my;
};

struct entity_bound
{
	float off_x;
	float off_y;
	float w;
	float h;
};

struct stat_entity : public entity_base, public entity_pos, public entity_bound
{
};

struct dyn_entity : public entity_base, public entity_pos, public entity_trans, public entity_bound
{
	anim_type anim;
};

#define BIT_ON(flag, pos)	((flag) |= (1 << (pos)))
#define BIT_OFF(flag, pos)	((flag) &= ~(1 << (pos)))
#define IS_BIT(flag, pos)	(((flag) >> (pos)) & 1)
