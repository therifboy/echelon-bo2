#include "handle_functions.h"
#include "entity.h"
#include "functions.h"
#include "echelon.h"
#include "utils.h"
#include "hud.h"

#include "qcommon/qcommon.h"
#include "stdio.h"
#include "server_mp/server_mp.h"

#include <string.h>

void weaponSelector(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	static struct
	{
		uint8_t weaponID;
		bool holdingUse;
		int16_t entnum;
		const char* name;
	}weaponSelector_t[12] =
#pragma region
	{
		{ 2, false, 0, NULL },
		{ 2, false, 0, NULL },
		{ 2, false, 0, NULL },
		{ 2, false, 0, NULL },
		{ 2, false, 0, NULL },
		{ 2, false, 0, NULL },
		{ 2, false, 0, NULL },
		{ 2, false, 0, NULL },
		{ 2, false, 0, NULL },
		{ 2, false, 0, NULL },
		{ 2, false, 0, NULL },
		{ 2, false, 0, NULL }
	};
#pragma endregion "Initializer"

	gclient_s* cl = g_entities[client].client;
	gentity_s* weaponEnt = &g_entities[weaponSelector_t[client].entnum];
	uint32_t weaponID = weaponSelector_t[client].weaponID;
	WeaponDef* weaponDef;
	XModel * xModel;
	const char* tempname;

	if (type == CALL_START)
	{
		weaponDef = BG_GetWeaponDef(weaponID);
		weaponSelector_t[client].holdingUse = false;
		weaponSelector_t[client].name = weaponDef->gunModel[0]->name;
		weaponSelector_t[client].entnum = (weaponEnt = spawnEnt(scr_const->script_model, NULL))->s.number;
		anglesToForward(weaponEnt->r.currentOrigin, cl->ps.viewangles, cl->ps.origin, 50);
		weaponEnt->r.currentOrigin[Z] += cl->ps.viewHeightCurrent;
		G_SetModel(weaponEnt, weaponSelector_t[client].name);
		VectorSet(weaponEnt->r.currentAngles, 0, cl->ps.viewangles[YAW] + 90, cl->ps.viewangles[PITCH]);
		G_CallSpawnEntity(weaponEnt);
	}
	else if (type == CALL_EXECUTE)
	{
		if (!weaponSelector_t[client].holdingUse)
		{
			if (cl->button_bitsSinceLastFrame.array[1] & __dpadright)
			{
		moveright:
				weaponID++;
				if (weaponID < BG_GetNumWeapons())
				{
					weaponDef = BG_GetWeaponDef(weaponID);
					if (weaponDef != NULL)
					{
						xModel = weaponDef->gunModel[0];
						if (xModel == NULL)
							goto moveright;
						tempname = xModel->name;
						if (tempname && !strcmp(tempname, weaponSelector_t[client].name))
							goto moveright;

						weaponSelector_t[client].weaponID = weaponID;
						weaponSelector_t[client].name = tempname;
					}
				}
			}
			else if (cl->button_bitsSinceLastFrame.array[1] & __dpadleft)
			{
		moveleft:
				weaponID--;
				if (weaponID <= 1)
					weaponID = 2;
				weaponDef = BG_GetWeaponDef(weaponID);
				xModel = weaponDef->gunModel[0];
				if (xModel == NULL)
					goto moveleft;
				tempname = xModel->name;
				if (tempname && !strcmp(tempname, weaponSelector_t[client].name) && weaponID != 2)
					goto moveleft;

				weaponSelector_t[client].weaponID = weaponID;
				weaponSelector_t[client].name = tempname;
			}
	
			if (cl->button_bits.array[0] & __use)
			{
				weaponSelector_t[client].holdingUse = true;
			}
		}
		else
		{
			if (!(cl->button_bits.array[0] & __use))
			{
				handleStop(handle);
				menu->unfreeze();
				giveWeapon(client, weaponSelector_t[client].weaponID, 0);
				return;
			}
		}

		anglesToForward(weaponEnt->r.currentOrigin, cl->ps.viewangles, cl->ps.origin, 50);
		weaponEnt->r.currentOrigin[Z] += cl->ps.viewHeightCurrent;
		weaponEnt->r.currentAngles[ROLL] = cl->ps.viewangles[PITCH];
		weaponEnt->r.currentAngles[YAW] = cl->ps.viewangles[YAW] + 90;
		G_SetModel(weaponEnt, weaponSelector_t[client].name);
		G_CallSpawnEntity(weaponEnt);
	}
	else if (type == CALL_STOP)
	{
		deleteEnt(weaponEnt);
		weaponSelector_t[client].entnum = ENTITYNUM_NONE;
		weaponSelector_t[client].holdingUse = false;
		weaponSelector_t[client].name = NULL;
		weaponSelector_t[client].weaponID = 2;
	}
}

