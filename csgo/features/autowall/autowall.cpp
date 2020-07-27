#include "autowall.hpp"
#include "../../sdk/interfaces/iv_engine_client.hpp"
#include "../../sdk/other/player_info.hpp"
#include "../../sdk/interfaces/i_surface_data.hpp"
#include "../../sdk/other/hitboxes.hpp"
#include "../../sdk/other/bsp_flags.hpp"
#include "../../sdk/interfaces/i_cvar.hpp"
#include "../../utils/other/sdk_math.hpp"
#include "../../sdk/interfaces/c_client_class.hpp"
#include <algorithm>

/* predefine */
namespace function {
	autowall c_autowall;

	/* trace line */
	void trace_line( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, C_BaseEntity* ignore, C_Trace* ptr ) {
		C_TraceFilter filter( ignore );
		trace->TraceRay( C_Ray( vecAbsStart, vecAbsEnd ), mask, &filter, ptr );
	}

	/* vectorto_vector_visible */
	bool vectorto_vector_visible( Vector src, Vector point ) {
		/* setup trae line */
		C_Trace trace_initialize;
		trace_line( src, point, mask_solid, csgo::m_local, &trace_initialize );
		C_Trace Trace;
		trace_line( src, point, mask_solid, trace_initialize.m_pEnt, &Trace );

		if ( Trace.flFraction == 1.0f || trace_initialize.flFraction == 1.0f )
			return true;

		return false;
	}

	/* can_hit_group_damage_multiple */
	float can_hit_group_damage_multiple( int iHitGroup ) {
		switch ( iHitGroup ) {
			case HITGROUP_HEAD: 
				return 4.f;
			case HITGROUP_STOMACH:
				return 1.25f;
			case HITGROUP_LEFTLEG:
			case HITGROUP_RIGHTLEG:
				return 0.75f;
		}

		return 1.0f;
	}

	/* handle_bullet_penetration */
	bool handle_bullet_penetration( WeaponInfo_t* wpn_data, fire_bullet_data_t& data, bool extracheck, Vector point );

	/* scale_damage */
	void scale_damage( int hitgroup, C_BaseEntity* enemy, float weapon_armor_ratio, float& current_damage ) {
		current_damage *= can_hit_group_damage_multiple( hitgroup );
		if ( enemy->armor_value( ) > 0.0f && hitgroup < HITGROUP_LEFTLEG ) {
			if ( hitgroup == HITGROUP_HEAD && !enemy->has_hemlet( ) )
				return;

			float armorscaled = ( weapon_armor_ratio * 0.5f ) * current_damage;
			if ( ( current_damage - armorscaled ) * 0.5f > enemy->armor_value( ) )
				armorscaled = current_damage - ( enemy->armor_value( ) * 2.0f );
			current_damage = armorscaled;
		}
	}

	/* signature */
	void util_clip_trace_to_players( const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, C_Trace* tr ) {
		static DWORD dword_adress = Utils::FindPattern( "client.dll", ( BYTE* )"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10", "xxxxxxxxxxxxxxxxxxxxxxxx????xxx" );

		if ( !dword_adress )
			return;

		_asm  {
			MOV		EAX, filter
			LEA		ECX, tr
			PUSH	ECX
			PUSH	EAX
			PUSH	mask
			LEA		EDX, vecAbsEnd
			LEA		ECX, vecAbsStart
			CALL	dword_adress
			ADD		ESP, 0xC
		}
	}

	/* simulate_fire_bullet */
	bool simulate_fire_bullet( C_BaseEntity* local, C_BaseCombatWeapon* weapon, fire_bullet_data_t& data ) {
		/* variables */
		data.penetrate_count = 4;
		data.trace_length = 0.0f;
		auto* wpn_data = weapon->weapon_info( );
		data.current_damage = ( float )wpn_data->damage;

		/* setup */
		while ( ( data.penetrate_count > 0 ) && ( data.current_damage >= 1.0f ) ) {
			/* setup data */
			data.trace_length_remaining = wpn_data->range - data.trace_length;
			Vector end_point = data.src + data.direction * data.trace_length_remaining;

			/* setup traceline & cliptotrace */
			trace_line( data.src, end_point, 0x4600400B, local, &data.enter_trace );
			util_clip_trace_to_players( data.src, end_point * 40.f, 0x4600400B, &data.filter, &data.enter_trace );

			/* setup entity break */
			if ( data.enter_trace.flFraction == 1.0f )
				break;

			/* setup hitgroup */
			if ( ( data.enter_trace.hitGroup <= 7 ) && ( data.enter_trace.hitGroup > 0 ) && ( local->get_team( ) != data.enter_trace.m_pEnt->get_team( ) ) ) {
				data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
				data.current_damage *= pow( wpn_data->range_modifier, data.trace_length * 0.002 );
				scale_damage( data.enter_trace.hitGroup, data.enter_trace.m_pEnt, wpn_data->armor_ratio, data.current_damage );
				return true;
			}

			/* setup handle bullet */
			if ( !handle_bullet_penetration( wpn_data, data, false, Vector( 0, 0, 0 ) ) )
				break;
		}
		return false;
	}

