#include "globals.hpp"

namespace csgo
{
	/* user_cmd & entity */
	CUserCmd* m_cmd = nullptr;
	C_BaseEntity* m_local = nullptr;
    
	/* vectors */
	Vector m_original_view;
	Vector m_real_angle;
	Vector m_fake_angle;
	Vector m_enemy_eye_angels[ 65 ];
	Vector m_aimbot_hitbox[ 65 ][ 28 ];

	/* bools */
	bool m_send_packet = true;
	bool m_lag_peek = false;
	bool m_shot[ 65 ];
	bool m_hit[ 65 ];
	bool m_update[ 65 ];
	bool m_allow_setup_bones;
	bool m_got_fake_matrix = false;

	/* ints */
	int m_target_index = -1;
	int m_missed_shots[ 65 ];

	/* other */
    std::uintptr_t uRandomSeed  = NULL;
	in_use backup = in_use{ };
	matrix3x4_t m_fake_matrix[ 128 ];

	/* fonts */
	DWORD esp_font;
	DWORD health_bar_font;
}
