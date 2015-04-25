#include "functions.h"
#include "stdio.h"
#include "entity.h"

#include "bgame/bg_public.h"
#include "server/server.h"
#include "server_mp/server_mp.h"
#include "qcommon/cm_public.h"

#include <string.h>
#include <math.h>

//----------------------------------------------------------------

float vec3distancesq(const vec3_t p1, const vec3_t p2)
{
	float x = p2[0] - p1[0];
	float y = p2[1] - p1[1];
	float z = p2[2] - p1[2];
	return x * x + y * y + z * z;
}

void viewOrigin(const gclient_s* gclient, vec3_t origin)
{
	origin[0] = gclient->ps.origin[0];
	origin[1] = gclient->ps.origin[1];
	origin[2] = gclient->ps.origin[2] + gclient->ps.viewHeightCurrent;
}

void anglesToForward(vec3_t out, const vec3_t angles, const vec3_t start, vec_t scale)
{
	vec3_t forward;
	AngleVectors(angles, forward, NULL, NULL);
	VectorMA(start, scale, forward, out);
}

int32_t nearestPlayer(int32_t client)
{
	int32_t closest = -1;
	float distance = 33554432.0f;
	gclient_s* gclient = g_entities[client].client;
	for (int32_t i = 0; i < 12; i++)
	{
		if (i == client)
			continue;
		gclient_s* nearest = g_entities[client].client;
		if (nearest->sess.connected != CON_CONNECTED)
			continue;
		if (nearest->sess.sessionState != SESS_STATE_PLAYING)
			continue;
		if (nearest->sess.cs.team == gclient->sess.cs.team)
			continue;
		float vec = vec3distancesq(gclient->ps.origin, nearest->ps.origin);
		if (vec < distance)
		{
			closest = i;
			distance = vec;
		}
	}
	return closest;
}

void giveWeapon(int32_t client, int32_t weapon, int32_t camo)
{
	gentity_s* entity = &g_entities[client];
	G_GivePlayerWeapon(&entity->client->ps, weapon, 0, camo);
	Add_Ammo(entity, weapon, 999, 1);
	char cmd[16];
	sprintf(cmd, "h %i", weapon);
	SV_GameSendServerCommand(client, SV_CMD_CAN_IGNORE, cmd);
}

void takeWeapon(int32_t client, int32_t weapon)
{
	gclient_s* cl = g_entities[client].client;
	BG_TakePlayerWeapon(&cl->ps, weapon);
}

void takeAllWeapons(int32_t client)
{
	gclient_s* gclient = g_entities[client].client;
	for(int32_t i = 0; i < 15; i++)
	{
		BG_TakePlayerWeapon(&gclient->ps, gclient->ps.heldWeapon[i].weapon);
	}
}

void playRumble(int32_t client, const char* rumble)
{
	G_AddEvent(&g_entities[client], EV_PLAY_RUMBLE_ON_ENT, G_RumbleIndex(rumble));
}

void playEffect(const vec3_t origin, const char* effect)
{
	gentity_s* ent = G_TempEntity(origin, EV_PLAY_FX);
	ent->s.eventParm = G_EffectIndex(effect);
	VectorSet(ent->s.lerp.apos.trBase, 270, 0, 0);
}

void playEffectOnTag(gentity_s* self, int32_t effect, uint16_t tag)
{
	if (!self->model)
		return;

	char buffer[0x40];
	snprintf(buffer, sizeof(buffer), "%03d%s", effect, SL_ConvertToString(tag));
	int32_t index = G_FindConfigStringIndex(buffer, 0x6FA, 0x100, 1, 0);
	G_AddEvent(self, EV_PLAY_FX_ON_TAG, index);
}

void playLoopSound(gentity_s* ent, const char* snd, int16_t fadeTime)
{
	ent->r.svFlags &= ~1;
	ent->r.broadcastTime = -1;
	ent->s.loopSoundId = G_SoundAliasIndex(snd);
	ent->s.loopSoundFade = fadeTime;
}

