#pragma once
#include "../../utils/other/utils.hpp"

class i_mem_alloc {
public:
	auto alloc( int nSize ) -> void* {
		using Fn = void* ( __thiscall* )( void*, int );
		return Utils::get_vfunc< Fn >( this, 1 )( this, nSize );
	}

	auto realloc( void* pMem, int nSize ) -> void* {
		using Fn = void* ( __thiscall* )( void*, void*, int );
		return Utils::get_vfunc< Fn >( this, 3 )( this, pMem, nSize );
	}

	auto free( void* pMem ) -> void {
		using Fn = void( __thiscall* )( void*, void* );
		return Utils::get_vfunc< Fn >( this, 5 )( this, pMem );
	}
};
extern i_mem_alloc* memory_alloc;