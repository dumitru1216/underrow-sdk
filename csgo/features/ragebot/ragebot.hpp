#pragma once
#include "../includes.hpp"

/* hitbox list */
enum hitbox_list_t {
	Head = 0,
	Neck,
	Pelvis,
	Stomach,
	LowerChest,
	Chest,
	UpperChest,
	RightThigh,
	LeftThigh,
	RightShin,
	LeftShin,
	RightFoot,
	LeftFoot,
	RightHand,
	LeftHand,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm
};

namespace function {
	class ragebot {
	public:
		/* important */
		matrix3x4_t matrix[ 65 ][ 128 ];
		void setup( );
		Vector hitscan( C_BaseEntity* pEnt );
		bool hit_chance( C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance );
		void auto_stop( );
		void linear_extrapolations( );

		/* ints */
		int sequence;
		int best_entity_damage;

		/* bools */
		bool backtrack[ 65 ];
		bool shot_backtrack[ 65 ];
	};
	extern ragebot c_ragebot;
}