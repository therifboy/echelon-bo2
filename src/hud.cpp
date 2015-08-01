#include "hud.h"
#include "game_mp/g_local_mp.h"

#include <string.h>

inline game_hudelem_s* G_HudElems(int index)
{
	return &g_hudelems[index];
}

game_hudelem_s* HudElem_Alloc(int client, he_type_t type)
{
	game_hudelem_s* hud;
	if (client == 0x3FF)
	{
		for (int index = 1008; index < 1024; index++)
		{
			if ((hud = G_HudElems(index))->elem.type == HE_TYPE_FREE)
			{
				hud->elem.type = type;
				hud->elem.targetEntNum = hud->clientNum = client;
				hud->archived = 1;
				hud->elem.ui3dWindow = -1;
				return hud;
			}
		}
	}
	else
	{
		for (int index = client * 84; index < ((client + 1) * 84); index++)
		{
			if ((hud = G_HudElems(index))->elem.type == HE_TYPE_FREE)
			{
				hud->elem.type = type;
				hud->elem.targetEntNum = hud->clientNum = client;
				hud->archived = 1;
				hud->elem.ui3dWindow = -1;
				return hud;
			}
		}
	}
	ClearAll(client);
	return HudElem_Alloc(client, type);
}

game_hudelem_s* SetShader(int client, const char* material, hudelem_color_t color, short width, short height, float x, float y)
{
	game_hudelem_s* elem = HudElem_Alloc(client, HE_TYPE_MATERIAL);
	elem->elem.materialIndex = G_MaterialIndex(material);
	elem->elem.color = color;
	elem->elem.width = width;
	elem->elem.height = height;
	elem->elem.x = x;
	elem->elem.y = y;
	return elem;
}

game_hudelem_s* SetShader(int client, int material, hudelem_color_t color, short width, short height, float x, float y)
{
	game_hudelem_s* elem = HudElem_Alloc(client, HE_TYPE_MATERIAL);
	elem->elem.materialIndex = material;
	elem->elem.color = color;
	elem->elem.width = width;
	elem->elem.height = height;
	elem->elem.x = x;
	elem->elem.y = y;
	return elem;
}

game_hudelem_s* SetText(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, float x, float y)
{
	game_hudelem_s* elem = HudElem_Alloc(client, HE_TYPE_TEXT);
	elem->elem.text = G_LocalizedStringIndex(text);
	elem->elem.color = color;
	elem->elem.glowColor = glowColor;
	elem->elem.fontScale = fontScale;
	elem->elem.font = font;
	elem->elem.x = x;
	elem->elem.y = y;
	return elem;
}

game_hudelem_s* SetText(int client, int text, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, float x, float y)
{
	game_hudelem_s* elem = HudElem_Alloc(client, HE_TYPE_TEXT);
	elem->elem.text = text;
	elem->elem.color = color;
	elem->elem.glowColor = glowColor;
	elem->elem.fontScale = fontScale;
	elem->elem.font = font;
	elem->elem.x = x;
	elem->elem.y = y;
	return elem;
}

void ClearAll(int client)
{
	if (client == 0x3FF)
    {
        for (int index = 1008; index < 1024; index++)
        {
			memset((void*)G_HudElems(index), 0, sizeof(game_hudelem_s));
        }
    }
    else
    {
        for (int index = client * 84; index < ((client + 1) * 84); index++)
        {
			memset((void*)G_HudElems(index), 0, sizeof(game_hudelem_s));
        }
    }
}

game_hudelem_s* SetFx(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, short letterTime, short decayStartTime, short decayDuration, int levelTime, int flags)
{
	game_hudelem_s* elem = SetText(client, text, color, glowColor, font, fontScale, 180 - strlen(text), 125);
	elem->elem.fxBirthTime = (levelTime == -1 ? level->time : levelTime);
	elem->elem.fxLetterTime = letterTime;
	elem->elem.fxDecayStartTime = decayStartTime;
	elem->elem.fxDecayDuration = decayDuration;
	elem->elem.flags |= flags;
	return elem;
}

game_hudelem_s* SetWayPoint(const char* material, hudelem_color_t color, int team, float x, float y, float z)
{
	game_hudelem_s* elem = HudElem_Alloc(0x3FF, HE_TYPE_WAYPOINT);
	elem->elem.offscreenMaterialIdx = G_MaterialIndex(material);
	elem->elem.x = x;
	elem->elem.y = y;
	elem->elem.z = z;
	elem->team = team;
	elem->elem.color = color;
	return elem;
}

