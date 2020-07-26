#include <thread>
#include "csgo_hooks.hpp"
#include "..\utils\other\utils.hpp"
#include "..\menu\external_include.hpp"
#include "..\sdk\other/hitboxes.hpp"
#include "libloaderapi.h"
#include  "..\utils/globals/globals.hpp"
#include "hook_helpers\hook_helper.hpp"

/* predefines */
hooks    c_hooks;
Event    g_Event;

void hooks::initialize() {
    /* get window handle */
    while (!(c_hooks.hCSGOWindow = FindWindowA("Valve001", nullptr))) {
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(50ms);
    }

    /* get interfaces */
    interfaces::Init();      

    /* get netvars after getting interfaces as we use them */
    g_pNetvars = std::make_unique<NetvarTree>();

    /* Hook WNDProc to capture mouse / keyboard input */
    if (c_hooks.hCSGOWindow)        
        c_hooks.pOriginalWNDProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(c_hooks.hCSGOWindow, GWLP_WNDPROC,
                                                                              reinterpret_cast<LONG_PTR>(c_hooks.WndProc)));

	DWORD shaderapidx9 = **(DWORD * *)(Utils::FindSignature( "shaderapidx9.dll", "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C" ) + 1);

    /* vmt_hooks */
	c_hooks.pClientHook     = std::make_unique<VMTHook>(g_pClientDll);
    c_hooks.pClientModeHook = std::make_unique<VMTHook>(g_pClientMode);
    c_hooks.pSurfaceHook	= std::make_unique<VMTHook>(csgo_surface);
	c_hooks.pPanelHook		= std::make_unique<VMTHook>(g_pPanel);
	c_hooks.pModelHook      = std::make_unique<VMTHook>(g_pModelRender);
	c_hooks.pRenderViewHook = std::make_unique<VMTHook>(g_pRenderView);
	c_hooks.D3DHook = std::make_unique<VMTHook>( (DWORD**)shaderapidx9 );

    /* table_functions */
	c_hooks.pClientHook    ->hook_function(vtable_indexes::frameStage, hooks::frame_stage_notify);
    c_hooks.pClientModeHook->hook_function(vtable_indexes::createMove, hooks::create_move);
	c_hooks.pClientModeHook->hook_function(vtable_indexes::view, hooks::override_view);
    c_hooks.pSurfaceHook   ->hook_function(vtable_indexes::lockCursor, hooks::lock_cursor);
	c_hooks.pPanelHook	   ->hook_function(vtable_indexes::paint, hooks::paint_traverse);
	c_hooks.pModelHook	   ->hook_function(vtable_indexes::dme, hooks::draw_model_execute);
	c_hooks.pRenderViewHook->hook_function(vtable_indexes::sceneEnd, hooks::scene_end);
	c_hooks.D3DHook->hook_function( vtable_indexes::end_scene, hooks::end_scene );
	c_hooks.D3DHook->hook_function( vtable_indexes::end_scene_reset, hooks::end_scene_reset );
	
    /* initialize event */
    g_Event.Init();

    /* create fonts */
    function::c_helpers.create_fonts( );

    Utils::Log("Hooking completed!");
}

void hooks::restore() {
    /* unhook every function we hooked and restore original one */
	Utils::Log("Unhooking in progress..."); {  
		c_hooks.pClientHook->unhook_function(vtable_indexes::frameStage);
        c_hooks.pClientModeHook->unhook_function(vtable_indexes::createMove);
		c_hooks.pClientModeHook->unhook_function(vtable_indexes::view);
        c_hooks.pSurfaceHook->unhook_function(vtable_indexes::lockCursor);
		c_hooks.pPanelHook->unhook_function(vtable_indexes::paint);
		c_hooks.pModelHook->unhook_function(vtable_indexes::dme);

        SetWindowLongPtr(c_hooks.hCSGOWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(c_hooks.pOriginalWNDProc));

        /* need to reset by-hand, global pointer so doesnt go out-of-scope */
        g_pNetvars.reset();  
    }
    Utils::Log("Unhooking succeded!");
}




