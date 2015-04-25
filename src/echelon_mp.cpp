#include "echelon_mp.h"
#include "functions.h"
#include "utils.h"
#include "handle_functions.h"
#include "entity.h"
#include "echelon.h"

#include "bgame/bg_local.h"
#include "server/server.h"
#include "server_mp/server_mp.h"
#include "qcommon/qcommon.h"
#include "stdio.h"

#include <stddef.h>

#define Player_Name(index)		((const char*)(G_CLIENTS + index * sizeof(gclient_s) + offsetof(gclient_s, sess.newnetname)))

#pragma region

const char* title_string[] = {
	"E",
	"c",
	"h",
	"e",
	"l",
	"o",
	"n"
};

const char* main_string[] = {
	"Main Mods",
    "Weapon Menu",
    "Player Menu",
    "All Players Menu",
    "Fun Menu",
	"Lobby Menu",
    "Vision Menu",
    "Model Menu",
    "Admin Menu",
    "Teleport Menu",
    "Camera Menu",
    "Host Menu",
    "Tracer Menu",
    "Sound Menu",
    "WeaponDef Menu",
    "User Interface"
};

const char* main_mods_string[] = {
	"Freeze",
	"God Mode",
	"No Spread",
	"Third Person",
	"Huds",
	"Invisible",
	"Sattelite",
	"Last Stand",
	"Slick",
	"Poison",
	"Low Gravity",
	"Infrared",
};

const char* weapon_string[] = {
	"Take Weapon",
	"Take All Weapons",
	"Weapon Selector",
	"Weapon List",
	"Drop Weapon",
	"Give Ammo",
	"Dump Weapons",
};

const char* player_string[] = {
	Player_Name(0),
	Player_Name(1),
	Player_Name(2),
	Player_Name(3),
	Player_Name(4),
	Player_Name(5),
	Player_Name(6),
	Player_Name(7),
	Player_Name(8),
	Player_Name(9),
	Player_Name(10),
	Player_Name(11)
};

const char* all_player_string[] = {
	"Freeze",
	"Kick",
	"Ban",
};

const char* fun_string[] = {
	"Throw Frag",
	"Noclip",
	"Teleport Gun",
	"Fire",
	"Speed x2",
	"All Perks",
	"Suicide",
	"RC-XD",
	"QR-Drone",
	"AI-Tank",
};

const char* lobby_string[] = {
	"Anti Quit",
	"Spawn Bot",
	"Merry Go Round",
	"Spawn Jail",
	"Fast Restart",
	"Map Restart",
	"Hear Everyone",
	"Slow Mo",
	"Jump Slowdown",
};

const char* vision_string[] = {
	"Enable Visions",
	"Disable Visions",
	"Default",
	"Infrared",
	"MpIntro",
	"Mortar",
	"Taser",
};

const char* model_string[] = {
	"Defaultactor",
    "Carepackage",
    "Defaultvehicle"
};

const char* admin_string[] = {
	"Aimbot",
    "Orbital",
	"Flyable Heli",
	"Flyable Jet",
	"Forge Mode",
};

const char* teleport_string[] = {
	"Save location",
	"Load location",
	"Teleort All To Location",
	"Teleport Me To Sky",
	"Teleport All To Sky",
	"Teleport All To Me"
};

const char* camera_string[] = {
	"Enable Camera",
	"Disable Camera",
	"View From Position",
	"View From Sky",
	"Lookat Next Ent",
	"Lookat Prev Ent",
	"Disable Lookat",
};

const char* host_string[] = {
	"Anti Join",
	"Anti Quit",
	"Launch Zombie",
	"Launch Multiplayer",
};

const char* tracer_string[] = {
	"Draw Interval",
	"Speed",
	"Beam Length",
	"Beam Width",
	"Screw Radius",
	"Screw Distance",
	"Fade Time",
	"Fade Width",
	"Brightness",
	"Random Color"
};

const char* sound_string[] = {
	"Cac_menu_enter"
};