game_hudelem_s* SetWayPoint(int material, hudelem_color_t color, int team, float x, float y, float z)
{
	game_hudelem_s* elem = HudElem_Alloc(0x3FF, HE_TYPE_WAYPOINT);
	elem->elem.offscreenMaterialIdx = material;
	elem->elem.x = x;
	elem->elem.y = y;
	elem->elem.z = z;
	elem->team = team;
	elem->elem.color = color;
	return elem;
}

game_hudelem_s* SetValue(int client, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, float value, float x, float y)
{
	game_hudelem_s* elem = HudElem_Alloc(client, HE_TYPE_VALUE);
	elem->elem.color = color;
    elem->elem.glowColor = glowColor;
    elem->elem.value = value;
    elem->elem.font = font;
    elem->elem.fontScale = fontScale;
    elem->elem.x = x;
	elem->elem.y = y;
    return elem;
}

game_hudelem_s* SetPlayerName(int client, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, int playerIndex, float x, float y)
{
	game_hudelem_s* elem = HudElem_Alloc(client, HE_TYPE_PLAYERNAME);
	elem->elem.color = color;
	elem->elem.glowColor = glowColor;
	elem->elem.value = playerIndex;
	elem->elem.font = font;
	elem->elem.fontScale = fontScale;
	elem->elem.x = x;
	elem->elem.y = y;
	return elem;
}

game_hudelem_s* SetTypeWriter(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, short letterTime, short decayStartTime, short decayDuration, int levelTime)
{
	game_hudelem_s* elem = SetFx(client, text, color, glowColor, font, fontScale, letterTime, decayStartTime, decayDuration, levelTime, HUDELEMFLAG_TYPEWRITER);
	return elem;
}

game_hudelem_s* SetDecodeFx(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, he_font_t font, float fontScale, short letterTime, short decayStartTime, short decayDuration, int levelTime)
{
	game_hudelem_s* elem = SetFx(client, text, color, glowColor, font, fontScale, letterTime, decayStartTime, decayDuration, levelTime, HUDELEMFLAG_COD7DECODE);
	return elem;
}

void FontScaleOverTime(game_hudelem_s* elem, short fontScaleDuration, float fontScale, int levelTime)
{
	elem->elem.fromFontScale = elem->elem.fontScale;
	elem->elem.fontScale = fontScale;
	elem->elem.fontScaleTime = fontScaleDuration;
	elem->elem.fontScaleStartTime = (levelTime == -1 ? level->time : levelTime);
	elem->elem.fromAlignOrg = elem->elem.alignOrg;
	elem->elem.fromAlignScreen = elem->elem.alignScreen;
}

void ScaleOverTime(game_hudelem_s* elem, short scaleDuration, short width, short height, int levelTime)
{
	elem->elem.fromWidth = elem->elem.width;
	elem->elem.fromHeight = elem->elem.height;
	elem->elem.width = width;
	elem->elem.height = height;
	elem->elem.scaleTime = scaleDuration;
	elem->elem.scaleStartTime = (levelTime == -1 ? level->time : levelTime);
	elem->elem.fromAlignOrg = elem->elem.alignOrg;
	elem->elem.fromAlignScreen = elem->elem.alignScreen;
}

void MoveOverTime(game_hudelem_s* elem, short moveDuration, float x, float y, int levelTime)
{
	elem->elem.fromX = elem->elem.x;
	elem->elem.fromY = elem->elem.y;
	elem->elem.x = x;
	elem->elem.y = y;
	elem->elem.moveTime = moveDuration;
	elem->elem.fromAlignScreen = elem->elem.alignScreen;
	elem->elem.fromAlignOrg = elem->elem.fromAlignOrg;
	elem->elem.moveStartTime = (levelTime == -1 ? level->time : levelTime);
	elem->elem.fromAlignOrg = elem->elem.alignOrg;
	elem->elem.fromAlignScreen = elem->elem.alignScreen;
}

void FadeOverTime(game_hudelem_s* elem, short fadeDuration, hudelem_color_t color, int levelTime)
{
	elem->elem.fromColor = elem->elem.color;
	elem->elem.color = color;
	elem->elem.fadeTime = fadeDuration;
	elem->elem.fadeStartTime = (levelTime == -1 ? level->time : levelTime);
}

void NullFx(game_hudelem_s* elem)
{
	elem->elem.fadeStartTime =
	elem->elem.fadeTime =
	elem->elem.fontScaleStartTime =
	elem->elem.fontScaleTime =
	elem->elem.fxBirthTime =
	elem->elem.fxDecayDuration =
	elem->elem.fxDecayStartTime =
	elem->elem.fxLetterTime =
	elem->elem.fxRedactDecayDuration =
	elem->elem.fxRedactDecayStartTime =
	elem->elem.moveStartTime =
	elem->elem.moveTime =
	elem->elem.scaleStartTime =
	elem->elem.scaleTime =
	elem->elem.time =
	elem->elem.duration = 0;
}

