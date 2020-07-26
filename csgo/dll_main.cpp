#include <thread>
#include "hooks\csgo_hooks.hpp"
#include "utils\other\utils.hpp"
#include "utils\globals\globals.hpp"
#include "menu/config_system/config.hpp"

HINSTANCE HThisModule;

int old_dll_attach( ) {
	/* initialize hooks */
	hooks::initialize( );

	/* run config system */
	variable.run( "sdk" );

	while ( true )
		Sleep( 10000000 );

	/* restore hooks */
	hooks::restore( );

	FreeLibraryAndExitThread( HThisModule, 0 );
}

BOOL WINAPI DllMain( HMODULE hModule, DWORD dwReason, LPVOID lpReserved ) {
	if ( dwReason == DLL_PROCESS_ATTACH && GetModuleHandleA( "csgo.exe" ) ) {
		CreateThread( NULL, NULL, ( LPTHREAD_START_ROUTINE ) old_dll_attach, NULL, NULL, NULL );
	}

	return TRUE;
}

