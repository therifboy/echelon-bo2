#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "game/g_local.h"
#include "game_mp/g_local_mp.h"

gentity_s* spawnEnt(uint16_t script, const vec3_t origin);
void deleteEnt(gentity_s* ent);
void playerLinkTo(gentity_s* ent, gentity_s* parent, uint16_t tag);
void playerLinkTo(gentity_s* ent, gentity_s* parent, uint16_t tag, const vec3_t originOffset);
void unlink(gentity_s* ent);
gentity_s* spawnHelicopter(gentity_s* owner, const vec3_t origin, const vec3_t angles, const char* type, const char* model);
gentity_s* spawnPlane(gentity_s* owner, uint16_t classname, const vec3_t startpoint);
gentity_s* spawnVehicle(const char* model, uint16_t targetname, const char* vehicleType, const vec3_t origin, const vec3_t angles);
void moveAxis(gentity_s* ent, int32_t axis, vec_t value, vec_t pfTotalTime, vec_t pfAccelTime, vec_t pfDecelTime);
void moveTo(gentity_s* ent, const vec3_t vPos, vec_t pfTotalTime, vec_t pfAccelTime, vec_t pfDecelTime);
void rotateAxis(gentity_s* ent, int32_t axis, vec_t value, vec_t pfTotalTime, vec_t pfAccelTime, vec_t pfDecelTime);
void rotateTo(gentity_s* ent, const vec3_t vAngles, vec_t pfTotalTime, vec_t pfAccelTime, vec_t pfDecelTime);
void physicsLaunch(gentity_s* ent, const vec3_t forcePoint, const vec3_t initialVelocity);
void setOrigin(gentity_s* ent, const vec3_t pos);
void setVehGoalPos(scr_vehicle_s* veh, const vec3_t goal, int32_t stopAtGoal);
gentity_s* spawnTurret(uint16_t classname, const vec3_t origin, /*const vec3_t angles,*/ const char* weapon);
gentity_s* dropWeapon(int32_t client, int32_t weapon, int32_t camo);
void setOwner(gentity_s* ent, gentity_s* owner);
void useVehicle(gentity_s* vehicle, gentity_s* owner, int32_t seatId);
void menuResponse(gentity_s* ent, const char* configString, const char* value);
void playerDamage(gentity_s* self, gentity_s* attacker, int32_t damage, meansOfDeath_t mod, int32_t weapon, hitLocation_t hitLoc);
void execThread(int32_t handle, uint32_t paramCount);
void execEntThread(gentity_s* ent, int32_t handle, uint32_t paramCount);

#endif /*__ENTITY_H__*/