const char* weapondef_string[] = {
	"Weapon Type"
};

const char* ui_string[] = {
	"Background",
	"Scrollbar",
	"Position"
};

const char* player_menu_string[] = {
	"God Mode",
	"Freeze",
	"Teleport to me",
	"Spectate",
	"Send to Jail",
	"Kick",
	"Ban",
	"Give Menu",
	"Verify Player",
	"Verify Vip",
	"Verify Admin",
	"Unverify",
};

const char* ui_background_string[] = {
	"Default Background",
	"Red Background",
	"Green Background",
	"Blue Background",
	"Yellow Background",
	"Purple Background",
	"Cyan Background",
	"Flashing Background",
};

const char* ui_scrollbar_string[] = {
	"Default Scrollbar",
	"Red Scrollbar",
	"Green Scrollbar",
	"Blue Scrollbar",
	"Yellow Scrollbar",
	"Purple Scrollbar",
	"Cyan Scrollbar",
};

const char* ui_position_string[] = {
	"Safe Area",
	"Left",
	"Center",
	"Right",
	"Top",
	"Middle",
	"Bottom",
	"Move Left",
	"Move Right",
	"Move Up",
	"Move Down"
};

#pragma endregion Arrays of const char pointers

#pragma region 

menu_s* main_child[] = {
	&main_mods_menu,
	&weapon_menu,
	&player_menu,
	&all_players_menu,
	&fun_menu,
	&lobby_menu,
	&vision_menu,
	&model_menu,
	&admin_menu,
	&teleport_menu,
	&camera_menu,
	&host_menu,
	&tracer_menu,
	&sound_menu,
	&weapondef_menu,
	&ui_menu
};

menu_s* player_child[] = {
	&player_menu_0,
	&player_menu_1,
	&player_menu_2,
	&player_menu_3,
	&player_menu_4,
	&player_menu_5,
	&player_menu_6,
	&player_menu_7,
	&player_menu_8,
	&player_menu_9,
	&player_menu_10,
	&player_menu_11,
};

menu_s* ui_child[] = {
	&ui_background_menu,
	&ui_scrollbar_menu,
	&ui_position_menu
};

#pragma endregion Arrays of child menus

#ifdef _DEBUG
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

void check_menu_size()
{
	BUILD_BUG_ON(ARRAY_SIZE(main_child) != ARRAY_SIZE(main_string));
	BUILD_BUG_ON(ARRAY_SIZE(player_child) != ARRAY_SIZE(player_string));
	BUILD_BUG_ON(ARRAY_SIZE(ui_child) != ARRAY_SIZE(ui_string));
}
#endif

#pragma region
title_s menu_title_mp = {
	0,
	STRING_ARRAY(title_string)
};

menu_s main_menu_mp = {
	TYPE_MENU,
	VERIF_PLAYER,
	0,
	0,
	STRING_ARRAY(main_string),
	(&main_menu_mp),
	{ reinterpret_cast<menuCallback>(main_child) }
};

menu_s main_mods_menu = {
	TYPE_FUNCTION,
	VERIF_PLAYER,
	0,
	0,
	STRING_ARRAY(main_mods_string),
	&main_menu_mp,
	{ menuFunction::main_mods }
};

menu_s weapon_menu = {
	TYPE_FUNCTION,
	VERIF_VIP,
	0,
	0,
	STRING_ARRAY(weapon_string),
	&main_menu_mp,
	{ menuFunction::weapon_menu }
};

menu_s player_menu = {
	TYPE_MENU,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_string),
	&main_menu_mp,
	{ reinterpret_cast<menuCallback>(player_child) }
};

menu_s all_players_menu = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(all_player_string),
	&main_menu_mp,
	{ menuFunction::all_player_menu }
};

menu_s fun_menu = {
	TYPE_FUNCTION,
	VERIF_VIP,
	0,
	0,
	STRING_ARRAY(fun_string),
	&main_menu_mp,
	{ menuFunction::fun_menu }
};

