#ifndef __G_LOCAL_H__
#define __G_LOCAL_H__

#include "../game_mp/g_public_mp.h"
#include "../bgame/bg_public.h"
#include "g_public.h"
#include "../universal/q_shared.h"

/* gclient_s->button_bits */
#define __jump			0x00200000
#define __stance		0x00000200
#define __use			0x04000000
#define __triangle		0x00000008
#define __attack		0x80000000
#define __ads			0x00100000
#define __prone			0x00800000
#define __crouch		0x00400000
#define __frag			0x00020000
#define __tactical		0x00010000
#define __melee			0x20000000
#define __sprint		0x40000000

/* gclient_s->button_bitsSinceLastFrame */
#define __dpadup		0x00040000
#define __dpaddown		0x00020000
#define __dpadleft		0x00010000
#define __dpadright		0x00008000

struct gentity_s
{
	entityState_s s; //0x0
	entityShared_t r; //0xF8
	struct gclient_s* client; //0x154
	struct actor_s* actor; //0x158
	struct sentient_s* sentient; //0x15C
	struct TurretInfo* pTurretInfo; //0x160
	struct Destructible* destructible; //0x164
	struct scr_vehicle_s* scr_vehicle; //0x168
	unsigned short model; //0x16C
	char physicsObject; //0x16E
	char takedamage; //0x16F
	char active; //0x170
	char nopickup; //0x171
	char handler; //0x172
	char team; //0x173
	char avoidHandle; //0x174
	unsigned short classname; //0x176
	unsigned short target; //0x178
	unsigned short targetname; //0x17A
	unsigned short script_noteworthy; //0x17C
	unsigned int attachIgnoreCollision; //0x180
	int spawnflags; //0x184
	int flags; //0x188
	int eventTime; //0x18C
	int freeAfterEvent; //0x190
	int unlinkAfterEvent; //0x194
	int clipmask; //0x198
	int processedFrame; //0x19C
	EntHandle parent; //0x1A0
	int nextthink; //0x1A4
	int health; //0x1A8
	int maxhealth; //0x1AC
	int damage; //0x1B0
	flame_timed_damage_t flame_timed_damage[4];
	int last_timed_radius_damage; //0x224
	int count; //0x228
	union
	{
		item_ent_t item[2];
		trigger_ent_t trigger;
		mover_ent_t mover;
		corpse_ent_t corpse;
		missile_ent_t missile;
		blend_ent_t blend;
		actor_ent_t actorInfo;
		spawner_ent_t spawner;
		zbarrier_ent_t zbarrier;
	}; //0x22C
	EntHandle missileTargetEnt; //0x28C
	struct
	{
		unsigned short notifyString; //0x290
		unsigned int index; //0x294
		char stoppable; //0x298
		int basetime; //0x29C
		int duration; //0x2A0
	}snd_wait; //0x290
	struct tagInfo_s* tagInfo; //0x2A4
	gentity_s *tagChildren; //0x2A8
	unsigned short attachModelNames[19]; //0x2AC
	unsigned short attachTagNames[19]; //0x2D2
	struct XAnimTree_s *pAnimTree;//0x2F8
	unsigned short disconnectedLinks; //0x2FC
	int iDisconnectTime; //0x300
	int useCount; //0x304
	int physObjId; //0x308
	gentity_s *nextFree; //0x30C
	int birthtime; //0x310
	int ikPlayerclipTerrainTime; //0x314
	int ikDisableTerrainMappingTime; //0x318
};

enum clientConnected_t
{
	CON_DISCONNECTED = 0x0,
	CON_CONNECTING = 0x1,
	CON_CONNECTED = 0x2,
};

enum sessionState_t
{
	SESS_STATE_PLAYING = 0x0,
	SESS_STATE_DEAD = 0x1,
	SESS_STATE_SPECTATOR = 0x2,
	SESS_STATE_INTERMISSION = 0x3,
};

struct playerTeamState_t
{
	int location;
};

struct score_s
{
	int ping;
	int status_icon;
	int place;
	int score;
	int kills;
	int assists;
	int deaths;
	int wagerWinnings;
	int scoreboardColumns[5];
	int downs;
	int revives;
	int headshots;
	int scoreMultiplier;
	int currentStreak;
};

