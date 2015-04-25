#ifndef __HANDLE_FUNCTIONS_H__
#define __HANDLE_FUNCTIONS_H__

#include "handle.h"

void weaponSelector(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void weaponList(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void aimBot(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void weaponDrop(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void teleportGun(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void noclip(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void orbital(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void flyableHeli(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void flyableJet(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void merryGoRound(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void magicBullet(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void forgeMode(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void driveableRc(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void flyableDrone(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void driveableTank(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void botSpawn(int32_t count, modmenu_s* menu, handle_t* handle, handleCall_t type);
void botSetTeam(int32_t botnum, modmenu_s* menu, handle_t* handle, handleCall_t type);
void botSetClass(int32_t botnum, modmenu_s* menu, handle_t* handle, handleCall_t type);
void humanTorch(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void turretSpawner(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void flashingBackground(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void mapRestart(int32_t fast_restart, modmenu_s* menu, handle_t* handle, handleCall_t type);
void suicide(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type);
void spawnJail(int32_t arg, modmenu_s* menu, handle_t* handle, handleCall_t type);

#endif /*__HANDLE_FUNCTIONS_H__*/
