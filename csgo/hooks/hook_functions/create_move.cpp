#include "../csgo_hooks.hpp"
#include "../../utils/globals/globals.hpp"
#include "../../utils/other/sdk_math.hpp"

bool __fastcall hooks::create_move( IClientMode* thisptr, void* edx, float sample_frametime, CUserCmd* pCmd ) {
	static auto o_create_move = c_hooks.pClientModeHook->GetOriginal<create_move_t>( vtable_indexes::createMove );
	o_create_move( thisptr, edx, sample_frametime, pCmd );

	if (!pCmd || !pCmd->command_number)
		return o_create_move;

	/* fixing invalid values */
	csgo::m_cmd = pCmd;
	csgo::m_local = g_pEntityList->GetClientEntity( csgo_engine->GetLocalPlayer( ) );
	csgo::m_send_packet = true;

	/* return function */
	if (!csgo::m_local)
		return o_create_move;

	/* frame partition */
	uintptr_t *framePtr;
	__asm mov framePtr, ebp;

	/* clamp angles & function original view */
	csgo::m_original_view = csgo::m_cmd->viewangles;
	g_Math.Clamp( csgo::m_cmd->viewangles );

	/* setup angles */
	if (csgo::m_send_packet)
		csgo::m_real_angle = csgo::m_cmd->viewangles;

	*(bool*)(*framePtr - 0x1C) = csgo::m_send_packet;

	csgo::m_cmd->buttons |= IN_BULLRUSH; // hehe

	return false;
}
