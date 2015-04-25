#include "entity.h"
#include "functions.h"
#include "stdio.h"
#include "handle.h"
#include "handle_functions.h"
#include "utils.h"

#include "game/g_local.h"
#include "server/server.h"

#include <string.h>

gentity_s* spawnEnt(ScriptString script, const vec3_t origin)
{
	FUNCTION;
	gentity_s* ent = G_Spawn();
	Scr_SetString(&ent->classname, script);
	if (origin != 0)
		VectorCopy(origin, ent->r.currentOrigin);
	ent->spawnflags = 0;
	if (!G_CallSpawnEntity(ent))
	{
		TTY_Write("G_CallSpawnEntity fail\n");
		G_FreeEntity(ent);
		return 0;
	}
	Scr_AddEntity(SCRIPTINSTANCE_SERVER, ent);
	return ent;
}

void deleteEnt(gentity_s* ent)
{
	if (level->currentEntityThink != ent->s.number)
	{
		G_FreeEntity(ent);
	}
}

void playerLinkTo(gentity_s* ent, gentity_s* parent, uint16_t tag)
{
	FUNCTION;
	gclient_s* cl = ent->client;
	cl->ps.linkFlags = 0;
	cl->link_useTagAnglesForViewAngles = true;
	cl->link_useBaseAnglesForViewClamp = false;
	cl->flags = 0;
	cl->link_viewClamp.min.start[0] =
	cl->link_viewClamp.min.start[1] =
	cl->link_viewClamp.min.current[0] = 
	cl->link_viewClamp.min.current[1] =
	cl->link_viewClamp.min.goal[0] =
	cl->link_viewClamp.min.goal[1] = -180;
	cl->link_viewClamp.max.start[0] =
	cl->link_viewClamp.max.start[1] =
	cl->link_viewClamp.max.current[0] = 
	cl->link_viewClamp.max.current[1] =
	cl->link_viewClamp.max.goal[0] =
	cl->link_viewClamp.max.goal[1] = 180;
	printf("G_EntLinkTo(): %08x\n", G_EntLinkTo(ent, parent, tag));
}

void playerLinkTo(gentity_s* ent, gentity_s* parent, uint16_t tag, const vec3_t originOffset)
{
	vec3_t angleOffset = { };
	gclient_s* cl = ent->client;
	cl->ps.linkFlags = 0;
	cl->link_useTagAnglesForViewAngles = true;
	cl->link_useBaseAnglesForViewClamp = false;
	cl->flags = 0;
	cl->link_viewClamp.min.start[0] =
	cl->link_viewClamp.min.start[1] =
	cl->link_viewClamp.min.current[0] = 
	cl->link_viewClamp.min.current[1] =
	cl->link_viewClamp.min.goal[0] =
	cl->link_viewClamp.min.goal[1] = -180;
	cl->link_viewClamp.max.start[0] =
	cl->link_viewClamp.max.start[1] =
	cl->link_viewClamp.max.current[0] = 
	cl->link_viewClamp.max.current[1] =
	cl->link_viewClamp.max.goal[0] =
	cl->link_viewClamp.max.goal[1] = 180;
	printf("G_EntLinkToWithOffset(): %08x\n", G_EntLinkToWithOffset(ent, parent, tag, originOffset, angleOffset));
}

void unlink(gentity_s* ent)
{
	if (ent->client)
	{
		if (ent->client->ps.eFlags & 0x4000)
		{
			VEH_UnlinkPlayer(ent, 0, "unlink");
		}
		else
		{
			G_EntUnlink(ent);
		}
		ent->client->linkAnglesFrac = 0;
		ent->client->flags = 0;
		ent->client->ps.linkFlags &= ~0x1;
	}
	else
	{
		G_EntUnlink(ent);
	}
}

gentity_s* spawnHelicopter(gentity_s* owner, const vec3_t origin, const vec3_t angles, const char* type, const char* model)
{
	FUNCTION;
	TTY_Write("G_SpawnStaticEntity\n");
	gentity_s* vehicle = G_SpawnStaticEntity(0x3E, 0x10, "vehicle");
	VectorCopy(origin, vehicle->r.currentOrigin);
	VectorCopy(angles, vehicle->r.currentAngles);
	TTY_Write("Scr_SetString\n");
	Scr_SetString(&vehicle->classname, scr_const->script_vehicle);
	TTY_Write("G_SpawnHelicopter\n");
	G_SpawnHelicopter(vehicle, owner, type, model);
	Scr_AddEntity(SCRIPTINSTANCE_SERVER, vehicle);
	return vehicle;
}

gentity_s* spawnPlane(gentity_s* owner, ScriptString classname, const vec3_t startpoint)
{
	gentity_s* plane = G_Spawn();
	Scr_SetString(&plane->classname, classname);
	VectorCopy(startpoint, plane->r.currentOrigin);
	plane->s.eType = ET_PLANE;
	plane->spawnflags = 0;
	plane->s.lerp.faction = owner->client->sess.cs.team | (owner->s.number * 0x800);
	printf("G_CallSpawnEntity(): %d\n", G_CallSpawnEntity(plane));
	Scr_AddEntity(SCRIPTINSTANCE_SERVER, plane);
	return plane;
}