menu_s lobby_menu = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(lobby_string),
	&main_menu_mp,
	{ menuFunction::lobby_menu }
};

menu_s vision_menu = {
	TYPE_FUNCTION,
	VERIF_PLAYER,
	0,
	0,
	STRING_ARRAY(vision_string),
	&main_menu_mp,
	{ menuFunction::vision_menu }
};

menu_s model_menu = {
	TYPE_FUNCTION,
	VERIF_VIP,
	0,
	0,
	STRING_ARRAY(model_string),
	&main_menu_mp,
	{ menuFunction::model_menu }
};

menu_s admin_menu = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(admin_string),
	&main_menu_mp,
	{ menuFunction::admin_menu }
};

menu_s teleport_menu = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(teleport_string),
	&main_menu_mp,
	{ menuFunction::teleport_menu }
};

menu_s camera_menu = {
	TYPE_FUNCTION,
	VERIF_VIP,
	0,
	0,
	STRING_ARRAY(camera_string),
	&main_menu_mp,
	{ menuFunction::camera_menu }
};

menu_s host_menu = {
	TYPE_FUNCTION,
	VERIF_HOST,
	0,
	0,
	STRING_ARRAY(host_string),
	&main_menu_mp,
	{ menuFunction::host_menu }
};

menu_s tracer_menu = {
	TYPE_FUNCTION,
	VERIF_HOST,
	0,
	0,
	STRING_ARRAY(tracer_string),
	&main_menu_mp,
	{ menuFunction::tracer_menu }
};

menu_s sound_menu = {
	TYPE_FUNCTION,
	VERIF_VIP,
	0,
	0,
	STRING_ARRAY(sound_string),
	&main_menu_mp,
	{ menuFunction::sound_menu }
};

menu_s weapondef_menu = {
	TYPE_FUNCTION,
	VERIF_HOST,
	0,
	0,
	STRING_ARRAY(weapondef_string),
	&main_menu_mp,
	{ menuFunction::weapondef_menu }
};

menu_s ui_menu = {
	TYPE_MENU,
	VERIF_PLAYER,
	0,
	0,
	STRING_ARRAY(ui_string),
	&main_menu_mp,
	{ reinterpret_cast<menuCallback>(ui_child) }
};

menu_s player_menu_0 = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_menu_string),
	&player_menu,
	{ menuFunction::player_menu<0> }
};

menu_s player_menu_1 = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_menu_string),
	&player_menu,
	{ menuFunction::player_menu<1> }
};

menu_s player_menu_2 = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_menu_string),
	&player_menu,
	{ menuFunction::player_menu<2> }
};

menu_s player_menu_3 = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_menu_string),
	&player_menu,
	{ menuFunction::player_menu<3> }
};

menu_s player_menu_4 = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_menu_string),
	&player_menu,
	{ menuFunction::player_menu<4> }
};

menu_s player_menu_5 = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_menu_string),
	&player_menu,
	{ menuFunction::player_menu<5> }
};

menu_s player_menu_6 = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_menu_string),
	&player_menu,
	{ menuFunction::player_menu<6> }
};

menu_s player_menu_7 = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_menu_string),
	&player_menu,
	{ menuFunction::player_menu<7> }
};

menu_s player_menu_8 = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_menu_string),
	&player_menu,
	{ menuFunction::player_menu<8> }
};

menu_s player_menu_9 = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_menu_string),
	&player_menu,
	{ menuFunction::player_menu<9> }
};

menu_s player_menu_10 = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_menu_string),
	&player_menu,
	{ menuFunction::player_menu<10> }
};

menu_s player_menu_11 = {
	TYPE_FUNCTION,
	VERIF_ADMIN,
	0,
	0,
	STRING_ARRAY(player_menu_string),
	&player_menu,
	{ menuFunction::player_menu<11> }
};

menu_s ui_background_menu = {
	TYPE_FUNCTION,
	VERIF_VIP,
	0,
	0,
	STRING_ARRAY(ui_background_string),
	&ui_menu,
	{ menuFunction::ui_background }
};

