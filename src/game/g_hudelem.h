#ifndef __G_HUDELEM_H__
#define __G_HUDELEM_H__

enum he_type_t
{
	FREE = 0,
	TEXT = 1,
	VALUE = 2,
	PLAYER_NAME = 3,
	MAP_NAME = 4,
	GAMETYPE = 5,
	WAR_GAME_DATA = 6,
	VALUE2 = 7,
	SHADER = 8,
	TIMER = 9,
	TIMER_UP = 10,
	TENTHS_TIMER = 11,
	TENTHS_TIMER_UP = 12,
	CLOCK = 13,
	CLOCK_UP = 14,
	WAY_POINT = 15
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
	short flag2;//0x7A 
};

struct game_hudelem_s
{
	hudelem_s elem;//0x0
	int clientNum;//0x7C
	int team;//0x80
	int archived;//0x84
};

#endif /* __G_HUDELEM_H__ */
