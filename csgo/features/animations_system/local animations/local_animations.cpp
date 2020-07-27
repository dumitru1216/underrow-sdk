#include "local_animations.hpp"
#include "../../../sdk/interfaces/iv_model_info.hpp"
#include "../../../sdk/interfaces/i_memory_alloc.hpp"
#include "../../../sdk/interfaces/i_cvar.hpp"

namespace function {
	local_animations c_local_animations;

	bool fresh_tick( ) {
		static int old_tick_count;

		if ( old_tick_count != global_vars->tickcount )
		{
			old_tick_count = global_vars->tickcount;
			return true;
		}
	}

	/* manage local animations */
	typedef std::array<float, 24> pose_paramater;
	void local_animations::manage_local_animations( C_BaseEntity* entity ) {
		if ( !entity || !entity->is_alive( ) || !csgo::m_cmd )
			return;

		static float proper_abs = entity->get_animation_state( )->m_flGoalFeetYaw;
		static std::array<float, 24> sent_pose_params = entity->pose_parametrer( );
		static animation_layer backup_layers[ 15 ];

		if ( fresh_tick( ) ) {
			std::memcpy( backup_layers, entity->anim_overlays( ), ( sizeof( animation_layer ) * entity->num_overlays( ) ) );
			entity->client_animations( true );
			entity->update_animation_state( entity->get_animation_state( ), csgo::m_cmd->viewangles );

			entity->get_animation_state( )->m_bOnGround = csgo::m_local->get_flags( ) & FL_ONGROUND;
			if ( entity->get_animation_state( )->m_iLastClientSideAnimationUpdateFramecount >= global_vars->framecount ) {
				entity->get_animation_state( )->m_iLastClientSideAnimationUpdateFramecount = global_vars->framecount - 1;
			}

			/* fixing hand ovelay */
			entity->update_client_side_animation( );

			/* view the packet is > 0 */
			if ( csgo::m_send_packet ) {
				proper_abs = entity->get_animation_state( )->m_flGoalFeetYaw;
				sent_pose_params = entity->pose_parametrer( );

			}
			/* fixing the feet rate */
			entity->get_animation_state( )->m_flFeetYawRate = 0.f;
			entity->get_animation_state( )->m_flUnknownFraction = 0.f;
		}

		entity->client_animations( false );
		entity->pose_parametrer( ) = sent_pose_params;
		std::memcpy( entity->anim_overlays( ), backup_layers, ( sizeof( animation_layer ) * entity->num_overlays( ) ) );
		entity->set_abs_angles( Vector( 0, proper_abs, 0.f ) );
		entity->invalidate_bone_cache( );
	}

	/* manager fake animation variables */
	bool m_should_update_fake = false;
	std::array< animation_layer, 15 > m_fake_layers; // 13
	std::array< float, 24 > m_fake_poses; // 20
	CBasePlayerAnimState* m_fake_state = nullptr;
	bool init_fake_anim = false;
	float m_fake_spawntime = 0.f;
	matrix3x4_t m_fake_position_matrix[ 128 ];

	/* manage fake animations */
	void local_animations::manage_fake_animations( ) {
		if ( !csgo::m_local || !csgo::m_local->is_alive( ) ) {
			m_should_update_fake = true;
			return;
		}

		if ( m_fake_spawntime != csgo::m_local->spawn_time( ) || m_should_update_fake ) {
			init_fake_anim = false;
			m_fake_spawntime = csgo::m_local->spawn_time( );
			m_should_update_fake = false;
		}

		if ( !init_fake_anim ) {
			m_fake_state = reinterpret_cast< CBasePlayerAnimState* > ( memory_alloc->alloc( sizeof( CBasePlayerAnimState ) ) );

			if ( m_fake_state != nullptr )
				csgo::m_local->create_animation_state( m_fake_state );

			init_fake_anim = true;
		}

		if ( csgo::m_send_packet ) {
			int OldFrameCount = global_vars->framecount;
			int OldTickCount = global_vars->tickcount;
			static auto host_timescale = cvar->FindVar( ( "host_timescale" ) );

			global_vars->framecount = time_to_ticks( csgo::m_local->get_simulation_time( ) );
			global_vars->tickcount = time_to_ticks( csgo::m_local->get_simulation_time( ) );

			std::memcpy( m_fake_layers.data( ), csgo::m_local->anim_overlays( ), sizeof( m_fake_layers ) );
			std::memcpy( m_fake_poses.data( ), csgo::m_local->pose_parametrer( ).data( ), sizeof( m_fake_poses ) );

			csgo::m_local->update_animation_state( m_fake_state, csgo::m_cmd->viewangles );

			const auto backup_absangles = csgo::m_local->get_abs_angles( );

			/* invalidate bone cache */
			csgo::m_local->get_most_recent_model_bone_counter( ) = 0;
			csgo::m_local->get_last_bone_setup_time( ) = -FLT_MAX;
			csgo::m_got_fake_matrix = csgo::m_local->SetupBones( csgo::m_fake_matrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING & ~BONE_USED_BY_ATTACHMENT, global_vars->curtime );

			memcpy( m_fake_position_matrix, csgo::m_fake_matrix, sizeof( m_fake_position_matrix ) );
			const auto org_tmp = csgo::m_local->GetRenderOrigin( );

			if ( csgo::m_got_fake_matrix ) {
				for ( auto& i : csgo::m_fake_matrix ) {
					i[ 0 ][ 3 ] -= org_tmp.x;
					i[ 1 ][ 3 ] -= org_tmp.y;
					i[ 2 ][ 3 ] -= org_tmp.z;
				}
			}

			csgo::m_local->set_abs_angles( backup_absangles ); // restore real abs rotation

			/* invalidate bone cache */
			csgo::m_local->get_most_recent_model_bone_counter( ) = 0;
			csgo::m_local->get_last_bone_setup_time( ) = -FLT_MAX;

			std::memcpy( csgo::m_local->anim_overlays( ), m_fake_layers.data( ), sizeof( m_fake_layers ) );
			std::memcpy( csgo::m_local->pose_parametrer( ).data( ), m_fake_poses.data( ), sizeof( m_fake_poses ) );

			global_vars->framecount = OldFrameCount;
			global_vars->tickcount = OldTickCount;
		}
		
	}
}