#pragma once
#include "../includes.hpp"
#include "../../sdk/interfaces/i_engine_trace.hpp"

struct fire_bullet_data_t {
	fire_bullet_data_t( const Vector& eyePos, C_BaseEntity* entity ) : src( eyePos ), filter( entity ) {

	}

	Vector          src;
	C_Trace         enter_trace;
	Vector          direction;
	C_TraceFilter   filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};

namespace function {
	class autowall {
	public:
		bool can_hit_floating_point( const Vector& point, const Vector& source );
		float damage( const Vector &point );
	};
	extern autowall c_autowall;
}