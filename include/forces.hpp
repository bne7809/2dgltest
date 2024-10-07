#pragma once

#include "entity_info.hpp"

namespace force
{
	float fric_y(const entity_info& in, float wl_mul);
	float fric_x(const entity_info& in, float gr_mul);
	void gravity(entity_info& out, float df);

	class temp
	{
	private:
		float fx;
		float fy;
		int time;
		int dur;
		bool active;
	public:
		temp() : active(false) {}
		void activate(float x, float y, int d);
		void apply(entity_info& out, int dt, float df);
	};
}