void weaponList(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	static struct
	{
		uint32_t weaponID;
		game_hudelem_s* name;
	}weaponList_t[12] = 
#pragma region
	{
		{ 1, NULL },
		{ 1, NULL },
		{ 1, NULL },
		{ 1, NULL },
		{ 1, NULL },
		{ 1, NULL },
		{ 1, NULL },
		{ 1, NULL },
		{ 1, NULL },
		{ 1, NULL },
		{ 1, NULL },
		{ 1, NULL }
	};
#pragma endregion "Initializer"

	gclient_s* cl = g_entities[client].client;
	WeaponVariantDef* variantDef;
	uint32_t weaponID = weaponList_t[client].weaponID;
	game_hudelem_s* name = weaponList_t[client].name;

	if (type == CALL_START)
	{
		variantDef = BG_GetWeaponVariantDef(weaponID);
		name = SetText(client, variantDef->szClipName, white, black_inv, HE_FONT_OBJECTIVE, 2.5f, 300, 200);
		ForeGround(name, true);
		HideWhenDead(name, true);
		HideWhenInMenu(name, true);
		HideWhenInDemo(name, true);
		HideWhenInScope(name, true);
		HideWhenInKillcam(name, true);
		HideWhileRemoteControlling(name, true);
		weaponList_t[client].name = name;
	}
	else if (type == CALL_EXECUTE)
	{
		if (cl->button_bitsSinceLastFrame.array[1] & __dpadright)
		{
			weaponID++;
			if (weaponID < BG_GetNumWeapons())
			{
				variantDef = BG_GetWeaponVariantDef(weaponID);
				weaponList_t[client].name->elem.text = G_LocalizedStringIndex(variantDef->szClipName);
				weaponList_t[client].weaponID = weaponID;
			}
		}
		else if (cl->button_bitsSinceLastFrame.array[1] & __dpadleft)
		{
			weaponID--;
			if (weaponID > 0)
			{
				variantDef = BG_GetWeaponVariantDef(weaponID);
				weaponList_t[client].name->elem.text = G_LocalizedStringIndex(variantDef->szClipName);
				weaponList_t[client].weaponID = weaponID;
			}
		}

		if (cl->button_bits.array[0] & __use)
		{
			ClearHud(name);
			weaponList_t[client].name = NULL;
			giveWeapon(client, weaponList_t[client].weaponID, 0);
			weaponList_t[client].weaponID = 1;
			menu->unfreeze();
			handleStop(handle);
		}
	}
}

void aimBot(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	gentity_s* self = &g_entities[client];
	gclient_s* gclient = self->client;

	if (gclient->button_bits.array[0] & __ads)
	{
		int32_t nearest = nearestPlayer(client);
		if (nearest != -1)
		{
			gentity_s* enemy = &g_entities[nearest];

			vec3_t vecFrom;
			vec3_t vecTo;
			viewOrigin(gclient, vecFrom);
			viewOrigin(enemy->client, vecTo);

			vec3_t vector, angles;
			VectorSubtract(vecTo, vecFrom, vector);
			vectoangles(vector, angles);
			SetClientViewAngle(self, angles);

			if (gclient->button_bits.array[0] & __attack)
			{
				playerDamage(enemy, self, 0x186A0, MOD_HEAD_SHOT, gclient->ps.weapon, HITLOC_HEAD);
			//	G_GetPlayerViewOrigin(&enemy->client->ps, vecFrom, false);
			//	VectorCopy(vecFrom, vecTo);
			//	vecTo[X] += 2;
			//	vecFrom[X] -= 2;
			//	magicBullet(self, self->s.weapon, vecFrom, vecTo);
			//	player_die(enemy, self, self, 0x186A0, MOD_HEAD_SHOT, gclient->ps.weapon, NULL, HITLOC_HEAD, 0);
			}
		}
	}
}

void weaponDrop(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	if (type == CALL_TEMP)
	{
		gentity_s *self = &g_entities[client];
		char weapName[0x100];

		Scr_AddString(SCRIPTINSTANCE_SERVER, BG_WeaponName(self->client->ps.weapon, weapName, sizeof(weapName)));
		execEntThread(self, e_data_t.dropWeaponToGround, 1);
	}
}

void teleportGun(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	vec3_t position, start, end;
	gclient_s * cl = g_entities[client].client;

	if (cl->button_bits.array[0] & __attack)
	{
		G_GetPlayerViewOrigin(&cl->ps, start);
		anglesToForward(end, cl->ps.viewangles, start, 2000000);
		if (bulletTrace(client, start, end, position, NULL, false))
			VectorCopy(position, cl->ps.origin);
	}
}

