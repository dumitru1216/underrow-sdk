#pragma once
#include "../includes.hpp"
#include "../../sdk/interfaces/c_global_vars_base.hpp"
#include <deque>

#define time_to_ticks( dt )		( (int)( 0.5 + (float)(dt) / globalvars->intervalPerTick ) )
#define ticks_to_time( t )		( globalvars->intervalPerTick *( t ) )

struct player_records_t {
	matrix3x4_t matrix[ 128 ];
	float velocity;
	float sim_time;
	bool shot;
};

namespace function {
	class lag_comp {
	public:
		int shot_tick[ 65 ];
		std::deque<player_records_t> player_record[ 65 ] = {  };
		void store_records( C_BaseEntity* pEnt );
		void clear_records( int i );
		float lerp_time( );
		bool valid_tick( int tick );

		template<class T, class U>
		T clamp( T in, U low, U high );
	};
	extern lag_comp c_lag_comp;
}