menu_s ui_scrollbar_menu = {
	TYPE_FUNCTION,
	VERIF_VIP,
	0,
	0,
	STRING_ARRAY(ui_scrollbar_string),
	&ui_menu,
	{ menuFunction::ui_scrollbar }
};

menu_s ui_position_menu = {
	TYPE_FUNCTION,
	VERIF_VIP,
	0,
	0,
	STRING_ARRAY(ui_position_string),
	&ui_menu,
	{ menuFunction::ui_position }
};

#pragma endregion Menus and submenus

#pragma region

void menuFunction::main_mods(modmenu_s* menu, int32_t client, int32_t scroll)
{
	gclient_s * cl = g_entities[client].client;

	bool state = false;

	switch (scroll)
	{
	case 0: // freeze
		state = ((cl->flags ^= 0x10) & 0x10);
		break;
	case 1: // god mode
		state = (cl->ps.otherFlags ^= 1) & 1;
		break;
	case 2: // no spread
		state = ((cl->ps.spreadOverrideState ^= 2) & 0x2);
		break;
	case 3: // third person
		state = ((cl->ps.bThirdPerson ^= 1) & 1);
		break;
	case 4: // huds
		state = ((cl->sess.cs.clientUIVisibilityFlags ^= 1) & 1);
		break;
	case 5: // invisible
		state = ((cl->ps.eFlags ^= 0x20) & 0x20);
		break;
	case 6: // satellite
		state = (cl->hasSatellite ^= 1);
		break;
	case 7: // last stand
		state = (cl->lastStand ^= 1);
		break;
	case 8: // slick
		state = ((cl->ps.pm_flags ^= 0x40000000) & 0x40000000);
		break;
	case 9: // poison
		state = ((cl->ps.poisoned ^= 1) & 0x1);
		break;
	case 10: // low gravity
		state = (cl->ps.gravity ^= 0x344) & 0x40;
		break;
	case 11: // infrared
		state = (cl->ps.otherFlags ^= 0x1000) & 0x1000;
		break;
	}

	printlnMod(client, menu->getMenu()->string[scroll], state);
}

void menuFunction::weapon_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	gentity_s* self = &g_entities[client];
	gclient_s* gclient = self->client;
	char weapName[0x100];
	/*
	"Take Weapon",
	"Take All Weapons",
	"Weapon Selector",
	"Weapon List",
	"Drop Weapon",
	"Give Ammo",
	"Dump Weapons",
	*/
	switch (scroll)
	{
	case 0:
		takeWeapon(client, self->client->ps.weapon);
		break;
	case 1:
		takeAllWeapons(client);
		break;
	case 2:
		menu->freeze();
		handleAlloc(NULL, client, menu, weaponSelector, 20, 0);
		break;
	case 3:
		menu->freeze();
		handleAlloc(NULL, client, menu, weaponList, 20, 0);
		break;
	case 4:
		handleAllocTemp(client, menu, weaponDrop, 0);
		break;
	case 5:
		Add_Ammo(self, self->client->ps.weapon, 999, 1);
		break;
	case 6:
		for (int32_t i = 0; i < 0xF; i++)
		{
			DBGPRINTF("weapon_%d:\t%s\n", i, BG_WeaponName(gclient->ps.heldWeapon[i].weapon, weapName, sizeof(weapName)));
		}
		break;
	}
}

void menuFunction::all_player_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	switch (scroll)
	{
	case 1:
		TTY_Write("ScrollID 1");
		break;
	}
}