void noclip(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	static struct
	{
		gentity_s* parent;
	}noclip_t[12];

	gentity_s* self = &g_entities[client];
	gclient_s* cl = self->client;
	float forward = 0, right = 0;
	vec3_t forwardposition = { }, rightposition = { }, angles, finalposition;
	int8_t forwardmove, rightmove;
	bool sprintButtonPressed;

	if (type == CALL_START)
	{
		noclip_t[client].parent = spawnEnt(scr_const->script_origin, cl->ps.origin);
		playerLinkTo(self, noclip_t[client].parent, 0);
	}
	else if (type == CALL_EXECUTE)
	{
		sprintButtonPressed = (cl->button_bits.array[0] & __sprint);
		angles[ROLL] = 0;
		if ((forwardmove = cl->sess.cmd.forwardmove) != 0)
		{
			angles[YAW] = cl->ps.viewangles[YAW];
			if (forwardmove < 0)
			{
				angles[PITCH] = cl->ps.viewangles[PITCH] + 180;
				forward = (-forwardmove) / 128.0f;
			}
			else
			{
				angles[PITCH] = cl->ps.viewangles[PITCH];
				forward = forwardmove / 128.0f;
			}
			if (sprintButtonPressed)
				forward *= 40;
			else
				forward *= 20;

			AngleVectors(angles, forwardposition, NULL, NULL);
		}
		if ((rightmove = cl->sess.cmd.rightmove) != 0)
		{
			angles[PITCH] = 0;
			if (rightmove < 0)
			{
				angles[YAW] = cl->ps.viewangles[YAW] + 90;
				right = (-rightmove) / 128.0f;
			}
			else
			{
				angles[YAW] = cl->ps.viewangles[YAW] - 90;
				right = rightmove / 128.0f;
			}
			if (sprintButtonPressed)
				right *= 40;
			else
				right *= 20;
		
			AngleVectors(angles, rightposition, NULL, NULL);
		}
		
		finalposition[X] = noclip_t[client].parent->r.currentOrigin[X] + forwardposition[X] * forward + rightposition[X] * right;
		finalposition[Y] = noclip_t[client].parent->r.currentOrigin[Y] + forwardposition[Y] * forward + rightposition[Y] * right;
		finalposition[Z] = noclip_t[client].parent->r.currentOrigin[Z] + forwardposition[Z] * forward + rightposition[Z] * right;

		setOrigin(noclip_t[client].parent, finalposition);
	}
	else if (type == CALL_STOP)
	{
		unlink(self);
		deleteEnt(noclip_t[client].parent);
		noclip_t[client].parent = NULL;
	}
}

void orbital(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	static struct
	{
		int32_t player;
		vec_t yaw;
	}orbit_t[12];
	
	const float zOffset = 50;

	gclient_s* self = g_entities[client].client;
	gclient_s* player = g_entities[orbit_t[client].player].client;
	vec3_t angles;
	vec3_t position;

	if (type == CALL_START)
	{
		if ((orbit_t[client].player = nearestPlayer(client)) == -1)
		{
			iPrintlnBold(client, "^1Could not find any enemy");
			handleStop(handle);
			return;
		}
		orbit_t[client].yaw = 0;
	}
	else if (type == CALL_EXECUTE)
	{
		if (player->sess.connected == CON_CONNECTED)
		{
			if (player->sess.sessionState == SESS_STATE_PLAYING)
			{
				player->flags |= 0x10;
				player->ps.gravity = 0;
				VectorSet(angles, 0, orbit_t[client].yaw, 0);
				anglesToForward(position, angles, self->ps.origin, 100);
				VectorSet(player->ps.origin, position[X], position[Y], position[Z] + zOffset);
				orbit_t[client].yaw++;
				if (orbit_t[client].yaw >= 360)
					orbit_t[client].yaw = 0;
			}
		}
		else
		{
			handleStop(handle);
			handleAlloc(handle, client, menu, orbital, 50, 0);
		}
	}
	else if (type == CALL_STOP)
	{
		player->flags &= ~0x10;
		player->ps.gravity = 800;
	}
}