struct netUInt64
{
	unsigned int high; //0x0
	unsigned int low; //0x4
};

struct clientState_s
{
	int clientIndex; //0x0
	int team; //0x4
	int ffaTeam; //0x8
	int modelindex; //0xC
	int riotShieldNext; //0x10
	int attachModelIndex[6]; //0x14
	int attachTagIndex[6]; //0x2C
	char name[0x20]; //0x44
	float maxSprintTimeMultiplier; //0x64
	int rank; //0x68
	int prestige; //0x6C
	int lastDamageTime; //0x70
	int lastStandStartTime; //0x74
	int turnedHumanTime; //0x78
	int beingRevived; //0x7C
	union
	{
		unsigned long long xuid;
		netUInt64 xuid64;
	}; //0x80
	union
	{
		unsigned long long leagueTeamID;
		netUInt64 leagueTeamID64;
	}; //0x88
	int leagueDivisionID; //0x90
	union
	{
		unsigned long long leagueSubdivisionID;
		netUInt64 leagueSubdivisionID64;
	}; //0x98
	int leagueSubdivisionRank; //0xA0
	int perks[2]; //0xA4
	int voiceConnectivityBits; //0xAC
	char clanAbbrev[8]; //0xB0
	int attachedVehEntNum; //0xB8
	int attachedVehSeat; //0xBC
	int needsRevive; //0xC0
	int clanAbbrevEV; //0xC4
	int vehAnimState; //0xC8
	score_s score;
	int clientUIVisibilityFlags; //0x114
	int offhandWeaponVisible; //0x118
};

struct clientSession_t
{
	sessionState_t sessionState; //0x0
	int spectatorClient; //0x4
	int killcamEntity; //0x8
	int killcamTargetEntity; //0xC
	int archiveTime; //0x10
	unsigned short scriptPersId; //0x14
	clientConnected_t connected; //0x18
	usercmd_s cmd; //0x1C
	usercmd_s oldcmd; // 0x58
	int localClient; //0x94
	int predictItemPickup; //0x98
	char newnetname[0x20]; //0x9C
	int maxHealth; //0xBC
	int enterTime; //0xC0
	playerTeamState_t teamState; //0xC4
	int voteCount; //0xC8
	int teamVoteCount; //0xCC
	float moveSpeedScaleMultiplier; //0xD0
	int viewmodelIndex; //0xD4
	int noSpectate; //0xD8
	int teamInfo; //0xDC
	clientState_s cs; //0xE0
	int psOffsetTime; //0x200
	int scoreboardColumnCache[26]; //0x204
};

struct viewClamp
{
	vec2_t start;
	vec2_t current;
	vec2_t goal;
};
 
struct viewClampState
{
	viewClamp min;
	viewClamp max;
	float accelTime;
	float decelTime;
	float totalTime;
	float startTime;
};

