#include "sdk_interfaces.hpp"
#include "../other/utils.hpp"

#include "../../sdk\interfaces/i_client_mode.hpp"
#include "../../sdk\interfaces/i_base_client_dll.hpp"
#include "../../sdk\interfaces/i_client_entity_list.hpp"
#include "../../sdk\interfaces/iv_engine_client.hpp"
#include "../../sdk\interfaces/c_prediction.hpp"
#include "../../sdk\interfaces/i_game_event.hpp"
#include "../../sdk\interfaces/i_surface.hpp"
#include "../../sdk\interfaces/i_engine_trace.hpp"
#include "../../sdk\interfaces/i_surface_data.hpp"
#include "../../sdk\interfaces/i_cvar.hpp"
#include "../../sdk\interfaces/iv_model_info.hpp"
#include "../../sdk\interfaces/c_model_render.hpp"
#include "../../sdk\interfaces/i_material.hpp"
#include "../../sdk\interfaces/iv_render_view.hpp"

#include <fstream>

#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#define enc_wstr(s) std::wstring(enc_str(s).begin(), enc_str(s).end())
#define enc_wchar(s) enc_wstr(s).c_str()

/* init interfaces */

IBaseClientDLL*     g_pClientDll    = nullptr;
IClientMode*        g_pClientMode   = nullptr;
IClientEntityList*  entity_list   = nullptr;
IVEngineClient*     csgo_engine      = nullptr;
CPrediction*        g_pPrediction   = nullptr;
IGameMovement*      g_pMovement     = nullptr;
IMoveHelper*        g_pMoveHelper   = nullptr;
CGlobalVarsBase*    globalvars   = nullptr;
IGameEventManager*  g_pEventManager = nullptr;
ISurface*           csgo_surface = nullptr;
IEngineTrace*       trace        = nullptr;
IPhysicsSurfaceProps* surface_props_data = nullptr;
ICVar*              cvar			= nullptr;
IPanel*				g_pPanel		= nullptr;
IVModelInfo*		model_info	= nullptr;
CModelRender*       g_pModelRender  = nullptr;
IMaterialSystem*    g_pMaterialSys  = nullptr;
IVRenderView*       g_pRenderView   = nullptr;

namespace interfaces
{
	template< class T >
	T* FindClass(std::string szModuleName, std::string szInterfaceName, bool bSkip = false)
	{
		if (szModuleName.empty() || szInterfaceName.empty())
			return nullptr;
		typedef PVOID(*CreateInterfaceFn)(const char* pszName, int* piReturnCode);
		CreateInterfaceFn hInterface = nullptr;
		while (!hInterface)
		{
			hInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(szModuleName.c_str()), "CreateInterface");
			Sleep(5);
		}

		char pszBuffer[256];
		for (int i = 0; i < 100; i++)
		{
			sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i);
			PVOID pInterface = hInterface(pszBuffer, nullptr);

			if (pInterface && pInterface != NULL)
			{
				if (bSkip)
					sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i + 1);

				Sleep(5);
				break;
			}
		}

		return (T*)hInterface(pszBuffer, nullptr);
	}

	template<typename T>
	T* CaptureInterface(const char* szModuleName, const char* szInterfaceVersion)
	{
		HMODULE moduleHandle = GetModuleHandleA(szModuleName);
		if (moduleHandle)   /* In case of not finding module handle, throw an error. */
		{
			CreateInterfaceFn pfnFactory = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(moduleHandle, "CreateInterface"));
			return reinterpret_cast<T*>(pfnFactory(szInterfaceVersion, nullptr));
		}
		Utils::Log("Error getting interface %", szInterfaceVersion);
		return nullptr;
	}

    void Init()
    {
        g_pClientDll    = FindClass<IBaseClientDLL>("client.dll", "VClient");
        g_pClientMode   = **reinterpret_cast<IClientMode***>    ((*reinterpret_cast<uintptr_t**>(g_pClientDll))[10] + 0x5u);  
        globalvars   = **reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(g_pClientDll))[11]  + 10); 
        entity_list   = FindClass<IClientEntityList>("client.dll", "VClientEntityList");
        csgo_engine       = FindClass<IVEngineClient>("engine.dll", "VEngineClient");
        g_pPrediction   = FindClass<CPrediction>("client.dll", "VClientPrediction");
        g_pMovement     = FindClass<IGameMovement>("client.dll", "GameMovement");
        g_pMoveHelper   = **reinterpret_cast<IMoveHelper***>((Utils::FindSignature("client.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 0x2));  
        g_pEventManager = CaptureInterface<IGameEventManager>("engine.dll", "GAMEEVENTSMANAGER002");
		csgo_surface = FindClass<ISurface>("vguimatsurface.dll", "VGUI_Surface");
		trace        = FindClass<IEngineTrace>("engine.dll", "EngineTraceClient");
		surface_props_data  = FindClass<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps");
		cvar         = FindClass<ICVar>("vstdlib.dll", "VEngineCvar");
		g_pPanel		= FindClass<IPanel>("vgui2.dll", "VGUI_Panel");
		model_info    = FindClass<IVModelInfo>("engine.dll", "VModelInfoClient");
		g_pModelRender  = FindClass<CModelRender>("engine.dll", "VEngineModel");
		g_pMaterialSys  = FindClass<IMaterialSystem>("materialsystem.dll", "VMaterialSystem");
		g_pRenderView   = FindClass<IVRenderView>("engine.dll", "VEngineRenderView");

		//thanks monarch (from yeti)
		std::ofstream("csgo\\materials\\FlatChams.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$no_fullbright" "0"
  "$ignorez"      "1"
  "$envmap"       "env_cubemap"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

    }
}