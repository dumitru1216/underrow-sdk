#include "prediction_system.hpp"
#include "../../sdk/interfaces/c_prediction.hpp"
#include "../../sdk/interfaces/c_global_vars_base.hpp"

/* predefine */
namespace function {
	using namespace prediction_system;
	float m_old_curtime;
	float m_old_frametime;
	int* m_nPredictionRandomSeed;
	int* m_pSetPredictionPlayer;
	static bool m_initialize = false;

	/* start prediction */
	void prediction_system::start_prediction( ) {
		/* checks */
		if ( !csgo::m_local->is_alive( ) || !g_pMoveHelper || !variable.Config.enable_aimbot )
			return;

		/* predict_seed & player */
		if ( !m_nPredictionRandomSeed || !m_pSetPredictionPlayer ) {
			m_nPredictionRandomSeed = *reinterpret_cast< int** >( Utils::FindSignature( "client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04" ) + 2 );
			m_pSetPredictionPlayer = *reinterpret_cast< int** >( Utils::FindSignature( "client.dll", "89 35 ? ? ? ? F3 0F 10 46" ) + 2 );
		}

		CMoveData data;
		memset( &data, 0, sizeof( CMoveData ) );
		csgo::m_local->set_current_command( csgo::m_cmd );
		g_pMoveHelper->SetHost( csgo::m_local );

		/* internal command */
		*m_nPredictionRandomSeed = csgo::m_cmd->random_seed;
		*m_pSetPredictionPlayer = uintptr_t( csgo::m_local );
		*reinterpret_cast< uint32_t* >( reinterpret_cast< uint32_t >( csgo::m_local ) + 0x3314 ) = reinterpret_cast< uint32_t >( csgo::m_cmd ); // lol
		*reinterpret_cast< uint32_t* >( reinterpret_cast< uint32_t >( csgo::m_local ) + 0x326C ) = reinterpret_cast< uint32_t >( csgo::m_cmd ); // lol

		/* run variable */
		m_old_curtime = globalvars->curtime;
		m_old_frametime = globalvars->frametime;

		/* fix tick base */
		csgo::uRandomSeed = *m_nPredictionRandomSeed;
		globalvars->curtime = csgo::m_local->get_tick_base( ) * globalvars->intervalPerTick;
		globalvars->frametime = globalvars->intervalPerTick;

		/* fix movement tick */
		g_pMovement->StartTrackPredictionErrors( csgo::m_local );

		/* setup movement */
		g_pPrediction->SetupMove( csgo::m_local, csgo::m_cmd, g_pMoveHelper, &data );
		g_pMovement->ProcessMovement( csgo::m_local, &data );
		g_pPrediction->FinishMove( csgo::m_local, csgo::m_cmd, &data );

		/* fix weapon bug */
		if ( csgo::m_local->get_active_weapon( ) )
			csgo::m_local->get_active_weapon( )->accuracy_penalty( );
	}

	/* end prediction */
	void prediction_system::end_prediction( ) {
		/* checks */
		if ( !csgo::m_local->is_alive( ) || !g_pMoveHelper || !variable.Config.enable_aimbot )
			return;

		/* restore tick prediction */
		g_pMovement->FinishTrackPredictionErrors( csgo::m_local );
		g_pMoveHelper->SetHost( nullptr );

		/* restore predictino of player */
		if ( m_nPredictionRandomSeed || m_pSetPredictionPlayer ) {
			*m_nPredictionRandomSeed = -1;
			*m_pSetPredictionPlayer = 0;
		}

		/* restore cutime, frametime */
		globalvars->curtime = m_old_curtime;
		globalvars->frametime = m_old_frametime;

		/* restore current command */
		csgo::m_local->set_current_command( NULL );
	}
}