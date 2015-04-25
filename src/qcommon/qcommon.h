#ifndef __QCOMMON_H__
#define __QCOMMON_H__

#include "../universal/q_shared.h"

/*
==============================================================

CMD

Command text buffering and command execution

==============================================================
*/

GAME_EXPORT void CBuf_AddText(int localClient, const char* text);

/*
==============================================================

DVAR

==============================================================
*/

GAME_EXPORT dvar_s* Dvar_FindVar(const char* dvarName);

extern dvar_s ** com_timescale;

// threads.cpp

GAME_EXPORT void* Sys_GetValue(int valueIndex);

#endif /* __QCOMMON_H__ */
