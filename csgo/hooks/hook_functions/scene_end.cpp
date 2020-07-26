#include "../csgo_hooks.hpp"

void __fastcall hooks::scene_end( void *ecx, void *edx )
{
	static auto o_scene_end = c_hooks.pRenderViewHook->GetOriginal<scene_end_t>( vtable_indexes::sceneEnd );
	o_scene_end( ecx, edx );

}