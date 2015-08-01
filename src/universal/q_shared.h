#ifndef __Q_SHARED_H__
#define __Q_SHARED_H__

#include "../export.h"
#include "../qcommon/bitarray.h"

// Angle indexes
#define PITCH			0
#define YAW				1
#define ROLL			2

// Origin indexes
#define X				0
#define Y				1
#define Z				2

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

#define DotProduct(x,y)			((x)[0]*(y)[0]+(x)[1]*(y)[1]+(x)[2]*(y)[2])
#define VectorSubtract(a,b,c)	((c)[0]=(a)[0]-(b)[0],(c)[1]=(a)[1]-(b)[1],(c)[2]=(a)[2]-(b)[2])
#define VectorAdd(a,b,c)		((c)[0]=(a)[0]+(b)[0],(c)[1]=(a)[1]+(b)[1],(c)[2]=(a)[2]+(b)[2])
#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define	VectorScale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	VectorMA(v, s, b, o)	((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))
#define VectorDistanceSq(a, b)	((((a)[0]-(b)[0])*((a)[0]-(b)[0]))+(((a)[1]-(b)[1])*((a)[1]-(b)[1]))+(((a)[2]-(b)[2])*((a)[2]-(b)[2])))
#define VectorDistance(a, b)	(sqrt(VectorDistanceSq(a, b)))

#define VectorClear(a)			((a)[0]=(a)[1]=(a)[2]=0)
#define VectorNegate(a,b)		((b)[0]=-(a)[0],(b)[1]=-(a)[1],(b)[2]=-(a)[2])
#define VectorSet(v, x, y, z)	((v)[0]=(x), (v)[1]=(y), (v)[2]=(z))
#define Vector4Copy(a,b)		((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2],(b)[3]=(a)[3])

union __m128
{
	float m128_f32[4];
	unsigned long long m128_u64[2];
	char m128_i8[16];
	short m128_i16[8];
	int m128_i32[4];
	long long m128_i64[2];
	char m128_u8[16];
	unsigned short m128_u16[8];
	unsigned int m128_u32[4];
};

// com_math.cpp

GAME_EXPORT void vectoangles(const vec3_t vector, vec3_t angles);

// com_math_anglevectors.cpp

GAME_EXPORT void AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);

/*
==========================================================

DVARS (dynamic variables)

==========================================================
*/

/* dvar->flags */
#define DVAR_ARCHIVE			(1 << 0)	// 0x0001
#define DVAR_USERINFO			(1 << 1)	// 0x0002
#define DVAR_SERVERINFO			(1 << 2)	// 0x0004
#define DVAR_SYSTEMINFO			(1 << 3)	// 0x0008
#define DVAR_INIT				(1 << 4)	// 0x0010
#define DVAR_LATCH				(1 << 5)	// 0x0020
#define DVAR_ROM				(1 << 6)	// 0x0040
#define DVAR_CHEAT				(1 << 7)	// 0x0080
#define DVAR_DEVELOPER			(1 << 8)	// 0x0100
#define DVAR_SAVED				(1 << 9)	// 0x0200
#define DVAR_NORESTART			(1 << 10)	// 0x0400
#define DVAR_CHANGEABLE_RESET	(1 << 12)	// 0x1000
#define DVAR_EXTERNAL			(1 << 14)	// 0x4000
#define DVAR_AUTOEXEC			(1 << 15)	// 0x8000
 
/* dvar->type */
enum dvarType_t
{
	DVAR_TYPE_BOOL = 0,
	DVAR_TYPE_FLOAT = 1,
	DVAR_TYPE_FLOAT_2 = 2,
	DVAR_TYPE_FLOAT_3 = 3,
	DVAR_TYPE_FLOAT_4 = 4,
	DVAR_TYPE_INT = 5,
	DVAR_TYPE_ENUM = 6,
	DVAR_TYPE_STRING = 7,
	DVAR_TYPE_COLOR = 8,
	DVAR_TYPE_INT64 = 9,
	DVAR_TYPE_LINEAR_COLOR_RGB = 10,
	DVAR_TYPE_COLOR_XYZ = 11,
	DVAR_TYPE_COUNT = 12,
};

