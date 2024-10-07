#pragma once

#include <chrono>

class game_clock
{
private:
	std::chrono::steady_clock::time_point last;
	int dt;
	float df;
public:
	game_clock() : last(std::chrono::steady_clock::now()), dt(0), df(0.0f) {}
	void tick();
	inline int get_dt() const { return dt; }
	inline float get_df() const { return df; }
};
