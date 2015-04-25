#ifndef __ECHELON_MP_H__
#define __ECHELON_MP_H__

#include "modmenu.h"

extern title_s menu_title_mp;

extern menu_s main_menu_mp;
extern menu_s main_mods_menu;
extern menu_s weapon_menu;
extern menu_s player_menu;
extern menu_s all_players_menu;
extern menu_s fun_menu;
extern menu_s lobby_menu;
extern menu_s vision_menu;
extern menu_s model_menu;
extern menu_s admin_menu;
extern menu_s teleport_menu;
extern menu_s camera_menu;
extern menu_s host_menu;
extern menu_s tracer_menu;
extern menu_s sound_menu;
extern menu_s weapondef_menu;
extern menu_s ui_menu;

extern menu_s player_menu_0;
extern menu_s player_menu_1;
extern menu_s player_menu_2;
extern menu_s player_menu_3;
extern menu_s player_menu_4;
extern menu_s player_menu_5;
extern menu_s player_menu_6;
extern menu_s player_menu_7;
extern menu_s player_menu_8;
extern menu_s player_menu_9;
extern menu_s player_menu_10;
extern menu_s player_menu_11;

extern menu_s ui_background_menu;
extern menu_s ui_scrollbar_menu;
extern menu_s ui_position_menu;

class menuFunction
{
public:
	static void main_mods(modmenu_s* menu, int32_t client, int32_t scroll);
	static void weapon_menu(modmenu_s* menu, int32_t client, int32_t scroll);
	static void all_player_menu(modmenu_s* menu, int32_t client, int32_t scroll);
	static void fun_menu(modmenu_s* menu, int32_t client, int32_t scroll);
	static void lobby_menu(modmenu_s* menu, int32_t client, int32_t scroll);
	static void vision_menu(modmenu_s* menu, int32_t client, int32_t scroll);
	static void model_menu(modmenu_s* menu, int32_t client, int32_t scroll);
	static void admin_menu(modmenu_s* menu, int32_t client, int32_t scroll);
	static void teleport_menu(modmenu_s* menu, int32_t client, int32_t scroll);
	static void camera_menu(modmenu_s* menu, int32_t client, int32_t scroll);
	static void host_menu(modmenu_s* menu, int32_t client, int32_t scroll);
	static void tracer_menu(modmenu_s* menu, int32_t client, int32_t scroll);
	static void sound_menu(modmenu_s* menu, int32_t client, int32_t scroll);
	static void weapondef_menu(modmenu_s* menu, int32_t client, int32_t scroll);

	template<int32_t playernum>
	static void player_menu(modmenu_s* menu, int32_t client, int32_t scroll);

	static void ui_background(modmenu_s* menu, int32_t client, int32_t scroll);
	static void ui_scrollbar(modmenu_s* menu, int32_t client, int32_t scroll);
	static void ui_position(modmenu_s* menu, int32_t client, int32_t scroll);

private:
	static void printlnMod(int32_t client, const char* mod, bool state);
};

#endif /*__ECHELON_MP_H__*/
