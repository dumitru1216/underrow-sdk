#pragma once
#include "../other/definitions.hpp"
#include "i_client_unknown.hpp"
#include "i_client_entity_list.hpp"
#include "c_input.hpp"
#include "../../utils\other\utils.hpp"
#include "../../utils\interfaces\netvar_manager.hpp"
#include "../other/data_map.hpp"

// class predefinition
class C_BaseCombatWeapon;
class CCSGOPlayerAnimState;

class c_anim_state
{
public:
	char pad[3];
	char bUnknown; //0x4
	char pad2[91];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];
};
class CCSGOPlayerAnimState
{
public:
	float GetDesyncDelta( )
	{
		auto animstate = uintptr_t( this );
		float duckammount = *( float* )( animstate + 0xA4 );
		float speedfraction = max( 0.f, min( *reinterpret_cast< float* >( animstate + 0xF8 ), 1.0f ) );

		float unk1 = ( ( *reinterpret_cast< float* > ( animstate + 0x11C ) * -0.3f ) - 0.2f ) * speedfraction;
		float unk2 = unk1 + 1.f;

		if ( duckammount > 0.f ) {
			float speedfactor = max( 0.f, min( 1.f, *reinterpret_cast< float* > ( animstate + 0xFC ) ) );
			unk2 += ( ( duckammount * speedfactor ) * ( 0.5f - unk2 ) );
		}
		return  *( float* )( animstate + 0x334 ) * unk2;
	}

	void* pThis;
	char pad2[ 91 ];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flLastClientSideAnimationUpdateTimeDelta;
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[ 4 ]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[ 4 ];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[ 8 ];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	bool m_bOnGround; //0x108
	bool m_bInHitGroundAnimation; //0x109
	char pad7[ 10 ];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flGroundFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	bool m_bJust_Landed;
	bool m_bJust_LeftGround;
	char pad_0120[ 2 ];
	float m_flDuckRate;
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[ 4 ]; //NaN
	float m_flUnknown3;
	char pad10[ 528 ];
};

class animation_layer {
public:
	char  pad_0000[20];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	unsigned int m_nOrder; //0x0014
	unsigned int m_nSequence; //0x0018
	float m_flPrevCycle; //0x001C
	float m_flWeight; //0x0020
	float m_flWeightDeltaRate; //0x0024
	float m_flPlaybackRate; //0x0028
	float m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038

struct animstate_pose_param_cache_t {
	std::uint8_t pad_0x0[ 0x4 ]; //0x0
	std::uint32_t m_idx; //0x4 
	char* m_name; //0x8
	void set_value( C_BaseEntity* e, float val );
};

class CBasePlayerAnimState
{
public:
	char pad[ 4 ];
	char bUnknown; //0x4
	char pad2[ 91 ];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[ 4 ]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[ 4 ];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[ 8 ];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[ 10 ];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[ 4 ]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[ 4 ]; //NaN
	float m_flUnknown3;
	char pad10[ 528 ];
	animstate_pose_param_cache_t m_jump_fall_pose; //0x0228

