#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "game/g_local.h"
#include "game_mp/g_local_mp.h"
#include "qcommon/qcommon.h"

#include <sys/prx.h>
#include <sys/timer.h>

// quite expensive sleep
#define GameSleep(ms)		(sys_timer_usleep((usecond_t)(((float)(ms*1000)) / com_timescale[0]->current.value)))

float vec3distancesq(const vec3_t p1, const vec3_t p2);
void viewOrigin(const gclient_s* gclient, vec3_t origin);
void anglesToForward(vec3_t forward, const vec3_t angles, const vec3_t start, vec_t scale);
int32_t nearestPlayer(int32_t client);

void giveWeapon(int32_t client, int32_t weapon, int32_t camo);
void takeWeapon(int32_t client, int32_t weapon);
void takeAllWeapons(int32_t client);

void playRumble(int32_t client, const char* rumble);
void playEffect(const vec3_t origin, const char* effect);
void playEffectOnTag(gentity_s* self, int32_t effect, uint16_t tag);
void playLoopSound(gentity_s* ent, const char* snd, int16_t fadeTime);

void stringJoin(char* buffer, size_t size, char character, const char* arr[], size_t count);

void iPrintln(int32_t client, const char* msg);
void iPrintlnBold(int32_t client, const char* msg);
void setClientDvar(int32_t client, int32_t dvarIndex, int32_t value);
void setFov(int32_t client, int32_t fov);
void setBlur(int32_t client, int32_t time, float blur);

void disableOffHandWeapons(gclient_s* client);
void enableOffHandWeapons(gclient_s* client);
void disableWeaponCycling(gclient_s* client);
void enableWeaponCycling(gclient_s* client);

bool bulletTrace(int32_t client, const vec3_t start, const vec3_t end, vec3_t position, int32_t* hitId, bool hitPlayers);
bool physicsTrace(gentity_s* self, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, vec3_t position, int32_t* hitId);
void fakeFire(int32_t client, const vec3_t origin, int32_t weapon);
void magicBullet(gentity_s* self, int32_t weapon, const vec3_t start, const vec3_t end);
void radiusDamage(const vec3_t origin, gentity_s* self, vec_t range, vec_t maxDamage, vec_t minDamage, meansOfDeath_t mod);

void setModel(gentity_s* ent, const char* model);
void setEnemyModel(gentity_s* ent, const char* model);

void playerCamLinkTo(gentity_s* self, gentity_s* parent, const char* tag);
void setTurretType(gentity_s* turret, turretType_t type);

void clearAllText();

void threadExit(uint64_t err);

#endif /*__FUNCTIONS_H__*/
