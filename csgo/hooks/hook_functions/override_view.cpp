#include "../csgo_hooks.hpp"
#include "../../menu/ui/gui.h"
#include "../../utils/globals/globals.hpp"
#include "../../menu/config_system/config.hpp"

void __fastcall hooks::override_view( void* ecx, void* edx, CViewSetup* pSetup )
{
	static auto o_overide_view = c_hooks.pClientModeHook->GetOriginal<override_view_t>( vtable_indexes::view );

	if (csgo_engine->IsConnected( ) && csgo_engine->IsInGame( )) {
		if (!csgo::m_local )
			return;

		if (!csgo::m_local->IsAlive( ))
			return;

		if (variable.Config.NoRecoil) {
			Vector viewPunch = csgo::m_local->GetViewPunchAngle( );
			Vector aimPunch = csgo::m_local->GetAimPunchAngle( );

			pSetup->angles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
			pSetup->angles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
			pSetup->angles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		}

		if ( variable.Config.Fov != 0 && !csgo::m_local->IsScoped( ))
			pSetup->fov = variable.Config.Fov;

		if ( variable.Config.NoZoom && csgo::m_local->IsScoped( ))
			pSetup->fov = ( variable.Config.Fov == 0) ? 90 : variable.Config.Fov;
	}

	o_overide_view( ecx, edx, pSetup );
}