void stringJoin(char* buffer, size_t size, char character, const char* arr[], size_t count)
{
	for(size_t i = 0; i < count; i++)
	{
		size_t len = strlen(arr[i]);
		size -= len;
		if (len <= 0)
			return;
		strncpy(buffer, arr[i], len);
		buffer += len;
		buffer[0] = character;
		buffer++;
	}
	buffer[-1] = '\0';
}

void iPrintln(int32_t client, const char* msg)
{
	char buffer[0x100];
	sprintf(buffer, "%c \"%s\"", 'O', msg);
	SV_GameSendServerCommand(client, SV_CMD_CAN_IGNORE, buffer);
}

void iPrintlnBold(int32_t client, const char* msg)
{
	char buffer[0x100];
	sprintf(buffer, "%c \"%s\"", '<', msg);
	SV_GameSendServerCommand(client, SV_CMD_CAN_IGNORE, buffer);
}

void setClientDvar(int32_t client, int32_t dvarIndex, int32_t value)
{
	char buffer[0x100];
	sprintf(buffer, "^ %i \"%i\"", dvarIndex, value);
	SV_GameSendServerCommand(client, SV_CMD_RELIABLE, buffer);
}

void setFov(int32_t client, int32_t fov)
{
	setClientDvar(client, 5, fov);
}

void setBlur(int32_t client, int32_t time, float blur)
{
	char buffer[0x20];
	snprintf(buffer, sizeof(buffer), "( %i %f %i %i", time, blur, 0, 1);
	SV_GameSendServerCommand(client, SV_CMD_RELIABLE, buffer);
}

void disableOffHandWeapons(gclient_s* client)
{
	client->ps.weapFlags |= 0x100;
}

void enableOffHandWeapons(gclient_s* client)
{
	client->ps.weapFlags &= ~0x100;
}

void disableWeaponCycling(gclient_s* client)
{
	client->ps.weapFlags |= 0x200;
}

void enableWeaponCycling(gclient_s* client)
{
	client->ps.weapFlags &= ~0x200;
}

bool bulletTrace(int32_t client, const vec3_t start, const vec3_t end, vec3_t position, int32_t* hitId, bool hitPlayers)
{
	trace_t trace;
	G_LocationalTrace(&trace, start, end, client, hitPlayers ? 0x280E033 : 0x806033, NULL, NULL);

	if (position)
	{
		position[0] = (end[0] - start[0]) * trace.fraction + start[0];
		position[1] = (end[1] - start[1]) * trace.fraction + start[1];
		position[2] = (end[2] - start[2]) * trace.fraction + start[2];
	}

	if (hitId)
		*hitId = Trace_GetEntityHitId(&trace);

	return trace.fraction < 1;
}

bool physicsTrace(gentity_s* self, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, vec3_t position, int32_t* hitId)
{
	trace_t trace;
	col_context_t capsule;
	memset(&capsule, 0, sizeof(capsule));
	capsule.passEntityNum0 = capsule.passEntityNum0 = 0x3FF;
	G_TraceCapsule(&trace, start, mins, maxs, end, self->s.number, MASK_CLIP|MASK_PHYSICS|MASK_VEHICLE, &capsule);

	if (position)
	{
		position[0] = (end[0] - start[0]) * trace.fraction + start[0];
		position[1] = (end[1] - start[1]) * trace.fraction + start[1];
		position[2] = (end[2] - start[2]) * trace.fraction + start[2];
	}

	if (hitId)
		*hitId = Trace_GetEntityHitId(&trace);

	return trace.fraction < 1;
}

void fakeFire(int32_t client, const vec3_t origin, int32_t weapon)
{
	gentity_s* ent = G_TempEntity(origin, EV_FAKE_FIRE);
	ent->s.eventParm = weapon;
	ent->s.otherEntityNum = client;
	ent->s.un1.eventParm2 = 1;
}

