#include "echelon.h"
#include "clientscript/clientscript.h"

modmenu_s Echelon[12];

echelon_scr_data_t e_data_t;

vec3_t jail_origin;

const hudelem_color_t red = { { 255, 0, 0, 255 } };
const hudelem_color_t yellow = { { 255, 255, 0, 255 } };
const hudelem_color_t green = { { 0, 255, 0, 255 } };
const hudelem_color_t cyan = { { 0, 255, 255, 255} };
const hudelem_color_t blue = { { 0, 0, 255, 255 } };
const hudelem_color_t magenta = { { 255, 0, 255, 255 } };
const hudelem_color_t white = { { 255, 255, 255, 255 } };
const hudelem_color_t black = { { 0, 0, 0, 255 } };

const hudelem_color_t red_inv = { { 255, 0, 0, 0 } };
const hudelem_color_t yellow_inv = { { 255, 255, 0, 0 } };
const hudelem_color_t green_inv = { { 0, 255, 0, 0 } };
const hudelem_color_t cyan_inv = { { 0, 255, 255, 0} };
const hudelem_color_t blue_inv = { { 0, 0, 255, 0 } };
const hudelem_color_t magenta_inv = { { 255, 0, 255, 0 } };
const hudelem_color_t white_inv = { { 255, 255, 255, 0 } };
const hudelem_color_t black_inv = { { 0, 0, 0, 0 } };

const hudelem_color_t back_colors[] = {
	{ { 34, 34, 34, 200 } },
	{ { 82, 34, 34, 200 } },
	{ { 34, 82, 34, 200 } },
	{ { 34, 34, 82, 200 } },
	{ { 82, 82, 34, 200 } },
	{ { 82, 34, 82, 200 } },
	{ { 34, 82, 82, 200 } }
};

const hudelem_color_t edge_colors[] = {
	{ { 90, 100, 160, 200 } },
	{ { 178, 82, 82, 200 } },
	{ { 82, 178, 82, 200 } },
	{ { 82, 82, 178, 200 } },
	{ { 178, 178, 82, 200 } },
	{ { 178, 82, 178, 200 } },
	{ { 82, 178, 178, 200 } }
};

const hudelem_color_t edge_dark_colors[] = {
	{ { 70, 80, 130, 200 } },
	{ { 153, 70, 70, 200 } },
	{ { 70, 153, 70, 200 } },
	{ { 70, 70, 153, 200 } },
	{ { 153, 153, 70, 200 } },
	{ { 153, 70, 153, 200 } },
	{ { 70, 153, 153, 200 } }
};

void LoadScriptData()
{
	//e_data_t.jail = Scr_GetFunctionHandle(SCRIPTINSTANCE_SERVER, "echelon", "jail", NULL, true); // custom script function that spawns a jail
	//e_data_t.forgeMode = Scr_GetFunctionHandle(SCRIPTINSTANCE_SERVER, "echelon", "forgeMode", NULL, true); // custom script function that handles forge mode for every client
	e_data_t.dropWeaponToGround = Scr_GetFunctionHandle(SCRIPTINSTANCE_SERVER, "maps/mp/gametypes/_weapons", "dropWeaponToGround", 0, true);
	e_data_t.spawn_bot = Scr_GetFunctionHandle(SCRIPTINSTANCE_SERVER, "maps/mp/bots/_bot", "spawn_bot", 0, true);
}