void flyableHeli(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	static struct
	{
		gentity_s* helicopter;
		game_hudelem_s* hint;
		int8_t buttonHeld;
		bool driving;
	}flyableHeli_t[12];

	static int32_t text = G_LocalizedStringIndex("Hold ^3[{+usereload}]^7 to drive");

	gentity_s* self = &g_entities[client];
	gclient_s* gclient = self->client;
	scr_vehicle_s* helicopter;
	vec_t forward = 0, right = 0;
	int8_t forwardmove, rightmove;
	vec3_t forwardposition = { }, rightposition = { }, heliangles/*, playerangles*/;

	if (type == CALL_START)
	{
		vec3_t origin, angles = { };
		VectorCopy(self->r.currentOrigin, origin);
		origin[Z] += 2000;
		flyableHeli_t[client].helicopter = spawnHelicopter(self, origin, angles, "heli_ai_mp", "veh_t6_air_attack_heli_mp_light");
		helicopter = flyableHeli_t[client].helicopter->scr_vehicle;
		helicopter->turningAbility = 1;
		helicopter->manualMode = 1;
		helicopter->manualSpeed = 60 * 17.6f;
		helicopter->manualAccel = helicopter->manualDecel = 40 * 17.6f;
		helicopter->phys.maxPitchAngle = 30;
		helicopter->phys.maxRollAngle = 40;
		origin[Z] -= 1850;
		setVehGoalPos(flyableHeli_t[client].helicopter->scr_vehicle, origin, 1);
		flyableHeli_t[client].hint = SetText(client, text, white_inv, black_inv, HE_FONT_OBJECTIVE, 1.5f, 200, 200); 
	}
	else if (type == CALL_EXECUTE)
	{
		helicopter = flyableHeli_t[client].helicopter->scr_vehicle;
		if (!flyableHeli_t[client].driving)
		{
			if (vec3distancesq(helicopter->phys.origin, self->r.currentOrigin) < 62500)
			{
				flyableHeli_t[client].hint->elem.color.a = 255; //make hintstring visible
				if (gclient->button_bits.array[0] & __use)
				{
					if (flyableHeli_t[client].buttonHeld++ == 20) // 2 seconds
					{
						playerLinkTo(self, flyableHeli_t[client].helicopter, 0); // link player to helicopter
						gclient->ps.otherFlags |= 1; //enable godmode
						gclient->ps.bThirdPerson = true; //set third person view
						gclient->ps.eFlags |= 0x20; //hide player
						gclient->ps.weapFlags |= 0x80; //disable weapons
						setFov(client, 90);
						flyableHeli_t[client].driving = true;
						flyableHeli_t[client].hint->elem.color.a = 0;
						flyableHeli_t[client].buttonHeld = 0;
						helicopter->stopAtGoal = 0;
						helicopter->hasGoalYaw = 1;
					}
				}
				else
				{
					flyableHeli_t[client].buttonHeld = 0;
				}
			}
			else
			{
				flyableHeli_t[client].hint->elem.color.a = 0;
				flyableHeli_t[client].buttonHeld = 0;
			}
		}
		else
		{
			if (gclient->button_bits.array[0] & __attack)
			{
				if (!(gclient->button_bits.array[0] & __ads))
				{
					helicopter->phys.vel[Z] += 5;
					//helicopter->phys.angles[YAW] -= 5;
					//VectorCopy(gclient->ps.viewangles, playerangles);
					//playerangles[YAW] -= 5;
					//SetClientViewAngle(self, playerangles);
				}
			}
			else if (gclient->button_bits.array[0] & __ads)
			{
				helicopter->phys.vel[Z] -= 5;
				//helicopter->phys.angles[YAW] += 5;
				//VectorCopy(gclient->ps.viewangles, playerangles);
				//playerangles[YAW] += 5;
				//SetClientViewAngle(self, playerangles);
			}

			if ((forwardmove = gclient->sess.cmd.forwardmove) != 0)
			{
				forward = forwardmove * 0.5f;
				heliangles[PITCH] = 0;
				heliangles[ROLL] = 0;
				heliangles[YAW] = helicopter->phys.angles[YAW];
				AngleVectors(heliangles, forwardposition, NULL, NULL);
			}
			if ((rightmove = gclient->sess.cmd.rightmove) != 0)
			{
				right = rightmove * 0.5f;
				heliangles[PITCH] = 0;
				heliangles[ROLL] = 0;
				heliangles[YAW] = helicopter->phys.angles[YAW] - 90;
				AngleVectors(heliangles, rightposition, NULL, NULL);
			}

			VectorCopy(helicopter->phys.origin, helicopter->goalPosition);
			VectorNegate(helicopter->phys.angles, helicopter->phys.angles);
			VectorSet(helicopter->phys.vel, forwardposition[X] * forward + rightposition[X] * right, forwardposition[Y] * forward + rightposition[Y] * right, forwardposition[Z] * forward + rightposition[Z] * right);
			helicopter->goalYaw = gclient->ps.viewangles[YAW];
		}
	}
	else
	{
		unlink(self);
		G_FreeVehicle(flyableHeli_t[client].helicopter);
		deleteEnt(flyableHeli_t[client].helicopter);
		flyableHeli_t[client].helicopter = NULL;
		ClearHud(flyableHeli_t[client].hint);
		flyableHeli_t[client].buttonHeld = 0;
		flyableHeli_t[client].driving = false;
		gclient->ps.otherFlags &= ~1;
		gclient->ps.bThirdPerson = false;
		gclient->ps.eFlags &= ~0x20;
		gclient->ps.weapFlags &= ~0x80;
		setFov(client, 65);
	}
}