gentity_s* spawnVehicle(const char* model, uint16_t targetname, const char* vehicleType, const vec3_t origin, const vec3_t angles)
{
	gentity_s* vehicle = G_SpawnStaticEntity(0x3E, 0x10, "vehicle");
	Scr_SetString(&vehicle->classname, scr_const->script_vehicle);
	G_SetModel(vehicle, model);
	Scr_SetString(&vehicle->targetname, targetname);
	VectorCopy(origin, vehicle->r.currentOrigin);
	VectorCopy(angles, vehicle->r.currentAngles);
	G_SpawnVehicle(vehicle, vehicleType, 0);
	Scr_AddEntity(SCRIPTINSTANCE_SERVER, vehicle);
	G_MakeVehicleUsable(vehicle, true);
	return vehicle;
}

void moveAxis(gentity_s* ent, int32_t axis, vec_t value, vec_t pfTotalTime, vec_t pfAccelTime, vec_t pfDecelTime)
{
#ifdef _DEBUG
	if (pfTotalTime <= 0)
	{
		printf("total time must be positive\n");
		return;
	}
	if (pfAccelTime < 0)
	{
		printf("accel time must be nonnegative\n");
		return;
	}
	if (pfDecelTime < 0)
	{
		printf("decel time must be nonnegative\n");
		return;
	}
	if (pfAccelTime + pfDecelTime > pfTotalTime)
	{
		printf("accel time plus decel time is greater than total time\n");
		return;
	}
#endif

	vec3_t newOrigin;
	VectorCopy(ent->r.currentOrigin, newOrigin);
	newOrigin[axis] += value;
	ScriptMover_SetupMove(ent, newOrigin, pfTotalTime, pfAccelTime, pfDecelTime);
}

void moveTo(gentity_s* ent, const vec3_t vPos, vec_t pfTotalTime, vec_t pfAccelTime, vec_t pfDecelTime)
{
#ifdef _DEBUG
	if (pfTotalTime <= 0)
	{
		printf("total time must be positive\n");
		return;
	}
	if (pfAccelTime < 0)
	{
		printf("accel time must be nonnegative\n");
		return;
	}
	if (pfDecelTime < 0)
	{
		printf("decel time must be nonnegative\n");
		return;
	}
	if (pfAccelTime + pfDecelTime > pfTotalTime)
	{
		printf("accel time plus decel time is greater than total time\n");
		return;
	}
#endif

	ScriptMover_SetupMove(ent, vPos, pfTotalTime, pfAccelTime, pfDecelTime);
}

void rotateAxis(gentity_s* ent, int32_t axis, vec_t value, vec_t pfTotalTime, vec_t pfAccelTime, vec_t pfDecelTime)
{
#ifdef _DEBUG
	if (pfTotalTime <= 0)
	{
		printf("total time must be positive\n");
		return;
	}
	if (pfAccelTime < 0)
	{
		printf("accel time must be nonnegative\n");
		return;
	}
	if (pfDecelTime < 0)
	{
		printf("decel time must be nonnegative\n");
		return;
	}
	if (pfAccelTime + pfDecelTime > pfTotalTime)
	{
		printf("accel time plus decel time is greater than total time\n");
		return;
	}
#endif

	vec3_t newAngles;
	VectorCopy(ent->r.currentAngles, newAngles);
	newAngles[axis] += value;
	ScriptMover_Rotate(ent, newAngles, pfTotalTime, pfAccelTime, pfDecelTime);
}

void rotateTo(gentity_s* ent, const vec3_t vAngles, vec_t pfTotalTime, vec_t pfAccelTime, vec_t pfDecelTime)
{
#ifdef _DEBUG
	if (pfTotalTime <= 0)
	{
		printf("total time must be positive\n");
		return;
	}
	if (pfAccelTime < 0)
	{
		printf("accel time must be nonnegative\n");
		return;
	}
	if (pfDecelTime < 0)
	{
		printf("decel time must be nonnegative\n");
		return;
	}
	if (pfAccelTime + pfDecelTime > pfTotalTime)
	{
		printf("accel time plus decel time is greater than total time\n");
		return;
	}
#endif

	ScriptMover_Rotate(ent, vAngles, pfTotalTime, pfAccelTime, pfDecelTime);
}

void physicsLaunch(gentity_s* ent, const vec3_t forcePoint, const vec3_t initialVelocity)
{
	if (forcePoint == 0)
	{
		vec3_t fp = { };
		forcePoint = fp;
	}
	if (initialVelocity == 0)
	{
		vec3_t iv = { };
		initialVelocity = iv;
	}
	ScriptMover_SetupPhysicsLaunch(&ent->s.lerp.pos, &ent->s.lerp.apos, forcePoint, initialVelocity);
}

