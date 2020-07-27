#pragma once
#include "c_global_vars_base.hpp"
#include "c_client_class.hpp"

class IBaseClientDLL
{
public:
    // Connect appsystem components, get global interfaces, don't run any other init code
    virtual int              Connect(CreateInterfaceFn appSystemFactory, Cglobal_varsBase *pGlobals) = 0;
    virtual int              Disconnect(void) = 0;
    virtual int              Init(CreateInterfaceFn appSystemFactory, Cglobal_varsBase *pGlobals) = 0;
    virtual void             PostInit() = 0;
    virtual void             Shutdown(void) = 0;
    virtual void             LevelInitPreEntity(char const* pMapName) = 0;
    virtual void             LevelInitPostEntity() = 0;
    virtual void             LevelShutdown(void) = 0;
    virtual ClientClass*     GetAllClasses(void) = 0;
};
extern IBaseClientDLL* g_pClientDll;