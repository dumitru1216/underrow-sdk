#include "../csgo_hooks.hpp"

void __fastcall hooks::draw_model_execute( void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix ) {
	static auto oDrawModelExecute = c_hooks.pModelHook->GetOriginal<draw_model_execute_t>( vtable_indexes::dme );

	/* functions */
	const char* model_name = model_info->GetModelName( (model_t*)info.pModel );
	C_BaseEntity* p_entity = entity_list->GetClientEntity( info.index );


	oDrawModelExecute( ecx, context, state, info, matrix );
}