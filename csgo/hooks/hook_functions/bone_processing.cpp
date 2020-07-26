#include "../csgo_hooks.hpp"

void __fastcall hooks::DoExtraBonesProcessing( void * ECX, void * EDX, void * unkn1, void * unkn2, void * unkn3, void * unkn4, CBoneBitList & unkn5, void * unkn6 )
{
	C_BaseEntity* pPlayerEntity = (C_BaseEntity*)ECX;

	if (!pPlayerEntity || pPlayerEntity == nullptr)
		return;

	if (!pPlayerEntity->IsAlive( ) || pPlayerEntity->IsDormant( ))
		return;

	if (!pPlayerEntity->AnimState( ))
		return;

	auto oDoExtraBonesProcessing = c_hooks.pPlayerHook[pPlayerEntity->EntIndex( )]->GetOriginal<ExtraBoneProcess_t>( vtable_indexes::extraBonePro );

	float Backup = pPlayerEntity->AnimState( )->m_flUnknownFraction;
	pPlayerEntity->AnimState( )->m_flUnknownFraction = 0;

	oDoExtraBonesProcessing( ECX, unkn1, unkn2, unkn3, unkn4, unkn5, unkn6 );

	pPlayerEntity->AnimState( )->m_flUnknownFraction = Backup;
}


void hooks::HookPlayers( )
{
	static bool Init[65];
	static bool Hooked[65];

	for (int i = 1; i < csgo_engine->GetMaxClients( ); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity( i );

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive( )
			|| pPlayerEntity->IsDormant( ))
		{
			if (Hooked[i])
				c_hooks.pPlayerHook[i]->Unhook( vtable_indexes::extraBonePro );

			Hooked[i] = false;
			continue;
		}

		if (!Init[i])
		{
			c_hooks.pPlayerHook[i] = std::make_unique<ShadowVTManager>( );
			Init[i] = true;
		}

		if (Hooked[i])
			c_hooks.pPlayerHook[i]->Unhook( vtable_indexes::extraBonePro );

		if (!Hooked[i])
		{
			c_hooks.pPlayerHook[i]->Setup( pPlayerEntity );
			c_hooks.pPlayerHook[i]->Hook( vtable_indexes::extraBonePro, hooks::DoExtraBonesProcessing );

			Hooked[i] = true;
		}
	}
}