union DvarLimits
{
	struct {
		int stringCount;
		const char **strings;
	} enumeration;
 
	struct {
		int min;
		int max;
	} integer;
 
	struct {
		float min;
		float max;
	} value, vector;
 
	struct {
		unsigned long long min;
		unsigned long long max;
	} integer64;
};

union DvarValue
{
	bool boolean;
	int integer;
	unsigned int unsignedInt;
	unsigned long long integer64;
	float value;
	float vector[4];
	const char *string;
	char color[4];
};

struct dvar_s
{
	const char *name;
	const char *description;
	int hash;
	unsigned int flags;
	dvarType_t type;
	bool modified;
	DvarValue current;
	DvarValue latched;
	DvarValue reset; 
	DvarLimits limits; 
	dvar_s *hashNext;
	int unk;
}__attribute__((aligned(4)));

/*
==============================================================

COLLISION DETECTION

==============================================================
*/

enum TraceHitType
{
	TRACE_HITTYPE_NONE = 0x0,
	TRACE_HITTYPE_ENTITY = 0x1,
	TRACE_HITTYPE_DYNENT_MODEL = 0x2,
	TRACE_HITTYPE_DYNENT_BRUSH = 0x3,
	TRACE_HITTYPE_GLASS = 0x4,
};

struct trace_t
{
	struct hybrid_vector
	{
	  __m128 vec;
	}normal; //0x0
	float fraction; //0x10
	int sflags; //0x14
	int cflags; //0x18
	TraceHitType hitType; //0x1C
	unsigned short hitId; //0x20
	unsigned short modelIndex; //0x22
	unsigned short partName; //0x24
	unsigned short boneIndex; //0x26
	unsigned short partGroup; //0x28
	bool allsolid; //0x2A
	bool startsolid; //0x2B
	bool walkable; //0x2C
	struct cStaticModel_s *staticModel; //0x30
	int hitPartition; //0x34
}__attribute__((aligned(16)));

//===================================================================

#define GENTITYNUM_BITS			10
#define MAX_GENTITIES			(1<<GENTITYNUM_BITS)
#define ENTITYNUM_NONE			(MAX_GENTITIES-1)
#define ENTITYNUM_WORLD			(MAX_GENTITIES-2)
#define ENTITYNUM_MAX_NORMAL	(MAX_GENTITIES-2)

//===================================================================

enum objectiveState_t
{
	OBJST_EMPTY = 0x0,
	OBJST_ACTIVE = 0x1,
	OBJST_INVISIBLE = 0x2,
	OBJST_DONE = 0x3,
	OBJST_CURRENT = 0x4,
	OBJST_FAILED = 0x5,
	OBJST_NUMSTATES = 0x6,
};

struct objective_t
{
	objectiveState_t state; //0x0
	vec3_t origin; //0x4
	short entNum; //0x10
	vec2_t size; //0x14
	int icon; //0x1C
	short ownerNum; //0x20
	unsigned short name; //0x22
	short teamMask; //0x24
	char progress; //0x26
	int clientUseMask[1]; //0x28
	char gamemodeFlags; //0x2C
	char flags; //0x2D
	char teamNum; //0x2E
};

struct PlayerVehicleState
{
	vec3_t origin;//0
	vec3_t angles;//0xC
	vec3_t velocity;//0x18
	vec3_t angVelocity;//0x24
	vec2_t tilt;//0x30
	vec2_t tiltVelocity;//0x38
	float targetHeightDelta;//0x40
	float lastGroundHeight;//0x44
	int entity;//0x48
	int flags;//0x4C
	bool fullPhysics; //0x50
};

