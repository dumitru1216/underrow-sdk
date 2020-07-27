#include "../csgo_hooks.hpp"
#include "../../menu/ui/gui.h"
#include "../../utils/globals/globals.hpp"
#include "../../menu/config_system/config.hpp"

void __stdcall hooks::frame_stage_notify( ClientFrameStage_t curStage ) {
	static auto o_frame_stage = c_hooks.pClientHook->GetOriginal<frame_stage_notify_t>( vtable_indexes::frameStage );

	function::c_world_esp.setup( );
	function::c_thirdperson.setup( curStage );

	if ( engine->is_in_game( ) && engine->is_connected( ) && csgo::m_local ) {
		if ( curStage == frame_net_update_end ) {
		
		}
		else if ( curStage == frame_render_start ) {
			if ( csgo::m_local->is_alive( ) ) {
				function::c_local_animations.manage_local_animations( csgo::m_local );
			}
		}
	}

	o_frame_stage( curStage );
}