void menuFunction::fun_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	static struct
	{
		handle_t noclip;
		handle_t teleportGun;
		handle_t driveableRc;
		handle_t flyableDrone;
		handle_t driveableTank;
	}fun_menu_t[12] = 
	{
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
	};

	gentity_s* ent = &g_entities[client];
	gclient_s* cl = ent->client;
	bool state = false;

	/*
	"Throw Frag",
	"Noclip",
	"Teleport Gun",
	"Fire",
	"Speed x2",
	"Suicide",
	"RC-XD",
	"QR-Drone",
	"AI-Tank",
	*/

	switch (scroll)
	{
	case 0: //throw frag
		cl->ps.weapFlags |= 0x40000;
		return;
	case 1: //noclip
		if (!handleStop(&fun_menu_t[client].noclip))
		{
			handleAlloc(&fun_menu_t[client].noclip, client, menu, noclip, 10, 0);
			state = true;
		}
		break;
	case 2: //teleport gun
		if (!handleStop(&fun_menu_t[client].teleportGun))
		{
			handleAlloc(&fun_menu_t[client].teleportGun, client, menu, teleportGun, 50, 0);
			state = true;
		}
		break;
	case 3: //fire
		fakeFire(client, cl->ps.origin, cl->ps.weapon);
		return;
	case 4: //speed x2
		state = (cl->sess.moveSpeedScaleMultiplier == 1);
		if (state)
			cl->sess.moveSpeedScaleMultiplier = 2;
		else
			cl->sess.moveSpeedScaleMultiplier = 1;
		break;
	case 5: //all perks
		state = (cl->ps.perks[0] != (uint32_t)-1);
		if (state)
		{
			cl->ps.perks[0] = cl->ps.perks[1] = -1;
		}
		else
		{
			cl->ps.perks[0] = cl->ps.perks[1] = 0;
		}
		break;
	case 6: //suicide
		handleAllocTemp(client, NULL, suicide, 0);
		return;
	case 7: //rc xd
		if (!handleStop(&fun_menu_t[client].driveableRc))
		{
			handleAlloc(&fun_menu_t[client].driveableRc, client, menu, driveableRc, 50, 0);
			state = true;
		}
		break;
	case 8: //qr drone
		if (!handleStop(&fun_menu_t[client].flyableDrone))
		{
			handleAlloc(&fun_menu_t[client].flyableDrone, client, menu, flyableDrone, 50, 0);
			state = true;
		}
		break;
	case 9: //ai tank
		if (!handleStop(&fun_menu_t[client].driveableTank))
		{
			handleAlloc(&fun_menu_t[client].driveableTank, client, menu, driveableTank, 50, 0);
			state = true;
		}
		break;
	}

	printlnMod(client, menu->getMenu()->string[scroll], state);
}

void menuFunction::lobby_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	/*
	"Anti Quit",
	"Spawn Bot",
	"Merry Go Round",
	"Spawn Jail",
	"Fast Restart",
	"Map Restart",
	"Hear Everyone",
	"SlowMo",
	"Jump Slowdown",
	*/

	bool state = false;

	switch (scroll)
	{
	case 0: //anti quit
		state = (level->matchState.unarchivedState.matchUIVisibilityFlags ^= UNARCHIVED_DISABLEINGAMEMENU) & UNARCHIVED_DISABLEINGAMEMENU;
		break;
	case 1: //spawn bot
		handleAllocTemp(1, menu, botSpawn, 0);
		return;
	case 2: //spawn merry go round
		handleAllocTemp(0, menu, merryGoRound, 0);
		return;
	case 3:
		VectorCopy(g_entities[client].r.currentOrigin, jail_origin);
		handleAlloc(NULL, 0, NULL, spawnJail, 500, 0);
		return;
	case 4: //fast restart
		handleAllocTemp(1, menu, mapRestart, 0);
		return;
	case 5: //map restart
		handleAllocTemp(0, menu, mapRestart, 0);
		return;
	case 6: //hear everyone
		state = (level->matchState.unarchivedState.talkFlags ^= TALK_EVERYONE_H_EVERYONE) & TALK_EVERYONE_H_EVERYONE;
		break;
	case 7: //slowmo
		state = com_timescale[0]->current.value == 1;
		if (state)
			com_timescale[0]->current.value = 0.1;
		else
			com_timescale[0]->current.value = 1;
		break;
	case 8: //jump slowdown
		state = (jump_slowdownEnable[0]->current.boolean ^= true) & true;
		break;
	}

	printlnMod(-1, menu->getMenu()->string[scroll], state);
}