struct SprintState
{
	int sprintButtonUpRequired;//0x0
	int sprintDelay;//0x4
	int lastSprintStart;//0x8
	int lastSprintEnd;//0xC
	int sprintStartMaxLength;//0x10
	int sprintDuration;//0x14
	int sprintCooldown;//0x18
};

struct MantleState
{
	float yaw; //0x0
	int timer; //0x4
	int transIndex; //0x8
	int flags; //0xC
};

struct playerState_s
{
	int commandTime;//0x0
	int pm_type;//0x4
	int bobCycle; //0x8
	int pm_flags; //0xC
	long long weapFlags;//0x10
	int otherFlags;//0x18
	int pm_time;//0x1C
	short ownerNum;//0x20
	short name;//0x22
	char progress;//0x24
	vec3_t origin;//0x28
	vec3_t velocity;//0x34
	int remoteEyesEnt;//0x40
	int remoteEyesTagname;//0x44
	int remoteControlEnt;//0x48
	int weaponTime;//0x4C
	int weaponDelay;//0x50
	int weaponTimeLeft;//0x54
	int weaponDelayLeft;//0x58
	int weaponIdleTime;//0x5C
	int grenadeTimeLeft;//0x60
	int throwBackGrenadeOwner;//0x64
	int throwBackGrenadeTimeLeft;//0x68
	int weaponRestrictKickTime;//0x6C
	bool mountAvailable;//0x70
	bool bRunLeftGun;//0x71
	bool bCarryingTurret;//0x72
	float mountPos[3];//0x74
	float mountDir;//0x80
	bool bThirdPerson;//0x84
	int foliageSoundTime;//0x88
	int gravity;//0x8C
	float leanf;//0x90
	int speed;//0x94
	float delta_angles[3];//0x98
	int groundEntityNum;//0xA4
	int moverEntityNum;//0xA8
	int moverTimestamp;//0xAC
	int groundType;//0xB0
	float vLadderVec[3];//0xB4
	int jumpTime;//0xC0
	float jumpOriginZ;//0xC4
	int slideTime;//0xC8
	int moveType;//0xCC
	int legsTimer;//0xD0
	int torsoTimer;//0xD4
	short legsAnim;//0xD8
	short torsoAnim;//0xDA
	int legsAnimDuration;//0xDC
	int torsoAnimDuration;//0xE0
	int damageTimer;//0xE4
	int damageDuration;//0xE8
	int dmgDirection;//0xEC
	int dmgType;//0xF0
	int corpseIndex;//0xF4
	int movementDir;//0xF8
	int eFlags;//0xFC
	int eFlags2;//0x100
	PlayerVehicleState vehicleState;//0x104
	short predictableEventSequence;//0x158
	short predictableEventSequenceOld;//0x15A
	int predictableEvents[4];//0x15C
	int predictableEventParms[4];//0x16C
	short unpredictableEventSequence; //0x17C
	short oldUnpredictableEventSequence;//0x17E
	int unpredictableEvents[4];//0x180
	int unpredictableEventParms[4];//0x190
	int clientNum;//0x1A0
	int offHandWeapon;//0x1A4
	int offhandSecondary;//0x1A8
	int offhandPrimary;//0x1AC
	int renderOptions;//0x1B0
	int momentum;//0x1B4
	int weapon;//0x1B8
	unsigned int lastStandPrevWeapon;//0x1BC
	int lastWeaponAltModeSwitch;//0x1C0
	int stowedWeapon;//0x1C4
	char unusedCompatibilityPadding;//0x1C8
	unsigned int meleeWeapon;//0x1CC
	int weaponstate;//0x1D0
	int weaponstateLeft;//0x1D4
	unsigned int weaponShotCount;//0x1D8
	unsigned int weaponShotCountLeft;//0x1DC
	float fWeaponPosFrac;//0x1E0
	int adsDelayTime;//0x1E4
	int spreadOverride;//0x1E8
	int spreadOverrideState;//0x1EC
	int weaponSpinLerp;//0x1F0
	int viewmodelIndex;//0x1F4
	float viewangles[3];//0x1F8
	int viewHeightTarget;//0x204
	float viewHeightCurrent;//0x208
	int viewHeightLerpTime;//0x20C
	int viewHeightLerpTarget;//0x210
	int viewHeightLerpDown;//0x214
	float viewAngleClampBase[2];//0x218
	float viewAngleClampRange[2];//0x220
	int damageEvent;//0x228
	int damageYaw;//0x22C
	int damagePitch;//0x230
	int damageCount;//0x234
	int stats[4];//0x238
	struct
	{
		int weapon;//0x0
		int renderOptions;//0x4
		float heatPercent;//0x8
		int fuelTankTime;//0xC
		int adsZoomSelect;//0x10
		bool overHeating;//0x14
		bool needsRechamber;//0x15
		bool heldBefore;//0x16
		bool quickReload;//0x17
		bool blockWeaponPickup;//0x18
		char model;//0x19
	} heldWeapon[0xF];//0x248
	int ammo[0xF];//0x3EC
	int ammoClip[0xF];//0x428
	float proneDirection;//0x464
	float proneDirectionPitch;//0x468
	float proneTorsoPitch;//0x46C
	int viewlocked;//0x470
	short viewlocked_entNum;//0x474
	int vehiclePos;//0x478
	int vehicleType;//0x47C
	int vehicleAnimBoneIndex;//0x480
	int linkFlags;//0x484
	float linkAngles[3];//0x488
	int cursorHint;//0x494
	int cursorHintString;//0x498
	int cursorHintEntIndex;//0x49C
	int cursorHintWeapon;//0x4A0
	int iCompassPlayerInfo;//0x4A4
	int spyplaneTypeEnabled;//0x4A8
	int satelliteTypeEnabled;//0x4AC
	int locationSelectionInfo;//0x4B0
	int locationSelectionType;//0x4B4
	SprintState sprintState; //0x4B8
	float lastDtpEnd;//0x4D4
	float fTorsoPitch;//0x4D8
	float fWaistPitch;//0x4DC
	float holdBreathScale;//0x4E0
	int holdBreathTimer;//0x4E4
	int chargeShotTimer;//0x4E8
	int chargeShotLevel;//0x4EC
	int shotsFiredFromChamber;//0x4F0
	float quickScopeScale;//0x4F4
	int quickScopeTimer;//0x4F8
	int clientFields;//0x4FC
	int clientFields2;//0x500
	int clientFields3;//0x504
	int entityStateClientFields;//0x508
	float moveSpeedScaleMultiplier;//0x50C
	MantleState mantleState;//0x510
	int vehicleAnimStage;//0x520
	int vehicleEntryPoint;//0x524
	unsigned int scriptedAnim;//0x528
	int scriptedAnimTime;//0x52C
	int meleeChargeEnt;//0x530
	int meleeChargeDist;//0x534
	int meleeChargeTime;//0x538
	int weapLockFlags;//0x53C
	int weapLockedEntnum;//0x540
	unsigned int airburstMarkDistance;//0x544
	unsigned int perks[2];//0x548
	int actionSlotType[4];//0x550
	int actionSlotParam[4];//0x560
	int inventoryWeapon;//0x570
	short wiiumoteAimX;//0x574
	short wiiumoteAimY;//0x576
	char wiiuControllerType;//0x578
	char vehicleDefIndex;//0x579
	int entityEventSequence;//0x57C
	int weapAnim;//0x580
	int weapAnimLeft;//0x584
	float aimSpreadScale;//0x588
	int shellshockIndex;//0x58C
	int shellshockTime;//0x590
	int shellshockDuration;//0x594
	float dofNearStart;//0x598
	float dofNearEnd;//0x59C
	float dofFarStart;//0x5A0
	float dofFarEnd;//0x5A4
	float dofNearBlur;//0x5A8
	float dofFarBlur;//0x5AC
	float dofViewmodelStart;//0x5B0
	float dofViewmodelEnd;//0x5B4
	int waterlevel; //0x5B8
	int smokeColorIndex; //0x5BC
	int hudElemLastAssignedSoundID; //0x5C0
	int adsZoomSelect;//0x5C4
	int adsZoomLatchTime;//0x5C8
	bool adsZoomLatchState;//0x5CC
	int adsPrevZoomSelect;//0x5D0
	int adsPrevZoomSelectTime;//0x5D4
	int artilleryInboundIconLocation;//0x5D8
	float visionSetLerpRatio;//0x5DC
	int poisoned;//0x5E0
	int binocs;//0x5E4
	int scriptCursorHintString; //0x5E8
	objective_t objectives[32]; //0x5EC
	int deltaTime; //0xBEC
	int killCamEntity; //0xBF0
	int killCamTargetEntity; //0xBF4
	int introShotsFired; //0xBF8
	struct
	{
		char current[0xF04]; //0xBFC
		char archival[0xF04]; //0x1B00
	}hud;
	char unk_2A04[0xC]; //0x2A04
};