void setOrigin(gentity_s* ent, const vec3_t pos)
{
	G_SetOrigin(ent, pos);
	if (ent->r.linked)
	{
		SV_LinkEntity(ent);
	}
}

void setVehGoalPos(scr_vehicle_s* veh, const vec3_t goal, int32_t stopAtGoal)
{
	VectorCopy(goal, veh->goalPosition);
	veh->stopAtGoal = stopAtGoal ? 1 : 0;
	veh->flags |= 1;
	veh->flags2 |= 2;
}

gentity_s* spawnTurret(uint16_t classname, const vec3_t origin, /*const vec3_t angles,*/ const char* weaponInfo)
{
	//gentity_s* turret = G_SpawnStaticEntity(0x4E, 0x20, "turret");
	//Scr_SetString(&turret->classname, classname);
	//VectorCopy(origin, turret->r.currentOrigin);
	//VectorCopy(angles, turret->r.currentAngles);
	//G_SpawnTurret(turret, weapon, 0);
	//turret->r.svFlags = 1;
	////turret->takedamage = 4; // optional
	//SV_LinkEntity(turret);
	//Scr_AddEntity(SCRIPTINSTANCE_SERVER, turret);
	//return turret;

	gentity_s* turret = SpawnTurretInternal(classname, origin, weaponInfo);
	turret->takedamage = 1;
	turret->r.svFlags = 4;
	SV_LinkEntity(turret);
	return turret;
}

gentity_s* dropWeapon(int32_t client, int32_t weapon, int32_t camo)
{
	uint16_t tag = scr_const->tag_weapon_right;
	gentity_s* ent = Drop_Item(&g_entities[client], weapon, 20, tag, 0);
	*(int32_t*)((uint32_t)ent + 0x22C) = 999;
	*(int32_t*)((uint32_t)ent + 0x230) = 999;
	ent->s.renderOptions = camo;
	Scr_AddEntity(SCRIPTINSTANCE_SERVER, ent);
	return ent;
}

void setOwner(gentity_s* ent, gentity_s* owner)
{
	int32_t ownerNum = owner->s.number;
	ent->s.lerp.faction = (ent->s.lerp.faction & 0xF) | (ownerNum << 4);
}

void useVehicle(gentity_s* vehicle, gentity_s* owner, int32_t seatId)
{
	if (owner->client->ps.eFlags & 0x4000)
		VEH_UnlinkPlayer(owner, 0, "useVehicle");

	VEH_LinkPlayer(vehicle, owner, seatId, false);
}

void menuResponse(gentity_s* ent, const char* configString, const char* value)
{
	Scr_AddString(SCRIPTINSTANCE_SERVER, value);
	Scr_AddString(SCRIPTINSTANCE_SERVER, configString);
	execEntThread(ent, g_scr_data->menuResponse, 2);
}

void playerDamage(gentity_s* self, gentity_s* attacker, int32_t damage, meansOfDeath_t mod, int32_t weapon, hitLocation_t hitLoc)
{
	//player_die(self, attacker, attacker, damage, mod, weapon, 0, hitLoc, 0);
	char weapName[0x400];

	Scr_AddInt(SCRIPTINSTANCE_SERVER, 0); //boneIndex
	Scr_AddInt(SCRIPTINSTANCE_SERVER, 0); //timeOffset
	Scr_AddConstString(SCRIPTINSTANCE_SERVER, G_GetHitLocationString(hitLoc)); //hitLoc
	Scr_AddUndefined(SCRIPTINSTANCE_SERVER); //vDir
	Scr_AddUndefined(SCRIPTINSTANCE_SERVER); //vPoint
	printf("weaponName: %s\n", BG_WeaponName(weapon, weapName, sizeof(weapName)));
	Scr_AddString(SCRIPTINSTANCE_SERVER, weapName); //weapon
	Scr_AddConstString(SCRIPTINSTANCE_SERVER, *modNames[mod]); //meansOfDeath
	Scr_AddInt(SCRIPTINSTANCE_SERVER, 0); //dflags
	Scr_AddInt(SCRIPTINSTANCE_SERVER, damage); //damage
	Scr_AddEntity(SCRIPTINSTANCE_SERVER, attacker); //attacker
	Scr_AddEntity(SCRIPTINSTANCE_SERVER, attacker); //inflictor
	execEntThread(self, g_scr_data->gametype.playerdamage, 11);
}

void execThread(int32_t handle, uint32_t paramCount)
{
	uint16_t tid = Scr_ExecThread(SCRIPTINSTANCE_SERVER, handle, paramCount);
	Scr_FreeThread(SCRIPTINSTANCE_SERVER, tid);
}

void execEntThread(gentity_s* ent, int32_t handle, uint32_t paramCount)
{
	uint16_t tid = Scr_ExecEntThread(ent, handle, paramCount);
	Scr_FreeThread(SCRIPTINSTANCE_SERVER, tid);
}