void menuFunction::vision_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	/*
	"Enable Visions",
	"Disable Visions",
	"Default",
	"Infrared",
	"MpIntro",
	"Mortar",
	"Taser",
	*/
	switch (scroll)
	{
	case 0:
		break;
	case 1:
		TTY_Write("ScrollID 1");
		break;
	}
}

void menuFunction::model_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	switch (scroll)
	{
	case 1:
		TTY_Write("ScrollID 1");
		break;
	}
}

void menuFunction::admin_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	/*
	"Aimbot",
    "Orbital",
	"Flyable Heli",
	"Flyable Jet",
	"Forge Mode",
	*/
	static struct
	{
		handle_t aimbot;
		handle_t orbital;
		handle_t flyableHeli;
		handle_t flyableJet;
		handle_t forgeMode;
	}admin_menu_t[12] =
#pragma region
	{
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
		{ INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE, INVALID_HANDLE },
	};
#pragma endregion "Initializer"
	bool state = false;

	switch (scroll)
	{
	case 0:
		if (!handleStop(&admin_menu_t[client].aimbot))
		{
			handleAlloc(&admin_menu_t[client].aimbot, client, menu, aimBot, 50, 0);
			state = true;
		}
		break;
	case 1:
		if (!handleStop(&admin_menu_t[client].orbital))
		{
			handleAlloc(&admin_menu_t[client].orbital, client, menu, orbital, 50, 0);
			state = true;
		}
		break;
	case 2:
		if (!handleStop(&admin_menu_t[client].flyableHeli))
		{
			handleAlloc(&admin_menu_t[client].flyableHeli, client, menu, flyableHeli, 50, 0);
			state = true;
		}
		break;
	case 3:
		if (!handleStop(&admin_menu_t[client].flyableJet))
		{
			handleAlloc(&admin_menu_t[client].flyableJet, client, menu, flyableJet, 50, 0);
			state = true;
		}
		break;
	case 4:
		state = true;
		handleAllocTemp(client, NULL, forgeMode, 0);
		/*if (!handleStop(&admin_menu_t[client].forgeMode))
		{
			handleAlloc(&admin_menu_t[client].forgeMode, client, menu, forgeMode, 50, 0);
			state = true;
		}*/
		break;
	}

	printlnMod(client, menu->getMenu()->string[scroll], state);
}

void menuFunction::teleport_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	/*
	"Save location",
	"Load location",
	"Teleort All To Location",
	"Teleport Me To Sky",
	"Teleport All To Sky",
	"Teleport All To Me"
	*/
	static struct
	{
		vec3_t location;
	}teleport_menu_t[12];

	gclient_s * gclient = g_entities[client].client;
	gclient_s* other;

	switch (scroll)
	{
	case 0:
		teleport_menu_t[client].location[X] = gclient->ps.origin[X];
		teleport_menu_t[client].location[Y] = gclient->ps.origin[Y];
		teleport_menu_t[client].location[Z] = gclient->ps.origin[Z];
		break;
	case 1:
		gclient->ps.origin[X] = teleport_menu_t[client].location[X];
		gclient->ps.origin[Y] = teleport_menu_t[client].location[Y];
		gclient->ps.origin[Z] = teleport_menu_t[client].location[Z];
		break;
	case 2:
		for (int32_t i = 0; i < 12; i++)
		{
			other = g_entities[i].client;
			if (i != client && other->sess.connected == CON_CONNECTED && other->sess.sessionState == SESS_STATE_PLAYING)
			{
				other->ps.origin[X] = teleport_menu_t[client].location[X];
				other->ps.origin[Y] = teleport_menu_t[client].location[Y];
				other->ps.origin[Z] = teleport_menu_t[client].location[Z];
			}
		}
		break;
	case 3:
		gclient->ps.origin[Z] += 100000;
		break;
	case 4:
		for (int32_t i = 0; i < 12; i++)
		{
			other = g_entities[i].client;
			if (i != client && other->sess.connected == CON_CONNECTED && other->sess.sessionState == SESS_STATE_PLAYING)
			{
				other->ps.origin[Z] += 100000;
			}
		}
		break;
	case 5:
		for (int32_t i = 0; i < 12; i++)
		{
			other = g_entities[i].client;
			if (i != client && other->sess.connected == CON_CONNECTED && other->sess.sessionState == SESS_STATE_PLAYING)
			{
				other->ps.origin[X] = gclient->ps.origin[X];
				other->ps.origin[Y] = gclient->ps.origin[Y];
				other->ps.origin[Z] = gclient->ps.origin[Z];
			}
		}
		break;
	}
}

