#include "lag_comp_system.hpp"
#include "../../sdk/interfaces/i_cvar.hpp"
#include "../../sdk/interfaces/iv_engine_client.hpp"
#include "../../sdk/other/player_info.hpp"

/* predefine */
namespace function {
	lag_comp c_lag_comp;

	/* lerp time */
	float lag_comp::lerp_time( ) {
		/* get convars */
		int update_rate = cvar->FindVar( "cl_updaterate" )->GetInt( );
		ConVar* min_update = cvar->FindVar( "sv_minupdaterate" );
		ConVar* max_update = cvar->FindVar( "sv_maxupdaterate" );

		/* update convars */
		if ( min_update && max_update )
			update_rate = max_update->GetInt( );

		/* get convars */
		float ratio = cvar->FindVar( "cl_interp_ratio" )->GetFloat( );

		/* update convars */
		if ( ratio == 0 )
			ratio = 1.0f;

		/* get convars */
		float lerp = cvar->FindVar( "cl_interp" )->GetFloat( );
		ConVar* cmin = cvar->FindVar( "sv_client_min_interp_ratio" );
		ConVar* cmax = cvar->FindVar( "sv_client_max_interp_ratio" );

		/* update convars */
		if ( cmin && cmax && cmin->GetFloat( ) != 1 )
			ratio = clamp( ratio, cmin->GetFloat( ), cmax->GetFloat( ) );

		return max( lerp, ( ratio / update_rate ) );
	}

	/* get valid tick */
	bool lag_comp::valid_tick( int tick ) {
		auto nci = csgo_engine->GetNetChannelInfo( );

		if ( !nci )
			return false;

		auto PredictedCmdArrivalTick = csgo::m_cmd->tick_count + 1 + time_to_ticks( nci->GetAvgLatency( FLOW_INCOMING ) + nci->GetAvgLatency( FLOW_OUTGOING ) );
		auto Correct = clamp( lerp_time( ) + nci->GetLatency( FLOW_OUTGOING ), 0.f, 1.f ) - ticks_to_time( PredictedCmdArrivalTick + time_to_ticks( lerp_time( ) ) - ( tick + time_to_ticks( lerp_time( ) ) ) );

		return ( abs( Correct ) <= 0.2f );
	}

	/* store records */
	void lag_comp::store_records( C_BaseEntity* entity ) {
		/* setup variables */
		player_records_t setup;
		static float shot_time[ 65 ];
		static float old_sim_time[ 65 ];

		/* fix setup bones */
		if ( entity != csgo::m_local )
			entity->fix_setup_bones( c_ragebot.matrix[ entity->EntIndex( ) ] );
	
		/* player record */
		if ( player_record[ entity->EntIndex( ) ].size( ) == 0 ) {
			/*
				** im setup : **
				* velocity
				* sim_time
				* shot
				* matrix
			*/
			setup.velocity = abs( entity->get_velocity( ).Length2D( ) );
			setup.sim_time = entity->get_simulation_time( );
			memcpy( setup.matrix, c_ragebot.matrix[ entity->EntIndex( ) ], ( sizeof( matrix3x4_t ) * 128 ) );
			setup.shot = false;

			/* setup player record */
			player_record[ entity->EntIndex( ) ].push_back( setup );
		}

		/* old simulation time */
		if ( old_sim_time[ entity->EntIndex( ) ] != entity->get_simulation_time( ) ) {
			/*
				** im setup : **
				* velocity
				* sim_time
				* shot
				* matrix
				* shot time
			*/

			setup.velocity = abs( entity->get_velocity( ).Length2D( ) );
			setup.sim_time = entity->get_simulation_time( );

			/* fix setup bones */
			if ( entity == csgo::m_local )
				entity->fix_setup_bones( c_ragebot.matrix[ entity->EntIndex( ) ] );

			/* setup */
			memcpy( setup.matrix, c_ragebot.matrix[ entity->EntIndex( ) ], ( sizeof( matrix3x4_t ) * 128 ) );

			/* fix entity */
			if ( entity->get_active_weapon( ) && !entity->is_knife_or_nade( ) ) {
				if ( shot_time[ entity->EntIndex( ) ] != entity->get_active_weapon( )->get_last_shot_time( ) ) {
					setup.shot = true;
					shot_time[ entity->EntIndex( ) ] = entity->get_active_weapon( )->get_last_shot_time( );
				}
				else
					setup.shot = false;
			}
			else {
				setup.shot = false;
				shot_time[ entity->EntIndex( ) ] = 0.f;
			}

			player_record[ entity->EntIndex( ) ].push_back( setup );
			old_sim_time[ entity->EntIndex( ) ] = entity->get_simulation_time( );
		}

		/* shot on ticks */
		shot_tick[ entity->EntIndex( ) ] = -1;
		if ( player_record[ entity->EntIndex( ) ].size( ) > 0 )
			for ( int tick = 0; tick < player_record[ entity->EntIndex( ) ].size( ); tick++ )
				if ( !valid_tick( time_to_ticks( player_record[ entity->EntIndex( ) ].at( tick ).sim_time ) ) )
					player_record[ entity->EntIndex( ) ].erase( player_record[ entity->EntIndex( ) ].begin( ) + tick );
					else if ( player_record[ entity->EntIndex( ) ].at( tick ).shot )
						shot_tick[ entity->EntIndex( ) ] = tick;
	}

	/* clear record */
	void lag_comp::clear_records( int i ) {
		if ( player_record[ i ].size( ) > 0 ) {
			for ( int tick = 0; tick < player_record[ i ].size( ); tick++ ) {
				player_record[ i ].erase( player_record[ i ].begin( ) + tick );
			}
		}
	}

	/* clamp */
	template<class T, class U>
	T lag_comp::clamp( T in, U low, U high ) {
		if ( in <= low )
			return low;

		if ( in >= high )
			return high;

		return in;
	}
}