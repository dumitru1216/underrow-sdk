#include "../csgo_hooks.hpp"
#include "../../menu/ui/gui.h"
#include <intrin.h>
#include "../../render_system/render/render.hpp"

static bool initialized = false;
void __stdcall hooks::end_scene( IDirect3DDevice9* pDevice ) {
	static auto o_end_scene = c_hooks.D3DHook->GetOriginal<end_scene_fn>( vtable_indexes::end_scene );

	/* end_scene gets called twice by csgo so we call it once */
	static auto ret = _ReturnAddress( );
	if ( ret != _ReturnAddress( ) ) {
		return o_end_scene( pDevice );
	}

	if ( !initialized ) {
		function::c_menu.apply_fonts( );
		function::c_menu.setup_resent( pDevice );
		initialized = true;
	}
	if ( initialized ) {
		function::c_menu.pre_render( pDevice );
		function::c_menu.post_render( );

		function::c_menu.think( );
		function::c_menu.end_present( pDevice );
	}
	
	render::filled_rect( 1, 1, 900, 20, D3DCOLOR_RGBA( 255, 0, 0, 255 ) );
	o_end_scene( pDevice );
}

void __stdcall hooks::end_scene_reset( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters )
{
	static auto o_end_scene_reset = c_hooks.D3DHook->GetOriginal<end_scene_reset_fn>( vtable_indexes::end_scene_reset );

	if ( !initialized )
		o_end_scene_reset( pDevice, pPresentationParameters );

	/* reset fonts */
	render::destroy_fonts( );

	function::c_menu.invalidate_objects( );
	auto hr = o_end_scene_reset;
	
	if ( SUCCEEDED( hr ) ) {
		/* create objects */
		function::c_menu.create_objects( pDevice );

		/* re-create fonts */
		render::create_fonts( );
	}

	return o_end_scene_reset( pDevice, pPresentationParameters );
}

/* directx hook */
void __fastcall hooks::lock_cursor( ISurface* thisptr, void* edx ) {
	static auto o_lock_cursor = c_hooks.pSurfaceHook->GetOriginal<lock_cursor_t>( vtable_indexes::lockCursor );

	/* doing here lock cursor */
	if ( function::c_menu.opened ) {
		csgo_surface->unlock_cursor( );
		return;
	}

	o_lock_cursor( thisptr, edx );
}

extern LRESULT ImGui_ImplDX9_WndProcHandler( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
LRESULT hooks::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	static bool pressed = false;

	if ( !pressed && GetAsyncKeyState( VK_INSERT ) ) {
		pressed = true;
	}
	else if ( pressed && !GetAsyncKeyState( VK_INSERT ) ) {
		pressed = false;

		function::c_menu.opened = !function::c_menu.opened;
	}

	if ( function::c_menu.opened && ImGui_ImplDX9_WndProcHandler( hWnd, uMsg, wParam, lParam ) )
		return true;

	return CallWindowProcA( c_hooks.pOriginalWNDProc, hWnd, uMsg, wParam, lParam );
}