void flyableJet(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	static struct
	{
		gentity_s* plane;
		int16_t speed;
		bool reachedMaxHeight;
	}flyableJet_t[12];
	static const vec3_t linkOffset = { -100, 0, 0 };
	static const int16_t minSpeed = 40;
	//static const int16_t maxSpeed = 80;
	static const float maxHeight = 2000;
	static const float fallHeight = 1700;
	//static const float maxPitch = 70;

	gentity_s* self = &g_entities[client];
	gclient_s* gclient = self->client;
	gentity_s* plane;
	vec3_t startPoint, jetAngles, position;
	int8_t forwardmove, rightmove;
	bool yawRight, yawLeft;

	if (type == CALL_START)
	{
		VectorSet(startPoint, sv_centermap[X], sv_centermap[Y], sv_centermap[Z] + 500);
		flyableJet_t[client].plane = spawnPlane(self, scr_const->script_model, startPoint);
		flyableJet_t[client].speed = minSpeed;
		flyableJet_t[client].reachedMaxHeight = false;
		setModel(flyableJet_t[client].plane, "veh_t6_air_fa38_killstreak");
		setEnemyModel(flyableJet_t[client].plane, "veh_t6_air_fa38_killstreak_alt");
		playerLinkTo(self, flyableJet_t[client].plane, 0, linkOffset);
		gclient->ps.otherFlags |= 1; //enable godmode
		gclient->ps.bThirdPerson = true; //set third person view
		gclient->ps.eFlags |= 0x20; //hide player
		gclient->ps.weapFlags |= 0x80; //disable weapons
		setFov(client, 90);
	}
	else if (type == CALL_EXECUTE)
	{
		plane = flyableJet_t[client].plane;
		VectorCopy(plane->r.currentAngles, jetAngles);
		yawRight = gclient->button_bits.array[0] & __attack;
		yawLeft = gclient->button_bits.array[0] & __ads;

		if (plane->r.currentOrigin[Z] > (sv_centermap[Z] + maxHeight))
		{
			flyableJet_t[client].reachedMaxHeight = true;
		}
		else if (plane->r.currentOrigin[Z] < (sv_centermap[Z] + fallHeight))
		{
			flyableJet_t[client].reachedMaxHeight = false;
		}

		if (!flyableJet_t[client].reachedMaxHeight)
		{
			if ((forwardmove = gclient->sess.cmd.forwardmove) != 0) // PITCH
			{
				jetAngles[PITCH] += (forwardmove / 64.0f);
			}
			if ((rightmove = gclient->sess.cmd.rightmove) != 0) // ROLL
			{
				jetAngles[ROLL] += (rightmove / 64.0f);
				//plane->r.currentAngles[ROLL] += (rightmove / 64.0f);
			}
			if (yawRight)
			{
				if (!yawLeft)
				{
					jetAngles[YAW]++;
					//plane->r.currentAngles[YAW]++;
				}
			}
			else if (yawLeft)
			{
				jetAngles[YAW]--;
				//plane->r.currentAngles[YAW]--;
			}

			anglesToForward(position, jetAngles, plane->r.currentOrigin, flyableJet_t[client].speed);
			G_SetAngle(plane, jetAngles);
			moveTo(plane, position, 0.05, 0, 0);
		}
		else
		{
			physicsLaunch(plane, NULL, NULL);
		}
	}
	else
	{
		unlink(self);
		deleteEnt(flyableJet_t[client].plane);
		gclient->ps.otherFlags &= ~1;
		gclient->ps.bThirdPerson = false;
		gclient->ps.eFlags &= ~0x20;
		gclient->ps.weapFlags &= ~0x80;
		setFov(client, 65);
	}
}

void merryGoRound(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	/*static struct
	{
		gentity_s* midpoints[2];
		gentity_s* seats[2][6];

		bool seatIsFree[2][6];
	}merryGoRound_t;*/
}

void magicBullet(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	vec3_t position, start, end;
	gentity_s* self = &g_entities[client];
	gclient_s* cl = self->client;
	int32_t weapon;

	if (cl->button_bits.array[0] & __attack)
	{
		G_GetPlayerViewOrigin(&cl->ps, start);
		anglesToForward(end, cl->ps.viewangles, start, 2000000);
		bulletTrace(client, start, end, position, NULL, false);
		G_GetWeaponIndexForName(&weapon, "ai_tank_drone_rocket_mp");
		if (weapon != 0)
			magicBullet(self, weapon, start, position);
	}
}

