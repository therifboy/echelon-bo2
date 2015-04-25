#ifndef __G_LOCAL_MP_H__
#define __G_LOCAL_MP_H__

#include "../export.h"
#include "../game/g_local.h"
#include "../clientscript/clientscript.h"

enum turretType_t
{
	TURRET_SENTRY,
	TURRET_TOW,
	TURRET_MICROWAVE,
	TURRET_WALLMOUNT
};

enum hitLocation_t
{
	HITLOC_NONE = 0x0,
	HITLOC_HELMET = 0x1,
	HITLOC_HEAD = 0x2,
	HITLOC_NECK = 0x3,
	HITLOC_TORSO_UPPER = 0x4,
	HITLOC_TORSO_MID = 0x5,
	HITLOC_TORSO_LOWER = 0x6,
	HITLOC_RIGHT_ARM_UPPER = 0x7,
	HITLOC_LEFT_ARM_UPPER = 0x8,
	HITLOC_RIGHT_ARM_LOWER = 0x9,
	HITLOC_LEFT_ARM_LOWER = 0xA,
	HITLOC_RIGHT_HAND = 0xB,
	HITLOC_LEFT_HAND = 0xC,
	HITLOC_RIGHT_LEG_UPPER = 0xD,
	HITLOC_LEFT_LEG_UPPER = 0xE,
	HITLOC_RIGHT_LEG_LOWER = 0xF,
	HITLOC_LEFT_LEG_LOWER = 0x10,
	HITLOC_RIGHT_FOOT = 0x11,
	HITLOC_LEFT_FOOT = 0x12,
	HITLOC_GUN = 0x13,
	HITLOC_RIOTSHIELD = 0x14
};

//=============================================================================

struct gameTypeScript_t
{
	char pszScript[0x40]; //0x0
	char pszName[0x40]; //0x40
	int bTeamBased; //0x80
};

struct scr_data_t
{
	int gamescript; //0x0
	int levelscript; //0x4
	int zombiescript; //0x8
		char unk0[4]; //0xC
	int teamset_multiteam_script; //0x10
	int teamset_default_script; //0x14
	struct
	{
		int main; //0x18
		int startupgametype; //0x1C
		int playerconnect; //0x20
		int playerdisconnect; //0x24
		int playerdamage; //0x28
		int playerkilled; //0x2C
		int playermelee; //0x30
			char unk0[0xC]; //0x34
		int actordamage; //0x40
		int actorkilled; //0x44
		int vehicledamage; //0x48
		int vehicleradiusdamage; //0x4C
		int playerlaststand; //0x50
		int playermigrated; //0x54
		int hostmigration; //0x58
		int hostmigrationsave; //0x5C
		int prehostmigrationsave; //0x60
		int iNumGameTypes; //0x64
		gameTypeScript_t list[32]; //0x68
	}gametype;
	int delete_; //0x10E8
	int initstructs; //0x10EC
	int createstruct; //0x10F0
	int findstruct; //0x10F4
		char playerCorpseInfo[8][0xD08]; //0x10F8
		char unk_7938[0x20D8]; //0x7938
	int destructibleEvent; //0x9A10
	int challengeComplete; //0x9A14
	int gunChallenceComplete; //0x9A18
	int rankUp; //0x9A1C
	int medal; //0x9A20
	int onAddPlayerStat; //0x9A24
	int onAddWeaponStat; //0x9A28
	int giveContractRewards; //0x9A2C
	int updateSpawnPoints; //0x9A30
	int glassSmash; //0x9A34
	int finalizeInitialization; //0x9A38
	int menuResponse; //0x9A3C
};

// g_client_mp.cpp

GAME_EXPORT void SetClientViewAngle(gentity_s* entity, const vec3_t angles);
GAME_EXPORT void G_GetPlayerViewOrigin(playerState_s* client, vec3_t origin);

// g_combat_mp.cpp

GAME_EXPORT void player_die(gentity_s* self, gentity_s* inflictor, gentity_s* attacker, int damage, meansOfDeath_t meansOfDeath, int weapon, const vec3_t vDir, hitLocation_t hitLoc, int psTimeOffset);
GAME_EXPORT int G_RadiusDamage(const vec3_t origin, gentity_s* inflictor, gentity_s* attacker, vec_t fInnerDamage, vec_t fOuterDamage, vec_t radius, vec_t coneAngleCos, const vec3_t coneDirection, gentity_s *ignore, meansOfDeath_t mod, int weapon);
GAME_EXPORT unsigned short G_GetHitLocationString(hitLocation_t hitLoc);

// g_scr_helicopter.cpp

GAME_EXPORT void G_SpawnHelicopter(gentity_s* vehent, gentity_s* owner, const char* type, const char* model);

// g_scr_main_mp.cpp

GAME_EXPORT void SetModelInternal(gentity_s* ent, const char* model);

// g_spawn_mp.cpp

GAME_EXPORT int G_CallSpawn(gentity_s* ent);
GAME_EXPORT int G_CallSpawnEntity(gentity_s* ent);
GAME_EXPORT void Scr_AddEntity(scriptInstance_t inst, gentity_s* ent);
GAME_EXPORT unsigned short Scr_ExecEntThread(gentity_s* ent, int handle, unsigned int paramCount);

// g_utils_mp.cpp

GAME_EXPORT void G_AddEvent(gentity_s* entity, entity_event_t event, unsigned int eventParm);
GAME_EXPORT gentity_s* G_TempEntity(const vec3_t origin, entity_event_t event);

GAME_EXPORT int G_RumbleIndex(const char* rumble);
GAME_EXPORT int G_EffectIndex(const char* effect);
GAME_EXPORT int G_LocalizedStringIndex(const char* string);
GAME_EXPORT int G_MaterialIndex(const char* material);
GAME_EXPORT int G_ModelIndex(const char* model);
GAME_EXPORT int G_FindConfigStringIndex(const char *name, int start, int max, int create, const char *errormsg);
GAME_EXPORT int G_TagIndex(const char* tag);
GAME_EXPORT int G_SoundAliasIndex(const char* sound);

GAME_EXPORT int G_EntLinkTo(gentity_s* ent, gentity_s* parent, unsigned int tag);
GAME_EXPORT int G_EntLinkToWithOffset(gentity_s* ent, gentity_s* parent, unsigned int tag, const vec3_t originOffset, const vec3_t angleOffset);
GAME_EXPORT void G_EntUnlink(gentity_s *ent);

GAME_EXPORT gentity_s* G_Spawn();
GAME_EXPORT void G_FreeEntity(gentity_s* ent);
GAME_EXPORT gentity_s* G_SpawnStaticEntity(int entnum, int count, const char* entType);

GAME_EXPORT void G_SetModel(gentity_s* ent, const char* model);
GAME_EXPORT void G_SetOrigin(gentity_s* ent, const vec3_t pos);
GAME_EXPORT void G_SetAngle(gentity_s* ent, const vec3_t angles);

//=============================================================================

extern ScriptString** modNames;

extern scr_data_t* g_scr_data;

#endif /* __G_LOCAL_MP_H__ */
