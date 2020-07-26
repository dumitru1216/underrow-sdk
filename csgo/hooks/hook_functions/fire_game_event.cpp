#include "../csgo_hooks.hpp"
#include "../../menu/ui/gui.h"
#include "../../utils/globals/globals.hpp"
#include "../../menu/config_system/config.hpp"

void Event::FireGameEvent( IGameEvent* event )
{
	if ( !event )
		return;

	if ( !csgo::m_local )
		return;

	auto attacker = g_pEntityList->GetClientEntity( csgo_engine->GetPlayerForUserID( event->GetInt( "attacker" ) ) );
	if ( !attacker )
		return;

	if ( attacker != csgo::m_local )
		return;

	int index = csgo_engine->GetPlayerForUserID( event->GetInt( "userid" ) );

	PlayerInfo_t pInfo;
	csgo_engine->GetPlayerInfo( index, &pInfo );

	csgo::m_hit[ index ] = true;

	if ( !variable.Config.Ak47meme )
		csgo_engine->ExecuteClientCmd( "play physics\\metal\\paintcan_impact_hard3.wav" );
};
