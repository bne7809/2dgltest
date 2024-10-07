#include "clock.hpp"

void game_clock::tick()
{
	std::chrono::steady_clock::time_point curr = std::chrono::steady_clock::now();
	dt = std::chrono::duration_cast<std::chrono::microseconds>(curr - last).count();
	df = static_cast<float>(dt) / 1000000.0f;
	last = curr;;
}
