#ifndef __SERVER_MP_H__
#define __SERVER_MP_H__

#include "../game/g_local.h"

struct svscmd_info_t
{
	char* cmd; //0x0
	int time; //0x4
	int type; //0x8
};

struct client_t
{
		char unk0[0x4B34]; //0x0
	svscmd_info_t reliableCommandInfo[128]; //0x4B34
		char unk1[0x1C]; //0x5134
	usercmd_s lastUsercmd; //0x5150
	int lastClientCommand; //0x518C
	char lastClientCommandString[1024]; //0x5190
	gentity_s *gentity; //0x5590
	char name[32]; //0x5594
		char pad[0x48BCC]; //55B4
};

//=============================================================================

struct serverStatic_t
{
		char pad[0x6C0]; //0x0
	client_t* clients; //0x6C0
};

//=============================================================================

extern serverStatic_t* svs;

extern float * sv_centermap;

// sv_ccmds_mp.cpp

GAME_EXPORT void SV_MapRestart(int fast_restart);

// sv_client_mp.cpp

GAME_EXPORT gentity_s* SV_AddTestClient();
GAME_EXPORT void SV_BanClient(client_t* cl);

// sv_init_mp.cpp

GAME_EXPORT void SV_SetConfigString(int index, const char* value);

#endif /* __SERVER_MP_H__ */