	float& m_flAbsRotation( ) {
		return *( float* )( ( uintptr_t )this + 0x80 );
	}
};

class C_BaseEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable
{
private:
    template<class T>
    T GetPointer(const int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
    // To get value from the pointer itself
    template<class T>
    T get_value(const int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }

	template <typename T>
	T& SetValue(uintptr_t offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

public:
	c_anim_state *anim_state() {
		return *reinterpret_cast< c_anim_state**>(uintptr_t(this) + 0x3914);
	}
	animation_layer *anim_overlays() {
		return *reinterpret_cast< animation_layer**>(uintptr_t(this) + 0x2980);
	}
	int num_overlays() {
		return 15;
	}
	void update_client_side_animation() {
		Utils::get_vfunc<void(__thiscall*)(void*)>(this, 223)(this);
	} 
	void client_animations(bool value) {
		static int m_bClientSideAnimation = g_pNetvars->GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");
		*reinterpret_cast<bool*>(uintptr_t(this) + m_bClientSideAnimation) = value;
	}
	int get_sequence() {
		static int m_nSequence = g_pNetvars->GetOffset("DT_BaseAnimating", "m_nSequence");
		return get_value<int>(m_nSequence);
	}
	void set_sequence(int Sequence) {
		static int m_nSequence = g_pNetvars->GetOffset("DT_BaseAnimating", "m_nSequence");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nSequence) = Sequence;
	}
	void simulated_every_tick(bool value) {
		static int m_bSimulatedEveryTick = g_pNetvars->GetOffset("DT_BaseEntity", "m_bSimulatedEveryTick");
		*reinterpret_cast<bool*>(uintptr_t(this) + m_bSimulatedEveryTick) = value;
	}
	Vector get_abs_origin() {
		return Utils::get_vfunc<Vector&(__thiscall*)(void*)>(this, 10)(this);
	}
    C_BaseCombatWeapon* get_active_weapon() {
        static int m_hActiveWeapon = g_pNetvars->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
        const auto weaponData      = get_value<CBaseHandle>(m_hActiveWeapon);
        return reinterpret_cast<C_BaseCombatWeapon*>(entity_list->GetClientEntityFromHandle(weaponData));
    }
    int get_team() {
        static int m_iTeamNum = g_pNetvars->GetOffset("DT_BaseEntity", "m_iTeamNum");
        return get_value<int>(m_iTeamNum);
    }
    EntityFlags get_flags() {
        static int m_fFlags = g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags");
        return get_value<EntityFlags>(m_fFlags);
    }
	void set_flags(int offset) {
		static int m_fFlags = g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags");
		*reinterpret_cast<int*>(uintptr_t(this) + m_fFlags) = offset;
	}
    MoveType_t get_move_type() {
        static int m_Movetype = g_pNetvars->GetOffset("DT_BaseEntity", "m_nRenderMode") + 1;
        return get_value<MoveType_t>(m_Movetype);
    }
	float get_simulation_time() {
		static int m_flSimulationTime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime");
		return get_value<float>(m_flSimulationTime);
	}
	float get_old_simulation_time() {
		static int m_flOldSimulationTime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime") + 4;
		return get_value<float>(m_flOldSimulationTime);
	}
	float get_lowerbody_yaw() {
		static int m_flLowerBodyYawTarget = g_pNetvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		return get_value<float>(m_flLowerBodyYawTarget);
	}
	void set_lowerbody_yaw(float value) {
		static int m_flLowerBodyYawTarget = g_pNetvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		*reinterpret_cast<float*>(uintptr_t(this) + m_flLowerBodyYawTarget) = value;
	}
	bool get_heavy_armor() {
		static int m_bHasHeavyArmor = g_pNetvars->GetOffset("DT_CSPlayer", "m_bHasHeavyArmor");
		return get_value<bool>(m_bHasHeavyArmor);
	}
	int armor_value() {
		static int m_ArmorValue = g_pNetvars->GetOffset("DT_CSPlayer", "m_ArmorValue");
		return get_value<int>(m_ArmorValue);
	}
	bool is_player( ) {
		return Utils::get_vfunc< bool( __thiscall* ) ( C_BaseEntity* ) >( this, 157 ) ( this );
	}
	int& m_fleffct( ) {
		static unsigned int _m_flMaxspeed = Utils::find_in_datamap( GetPredDescMap( ), "m_fEffects" );
		return *( int* )( ( uintptr_t )this + _m_flMaxspeed );
	}
	bool has_hemlet() {
		static int m_bHasHelmet = g_pNetvars->GetOffset("DT_CSPlayer", "m_bHasHelmet");
		return get_value<bool>(m_bHasHelmet);
	}
    bool get_life_state() {
        static int m_lifeState = g_pNetvars->GetOffset("DT_BasePlayer", "m_lifeState");
        return get_value<bool>(m_lifeState);
    }
	bool is_scoped() {
		static int m_bIsScoped = g_pNetvars->GetOffset("DT_CSPlayer", "m_bIsScoped");
		return get_value<bool>(m_bIsScoped);
	}
	int get_health( ) {
		static int m_iHealth = g_pNetvars->GetOffset( "DT_BasePlayer", "m_iHealth" );
		return get_value<int>( m_iHealth );
	}
    bool is_alive() { 
		return this->get_health() > 0 && this->get_life_state() == 0; 
	}
    bool is_imune() {
        static int m_bGunGameImmunity = g_pNetvars->GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
        return get_value<bool>(m_bGunGameImmunity);
    }
    int get_tick_base() {
        static int m_nTickBase = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
        return get_value<int>(m_nTickBase);
    }
	int get_shots_fired() {
		static int m_iShotsFired = g_pNetvars->GetOffset("DT_CSPlayer", "cslocaldata", "m_iShotsFired");
		return get_value<int>(m_iShotsFired);
	}
	void set_tickbase(int TickBase) {
		static int m_nTickBase = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nTickBase) = TickBase;
	}
	Vector get_eye_angles() {
		static int m_angEyeAngles = g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles");
		return get_value<Vector>(m_angEyeAngles);
	}
	void set_eye_angles(Vector Angle) {
		static int m_angEyeAngles = g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_angEyeAngles) = Angle;
	}
    Vector get_origin() {
        static int m_vecOrigin = g_pNetvars->GetOffset("DT_BaseEntity", "m_vecOrigin");
        return get_value<Vector>(m_vecOrigin);
    }
	Vector get_old_origin() {
		static int m_vecOldOrigin = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_flFriction") + 12;
		return get_value<Vector>(m_vecOldOrigin);
	}
	Vector get_network_origin() {
		static int m_vecNetworkOrigin = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_flFriction") + 40;
		return get_value<Vector>(m_vecNetworkOrigin);
	} 
	void set_origin(Vector Origin) {
		static int m_vecOrigin = g_pNetvars->GetOffset("DT_BaseEntity", "m_vecOrigin");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_vecOrigin) = Origin;
	}
	Vector get_velocity() {
		static int m_vecVelocity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		return get_value<Vector>(m_vecVelocity);
	}
	void set_velocity(Vector velocity) {
		static int m_vecVelocity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_vecVelocity) = velocity;
	}
	Vector get_aim_punch_angle() {
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x302C));
	}
	datamap_t* GetPredDescMap( ) {
		typedef datamap_t* ( __thiscall* o_GetPredDescMap )( void* );
		return Utils::get_vfunc<o_GetPredDescMap>( this, 17 )( this );
	}
	int& m_iflags( ) {
		static unsigned int _m_flMaxspeed = Utils::find_in_datamap( GetPredDescMap( ), "m_iEFlags" );
		return *( int* )( ( uintptr_t )this + _m_flMaxspeed );
	}
	datamap_t* GetDataDescMap( ) {
		typedef datamap_t* ( __thiscall* o_GetPredDescMap )( void* );
		return Utils::get_vfunc<o_GetPredDescMap>( this, 15 )( this );
	}
	Vector& get_abs_velocity( ) {
		static unsigned int _m_flMaxspeed = Utils::find_in_datamap( GetPredDescMap( ), "m_vecAbsVelocity" );
		return *( Vector* )( ( uintptr_t )this + _m_flMaxspeed );
	}
	float_t spawn_time( ) {
		return *( float_t* )( ( uintptr_t )this + 0xA360 );
	}
	Vector get_view_punch_angle() {
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x3020));
	}
	template<typename FuncType>
	__forceinline static FuncType call_vfunction( void* ppClass, int index ) {
		int* pVTable = *( int** )ppClass;
		int dwAddress = pVTable[ index ];
		return ( FuncType )( dwAddress );
	}
	uint32_t& get_most_recent_model_bone_counter( ) {
		return *reinterpret_cast< uint32_t* >( uintptr_t( this ) + 0x2690 );
	}
	float& get_last_bone_setup_time( ) {
		return *reinterpret_cast< float* >( uintptr_t( this ) + 0x2924 );
	}
	Vector& get_abs_angles( ) {
		if ( !this )
			return Vector( );
		typedef Vector& ( __thiscall* OriginalFn )( void* );
		return call_vfunction<OriginalFn>( this, 11 )( this );
	}
	std::array< float, 24 >& pose_parametrer( ) {
		static int _m_flPoseParameter = g_pNetvars->GetOffset( "DT_BaseAnimating", "m_flPoseParameter" );
		return *reinterpret_cast< std::array<float, 24>* >( reinterpret_cast< std::uintptr_t >( this ) + _m_flPoseParameter );
	}
    Vector get_eye_position()  {
		Vector ret;
		typedef void(__thiscall *OrigFn)(void *, Vector&);
		Utils::get_vfunc<OrigFn>(this, 284)(this, ret);
		return ret;
	}
	ICollideable* get_collideable() {
		return (ICollideable*)((DWORD)this + 0x318);
	}
	void set_current_command(CUserCmd *cmd) {
		static int m_hConstraintEntity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_hConstraintEntity");
		*reinterpret_cast<CUserCmd**>(uintptr_t(this) + m_hConstraintEntity - 0xC) = cmd;
	}
	char* get_armor_name( ) {
		if ( armor_value( ) > 0 ) {
			if ( has_hemlet( ) )
				return "hk";
			else
				return "k";
		}
		else
			return " ";
	}

	/* we set here c_entity.cpp extern defs */
	void set_abs_angles( Vector angles );
	void set_abs_origin( Vector origin );
	void set_abs_velocity( Vector velocity );
	void fix_setup_bones(matrix3x4_t *Matrix);
	Vector get_hitbox_position(int Hitbox, matrix3x4_t *Matrix, float *Radius);
	Vector get_hitbox_position(int Hitbox, matrix3x4_t *Matrix);
	bool is_knife_or_nade( );
	bool is_enemy( );
	float fire_rate( );
	CBasePlayerAnimState* get_animation_state( );
	static void update_animation_state( CBasePlayerAnimState* state, Vector angle );
	static void reset_animation_state( CBasePlayerAnimState* state );
	void create_animation_state( CBasePlayerAnimState* state );
	void invalidate_bone_cache( void );
};