void menuFunction::camera_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	static int16_t entnum[12];
	char buffer[0x40];
	gclient_s* cl = g_entities[client].client;
	/*
	"Enable Camera",
	"Disable Camera",
	"View From Position",
	"View From Sky",
	"Lookat Next Ent",
	"Lookat Prev Ent",
	"Disable Lookat",
	*/

	switch (scroll)
	{
	case 0: // enable camera
		SV_GameSendServerCommand(client, SV_CMD_RELIABLE, "X 1");
		break;
	case 1: // disable camera
		SV_GameSendServerCommand(client, SV_CMD_RELIABLE, "X 0");
		break;
	case 2: // view from pos
		snprintf(buffer, sizeof(buffer), "X 2 %f %f %f %f %f %f", cl->ps.origin[0], cl->ps.origin[1], cl->ps.origin[2] + cl->ps.viewHeightCurrent, cl->ps.viewangles[0], cl->ps.viewangles[1], cl->ps.viewangles[2]);
		SV_GameSendServerCommand(client, SV_CMD_RELIABLE, buffer);
		break;
	case 3: // view from sky
		snprintf(buffer, sizeof(buffer), "X 2 %f %f %f %f %f %f", sv_centermap[0], sv_centermap[1], sv_centermap[2] + 1000.0f, 0.0f, 0.0f, 0.0f);
		SV_GameSendServerCommand(client, SV_CMD_RELIABLE, buffer);
		break;
	case 4: // lookat next ent
		if (entnum[client] < ENTITYNUM_WORLD)
		{
			entnum[client]++;
			snprintf(buffer, sizeof(buffer), "X 16 %d", entnum[client]);
			SV_GameSendServerCommand(client, SV_CMD_RELIABLE, buffer);
		}
		break;
	case 5: // lookat prev ent
		if (entnum[client] > 0)
		{
			entnum[client]--;
			snprintf(buffer, sizeof(buffer), "X 16 %d", entnum[client]);
			SV_GameSendServerCommand(client, SV_CMD_RELIABLE, buffer);
		}
		break;
	case 6: // disable lookat
		SV_GameSendServerCommand(client, SV_CMD_RELIABLE, "X 32");
		break;
	}
}

void menuFunction::host_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	switch (scroll)
	{
	case 1:
		TTY_Write("ScrollID 1");
		break;
	}
}

void menuFunction::tracer_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	switch (scroll)
	{
	case 1:
		TTY_Write("ScrollID 1");
		break;
	}
}

void menuFunction::sound_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	switch (scroll)
	{
	case 1:
		TTY_Write("ScrollID 1");
		break;
	}
}

void menuFunction::weapondef_menu(modmenu_s* menu, int32_t client, int32_t scroll)
{
	switch (scroll)
	{
	case 1:
		TTY_Write("ScrollID 1");
		break;
	}
}