struct gclient_s
{
	playerState_s ps; //0x0
	playerState_s lastPlayerPS; //0x2A10
	clientSession_t sess; //0x5420
	int spectatorClient; //0x5690
	int flags; //0x5694
	int lastCmdTime; //0x5698
	bitarray<64> button_bits; //0x569C
	bitarray<64> oldbutton_bits; //0x56A4
	bitarray<64> latched_button_bits; //0x56AC
	bitarray<64> button_bitsSinceLastFrame; //0x56B4
	float fGunPitch; //0x56BC
	float fGunYaw; //0x56C0
	int damage_blood; //0x56C4
	float damage_from[3]; //0x56C8
	int damage_fromWorld; //0x56D4
	int inactivityTime; //0x56D8
	int inactivityWarning; //0x56DC
	int lastVoiceTime; //0x56E0
	int outWaterTime; //0x56E4
	int switchSeatTime; //0x56E8
	float currentAimSpreadScale; //0x56EC
	int dropWeaponTime; //0x56F0
	EntHandle pLookatEnt; //0x56F4
	vec4_t prevLinkedInvQuat; //0x56F8
	bool prevLinkAnglesSet; //0x5708
	bool link_doCollision; //0x5709
	bool link_useTagAnglesForViewAngles; //0x570A
	bool link_useBaseAnglesForViewClamp; //0x570B
	float linkAnglesFrac; //0x570C
	viewClampState link_viewClamp; //0x5710
	EntHandle useHoldEntity; //0x5750
	int useHoldTime; //0x5754
	int useButtonDone; //0x5758
	int iLastCompassPlayerInfoEnt; //0x575C
	int compassPingTime; //0x5760
	int damageTime; // 0x5764
	float v_dmg_roll; // 0x5768
	float v_dmg_pitch; // 0x576C
	vec3_t swayViewAngles; // 0x5770
	vec3_t swayOffset; // 0x577C
	vec3_t swayAngles; // 0x5788
	vec3_t baseAngles; // 0x5794
	vec3_t baseOrigin; // 0x57A0
	vec3_t recoilAngles; // 0x57AC
	float fLastIdleFactor; // 0x57B8
	int weaponIdleTime; // 0x57BC
	vec3_t recoilSpeed; // 0x57C0
	int previousRecoilTime; // 0x57CC
	float previousRecoilRatio; // 0x57D0
	int lastServerTime; // 0x57D4
	int lastSpawnTime; // 0x57D8
	int lastWeapon; // 0x57DC
	bool previouslyFiring; // 0x57E0
	bool previouslyUsingNightVision; // 0x57E1
	bool previouslyDTP; // 0x57E2
	bool previouslyBeganWeaponRaise; // 0x57E3
	bool previouslySprinting; // 0x57E4
	int hasSpyplane; //0x57E8
	int hasSatellite; //0x57EC
	int revive; //0x57F0
	int reviveTime; // 0x57F4
	int disallowVehicleUsage; // 0x57F8
	unsigned short attachShieldTagName; // 0x57FC
	int lastStand; //0x5800
	int lastStandTime; //0x5804
};

struct SpawnVar
{
	bool spawnVarsValid; //0x0
	int numSpawnVars; //0x4
	char *spawnVars[64][2]; //0x8
	int numSpawnVarChars; //0x208
	char spawnVarChars[2048]; //0x20C
};

/* Match Talk Flags */
#define TALK_EVERYONE_H_EVERYONE		(1<<0)
#define TALK_DEAD_CHAT_WITH_DEAD		(1<<1)
#define TALK_DEAD_CHAT_WITH_TEAM		(1<<2)
#define TALK_DEAD_H_TEAM_LIVING			(1<<3)
#define TALK_DEAD_H_ALL_LIVING			(1<<4)
#define TALK_DEAD_HEAR_KILLER			(1<<5)
#define TALK_KILERS_HEAR_VICTIM			(1<<6)

/* Archived UI Visibility Flags */
#define ARCHIVED_RADAR_ALLIES				(1<<2)
#define ARCHIVED_RADAR_AXIS					(1<<3)
#define ARCHIVED_BOMB_TIMER					(1<<5)
#define ARCHIVED_BOMB_TIMER_A				(1<<6)
#define ARCHIVED_BOMB_TIMER_B				(1<<7)
#define ARCHIVED_AMMOCOUNTERHIDE			(1<<8)
#define ARCHIVED_OVERTIME					(1<<14)

/* Unarchived UI Visibility Flags */
#define UNARCHIVED_FINAL_KILLCAM			(1<<0)
#define UNARCHIVED_ROUND_END_KILLCAM		(1<<1)
#define UNARCHIVED_ENABLE_POPUPS			(1<<4)
#define UNARCHIVED_HUD_HARDCORE				(1<<9)
#define UNARCHIVED_PREGAME					(1<<10)
#define UNARCHIVED_DRAWSPECTATORMESSAGES	(1<<11)
#define UNARCHIVED_DISABLEINGAMEMENU		(1<<12)
#define UNARCHIVED_GAME_ENDED				(1<<13)

struct ArchivedMatchState
{
	int matchUIVisibilityFlags; //0x0
	int bombTimer[2]; //0x4
	int roundsPlayed; //0xC
	int worldFields[8]; //0x10
};

