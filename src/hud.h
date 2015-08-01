#ifndef __HUD_H__
#define __HUD_H__

#include "game/g_local.h"

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

//private:
game_hudelem_s* G_HudElems(int index);
game_hudelem_s* HudElem_Alloc(int client, he_type_t type);

//public:
game_hudelem_s* SetShader(int client, const char* material, hudelem_color_t color, short width, short height, float x, float y);
game_hudelem_s* SetShader(int client, int material, hudelem_color_t color, short width, short height, float x, float y);
game_hudelem_s* SetText(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, float x, float y);
game_hudelem_s* SetText(int client, int text, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, float x, float y);
void ClearAll(int client);

//private:
game_hudelem_s* SetFx(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, short letterTime, short decayStartTime, short decayDuration, int levelTime, int flags);

//public:
game_hudelem_s* SetWayPoint(const char* material, hudelem_color_t color, int team, float x, float y, float z);
game_hudelem_s* SetWayPoint(int material, hudelem_color_t color, int team, float x, float y, float z);
game_hudelem_s* SetValue(int client, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, float value, float x, float y);
game_hudelem_s* SetPlayerName(int client, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, int playerIndex, float x, float y);
game_hudelem_s* SetTypeWriter(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, short letterTime = 150, short decayStartTime = 6000, short decayDuration = 1000, int levelTime = -1);
game_hudelem_s* SetDecodeFx(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, short letterTime = 150, short decayStartTime = 6000, short decayDuration = 1000, int levelTime = -1);

void FontScaleOverTime(game_hudelem_s* elem, short fontScaleDuration, float fontScale, int levelTime);
void ScaleOverTime(game_hudelem_s* elem, short scaleDuration, short width, short height, int levelTime = -1);
void MoveOverTime(game_hudelem_s* elem, short moveDuration, float x, float y, int levelTime = -1);
void FadeOverTime(game_hudelem_s* elem, short fadeDuration, hudelem_color_t color, int levelTime = -1);

void NullFx(game_hudelem_s* elem);
void ClearHud(game_hudelem_s* elem);

void FadeWhenTargeted(game_hudelem_s* elem, bool value);
void HideWhenDead(game_hudelem_s* elem, bool value);
void HideWhenInKillcam(game_hudelem_s* elem, bool value);
void HideWhenInDemo(game_hudelem_s* elem, bool value);
void HideWhenInMenu(game_hudelem_s* elem, bool value);
void HideWhenInScope(game_hudelem_s* elem, bool value);
void HideWhileRemoteControlling(game_hudelem_s* elem, bool value);
void ForeGround(game_hudelem_s* elem, bool value);
void Font(game_hudelem_s* elem, he_font_t font);
void AlignX(game_hudelem_s* elem, alignX_t align);
void AlignY(game_hudelem_s* elem, alignY_t align);
void HorzAlign(game_hudelem_s* elem, horzAlign_t align);
void VertAlign(game_hudelem_s* elem, vertAlign_t align);

bool FadeWhenTargeted(game_hudelem_s* elem);
bool HideWhenDead(game_hudelem_s* elem);
bool HideWhenInKillcam(game_hudelem_s* elem);
bool HideWhenInDemo(game_hudelem_s* elem);
bool HideWhenInMenu(game_hudelem_s* elem);
bool HideWhenInScope(game_hudelem_s* elem);
bool HideWhileRemoteControlling(game_hudelem_s* elem);
bool ForeGround(game_hudelem_s* elem);

#endif /* __HUD_H__ */
