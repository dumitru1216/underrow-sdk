#include "ragebot.hpp"
#include "../../utils/other/sdk_math.hpp"
#include "../../sdk/interfaces/i_cvar.hpp"
#include "../../sdk/other/hitboxes.hpp"

/* predefine */
namespace function {
	ragebot c_ragebot;

	/* autostop */
	void ragebot::auto_stop( ) {
		/* checks */
		if ( variable.Config.auto_stop )
			return;

		/* get velocity */
		Vector velocity = csgo::m_local->get_velocity( );
		static float speed = 450.f;

		/* fix angles */
		Vector direction;
		Vector real_view;
		math.vector_angles( velocity, direction );
		engine->GetViewAngles( real_view );
		direction.y = real_view.y - direction.y;

		Vector forward;
		math.angle_vectors( direction, &forward );
		Vector negative_direction = forward * -speed;

		/* fix walk speed */
		csgo::m_cmd->forwardmove = negative_direction.x;
		csgo::m_cmd->sidemove = negative_direction.y;
	}

	/* hit_chance */
	bool ragebot::hit_chance( C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance ) {
		/* fix angle speed */
		float seeds = ( variable.Config.Hitchance == 1 ) ? 356.f : 256.f;
		Angle -= ( csgo::m_local->get_aim_punch_angle( ) * cvar->FindVar( "weapon_recoil_scale" )->GetFloat( ) );

		/* fix angles */
		Vector forward, right, up;
		math.angle_vectors( Angle, &forward, &right, &up );

		/* calculate hits & spread */
		int hits = 0, neededHits = ( seeds * ( chance / 100.f ) );
		float weapon_spread = pWeapon->get_spread( ), weapon_inaccuracy = pWeapon->get_inaccuracy( );

		/* check all entity visible */
		for ( int i = 0; i < seeds; i++ ) {
			/* calculate innacuracy and spead */
			float inaccuracy = math.random_float( 0.f, 1.f ) * weapon_inaccuracy;
			float spread = math.random_float( 0.f, 1.f ) * weapon_spread;

			/* spread view */
			Vector spread_view( ( cos( math.random_float( 0.f, 2.f * M_PI ) ) * inaccuracy ) + ( cos( math.random_float( 0.f, 2.f * M_PI ) ) * spread ), ( sin( math.random_float( 0.f, 2.f * M_PI ) ) * inaccuracy ) + ( sin( math.random_float( 0.f, 2.f * M_PI ) ) * spread ), 0 ), direction;
			direction = Vector( forward.x + ( spread_view.x * right.x ) + ( spread_view.y * up.x ), forward.y + ( spread_view.x * right.y ) + ( spread_view.y * up.y ), forward.z + ( spread_view.x * right.z ) + ( spread_view.y * up.z ) ).Normalize( );

			/* fix angles */
			Vector viewangles_spread, view_forword;
			math.vector_angles( direction, up, viewangles_spread );
			math.normalize_angles( viewangles_spread );

			math.angle_vectors( viewangles_spread, &view_forword );
			view_forword.NormalizeInPlace( );

			/* info about view system */
			view_forword = csgo::m_local->get_eye_position( ) + ( view_forword * pWeapon->weapon_info( )->range );

			/* fix trace */
			C_Trace Trace;
			trace->ClipRayToEntity( C_Ray( csgo::m_local->get_eye_position( ), view_forword ), mask_shot | contents_grate, pEnt, &Trace );

			if ( Trace.m_pEnt == pEnt )
				hits++;

			/* calculate damage */
			if ( ( ( hits / seeds ) * 100.f ) >= chance )
				return true;

			if ( ( seeds - i + hits ) < neededHits )
				return false;
		}

		return false;
	}