struct UnarchivedMatchState
{
	int teamScores[TEAM_MAX]; //0x0
	int matchUIVisibilityFlags; //0x24
	int scoreboardColumnTypes[5]; //0x28
	char sideHasMeat; //0x3C
	char initialPlayersConnected; //0x3D
	int talkFlags; //0x40
};

struct MatchState
{
	int index; //0x0
	ArchivedMatchState archivedState; //0x4
	UnarchivedMatchState unarchivedState; //0x34
	unsigned int pad[1]; //0x78
}__attribute__((aligned(8)));

struct level_locals_t
{
	gclient_s* clients; //0x0
	gentity_s *gentities; //0x4
	int gentitySize; //0x8
	int num_entities; //0xC
	gentity_s* firstFreeEnt; //0x10
	gentity_s* lastFreeEnt; //0x14
	struct scr_vehicle_s *vehicles; //0x18
	gentity_s *firstFreeActor; //0x1C
	gentity_s *lastFreeActor; //0x20
	int num_actors; //0x24
	struct sentient_s *sentients; //0x28
	struct actor_s *actors; //0x2C
	int actorCorpseCount; //0x30
	struct Destructible* destructibles; //0x34 ?
	struct TurretInfo *turrets; //0x38
	int logFile; //0x3C
	int initializing; //0x40
	int clientIsSpawning; //0x44
	int maxclients; //0x48
	int teamSortedClients[18]; //0x4C
	objective_t objectives[0x20]; //0x94
	int objectivesClientMask[32][2]; //0x694
	int framenum; //0x794
	int time; //0x798
	int previousTime; //0x79C
	int frametime; //0x7A0
	int startTime; //0x7A4
	SpawnVar spawnVar; //0x7A8
	EntHandle droppedWeaponCue[0x20]; //0x11B4
	int savepersist; //0x1234
	float fFogOpaqueDist; //0x1238
	float fFogOpaqueDistSqrd; //0x123C
	int bPlayerIgnoreRadiusDamage; //0x1240
	int bPlayerIgnoreRadiusDamageLatched; //0x1244
	int currentEntityThink; //0x1248
	int registerWeapons; //0x124C
	int bRegisterItems; //0x1250
	int bRegisterLeaderboards; //0x1254
	struct cached_tag_mat_t
	{
		int time; //0x0
		int entnum; //0x4
		unsigned short name; //0x8
		vec3_t tagMat[4]; //0xC
	}cachedTagMat; //0x1258
	cached_tag_mat_t cachedEntTargetTagMat; //0x1294
	struct trigger_info_t
	{
		unsigned short entnum; //0x0
		unsigned short otherEntnum; //0x2
		int useCount; //0x4
		int otherUseCount; //0x8
	}pendingTriggerList[256]; //0x12D0
	trigger_info_t currentTriggerList[256]; //0x1ED0
	int pendingTriggerListSize; //0x2AD0
	int currentTriggerListSize; //0x2AD4
	int openScriptIOFileHandles[1]; //0x2AD8
	char *openScriptIOFileBuffers[1]; //0x2ADC
	struct com_parse_mark_t
	{
		int lines; //0x0
		const char *text; //0x4
		int ungetToken; //0x8
		int backup_lines; //0xC
		const char *backup_text; //0x10
	}currentScriptIOLineMark[1]; //0x2AE0
	int scriptPrintChannel; //0x2AF4
	float compassMapUpperLeft[2]; //0x2AF8
	float compassMapWorldSize[2]; //0x2B00
	float compassNorth[2]; //0x2B08
	int finished; //0x2B10
	int manualNameChange; //0x2B14
	unsigned short modelMap[512]; //0x2B18
	int disable_grenade_suicide; //0x2F18
	int numConnectedClients; //0x2F1C
	int sortedClients[18]; //0x2F20
	int bUpdateScoresForIntermission; //0x2F68
	int numVotingClients; //0x2F6C
	MatchState matchState __attribute__((aligned(32))); //0x2F80
	int teamScores[10]; //0x3000
	int lastTeammateHealthTime; //0x3028
	unsigned int teamHasSpyplane[10]; //0x302C
	unsigned int teamHasSatellite[10]; //0x3054
	unsigned int teamHasMeat[10]; //0x307C
	char voteString[0x400]; //0x30A4
	char voteDisplayString[0x400]; //0x34A4
	int voteTime; //0x38A4
	int voteExecuteTime; //0x38A8
	int voteYes; //0x38AC
	int voteNo; //0x38B0
	int currentActorClone; //0x38B4
	int iSearchFrame; //0x38B8
	struct sv_FxVisBlock_t
	{
		gentity_s* fxEnt;
		float radius;
	}fxVisibilityEnts[32]; //0x38BC
	int currentPlayerClone; //0x39BC
	bool hostMigrationActive; //0x39C0
	int hostMigrationStart; //0x39C4
	struct actorAntilagFrame_t
	{
		vec3_t position[32]; //0x0
		vec3_t angles[32]; //0x180
		char useCount[32]; //0x300
		char inUse[32]; //0x320
		int time; //0x340
	}actorAntilagFrames[20]; //0x39C8
	int nextActorAntilagFrame; //0x7B18
	struct vehicleAntilagFrame_t
	{
		vec3_t position[16]; //0x0
		vec3_t angles[16]; //0xC0
		char useCount[16]; //0x180
		char inUse[16]; //0x190
		int time; //0x1A0
	}vehicleAntilagFrames[20]; //0x7B1C
	int nextVehicleAntilagFrame; //0x9BEC
	struct ZBarrierType *zbarrierTypes[255]; //0x9BF0
}; //0xA000