struct usercmd_s
{
	int serverTime; //0x0
	bitarray<51> buttons; //0x4
	int viewangles[3]; //0xC
	int weapon; // 0x18
	int offHandIndex; //0x1C
	int lastWeaponAltModeSwitch; //0x20
	signed char forwardmove; //0x24
	signed char rightmove; //0x25
	signed char upmove; //0x26
	signed char pitchmove; //0x27
	signed char yawmove; //0x28
	signed char rollmove; //0x29
		char pad[0x12]; //0x2A
};

//===================================================================

struct trajectory_t
{
	char trType; //0x0
	int trTime; //0x4
	int trDuration; //0x8
	vec3_t trBase; //0xC
	vec3_t trDelta; //0x18
};

struct LerpEntityStatePlayer
{
	float leanf; //0x0
	int movementDir; //0x4
	int moveType; //0x8
	int primaryWeapon; //0xC
	int stowedWeapon; //0x10
	int offhandWeapon; //0x14
	int meleeWeapon; //0x18
	char meleeWeaponCamo; //0x1C
	char meleeWeaponModel; //0x1D
	char vehicleType; //0x1E
	char vehicleAnimBoneIndex; //0x1F
	char vehicleSeat; //0x20
	char stowedWeaponCamo; //0x21
	char weaponHeat; //0x22
	char proneLegsAngle; //0x23
};

