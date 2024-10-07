#include "cmd_buff.hpp"

void cmd_buff::push(entity_cmd c)
{
	if(index < BUFF_SIZE)
	{
		buff[index] = c;
		index++;
	}
}