	/* setup hitscan */
	Vector ragebot::hitscan( C_BaseEntity* pEnt ) {
		/* variables */
		float damage_array[ 28 ];
		float temporary_damage = 0.f;
		Vector temporary_hitbox = { 0,0,0 };
		static int hitbox_for_multipoint[ ] = { 2,2,4,4,6,6 };
		float angle_to_local = math.calc_angle( csgo::m_local->get_origin( ), pEnt->get_origin( ) ).y;

		/* fix angles */
		Vector2D mutipoint_xy = { ( sin( math.grd_to_bog( angle_to_local ) ) ),( cos( math.grd_to_bog( angle_to_local ) ) ) };
		Vector2D mutipoint_xy180 = { ( sin( math.grd_to_bog( angle_to_local + 180 ) ) ) ,( cos( math.grd_to_bog( angle_to_local + 180 ) ) ) };
		Vector2D mutipoint[ ] = { Vector2D( mutipoint_xy.x, mutipoint_xy.y ), Vector2D( mutipoint_xy180.x, mutipoint_xy180.y ) };
	
		/* define scan */
		std::vector<int> scan;
		int head_height = 0;

		/* velocity shot improving accuracy */
		float csgo_velocity = abs( pEnt->get_velocity( ).Length2D( ) );
		if ( !variable.Config.delay_shot && csgo_velocity > 29.f )
			csgo_velocity = 30.f;

		/* hitscan */
		if ( csgo_velocity <= 215.f ) {
			/* hitscan type */
			switch ( variable.Config.hitscan_type ) {
				/* head only */
				case 0: {
					scan.push_back( ( int )hitbox_list_t::Head );
				} break;
				/* hitscan without head aim only */
				case 1: {
					scan.push_back( ( int )hitbox_list_t::Pelvis );
					scan.push_back( ( int )hitbox_list_t::Stomach );
					scan.push_back( ( int )hitbox_list_t::Chest );
					scan.push_back( ( int )hitbox_list_t::UpperChest );
					scan.push_back( ( int )hitbox_list_t::LeftUpperArm );
					scan.push_back( ( int )hitbox_list_t::RightUpperArm );
					scan.push_back( ( int )hitbox_list_t::LeftLowerArm );
					scan.push_back( ( int )hitbox_list_t::RightLowerArm );
					scan.push_back( ( int )hitbox_list_t::Neck );
					scan.push_back( ( int )hitbox_list_t::LeftThigh );
					scan.push_back( ( int )hitbox_list_t::RightThigh );
					scan.push_back( ( int )hitbox_list_t::LeftFoot );
					scan.push_back( ( int )hitbox_list_t::RightFoot );
					scan.push_back( ( int )hitbox_list_t::LeftShin );
					scan.push_back( ( int )hitbox_list_t::RightShin );
				} break;
				/* normal hitscan */
				case 2: {
					if ( variable.Config.multi_point ) {
						if ( variable.Config.head ) {
							scan.push_back( ( int )hitbox_list_t::Head );
						}
						if ( variable.Config.body ) {
							scan.push_back( ( int )hitbox_list_t::Pelvis );
							scan.push_back( ( int )hitbox_list_t::Stomach );
							scan.push_back( ( int )hitbox_list_t::Chest );
							scan.push_back( ( int )hitbox_list_t::UpperChest );
						}
						if ( variable.Config.arms ) {
							scan.push_back( ( int )hitbox_list_t::LeftUpperArm );
							scan.push_back( ( int )hitbox_list_t::RightUpperArm );
							scan.push_back( ( int )hitbox_list_t::LeftLowerArm );
							scan.push_back( ( int )hitbox_list_t::RightLowerArm );
						}
						if ( variable.Config.neck ) {
							scan.push_back( ( int )hitbox_list_t::Neck );
						}
						if ( variable.Config.legs ) {
							scan.push_back( ( int )hitbox_list_t::LeftThigh );
							scan.push_back( ( int )hitbox_list_t::RightThigh );
							scan.push_back( ( int )hitbox_list_t::LeftFoot );
							scan.push_back( ( int )hitbox_list_t::RightFoot );
							scan.push_back( ( int )hitbox_list_t::LeftShin );
							scan.push_back( ( int )hitbox_list_t::RightShin );
						}
					}
				} break;
			}
			head_height = variable.Config.head_scale;
		}

		/* setup hitbox */
		Vector Hitbox;
		int best_hitbox_position = 0;

		/* fix scaning */
		for ( int hitbox : scan ) {

			if ( hitbox < 19 )
				Hitbox = pEnt->get_hitbox_position( hitbox, matrix[ pEnt->EntIndex( ) ] );
			else if ( hitbox > 18 && hitbox < 25 )
			{
				float Radius = 0;
				Hitbox = pEnt->get_hitbox_position( hitbox_for_multipoint[ hitbox - 19 ], matrix[ pEnt->EntIndex( ) ], &Radius );
				Radius *= ( variable.Config.body_scale / 100.f );
				Hitbox = Vector( Hitbox.x + ( Radius * mutipoint[ ( ( hitbox - 19 ) % 2 ) ].x ), Hitbox.y - ( Radius * mutipoint[ ( ( hitbox - 19 ) % 2 ) ].y ), Hitbox.z );
			}
			else if ( hitbox > 24 && hitbox < 28 )
			{
				float Radius = 0;
				Hitbox = pEnt->get_hitbox_position( 0, matrix[ pEnt->EntIndex( ) ], &Radius );
				Radius *= ( head_height / 100.f );
				if ( hitbox != 27 )
					Hitbox = Vector( Hitbox.x + ( Radius * mutipoint[ ( ( hitbox - 25 ) % 2 ) ].x ), Hitbox.y - ( Radius * mutipoint[ ( ( hitbox - 25 ) % 2 ) ].y ), Hitbox.z );
				else
					Hitbox += Vector( 0, 0, Radius );
			}

			float Damage = c_autowall.damage( Hitbox );
			if ( Damage > 0.f )
				damage_array[ hitbox ] = Damage;
			else
				damage_array[ hitbox ] = 0;

			if ( damage_array[ hitbox ] > temporary_damage )
			{
				temporary_hitbox = Hitbox;
				best_hitbox_position = hitbox;
				temporary_damage = damage_array[ hitbox ];
			}

			csgo::m_aimbot_hitbox[ pEnt->EntIndex( ) ][ hitbox ] = Hitbox;
		}

		/* finish */
		player_records_t pPlayerEntityRecord = c_lag_comp.player_record[ pEnt->EntIndex( ) ].at( 0 );
		backtrack[ pEnt->EntIndex( ) ] = false;
		shot_backtrack[ pEnt->EntIndex( ) ] = false;

		if ( temporary_damage >= variable.Config.min_damage ) {
			best_entity_damage = temporary_damage;

			if ( ( best_hitbox_position == 25 || best_hitbox_position == 26 || best_hitbox_position == 27 ) && abs( damage_array[ HITBOX_HEAD ] - damage_array[ best_hitbox_position ] ) <= 10.f )
				return pEnt->get_hitbox_position( HITBOX_HEAD, matrix[ pEnt->EntIndex( ) ] );
			else if ( ( best_hitbox_position == 19 || best_hitbox_position == 20 ) && damage_array[ HITBOX_PELVIS ] > 30 )
				return pEnt->get_hitbox_position( HITBOX_PELVIS, matrix[ pEnt->EntIndex( ) ] );
			else if ( ( best_hitbox_position == 21 || best_hitbox_position == 22 ) && damage_array[ HITBOX_THORAX ] > 30 )
				return pEnt->get_hitbox_position( HITBOX_THORAX, matrix[ pEnt->EntIndex( ) ] );
			else if ( ( best_hitbox_position == 23 || best_hitbox_position == 24 ) && damage_array[ HITBOX_UPPER_CHEST ] > 30 )
				return pEnt->get_hitbox_position( HITBOX_UPPER_CHEST, matrix[ pEnt->EntIndex( ) ] );

			return temporary_hitbox;
		}
		return Vector( 0, 0, 0 );
	}

