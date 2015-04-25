#ifndef __ECHELON_H__
#define __ECHELON_H__

#include "universal/q_shared.h"
#include "game/g_hudelem.h"
#include "modmenu.h"

extern modmenu_s Echelon[12];

struct echelon_scr_data_t
{
	int32_t jail;
	int32_t dropWeaponToGround;
	int32_t spawn_bot;
	int32_t forgeMode;
};

extern echelon_scr_data_t e_data_t;

extern vec3_t jail_origin;

extern const hudelem_color_t red;
extern const hudelem_color_t yellow;
extern const hudelem_color_t green;
extern const hudelem_color_t cyan;
extern const hudelem_color_t blue;
extern const hudelem_color_t magenta;
extern const hudelem_color_t white;
extern const hudelem_color_t black;

extern const hudelem_color_t red_inv;
extern const hudelem_color_t yellow_inv;
extern const hudelem_color_t green_inv;
extern const hudelem_color_t cyan_inv;
extern const hudelem_color_t blue_inv;
extern const hudelem_color_t magenta_inv;
extern const hudelem_color_t white_inv;
extern const hudelem_color_t black_inv;

extern const hudelem_color_t back_colors[7];
extern const hudelem_color_t edge_colors[7];
extern const hudelem_color_t edge_dark_colors[7];

void LoadScriptData();

#endif /*__ECHELON_H__*/