	/* trace to exit */
	bool trace_to_exit_alt( Vector& end, C_Trace& tr, Vector start, Vector vEnd, C_Trace* trace ) {
		typedef bool( __fastcall* TraceToExitFn )( Vector&, C_Trace&, float, float, float, float, float, float, C_Trace* );
		static TraceToExitFn o_trace_to_exit = ( TraceToExitFn )Utils::FindSignature( "client.dll", "55 8B EC 83 EC 30 F3 0F 10 75" );

		if ( !o_trace_to_exit )
			return false;

		return o_trace_to_exit( end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace );
	}

	/* bulet penetration */
	bool handle_bullet_penetration( WeaponInfo_t* wpn_data, fire_bullet_data_t& data, bool extracheck, Vector point ) {
		/* variables */
		surfacedata_t* enter_surface_data = surface_props_data->GetSurfaceData( data.enter_trace.surface.surfaceProps );
		int enter_material = enter_surface_data->game.material;
		float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
		data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
		data.current_damage *= pow( wpn_data->range_modifier, ( data.trace_length * 0.002 ) );

		/* setup penetrate cound */
		if ( ( data.trace_length > 3000.f ) || ( enter_surf_penetration_mod < 0.1f ) )
			data.penetrate_count = 0;
		if ( data.penetrate_count <= 0 )
			return false;

		/* setup dummy */
		Vector dummy;
		C_Trace trace_exit;
		if ( !trace_to_exit_alt( dummy, data.enter_trace, data.enter_trace.end, data.direction, &trace_exit ) )
			return false;

		/* variables */
		surfacedata_t* exit_surface_data = surface_props_data->GetSurfaceData( trace_exit.surface.surfaceProps );
		int exit_material = exit_surface_data->game.material;
		float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
		float final_damage_modifier = 0.16f;
		float combined_penetration_modifier = 0.0f;

		/* setup penetrate cound */
		if ( ( ( data.enter_trace.contents & contents_grate ) != 0 ) || ( enter_material == 89 ) || ( enter_material == 71 ) ) {
			combined_penetration_modifier = 3.0f;
			final_damage_modifier = 0.05f;
		}
		else
			combined_penetration_modifier = ( enter_surf_penetration_mod + exit_surf_penetration_mod ) * 0.5f;

		/* setup enter material */
		if ( enter_material == exit_material ) {
			if ( exit_material == 87 || exit_material == 85 )combined_penetration_modifier = 3.0f;
			else if ( exit_material == 76 )combined_penetration_modifier = 2.0f;
		}

		/* setup the materials */
		float v34 = fmaxf( 0.f, 1.0f / combined_penetration_modifier );
		float v35 = ( data.current_damage * final_damage_modifier ) + v34 * 3.0f * fmaxf( 0.0f, ( 3.0f / wpn_data->penetration ) * 1.25f );

		/* tickness damage */
		float thickness = VectorLength( trace_exit.end - data.enter_trace.end );
		if ( extracheck )
			if ( !vectorto_vector_visible( trace_exit.end, point ) )
				return false;

		thickness *= thickness;
		thickness *= v34;
		thickness /= 24.0f;

		/* lost damage */
		float lost_damage = fmaxf( 0.0f, v35 + thickness );
		if ( lost_damage > data.current_damage )
			return false;
		if ( lost_damage >= 0.0f )
			data.current_damage -= lost_damage;
		if ( data.current_damage < 1.0f )
			return false;
		data.src = trace_exit.end;
		data.penetrate_count--;

		return true;
	}

	/* damage */
	float autowall::damage( const Vector& point ) {
		auto data = fire_bullet_data_t( csgo::m_local->get_eye_position( ), csgo::m_local );

		Vector angles;
		angles = math.calc_angle( data.src, point );
		math.angle_vectors( angles, &data.direction );
		VectorNormalize( data.direction );

		if ( simulate_fire_bullet( csgo::m_local, csgo::m_local->get_active_weapon( ), data ) )
			return data.current_damage;

		return 0.f;
	}

	/* can hit point */
	bool autowall::can_hit_floating_point( const Vector& point, const Vector& source ) {
		/* checks */
		if ( !csgo::m_local )
			return false;

		/* restore data */
		fire_bullet_data_t data = fire_bullet_data_t( source, csgo::m_local );

		/* fix angles */
		Vector angles = math.calc_angle( data.src, point );
		math.angle_vectors( angles, &data.direction );
		VectorNormalize( data.direction );

		/* setup weapon damage */
		C_BaseCombatWeapon* pWeapon = ( C_BaseCombatWeapon* )csgo::m_local->get_active_weapon( );
		if ( !pWeapon )
			return false;

		data.penetrate_count = 1;
		data.trace_length = 0.0f;

		/* setup weapon data */
		WeaponInfo_t* weaponData = pWeapon->weapon_info( );
		if ( !weaponData )
			return false;

		data.current_damage = ( float )weaponData->damage;
		data.trace_length_remaining = weaponData->range - data.trace_length;
		Vector end = data.src + ( data.direction * data.trace_length_remaining );
		trace_line( data.src, end, mask_shot | contents_hitbox, csgo::m_local, &data.enter_trace );

		if ( vectorto_vector_visible( data.src, point ) || handle_bullet_penetration( weaponData, data, true, point ) )
			return true;

		return false;
	}
}