struct TurretInfo
{
		char unk_0[8]; //0x0
	int flags; //0x8
		char unk_C[0x110]; //0xC
};

struct scr_vehicle_s
{
		char unk0[0x18]; //0x0
	float pathWidth; //0x18
		char unk1[4]; //0x1C
	float pathWidthLookAheadFrac; //0x20
	float pathDistanceTraveled; //0x24
	float pathPos[3]; //0x28
		char unk2[0xC]; //0x34
	float pathLookPos[3]; //0x40
		char unk3[0x164]; //0x4C
	struct
	{
		float origin[3]; //0x1B0
		float prevOrigin[3]; //0x1BC
		float angles[3]; //0x1C8
		float prevAngles[3]; //0x1D4
		float maxAngleVel[3]; //0x1E0
		float yawAccel; //0x1EC
		float yawDecel; //0x1F0
		float mins[3]; //0x1F4
		float maxs[3]; //0x200
		float vel[3]; //0x20C
		float bodyVel[3]; //0x218
		float rotVel[3]; //0x224
		float accel[3]; //0x230
		float maxPitchAngle; //0x23C
		float maxRollAngle; //0x240
			char unk0[0x84]; //0x244
	}phys;
	int entnum; //0x2C8
	short infoIdx; //0x2CC
	int flags; //0x2D0
	int team; //0x2D4
	int flags2; //0x2D8 i made up the name
	struct
	{
			char unk0[0x24]; //0x2DC
		float rotScale; //0x300
	}turret;
	struct
	{
		int jitterPeriodMin; //0x304
		int jitterPeriodMax; //0x308
		int jitterEndTime; //0x30C
		float jitterOffsetRange[3]; //0x310
			char unk0[0x24]; //0x31C
	}jitter;
	struct
	{
		float hoverRadius; //0x340
		float hoverSpeed; //0x344
		float hoverAccel; //0x348
			char unk0[0x194]; //0x34C
	}hover;

	unsigned short lookAtText0; //0x4E0
	unsigned short lookAtText1; //0x4E2
	int manualMode; //0x4E4
	float manualSpeed; //0x4E8
	float manualAccel; //0x4EC
	float manualDecel; //0x4F0
	float speed; //0x4F4
	float maxSpeed; //0x4F8
	float maxDragSpeed; //0x4FC
	float turningAbility; //0x500
	int hasTarget; //0x504
	int hasTargetYaw; //0x508
	int hasGoalYaw; //0x50C
	int stopAtGoal; //0x510
	int stopping; //0x514 not sure
	int targetEnt; //0x518
	int lookAtEnt; //0x51C not sure
	float targetOrigin[3]; //0x520
	float targetOffset[3]; //0x52C
	float targetYaw; //0x538
	float goalPosition[3]; //0x53C
	float goalYaw; //0x548
	float prevGoalYaw; //0x54C
	float yawOverShoot; //0x550
		char unk4[0x24]; //0x554
	int hasDefaultPitch; //0x578
	float defaultPitch; //0x57C
		char unk5[0xC4]; //0x580
	float nearGoalNotifyDist;//0x644
	float joltDir[2]; //0x648
	float joltTime; //0x650
	float joltWave; //0x654
	float joltSpeed; //0x658
	float joltDecel; //0x65C
		char pad[0x1C00]; //0x660
};

