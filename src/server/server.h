#ifndef __SERVER_H__
#define __SERVER_H__

// server.h

#include "../export.h"
#include "../universal/q_shared.h"
#include "../game/g_local.h"

//=============================================================================

enum svscmd_type
{
	SV_CMD_CAN_IGNORE = 0x0,
	SV_CMD_RELIABLE = 0x1,
};

struct col_context_t
{
	char unk_0[0x10]; //0x0
	int passEntityNum0; //0x10
	int passEntityNum1; //0x14
	char unk_18[0x10]; //0x18
};

// sv_game.cpp

GAME_EXPORT void SV_GameSendServerCommand(int client, svscmd_type type, const char* cmd);

// sv_world.cpp

GAME_EXPORT void SV_LinkEntity(gentity_s* ent);
GAME_EXPORT void G_LocationalTrace(trace_t* results, const vec3_t start, const vec3_t end, int entnum, int mask, char *priorityMap, int (*collide_entity_func)(int, col_context_t *));
GAME_EXPORT void G_TraceCapsule(trace_t* results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int entnum, int mask, col_context_t *context);

#endif /* __SERVER_H__ */