	/* setup */
	void ragebot::setup( ) {
		/* variables & calls */
		if ( !engine->is_in_game( ) ) return;

		Vector Aimpoint = { 0,0,0 };
		C_BaseEntity* Target = nullptr;
		int targetID = 0;
		int tempDmg = 0;
		static bool shot = false;

		for ( int i = 1; i <= engine->GetMaxClients( ); ++i ) {
			C_BaseEntity* pPlayerEntity = entity_list->GetClientEntity( i );

			if ( !pPlayerEntity || !pPlayerEntity->is_alive( ) || pPlayerEntity->IsDormant( ) ) {
				c_lag_comp.clear_records( i );
				continue;
			}

			c_lag_comp.store_records( pPlayerEntity );
			if ( pPlayerEntity == csgo::m_local || pPlayerEntity->get_team( ) == csgo::m_local->get_team( ) ) continue;

			/* original enemy angles */
			csgo::m_enemy_eye_angels[ i ] = pPlayerEntity->get_eye_angles( );

			if ( c_lag_comp.player_record[ i ].size( ) == 0 || !csgo::m_local->is_alive( ) || !variable.Config.enable_aimbot ) continue;
			if ( !csgo::m_local->get_active_weapon( ) || csgo::m_local->is_knife_or_nade( ) ) continue;

			best_entity_damage = 0;
			Vector Hitbox = hitscan( pPlayerEntity );
			if ( Hitbox != Vector( 0, 0, 0 ) && tempDmg <= best_entity_damage ) {
				Aimpoint = Hitbox;
				Target = pPlayerEntity;
				targetID = Target->EntIndex( );
				tempDmg = best_entity_damage;
			}
		}

		if ( !csgo::m_local->is_alive( ) ) {
			shot = false;
			return;
		}

		if ( !csgo::m_local->get_active_weapon( ) || csgo::m_local->is_knife_or_nade( ) ) {
			shot = false;
			return;
		}

		float flServerTime = csgo::m_local->get_tick_base( ) * global_vars->intervalPerTick;
		bool canShoot = ( csgo::m_local->get_active_weapon( )->get_next_primary_attack( ) <= flServerTime );
		if ( Target ) {
			csgo::m_target_index = targetID;
			float SimulationTime = 0.f;

			/* setup backtack */
			if ( backtrack[ targetID ] )
				SimulationTime = c_lag_comp.player_record[ targetID ].at( 0 ).sim_time;
			else
				SimulationTime = c_lag_comp.player_record[ targetID ].at( c_lag_comp.player_record[ targetID ].size( ) - 1 ).sim_time;

			/* restore backtrack */
			if ( shot_backtrack[ targetID ] )
				SimulationTime = c_lag_comp.player_record[ targetID ].at( c_lag_comp.shot_tick[ targetID ] ).sim_time;

			/* fix angles */
			Vector Angle = math.calc_angle( csgo::m_local->get_eye_position( ), Aimpoint );
			if ( csgo::m_local->get_velocity( ).Length( ) >= ( csgo::m_local->get_active_weapon( )->weapon_info( )->max_speed_alt * .34f ) - 5 && !GetAsyncKeyState( VK_SPACE ) )
				auto_stop( );

			/* fix hitchance */
			if ( !( csgo::m_cmd->buttons & IN_ATTACK ) && canShoot && hit_chance( Target, csgo::m_local->get_active_weapon( ), Angle, Aimpoint, variable.Config.hitchance_value ) ) {

				if ( !backtrack[ targetID ] && !shot_backtrack[ targetID ] )
					csgo::m_shot[ targetID ] = true;


				csgo::m_send_packet = true;
				shot = true;

				csgo::m_cmd->viewangles = Angle - ( csgo::m_local->get_aim_punch_angle( ) * cvar->FindVar( "weapon_recoil_scale" )->GetFloat( ) );
				csgo::m_cmd->buttons |= IN_ATTACK;
				csgo::m_cmd->tick_count = time_to_ticks( SimulationTime + c_lag_comp.lerp_time( ) );
			}
		}
	}
}