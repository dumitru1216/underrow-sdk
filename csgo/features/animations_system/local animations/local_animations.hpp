#pragma once
#include "../../includes.hpp"

namespace function {
	class local_animations {
	public:
		void manage_local_animations( C_BaseEntity* entity );
		matrix3x4_t real_matrix[ 128 ];
		Vector origin_local[ 128 ];
		bool get_local_matrix;

	public:

		void manage_fake_animations( );
		CCSGOPlayerAnimState* m_nState;
		matrix3x4_t fake_matrix[ 128 ];
	};
	extern local_animations c_local_animations;
}