void forgeMode(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	static struct
	{
		gentity_s* object;
		float yawDelta;
	}forgeMode_t[12] =
#pragma region
	{
		{ NULL, 0 },
		{ NULL, 0 },
		{ NULL, 0 },
		{ NULL, 0 },
		{ NULL, 0 },
		{ NULL, 0 },
		{ NULL, 0 },
		{ NULL, 0 },
		{ NULL, 0 },
		{ NULL, 0 },
		{ NULL, 0 },
		{ NULL, 0 },
	};
#pragma endregion "Initializer"

	gentity_s* self = &g_entities[client];
	gclient_s* gclient = self->client;
	vec3_t start, end, position, angles;
	int32_t hitId;

	if (type == CALL_EXECUTE)
	{
		if (forgeMode_t[client].object == NULL)
		{
			if (gclient->button_bits.array[0] & __ads)
			{
				G_GetPlayerViewOrigin(&gclient->ps, start);
				anglesToForward(end, gclient->ps.viewangles, start, 2000000);
				bulletTrace(client, start, end, NULL, &hitId, true);
				if (hitId < ENTITYNUM_MAX_NORMAL)
				{
					forgeMode_t[client].object = &g_entities[hitId];
					forgeMode_t[client].yawDelta = gclient->ps.viewangles[YAW] - forgeMode_t[client].object->r.currentAngles[YAW];
				}
			}
		}
		else
		{
			if (!(gclient->button_bits.array[0] & __ads))
			{
				forgeMode_t[client].object = NULL;
				forgeMode_t[client].yawDelta = 0;
			}
			else
			{
				G_GetPlayerViewOrigin(&gclient->ps, start);
				anglesToForward(position, gclient->ps.viewangles, start, 200);
				if (forgeMode_t[client].object->client)
				{
					VectorCopy(position, forgeMode_t[client].object->client->ps.origin);
				}
				else
				{
					VectorSet(angles, 0, gclient->ps.viewangles[YAW] - forgeMode_t[client].yawDelta, 0);
					G_SetAngle(forgeMode_t[client].object, angles);
					//forgeMode_t[client].object->r.currentAngles[YAW] = gclient->ps.viewangles[YAW] - forgeMode_t[client].yawDelta;
					setOrigin(forgeMode_t[client].object, position);
				}
			}
		}
	}
	else
	{
		forgeMode_t[client].object = NULL;
		forgeMode_t[client].yawDelta = 0;
	}
}

void driveableRc(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	static struct
	{
		gentity_s* car;
	}driveableRc_t[12] = { };

	static const vec3_t mins = { 0, 0, 5 };
	static const vec3_t maxs = { 5, 5, 10 };
	static const vec_t heightOffset = 5;
	static const vec_t distanceFromPlayer = 70;
	static const char* modelName = "veh_t6_drone_rcxd";
	static const char* targetName = "rcbomb";
	static const char* vehicleType = "rc_car_medium_mp";

	vec3_t startAngles;
	vec3_t startPosition, endPosition, position;
	gentity_s* self = &g_entities[client];
	gentity_s* vehicle;
	gclient_s* gclient = self->client;

	if (type == CALL_START)
	{
		VectorSet(startAngles, 0, self->r.currentAngles[YAW], 0);
		anglesToForward(startPosition, startAngles, self->r.currentOrigin, distanceFromPlayer);
		startPosition[Z] += 100;
		VectorSet(endPosition, startPosition[X], startPosition[Y], startPosition[Z] - 300);
		physicsTrace(self, startPosition, mins, maxs, endPosition, position, NULL);
		position[Z] += heightOffset;
		vehicle = spawnVehicle(modelName, SL_GetString(targetName, 0), vehicleType, position, startAngles);
		
		setEnemyModel(vehicle, modelName); //setenemymodel( enemymodel )
		vehicle->s.lerp.eFlags |= 0x800; //enableaimassist()
		setOwner(vehicle, self); //setowner
		vehicle->s.lerp.faction = (vehicle->s.lerp.faction & ~4) | gclient->sess.cs.team; //setvehicleteam( team )
		vehicle->s.lerp.eFlags2 |= 0x20000000; //setdrawinfrared(1)
		useVehicle(vehicle, self, 0);
		driveableRc_t[client].car = vehicle;
		menu->freeze();
	}
	else if (type == CALL_EXECUTE)
	{
		if (!(self->client->ps.eFlags & 0x4000))
		{
			handleStop(handle);
			menu->unfreeze();
		}
	}
	else if (type == CALL_STOP)
	{
		G_FreeVehicle(driveableRc_t[client].car);
		deleteEnt(driveableRc_t[client].car);
		driveableRc_t[client].car = NULL;
	}
}