class C_BaseCombatWeapon : public C_BaseEntity
{
private:
    template<class T>
    T GetPointer(const int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
    // To get value from the pointer itself
    template<class T>
    T get_value(const int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }

public:
    ItemDefinitionIndex get_item_definition_index() {
        static int m_iItemDefinitionIndex = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
        return get_value<ItemDefinitionIndex>(m_iItemDefinitionIndex);
    }
	float get_inaccuracy() {
		typedef float(__thiscall* oInaccuracy)(PVOID);
		return Utils::get_vfunc< oInaccuracy >(this, 482)(this);
	}
	float get_spread() {
		typedef float(__thiscall* oWeaponSpread)(PVOID);
		return Utils::get_vfunc< oWeaponSpread >(this, 452)(this);
	}
	float get_accuracy_penalty() {
		static int m_fAccuracyPenalty = g_pNetvars->GetOffset("DT_WeaponCSBase", "m_fAccuracyPenalty");
		return get_value<float>(m_fAccuracyPenalty);
	}
	float get_last_shot_time() {
		static int m_fLastShotTime = g_pNetvars->GetOffset("DT_WeaponCSBase", "m_fLastShotTime");
		return get_value<float>(m_fLastShotTime);
	}
	void accuracy_penalty() {
		typedef void(__thiscall *OrigFn)(void *);
		return Utils::get_vfunc<OrigFn>(this, 483)(this);
	}
    float get_next_primary_attack() {
        static int m_flNextPrimaryAttack = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
        return get_value<float>(m_flNextPrimaryAttack);
    }
    int get_ammo() {
        static int m_iClip1 = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "m_iClip1");
        return get_value<int>(m_iClip1);
    }
    WeaponInfo_t* weapon_info() {
        return Utils::call_vfunc<460, WeaponInfo_t*>(this);
    }
    std::string get_name() {
        return std::string(this->weapon_info()->weapon_name);
    }
};
