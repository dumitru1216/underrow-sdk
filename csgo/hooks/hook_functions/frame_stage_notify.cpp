#include "../csgo_hooks.hpp"
#include "../../menu/ui/gui.h"
#include "../../utils/globals/globals.hpp"
#include "../../menu/config_system/config.hpp"

void __stdcall hooks::frame_stage_notify( ClientFrameStage_t curStage ) {
	static auto o_frame_stage = c_hooks.pClientHook->GetOriginal<frame_stage_notify_t>( vtable_indexes::frameStage );

	o_frame_stage( curStage );
}