enum he_type_t
{
	HE_TYPE_FREE = 0x0,
	HE_TYPE_TEXT = 0x1,
	HE_TYPE_VALUE = 0x2,
	HE_TYPE_PLAYERNAME = 0x3,
	HE_TYPE_MAPNAME = 0x4,
	HE_TYPE_GAMETYPE = 0x5,
	HE_TYPE_DAMAGE_INDICATOR = 0x6,
	HE_TYPE_SCORE = 0x7,
	HE_TYPE_MATERIAL = 0x8,
	HE_TYPE_TIMER_DOWN = 0x9,
	HE_TYPE_TIMER_UP = 0xA,
	HE_TYPE_TENTHS_TIMER_DOWN = 0xB,
	HE_TYPE_TENTHS_TIMER_UP = 0xC,
	HE_TYPE_CLOCK_DOWN = 0xD,
	HE_TYPE_CLOCK_UP = 0xE,
	HE_TYPE_WAYPOINT = 0xF,
	HE_TYPE_PERKS = 0x10,
	HE_TYPE_COUNT = 0x11,
};

enum he_font_t
{
	HE_FONT_DEFAULT = 0x0,
	HE_FONT_BIGFIXED = 0x1,
	HE_FONT_SMALLFIXED = 0x2,
	HE_FONT_OBJECTIVE = 0x3,
	HE_FONT_BIG = 0x4,
	HE_FONT_SMALL = 0x5,
	HE_FONT_EXTRABIG = 0x6,
	HE_FONT_EXTRASMALL = 0x7,
	HE_FONT_COUNT = 0x8,
};

enum hudelem_flag_t
{
	HUDELEMFLAG_FOREGROUND = 0x1,
	HUDELEMFLAG_HIDEWHENDEAD = 0x2,
	HUDELEMFLAG_HIDEWHENINMENU = 0x4,
	HUDELEMFLAG_FONTSTYLE3D_SHADOWED = 0x8,
	HUDELEMFLAG_FONTSTYLE3D_SHADOWED_MORE = 0x10,
	HUDELEMFLAG_FONT3D_USE_GLOWCOLOR = 0x20,
	HUDELEMFLAG_HIDEWHENINKILLCAM = 0x40,
	HUDELEMFLAG_REDACT = 0x80,
	HUDELEMFLAG_FADEWHENTARGETED = 0x100,
	HUDELEMFLAG_HIDEWHENINDEMO = 0x200,
	HUDELEMFLAG_HIDEWHILEREMOTECONTROLING = 0x400,
	HUDELEMFLAG_COD7DECODE = 0x800,
	HUDELEMFLAG_NO_OFFSCREEN_WAYPOINT = 0x1000,
	HUDELEMFLAG_HIDEWHENINSCOPE = 0x2000,
	HUDELEMFLAG_TYPEWRITER = 0x4000,
	HUDELEMFLAG_IMMUNE_TO_DEMO_GAMEHUD_SETTINGS = 0x8000,
	HUDELEMFLAG_IMMUNE_TO_DEMO_FREECAMERA = 0x10000,
	HUDELEMFLAG_DEBUG = 0x20000,
	HUDELEMFLAGBITS = 0x12,
};

union hudelem_color_t
{
	struct
	{
		char r;
		char g;
		char b;
		char a;
	};
	int rgba;
};

