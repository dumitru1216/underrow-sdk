#include "thirdperson.hpp"
#include "../../../sdk/interfaces/i_cvar.hpp"
#include "../../../sdk/interfaces/c_prediction.hpp"

namespace function {
	thirdperson c_thirdperson;

	void thirdperson::setup( ClientFrameStage_t curStage ) {
		static bool init = false;

		/* checks */
		if ( !engine->is_in_game( ) || !engine->is_connected( ) || !csgo::m_local )
			return;
	
		if ( GetKeyState( VK_MBUTTON ) && csgo::m_local->is_alive( ) )
		{
			if ( init )
			{
				ConVar* sv_cheats = cvar->FindVar( "sv_cheats" );
				*( int* )( ( DWORD )&sv_cheats->fnChangeCallback + 0xC ) = 0; // ew
				sv_cheats->SetValue( 1 );
				engine->ExecuteClientCmd( "thirdperson" );
			}
			init = false;
		}
		else
		{
			if ( !init )
			{
				ConVar* sv_cheats = cvar->FindVar( "sv_cheats" );
				*( int* )( ( DWORD )&sv_cheats->fnChangeCallback + 0xC ) = 0; // ew
				sv_cheats->SetValue( 1 );
				engine->ExecuteClientCmd( "firstperson" );
			}
			init = true;
		}

		if ( curStage == frame_render_start && GetKeyState( VK_MBUTTON ) && csgo::m_local->is_alive( ) )
		{
			prediction->SetLocalViewAngles( Vector( csgo::m_real_angle.x, csgo::m_real_angle.y, 0 ) ); // lol
		}
	}
}