template<int32_t playernum>
void menuFunction::player_menu(modmenu_s*, int32_t client, int32_t scroll)
{
	gclient_s* player = g_entities[playernum].client;
	gclient_s* self = g_entities[client].client;
	char buffer[0x400];

	switch (scroll)
	{
	case 0: //godmode
		player->ps.otherFlags |= 1;
		break;
	case 1: //freeze
		player->flags |= 0x10;
		break;
	case 2: //teleport
		player->ps.origin[X] = self->ps.origin[X];
		player->ps.origin[Y] = self->ps.origin[Y];
		player->ps.origin[Z] = self->ps.origin[Z];
		break;
	case 3: //spectate
		self->sess.sessionState = SESS_STATE_SPECTATOR;
		self->sess.spectatorClient = playernum;
		break;
	case 4: //send to jail

		G_AddEvent(&g_entities[playernum], EV_GIB, 0);
		break;
	case 5: //kick
		snprintf(buffer, sizeof(buffer), "tempBanClient %d\n", playernum);
		CBuf_AddText(0, buffer);
		break;
	case 6: //ban
		snprintf(buffer, sizeof(buffer), "banClient %d\n", playernum);
		CBuf_AddText(0, buffer);
		break;
	case 7: //give menu
		Echelon[playernum].setClient(playernum);
		Echelon[playernum].setBackColor(back_colors[0]);
		Echelon[playernum].setEdgeColor(edge_colors[0], edge_dark_colors[0]);
		Echelon[playernum].verify(VERIF_PLAYER);
		Echelon[playernum].enable();
		break;
	case 8: //player
		Echelon[playernum].verify(VERIF_PLAYER);
		break;
	case 9: //vip
		Echelon[playernum].verify(VERIF_VIP);
		break;
	case 10: //admin
		Echelon[playernum].verify(VERIF_ADMIN);
		break;
	case 11: //unverify
		Echelon[playernum].verify(VERIF_UNVERIFIED);
		break;
	}
}

void menuFunction::ui_background(modmenu_s* menu, int32_t client, int32_t scroll)
{
	static struct
	{
		handle_t flashingBackground;
	}ui_background_t[12] = 
	{
		{ INVALID_HANDLE },
		{ INVALID_HANDLE },
		{ INVALID_HANDLE },
		{ INVALID_HANDLE },
		{ INVALID_HANDLE },
		{ INVALID_HANDLE },
		{ INVALID_HANDLE },
		{ INVALID_HANDLE },
		{ INVALID_HANDLE },
		{ INVALID_HANDLE },
		{ INVALID_HANDLE },
		{ INVALID_HANDLE },
	};
	/*
	"Default Background",
	"Red Background",
	"Green Background",
	"Blue Background",
	"Yellow Background",
	"Purple Background",
	"Cyan Background",
	"Flashing Background",
	*/
	switch (scroll)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		handleStop(&ui_background_t[client].flashingBackground);
		menu->setBackColor(back_colors[scroll]);
		break;
	case 7:
		handleAlloc(&ui_background_t[client].flashingBackground, client, menu, flashingBackground, 2000, 0);
		break;
	}
}

void menuFunction::ui_scrollbar(modmenu_s* menu, int32_t client, int32_t scroll)
{
	/*
	"Default Scrollbar",
	"Red Scrollbar",
	"Green Scrollbar",
	"Blue Scrollbar",
	"Yellow Scrollbar",
	"Purple Scrollbar",
	"Cyan Scrollbar",*/

	menu->setEdgeColor(edge_colors[scroll], edge_dark_colors[scroll]);
}

void menuFunction::ui_position(modmenu_s* menu, int32_t client, int32_t scroll)
{
	/*
	"Safe Area",
	"Left",
	"Center",
	"Right",
	"Top",
	"Middle",
	"Bottom",
	"Move Left",
	"Move Right",
	"Move Up",
	"Move Down"
	*/

	switch (scroll)
	{
	case 0:
		menu->setSafeArea();
		break;
	}
}

void menuFunction::printlnMod(int32_t client, const char* mod, bool state)
{
	static const char* stateString[2] = { "Off", "On" };
	static const int32_t stateInt[2] = { 1, 2 };
	char buffer[0x40];
	snprintf(buffer, sizeof(buffer), "%s [^%i%s^7]", mod, stateInt[state], stateString[state]);
	iPrintlnBold(client, buffer);
}

#pragma endregion menuFunction functions