struct LerpEntityStateMissile
{
	int launchTime; //0x0
	int parentClientNum; //0x4
	int fuseTime; //0x8
	int forcedDud; //0xC
	int autoDetonateTime; //0x10
};

struct LerpEntityStateAnonymous
{
	int data[8]; //0x0
};

struct LerpEntityStateLoopFx
{
	float cullDist; //0x0
	int period; //0x4
};

struct LerpEntityStateTurret
{
	float gunAngles[3]; //0x0
	int ownerNum; //0xC
	int heatVal; //0x10
	int overheating; //0x14
	int pivotOffset; //0x18
	int flags; //0x1C
};

struct LerpEntityStateVehicle
{
	float steerYaw; //0x0
	float bodyRoll; //0x4
	struct
	{
		short pitch;
		short yaw;
	}gunnerAngles[4]; //0x8
	short throttle; //0x18
	short gunPitch; //0x1A
	short gunYaw; //0x1C
	char targetRotorSpeed; //0x1E
};

struct LerpEntityStateActor
{
	struct
	{
		int actornum;
	}index; //0x0
	int species; //0x4
	int team; //0x8
	int enemy; //0xC
	int freeCameraLockOnAllowed; //0x10
	struct
	{
		short fBodyPitch;
	}proneInfo;//0x14
	short aiType; //0x16
};

