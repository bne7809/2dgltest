#pragma once

#include "entity_info.hpp"

class cmd_buff
{
public:
private:
	static constexpr int BUFF_SIZE = 5;

	entity_cmd buff[BUFF_SIZE];
	int index;
public:
	cmd_buff() : index(0) {}

	inline bool empty() const { return (index == 0); }
	inline int num_cmds() const { return index; }
	inline entity_cmd get_cmd(int i) const { return buff[i]; }
	inline void clear() { index = 0; }

	void push(entity_cmd c);
};