void magicBullet(gentity_s* self, int32_t weapon, const vec3_t start, const vec3_t end)
{
	static const vec3_t gunVel = { };
	WeaponDef* weapDef = BG_GetWeaponDef(weapon);
	WeaponVariantDef* weapVariantDef = BG_GetWeaponVariantDef(weapon);
	weaponParms parms;
	memset(&parms, 0, sizeof(parms));
	float fraction, length, x, y, z;
	vec3_t targetOffset = { }, angles, forward;
	gentity_s* rocket = NULL;

	parms.weapDef = weapDef;
	parms.weapVariantDef = weapVariantDef;
	parms.weapon = weapon;
	VectorCopy(start, parms.muzzleTrace);

	x = end[0] - start[0];
	y = end[1] - start[1];
	z = end[2] - start[2];

	length = sqrt(x * x + y * y + z * z);

	fraction = length >= 0 ? 1 / length : 1;
	VectorSet(parms.forward, x * fraction, y * fraction, z * fraction);
	VectorCopy(parms.forward, forward);

	switch (weapDef->weaponType)
	{
	case WEAPTYPE_BULLET:
		Bullet_Fire(self, 0, &parms, NULL, level->time);
		break;
	case WEAPTYPE_PROJECTILE:
		rocket = Weapon_RocketLauncher_Fire(self, weapon, 0, &parms, gunVel, NULL, targetOffset, 0);
		break;
	default:
		return;
	}

	gentity_s* tempent = G_TempEntity(start, EV_FIRE_WEAPON_SCRIPTED);
	vectoangles(angles, forward);
	G_SetAngle(tempent, angles);
	tempent->s.weapon = weapon;
	tempent->s.eventParms[tempent->s.eventSequence] = 0;
	tempent->s.eventParm = self->s.number;
	if (rocket)
		Scr_AddEntity(SCRIPTINSTANCE_SERVER, rocket);
}

void radiusDamage(const vec3_t origin, gentity_s* self, meansOfDeath_t mod, vec_t maxDamage, vec_t minDamage, vec_t range)
{
	G_RadiusDamage(origin, self, self, maxDamage, minDamage, range, 1, NULL, self, mod, 0);
}

void teleportToGround(int32_t client, const vec3_t pos)
{

}

void setModel(gentity_s* ent, const char* model)
{
	SetModelInternal(ent, model);
}

void setEnemyModel(gentity_s* ent, const char* model)
{
	ent->s.enemyModel = G_ModelIndex(model);
}

void playerCamLinkTo(gentity_s* self, gentity_s* parent, const char* tag)
{
	gclient_s* gclient = self->client;
	gclient->ps.otherFlags |= 0x2000;
	gclient->ps.remoteEyesEnt = parent->s.number;
	gclient->ps.remoteEyesTagname = G_TagIndex(tag);
}

void setTurretType(gentity_s* turret, turretType_t type)
{
	switch (type)
	{
	case TURRET_SENTRY:
		turret->pTurretInfo->flags |= 0x400000;
		turret->s.lerp.u.turret.flags |= 1;
		break;
	case TURRET_TOW:
		turret->pTurretInfo->flags |= 0x40000;
		turret->s.lerp.u.turret.flags |= 2;
		break;
	case TURRET_MICROWAVE:
	case TURRET_WALLMOUNT:
		turret->pTurretInfo->flags |= 0x20000;
		turret->s.lerp.u.turret.flags |= 0x80;
		break;
	}
}

void clearAllText()
{
	for (int32_t i = 0; i < 0x200; i++)
	{
		SV_SetConfigString(0x1E8 + i, "");
	}
}

void threadExit(uint64_t err)
{
	system_call_1(41, err);
}

void setPerk(gclient_s* client, const char* name)
{
	int32_t perkIndex = BG_GetPerkIndexForName(name);
	if (perkIndex == 63)
	{
		DBGPRINTF("Unknown perk: %s\n", name);
		return;
	}
	int32_t id = perkIndex / 32; // divide the index by the size of an int in bits
	client->ps.perks[id] |= (1 << perkIndex);
	client->sess.cs.perks[id] |= (1 << perkIndex);
}

void unsetPerk(gclient_s* client, const char* name)
{
	int32_t perkIndex = BG_GetPerkIndexForName(name);
	if (perkIndex == 63)
	{
		DBGPRINTF("Unknown perk: %s\n", name);
		return;
	}
	int32_t id = perkIndex / 32;
	client->ps.perks[id] &= ~(1 << perkIndex);
	client->sess.cs.perks[id] &= ~(1 << perkIndex);
}
