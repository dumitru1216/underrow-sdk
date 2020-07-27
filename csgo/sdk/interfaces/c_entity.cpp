#include "c_entity.hpp"
#include "iv_model_info.hpp"
#include "c_global_vars_base.hpp"
#include "..\../utils\globals\globals.hpp"
#include "..\../utils\other\sdk_math.hpp"

Vector C_BaseEntity::get_hitbox_position(int Hitbox, matrix3x4_t *Matrix, float *Radius)
{
	studiohdr_t* hdr = model_info->GetStudiomodel(this->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
	mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

	if (hitbox)
	{
		Vector vMin, vMax, vCenter, sCenter;
		math.VectorTransform(hitbox->min, Matrix[hitbox->bone], vMin);
		math.VectorTransform(hitbox->max, Matrix[hitbox->bone], vMax);
		vCenter = (vMin + vMax) * 0.5;

		*Radius = hitbox->radius;

		return vCenter;
	}
	
	return Vector(0, 0, 0);
}

Vector C_BaseEntity::get_hitbox_position(int Hitbox, matrix3x4_t *Matrix) // any public source
{
	studiohdr_t* hdr = model_info->GetStudiomodel(this->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
	mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

	if (hitbox)
	{
		Vector vMin, vMax, vCenter, sCenter;
		math.VectorTransform(hitbox->min, Matrix[hitbox->bone], vMin);
		math.VectorTransform(hitbox->max, Matrix[hitbox->bone], vMax);
		vCenter = (vMin + vMax) * 0.5;

		return vCenter;
	}

	return Vector(0, 0, 0);
}

void C_BaseEntity::fix_setup_bones(matrix3x4_t *Matrix)
{
	int Backup = *(int*)((uintptr_t)this + 0x274);
	*(int*)((uintptr_t)this + 0x274) = 0;
	Vector absOriginBackupLocal = this->get_abs_origin();
	this->set_abs_origin(this->get_origin());
	this->SetupBones(Matrix, 128, 0x00000100, global_vars->curtime);
	this->set_abs_origin(absOriginBackupLocal);
	*(int*)((uintptr_t)this + 0x274) = Backup;
}

//SanekGame https://www.unknowncheats.me/forum/1798568-post2.html

void C_BaseEntity::set_abs_angles(Vector angles)
{
	using Fn = void(__thiscall*)(C_BaseEntity*, const Vector& angles);
	static Fn AbsAngles = (Fn)(Utils::FindPattern("client.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));

	AbsAngles(this, angles);
}

void C_BaseEntity::set_abs_origin(Vector origin)
{
	using Fn = void(__thiscall*)(void*, const Vector &origin);
	static Fn AbsOrigin = (Fn)Utils::find_signature("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");

	AbsOrigin(this, origin);
}

void C_BaseEntity::set_abs_velocity(Vector velocity) // i dont remember
{
	using Fn = void(__thiscall*)(void*, const Vector &velocity);
	static Fn AbsVelocity = (Fn)Utils::find_signature("client.dll", "55 8B EC 83 E4 F8 83 EC 0C 53 56 57 8B 7D 08 8B F1 F3");

	AbsVelocity(this, velocity);
}

bool C_BaseEntity::is_knife_or_nade()
{
	if (!this)
		return false;
	if (!this->is_alive())
		return false;

	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->get_active_weapon();

	if (!pWeapon)
		return false;

	std::string WeaponName = pWeapon->get_name();

	if (WeaponName == "weapon_knife")
		return true;
	else if (WeaponName == "weapon_incgrenade")
		return true;
	else if (WeaponName == "weapon_decoy")
		return true;
	else if (WeaponName == "weapon_flashbang")
		return true;
	else if (WeaponName == "weapon_hegrenade")
		return true;
	else if (WeaponName == "weapon_smokegrenade")
		return true;
	else if (WeaponName == "weapon_molotov")
		return true;

	return false;
}

float C_BaseEntity::fire_rate()
{
	if (!this)
		return 0.f;
	if (!this->is_alive())
		return 0.f;
	if (this->is_knife_or_nade())
		return 0.f;

	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->get_active_weapon();

	if (!pWeapon)
		return false;

	std::string WeaponName = pWeapon->get_name();

	if (WeaponName == "weapon_glock")
		return 0.15f;
	else if (WeaponName == "weapon_hkp2000")
		return 0.169f;
	else if (WeaponName == "weapon_p250")//the cz and p250 have the same name idky same with other guns
		return 0.15f;
	else if (WeaponName == "weapon_tec9")
		return 0.12f;
	else if (WeaponName == "weapon_elite")
		return 0.12f;
	else if (WeaponName == "weapon_fiveseven")
		return 0.15f;
	else if (WeaponName == "weapon_deagle")
		return 0.224f;
	else if (WeaponName == "weapon_nova")
		return 0.882f;
	else if (WeaponName == "weapon_sawedoff")
		return 0.845f;
	else if (WeaponName == "weapon_mag7")
		return 0.845f;
	else if (WeaponName == "weapon_xm1014")
		return 0.35f;
	else if (WeaponName == "weapon_mac10")
		return 0.075f;
	else if (WeaponName == "weapon_ump45")
		return 0.089f;
	else if (WeaponName == "weapon_mp9")
		return 0.070f;
	else if (WeaponName == "weapon_bizon")
		return 0.08f;
	else if (WeaponName == "weapon_mp7")
		return 0.08f;
	else if (WeaponName == "weapon_p90")
		return 0.070f;
	else if (WeaponName == "weapon_galilar")
		return 0.089f;
	else if (WeaponName == "weapon_ak47")
		return 0.1f;
	else if (WeaponName == "weapon_sg556")
		return 0.089f;
	else if (WeaponName == "weapon_m4a1")
		return 0.089f;
	else if (WeaponName == "weapon_aug")
		return 0.089f;
	else if (WeaponName == "weapon_m249")
		return 0.08f;
	else if (WeaponName == "weapon_negev")
		return 0.0008f;
	else if (WeaponName == "weapon_ssg08")
		return 1.25f;
	else if (WeaponName == "weapon_awp")
		return 1.463f;
	else if (WeaponName == "weapon_g3sg1")
		return 0.25f;
	else if (WeaponName == "weapon_scar20")
		return 0.25f;
	else if (WeaponName == "weapon_mp5sd")
		return 0.08f;
	else
		return .0f;
	
}

bool C_BaseEntity::is_enemy()
{
	return this->get_team() != csgo::m_local->get_team();
}

CBasePlayerAnimState* C_BaseEntity::get_animation_state( )
{
	return *( CBasePlayerAnimState** )( ( DWORD )this + 0x3914 );
}

void C_BaseEntity::update_animation_state( CBasePlayerAnimState* state, Vector angle )
{
	static auto UpdateAnimState = Utils::find_signature(
		"client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" );

	if ( !UpdateAnimState )
		return;

	__asm {
		push 0
	}

	__asm
	{
		mov ecx, state

		movss xmm1, dword ptr[ angle + 4 ]
		movss xmm2, dword ptr[ angle ]

		call UpdateAnimState
	}
}

void C_BaseEntity::reset_animation_state( CBasePlayerAnimState* state )
{
	using ResetAnimState_t = void( __thiscall* )( CBasePlayerAnimState* );
	static auto ResetAnimState = ( ResetAnimState_t )Utils::find_signature( "client.dll", "56 6A 01 68 ? ? ? ? 8B F1" );
	if ( !ResetAnimState )
		return;

	ResetAnimState( state );
}

void C_BaseEntity::create_animation_state( CBasePlayerAnimState* state )
{
	using CreateAnimState_t = void( __thiscall* )( CBasePlayerAnimState*, C_BaseEntity* );
	static auto CreateAnimState = ( CreateAnimState_t )Utils::find_signature( "client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" );
	if ( !CreateAnimState )
		return;

	CreateAnimState( state, this );
}

void C_BaseEntity::invalidate_bone_cache( void ) {
	static auto invalidate_bone_cache_fn = Utils::find_signature( "client.dll", "80 3D ?? ?? ?? ?? ?? 74 16 A1 ?? ?? ?? ?? 48 C7 81" );
	reinterpret_cast< void( __fastcall* ) ( void* ) > ( invalidate_bone_cache_fn ) ( this );
}