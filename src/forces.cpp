#include "forces.hpp"

namespace force
{
	float fric_y(const entity_info& in, float wl_mul)
	{
		const unsigned int wlsld = (in.vel_y > 0.0f) && ((in.state & entity_state::COLLX) == entity_state::COLLX);

		return (wlsld * wl_mul + !wlsld);
	}

	float fric_x(const entity_info& in, float gr_mul)
	{
		const unsigned int grnd = ((in.state & entity_state::GROUNDED) == entity_state::GROUNDED);

		return (grnd + gr_mul * !grnd);
	}

	void gravity(entity_info& out, float df)
	{
		out.vel_y += ((out.vel_y < 0.0f) + 2.0f * (out.vel_y >= 0.0f)) * out.acc_y * df;
	}

	void temp::activate(float x, float y, int d)
	{
		fx = x;
		fy = y;
		dur = d;
		active = true;
		time = 0;
	}

	void temp::apply(entity_info& out, int dt, float df)
	{
		if(active)
		{
			out.x += fx * df;
			out.vel_y += fy * df;
			
			time += dt;
			if(time > dur)
				active = false;
		}
	}
}
