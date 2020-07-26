#pragma once
#include "../../sdk\interfaces/c_input.hpp"
#include "../../sdk\interfaces/c_entity.hpp"

namespace csgo
{
	/* user_cmd & entity */
	extern CUserCmd* m_cmd;
	extern C_BaseEntity* m_local;

	/* vectors */
	extern Vector m_original_view;
	extern Vector m_real_angle;
	extern Vector m_fake_angle;
	extern Vector m_enemy_eye_angels[ 65 ];
	extern Vector m_aimbot_hitbox[ 65 ][ 28 ];

	/* bools */
	extern bool m_send_packet;
	extern bool m_lag_peek;
	extern bool m_shot[ 65 ];
	extern bool m_hit[ 65 ];

	/* ints */
	extern int m_target_index;
	extern int m_missed_shots[ 65 ];

	/* other */
    extern std::uintptr_t uRandomSeed;

	/* fonts */
	extern DWORD courier_new;
	extern DWORD tahoma;
}