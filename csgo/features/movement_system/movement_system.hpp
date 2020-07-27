#pragma once
#include "../includes.hpp"

namespace function {
	class movement_system {
	public:
		void setup_movement( CUserCmd* cmd, Vector& wishangle );
	};
	extern movement_system c_movement;
}