void ClearHud(game_hudelem_s* elem)
{
	memset(elem, 0, sizeof(game_hudelem_s));
}

void FadeWhenTargeted(game_hudelem_s* elem, bool value)
{
	if (value)
		elem->elem.flags |= HUDELEMFLAG_FADEWHENTARGETED;
	else
		elem->elem.flags &= ~HUDELEMFLAG_FADEWHENTARGETED;
}

void HideWhenDead(game_hudelem_s* elem, bool value)
{
	if (value)
		elem->elem.flags |= HUDELEMFLAG_HIDEWHENDEAD;
	else
		elem->elem.flags &= ~HUDELEMFLAG_HIDEWHENDEAD;
}

void HideWhenInKillcam(game_hudelem_s* elem, bool value)
{
	if (value)
		elem->elem.flags |= HUDELEMFLAG_HIDEWHENINKILLCAM;
	else
		elem->elem.flags &= ~HUDELEMFLAG_HIDEWHENINKILLCAM;
}

void HideWhenInDemo(game_hudelem_s* elem, bool value)
{
	if (value)
		elem->elem.flags |= HUDELEMFLAG_HIDEWHENINDEMO;
	else
		elem->elem.flags &= ~HUDELEMFLAG_HIDEWHENINDEMO;
}

void HideWhenInMenu(game_hudelem_s* elem, bool value)
{
	if (value)
		elem->elem.flags |= HUDELEMFLAG_HIDEWHENINMENU;
	else
		elem->elem.flags &= ~HUDELEMFLAG_HIDEWHENINMENU;
}

void HideWhenInScope(game_hudelem_s* elem, bool value)
{
	if (value)
		elem->elem.flags |= HUDELEMFLAG_HIDEWHENINSCOPE;
	else
		elem->elem.flags &= ~HUDELEMFLAG_HIDEWHENINSCOPE;
}

void HideWhileRemoteControlling(game_hudelem_s* elem, bool value)
{
	if (value)
		elem->elem.flags |= HUDELEMFLAG_HIDEWHILEREMOTECONTROLING;
	else
		elem->elem.flags &= ~HUDELEMFLAG_HIDEWHILEREMOTECONTROLING;
}

void ForeGround(game_hudelem_s* elem, bool value)
{
	if (value)
		elem->elem.flags |= HUDELEMFLAG_FOREGROUND;
	else
		elem->elem.flags &= ~HUDELEMFLAG_FOREGROUND;
}

void Font(game_hudelem_s* elem, he_font_t font)
{
	elem->elem.font = font;
}

void AlignX(game_hudelem_s* elem, alignX_t align)
{
	elem->elem.alignOrg &= ~0xC;
	elem->elem.alignOrg |= (align << 2);
}

void AlignY(game_hudelem_s* elem, alignY_t align)
{
	elem->elem.alignOrg &= ~0x3;
	elem->elem.alignOrg |= (align);
}

void HorzAlign(game_hudelem_s* elem, horzAlign_t align)
{
	elem->elem.alignScreen &= 0x0F;
	elem->elem.alignScreen |= (align << 4);
}

void VertAlign(game_hudelem_s* elem, vertAlign_t align)
{
	elem->elem.alignScreen &= 0xF0;
	elem->elem.alignScreen |= (align);
}

bool FadeWhenTargeted(game_hudelem_s* elem)
{
	return (elem->elem.flags & HUDELEMFLAG_FADEWHENTARGETED);
}

bool HideWhenDead(game_hudelem_s* elem)
{
	return (elem->elem.flags & HUDELEMFLAG_HIDEWHENDEAD);
}

bool HideWhenInKillcam(game_hudelem_s* elem)
{
	return (elem->elem.flags & HUDELEMFLAG_HIDEWHENINKILLCAM);
}

bool HideWhenInDemo(game_hudelem_s* elem)
{
	return (elem->elem.flags & HUDELEMFLAG_HIDEWHENINDEMO);
}

bool HideWhenInMenu(game_hudelem_s* elem)
{
	return (elem->elem.flags & HUDELEMFLAG_HIDEWHENINMENU);
}

bool HideWhenInScope(game_hudelem_s* elem)
{
	return (elem->elem.flags & HUDELEMFLAG_HIDEWHENINSCOPE);
}

bool HideWhileRemoteControlling(game_hudelem_s* elem)
{
	return (elem->elem.flags & HUDELEMFLAG_HIDEWHILEREMOTECONTROLING);
}

bool ForeGround(game_hudelem_s* elem)
{
	return (elem->elem.flags & HUDELEMFLAG_FOREGROUND);
}