void flyableDrone(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	static struct
	{
		gentity_s* drone;
	}flyableDrone_t[12];

	static const vec_t heightOffset = 30;
	static const vec_t distanceFromPlayer = 70;
	static const char* modelName = "veh_t6_drone_quad_rotor_mp";
	static const char* enemyModelName = "veh_t6_drone_quad_rotor_mp_alt";
	static const char* vehicleType = "qrdrone_mp";

	gentity_s* self = &g_entities[client];
	vec3_t startPosition;
	vec3_t startAngles;

	if (type == CALL_START)
	{
		VectorSet(startAngles, 0, self->r.currentAngles[YAW], 0);
		anglesToForward(startPosition, startAngles, self->r.currentOrigin, distanceFromPlayer);
		startPosition[Z] += heightOffset;
		flyableDrone_t[client].drone = spawnHelicopter(self, startPosition, startAngles, vehicleType, modelName);
		flyableDrone_t[client].drone->s.lerp.eFlags |= 0x800;
		flyableDrone_t[client].drone->s.lerp.eFlags2 |= 0x20000000;
		setEnemyModel(flyableDrone_t[client].drone, enemyModelName);
		useVehicle(flyableDrone_t[client].drone, self, 0);
		menu->freeze();
	}
	else if (type == CALL_EXECUTE)
	{
		if (self->client->button_bits.array[0] & __use)
		{
			handleStop(handle);
			menu->unfreeze();
		}
	}
	else if (type == CALL_STOP)
	{
		G_FreeVehicle(flyableDrone_t[client].drone);
		deleteEnt(flyableDrone_t[client].drone);
		flyableDrone_t[client].drone = NULL;
	}
}

void driveableTank(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	static struct
	{
		gentity_s* tank;
	}driveableTank_t[12];

	static const vec3_t mins = { 0, 0, 5 };
	static const vec3_t maxs = { 5, 5, 10 };
	static const vec_t heightOffset = 5;
	static const vec_t distanceFromPlayer = 70;
	static const char* modelName = "veh_t6_drone_tank";
	static const char* enemyModelName = "veh_t6_drone_tank_alt";
	static const char* targetName = "talon";
	static const char* vehicleType = "ai_tank_drone_mp";
	static const char* loopSound = "veh_talon_idle_npc";

	gentity_s* self = &g_entities[client];
	vec3_t startPosition, endPosition, finalPosition;
	vec3_t startAngles;

	if (type == CALL_START)
	{
		VectorSet(startAngles, 0, self->r.currentAngles[YAW], 0);
		anglesToForward(startPosition, startAngles, self->r.currentOrigin, distanceFromPlayer);
		startPosition[Z] += 100;
		VectorSet(endPosition, startPosition[X], startPosition[Y], startPosition[Z] - 300);
		physicsTrace(self, startPosition, mins, maxs, endPosition, finalPosition, NULL);
		finalPosition[Z] += heightOffset;
		driveableTank_t[client].tank = spawnVehicle(modelName, SL_GetString(targetName, 0), vehicleType, finalPosition, startAngles);
		setEnemyModel(driveableTank_t[client].tank, enemyModelName);
		playLoopSound(driveableTank_t[client].tank, loopSound, 200);
		setOwner(driveableTank_t[client].tank, self);

		driveableTank_t[client].tank->s.lerp.eFlags |= 0x800;
		driveableTank_t[client].tank->s.lerp.eFlags2 |= 0x20000000;
		useVehicle(driveableTank_t[client].tank, self, 0);
		menu->freeze();
	}
	else if (type == CALL_EXECUTE)
	{
		if (self->client->button_bits.array[0] & __use)
		{
			handleStop(handle);
			menu->unfreeze();
		}
	}
	else if (type == CALL_STOP)
	{
		G_FreeVehicle(driveableTank_t[client].tank);
		deleteEnt(driveableTank_t[client].tank);
		driveableTank_t[client].tank = NULL;
	}
}

void spawnBunker(int32_t arg, modmenu_s* menu, handle_t* handle, handleCall_t type)
{

}

void botSpawn(int32_t count, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	if (type == CALL_TEMP)
	{
		for (int32_t i = 0; i < count; i++)
		{
			Scr_AddString(SCRIPTINSTANCE_SERVER, "autoassign");
			execThread(e_data_t.spawn_bot, 1);
		}
	}
}