struct LerpEntityStateScriptMover
{
	char attachTagIndex[4]; //0x0
	int attachedTagIndex; //0x4
	short attachModelIndex[4]; //0x8
	short animScriptedAnim; //0x10
	short attachedEntNum; //0x12
	int attachedEntTime; //0x14
	short exploderIndex; //0x18
		char unk0[2]; //0x1A
	char flags; //0x1C
		char unk1[1]; //0x1D
	short aiType; //0x1E
};

struct LerpEntityStateZBarrier
{
	int barrierTypeIndex; //0x0
	struct
	{
		char flags;
		char animTime;
	}pieces[6]; //0x4
};

union LerpEntityStateTypeUnion
{
	LerpEntityStatePlayer player;
	LerpEntityStateMissile missile;
	LerpEntityStateAnonymous anonymous;
	LerpEntityStateLoopFx loopFx;
	LerpEntityStateTurret turret;
	LerpEntityStateVehicle vehicle;
	LerpEntityStateActor actor;
	LerpEntityStateScriptMover scriptMover;
	LerpEntityStateZBarrier zbarrier;
};

struct LerpEntityState
{
	int eFlags; //0x0
	int eFlags2; //0x4
	trajectory_t pos;//0x8
	trajectory_t apos;//0x2C
	LerpEntityStateTypeUnion u; //0x50
	short useCount; //0x74
	union
	{
		char iHeadIconTeam;
		short teamAndOwnerIndex;
	}faction; //0x76
	unsigned int clientFields; //0x78
};

struct entityState_s
{
	int number; //0x0
	LerpEntityState lerp; //0x4
	int time2; //0x80
	int loopSoundId; //0x84
	int solid; //0x88
	int renderOptions; //0x8C
	union
	{
		struct
		{
			int legsAnim; //0x90
			int torsoAnim; //0x94
			float fTorsoPitch; //0x98
			float fWaistPitch; //0x9C
		}anim;
		struct
		{
			char fov; //0x90
			char treeId; //0x91
			char animId; //0x92
		}moverState;
		struct
		{
			short flags; //0x90
			char vehicleDefIndex; //0x92
			char treeId; //0x93
			short animId; //0x94
			short attachModelIndex[2]; //0x96
			char attachTagIndex[2]; //0x9A
		}vehicleState;
		struct
		{
			short state; //0x90
			char subState; //0x92
			float fLeanAmount; //0x94
			float fAimUpDown; //0x98
			float fAimLeftRight; //0x9C
		}animState;
	}un2; //0x90
	union
	{
		char unk0[4]; //0xA0
	}un3; //0xA0
	int partBits[5]; //0xA4
	int clientLinkInfo; //0xB8
		char unk1[4]; //0xBC
	char events[4]; //0xC0
	int eventParms[4]; //0xC4
	unsigned int eventParm; //0xD4
	short eType; //0xD8
	short groundEntityNum; //0xDA
	union
	{
		char unk2[2]; //0xDC
	}index; //0xDC
	short otherEntityNum; //0xDE
	short attackerEntityNum; //0xE0
	short enemyModel; //0xE2
	int weapon; //0xE4
	int lastStandPrevWeapon; //0xE8
	unsigned short targetname; //0xEC
	short loopSoundFade; //0xEE
	short eventSequence; //0xF0
	char surfType; //0xF2
	char clientNum; //0xF3
	char iHeadIcon; //0xF4
	char weaponModel; //0xF5
	union
	{
		char hintString; //0xF6
		char helicopterStage; //0xF6
		char eventParm2; //0xF6
	}un1; // 0xF6
};

//===================================================================

struct va_info_t
{
	char va_string[4][0x400];
	int index;
};

#endif /* __Q_SHARED_H__ */
