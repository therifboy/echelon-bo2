#include "modmenu.h"

#include "bgame/bg_public.h"
#include "game_mp/g_local_mp.h"

void precacheWeaponModels()
{
	WeaponDef* weaponDef;
	XModel * xModel;
	const char* model;

	int32_t numweapons = BG_GetNumWeapons();

	for (int32_t weapID = 0; weapID < numweapons; weapID++)
	{
		weaponDef = BG_GetWeaponDef(weapID);

		xModel = weaponDef->gunModel[0];
		if (xModel == 0)
			continue;
		
		model = xModel->name;
		if (model == 0)
			continue;
		
		G_ModelIndex(model);
	}
}

void precacheShaders()
{
	material_white = G_MaterialIndex("white");
}

void precacheModels()
{
	G_ModelIndex("veh_t6_air_fa38_killstreak");
	G_ModelIndex("veh_t6_air_fa38_killstreak_alt");
}

void precacheEffects()
{
	G_EffectIndex("maps/mp_maps/fx_mp_exp_bomb_fire_streamer");
}
