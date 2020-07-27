#pragma once
#include "../../includes.hpp"
#include "../../../sdk/interfaces/i_surface.hpp"

/* test postion */
struct test_position_t {
	Vector pos;
	float simulation_time;
};

namespace function {
	class player_esp {
	public:
		void setup( );
	private:
		struct
		{
			int left, top, right, bottom;
		}box;

		color_t color;
		color_t textcolor;
		color_t skelecolor;
		DWORD font;
		int offsetY;

		void setup_visuals( C_BaseEntity* entity, float alpha, matrix3x4_t* bone_matrix = nullptr );
	};
	extern player_esp c_player_esp;
}