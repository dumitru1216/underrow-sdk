#include "../csgo_hooks.hpp"
#include "../../menu/ui/gui.h"
#include "../../utils/globals/globals.hpp"
#include "../../menu/config_system/config.hpp"

void __fastcall hooks::paint_traverse( PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce ) {
	static auto o_paint_traverse = c_hooks.pPanelHook->GetOriginal<paint_traverse_t>( vtable_indexes::paint );
	static unsigned int panelID, panelHudID;

	if ( !panelHudID ) {
		if ( !strcmp( "HudZoom", g_pPanel->GetName( vguiPanel ) ) ) {
			panelHudID = vguiPanel;
		}
	}

	if ( panelHudID == vguiPanel && csgo::m_local && csgo::m_local->is_alive( ) && variable.Config.NoScope ) {
		if ( csgo::m_local->is_scoped( ) )
			return;
	} 

	o_paint_traverse( pPanels, vguiPanel, forceRepaint, allowForce );

	if ( !panelID ) {
		if ( !strcmp( "MatSystemTopPanel", g_pPanel->GetName( vguiPanel ) ) ) {
			panelID = vguiPanel;
			c_hooks.bInitializedDrawManager = true;
		}
	}

	if ( panelID == vguiPanel ) {
		function::c_player_esp.setup( );
	}
}
