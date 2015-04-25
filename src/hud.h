#ifndef __HUD_H__
#define __HUD_H__

#include "game/g_hudelem.h"

enum font_t
{
	FONT_DEFAULT = 0x0,
    FONT_BIGFIXED = 0x1,
    FONT_SMALLFIXED = 0x2,
    FONT_OBJECTIVE = 0x3,
    FONT_BIG = 0x4,
    FONT_SMALL = 0x5,
    FONT_EXTRABIG = 0x6,
    FONT_EXTRASMALL = 0x7
};

enum fontStyle3D_t
{
	FONTSTYLE_NORMAL = 0x0,
	FONTSTYLE_SHADOWED = 0x1,
	FONTSTYLE_SHADOWEDMORE = 0x2
};

enum alignX_t
{
	ALIGN_X_RIGHT = 0x0,
	ALIGN_X_CENTER = 0x1,
	ALIGN_X_LEFT = 0x2
};

enum alignY_t
{
	ALIGN_Y_BOTTOM = 0x0,
	ALIGN_Y_MIDDLE = 0x1,
	ALIGN_Y_TOP = 0x2
};

enum horzAlign_t
{
	HORZALIGN_SUBLEFT = 0x0,
	HORZALIGN_LEFT = 0x1,
	HORZALIGN_CENTER = 0x2,
	HORZALIGN_RIGHT = 0x3,
	HORZALIGN_FULLSCREEN = 0x4,
	HORZALIGN_NOSCALE = 0x5,
	HORZALIGN_ALIGN_TO_640 = 0x6,
	HORZALIGN_CENTER_SAFE_AREA = 0x7,
	HORZALIGN_USER_LEFT = 0x8,
	HORZALIGN_USER_CENTER = 0x9,
	HORZALIGN_USER_RIGHT = 0xA
};

enum vertAlign_t
{
	VERTALIGN_SUBTOP = 0x0,
	VERTALIGN_TOP = 0x1,
	VERTALIGN_MIDDLE = 0x2,
	VERTALIGN_BOTTOM = 0x3,
	VERTALIGN_FULLSCREEN = 0x4,
	VERTALIGN_NOSCALE = 0x5,
	VERTALIGN_ALIGN_TO_480 = 0x6,
	VERTALIGN_CENTER_SAFE_AREA = 0x7,
	VERTALIGN_USER_TOP = 0x8,
	VERTALIGN_USER_CENTER = 0x9,
	VERTALIGN_USER_BOTTOM = 0xA
};

extern game_hudelem_s * g_hudelems;

class hudElement : public game_hudelem_s
{
private:
	static hudElement* G_HudElems(int index);
	static hudElement* HudElem_Alloc(int client, he_type_t type);

public:
	static hudElement* SetShader(int client, const char* material, hudelem_color_t color, short width, short height, float x, float y);
	static hudElement* SetShader(int client, int material, hudelem_color_t color, short width, short height, float x, float y);
	static hudElement* SetText(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, float x, float y);
	static hudElement* SetText(int client, int text, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, float x, float y);
	static void ClearAll(int client);

private:
	static hudElement* SetFx(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, short letterTime, short decayStartTime, short decayDuration, int levelTime, int flags);

public:
	static hudElement* SetWayPoint(const char* material, hudelem_color_t color, int team, float x, float y, float z);
	static hudElement* SetWayPoint(int material, hudelem_color_t color, int team, float x, float y, float z);
	static hudElement* SetValue(int client, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, float value, float x, float y);
	static hudElement* SetPlayerName(int client, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, int playerIndex, float x, float y);
	static hudElement* SetTypeWriter(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, short letterTime = 150, short decayStartTime = 6000, short decayDuration = 1000, int levelTime = -1);
	static hudElement* SetDecodeFx(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, short letterTime = 150, short decayStartTime = 6000, short decayDuration = 1000, int levelTime = -1);

	void FontScaleOverTime(short fontScaleDuration, float fontScale, int levelTime);
	void ScaleOverTime(short scaleDuration, short width, short height, int levelTime = -1);
	void MoveOverTime(short moveDuration, float x, float y, int levelTime = -1);
	void FadeOverTime(short fadeDuration, hudelem_color_t color, int levelTime = -1);

	void NullFx();
	void Clear();

	void FadeWhenTargeted(bool value);
	void HideWhenDead(bool value);
	void HideWhenInKillcam(bool value);
	void HideWhenInDemo(bool value);
	void HideWhenInMenu(bool value);
	void HideWhenInScope(bool value);
	void HideWhileRemoteControlling(bool value);
	void ForeGround(bool value);
	void Font(font_t font);
	void AlignX(alignX_t align);
	void AlignY(alignY_t align);
	void HorzAlign(horzAlign_t align);
	void VertAlign(vertAlign_t align);

	bool FadeWhenTargeted();
	bool HideWhenDead();
	bool HideWhenInKillcam();
	bool HideWhenInDemo();
	bool HideWhenInMenu();
	bool HideWhenInScope();
	bool HideWhileRemoteControlling();
	bool ForeGround();
};

#endif /* __HUD_H__ */