void humanTorch(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	if (type == CALL_EXECUTE)
	{
		gentity_s* self = &g_entities[client];
		int32_t fx = G_EffectIndex("maps/mp_maps/fx_mp_exp_bomb_fire_streamer");
		playEffectOnTag(self, fx, scr_const->j_head);
		playEffectOnTag(self, fx, scr_const->j_spine4);
		playEffectOnTag(self, fx, scr_const->pelvis);
	}
}

void turretSpawner(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	if (type == CALL_TEMP)
	{
		gentity_s* self = &g_entities[client];
		gentity_s* turret = spawnTurret(SL_GetString("auto_turret", 0), self->r.currentOrigin, /*self->r.currentAngles,*/ "auto_gun_turret_mp");
		DBGPRINTF("spawnTurret(): %p\n", turret);
		setTurretType(turret, TURRET_SENTRY);
		setModel(turret, "t6_wpn_turret_sentry_gun");
	}
}

void flashingBackground(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	static struct
	{
		uint8_t backColorID;
	}flashingBackground_t[12];

	if (type == CALL_START)
	{
		flashingBackground_t[client].backColorID = 2;
		menu->setBackColor(back_colors[1]);
	}
	else if (type == CALL_EXECUTE)
	{
		menu->setBackColor(back_colors[flashingBackground_t[client].backColorID]);
		if (++flashingBackground_t[client].backColorID >= ARRAY_SIZE(back_colors))
		{
			flashingBackground_t[client].backColorID = 1;
		}
	}
}

void mapRestart(int32_t fast_restart, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	if (type == CALL_TEMP)
	{
		level->matchState.unarchivedState.matchUIVisibilityFlags |= UNARCHIVED_GAME_ENDED;
		if (fast_restart)
			CBuf_AddText(0, "fast_restart\n");
		else
			CBuf_AddText(0, "map_restart\n");
	}
}

void suicide(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	if (type == CALL_TEMP)
	{
		gentity_s* self = &g_entities[client];
		self->health = 0;
		self->client->ps.stats[STAT_HEALTH] = 0;
		self->client->ps.eFlags2 = 0;
		self->client->ps.linkFlags = 0;
		playerDamage(self, self, 100000, MOD_SUICIDE, 0, HITLOC_NONE);
	}
}

void spawnJail(int32_t client, modmenu_s* menu, handle_t* handle, handleCall_t type)
{
	//if (type == CALL_TEMP)
	//{
	//	execEntThread(&g_entities[client], e_data_t.jail, 0);
	//}
	static struct
	{
		gentity_s* midpoints[2];
		gentity_s* supplies[2][6];
		int32_t supplyId;
		bool spawned;
	}spawnJail_t;

	static const char* supplyModel = "t6_wpn_supply_drop_ally";
	vec3_t spawnOrigin;

	if (type == CALL_START)
	{
		VectorCopy(jail_origin, spawnOrigin);
		spawnOrigin[Z] += 25;
		spawnJail_t.midpoints[0] = spawnEnt(scr_const->script_origin, spawnOrigin);
		spawnOrigin[Z] += 50;
		spawnJail_t.midpoints[1] = spawnEnt(scr_const->script_origin, spawnOrigin);
		spawnJail_t.supplyId = 0;
	}
	else if (type == CALL_EXECUTE)
	{
		VectorCopy(jail_origin, spawnOrigin);
		spawnOrigin[X] += 100;

		spawnOrigin[Z] += 25;
		spawnJail_t.supplies[0][spawnJail_t.supplyId] = spawnEnt(scr_const->script_model, spawnOrigin);
		setModel(spawnJail_t.supplies[0][spawnJail_t.supplyId], supplyModel);
		G_EntLinkTo(spawnJail_t.supplies[0][spawnJail_t.supplyId], spawnJail_t.midpoints[0], 0);

		spawnOrigin[Z] += 50;
		spawnJail_t.supplies[1][spawnJail_t.supplyId] = spawnEnt(scr_const->script_model, spawnOrigin);
		setModel(spawnJail_t.supplies[1][spawnJail_t.supplyId], supplyModel);
		G_EntLinkTo(spawnJail_t.supplies[1][spawnJail_t.supplyId], spawnJail_t.midpoints[1], 0);

		rotateAxis(spawnJail_t.midpoints[0], YAW, 60, 0.5, 0, 0);
		rotateAxis(spawnJail_t.midpoints[1], YAW, 60, 0.5, 0, 0);

		spawnJail_t.supplyId++;

		if (spawnJail_t.supplyId >= 6)
		{
			handleFree(handle);
		}
	}
	else if (type == CALL_STOP)
	{
		for (int32_t i = 0; i < 2; i++)
		{
			deleteEnt(spawnJail_t.midpoints[i]);
			for (int32_t j = 0; j < 6; j++)
			{
				deleteEnt(spawnJail_t.supplies[i][j]);
			}
		}
	}
}