struct hudelem_s
{
	float x;//0x0
	float y;//0x4
	float z;//0x8
	float fontScale;//0xC
	float fromFontScale;//0x10
	int fontScaleStartTime;//0x14
	hudelem_color_t color;//0x18
	hudelem_color_t fromColor;//0x1C
	int fadeStartTime;//0x20
	int scaleStartTime;//0x24
	float fromX;//0x28
	float fromY;//0x2C
	int moveStartTime;//0x30
	int time;//0x34
	int duration;//0x38
	float value;//0x3C
	float sort;//0x40
	hudelem_color_t glowColor;//0x44
	int fxBirthTime;//0x48
	int flags;//0x4C
	short targetEntNum;//0x50
	short fontScaleTime;//0x52
	short fadeTime;//0x54
	short label;//0x56
	short width;//0x58
	short height;//0x5A
	short fromWidth;//0x5C
	short fromHeight;//0x5E
	short scaleTime;//0x60
	short moveTime;//0x62
	short text;//0x64
	unsigned short fxLetterTime;//0x66
	unsigned short fxDecayStartTime;//0x68
	unsigned short fxDecayDuration;//0x6A
	unsigned short fxRedactDecayStartTime;//0x6C
	unsigned short fxRedactDecayDuration;//0x6E
	char type;//0x70
	char font;//0x71
	char alignOrg;//0x72
	char alignScreen;//0x73
	char materialIndex;//0x74
	char offscreenMaterialIdx;//0x75
	char fromAlignOrg;//0x76
	char fromAlignScreen;//0x77
	char soundID;//0x78
	char ui3dWindow;//0x79
};

struct game_hudelem_s
{
	hudelem_s elem;//0x0
	int clientNum;//0x7C
	int team;//0x80
	int archived;//0x84
};

// g_items.cpp

GAME_EXPORT void Add_Ammo(gentity_s* ent, int weaponIndex, int count, int fillclip);
GAME_EXPORT gentity_s* Drop_Item(gentity_s* ent, int weapon, float height, unsigned short tag, float velocity);

// g_weapon.cpp

GAME_EXPORT void G_GivePlayerWeapon(playerState_s* ps, int iWeaponIndex, char altModelIndex, int weaponOptions);
GAME_EXPORT gentity_s* Weapon_RocketLauncher_Fire(gentity_s* attacker, int weapon, float spread, weaponParms* wp, const vec3_t gunVel, gentity_s* target, const vec3_t targetOffset, bool);
GAME_EXPORT void G_GetWeaponIndexForName(int* weaponIndex, const char* name);

// g_scr_vehicle.cpp

GAME_EXPORT void G_SpawnVehicle(gentity_s *ent, const char *typeName, int load);
GAME_EXPORT void G_FreeVehicle(gentity_s *ent);
GAME_EXPORT void G_MakeVehicleUsable(gentity_s* ent, bool usable);
GAME_EXPORT void VEH_UnlinkPlayer(gentity_s* ent, bool changingSeats, const char* error);
GAME_EXPORT void VEH_LinkPlayer(gentity_s* vehicle, gentity_s* occupant, int seatIndex, bool changingSeats);

// g_scr_mover.cpp

GAME_EXPORT void ScriptMover_SetupMove(gentity_s *ent, const vec3_t vPos, vec_t pfTotalTime, vec_t pfAccelTime, vec_t pfDecelTime);
GAME_EXPORT void ScriptMover_Rotate(gentity_s *ent, const vec3_t vRot, vec_t pfTotalTime, vec_t pfAccelTime, vec_t pfDecelTime);
GAME_EXPORT void ScriptMover_SetupPhysicsLaunch(trajectory_t *pTr, trajectory_t *paTr, const vec3_t contact_point, const vec3_t initial_force);

// bullet.cpp

GAME_EXPORT void Bullet_Fire(gentity_s* attacker, float spread, weaponParms* wp, gentity_s* weaponEnt, int gameTime);

// turret.cpp

GAME_EXPORT void G_SpawnTurret(gentity_s* turret, const char* weapon, SpawnVar* spawnVar);
GAME_EXPORT gentity_s* SpawnTurretInternal(unsigned short classname, const vec3_t origin, const char * weaponinfoname); // undefined yet

extern  level_locals_t * level;

extern gentity_s * g_entities;

extern game_hudelem_s * g_hudelems;

#endif /* __G_LOCAL_H__ */
