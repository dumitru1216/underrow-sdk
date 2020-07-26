#pragma once
#include "recv.hpp"
#include "../other/definitions.hpp"
#include "i_client_networkable.hpp"

class ClientClass;

typedef IClientNetworkable*   (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable*   (*CreateEventFn)();

class ClientClass
{
public:
    CreateClientClassFn      pCreateFn;
    CreateEventFn            pCreateEventFn;
    char*                    pNetworkName;
    RecvTable*               pRecvTable;
    ClientClass*             pNext;
    int						 ClassID;
};