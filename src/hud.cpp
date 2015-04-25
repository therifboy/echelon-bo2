#include "hud.h"
#include "game/g_local.h"
#include "game_mp/g_local_mp.h"

#include <string.h>

inline hudElement* hudElement::G_HudElems(int index)
{
	return (hudElement*)(&g_hudelems[index]);
}

hudElement* hudElement::HudElem_Alloc(int client, he_type_t type)
{
	hudElement* hud;
	if (client == 0x3FF)
	{
		for (int index = 1008; index < 1024; index++)
		{
			if ((hud = G_HudElems(index))->elem.type == FREE)
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
			if ((hud = G_HudElems(index))->elem.type == FREE)
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

hudElement* hudElement::SetShader(int client, const char* material, hudelem_color_t color, short width, short height, float x, float y)
{
	hudElement* elem = HudElem_Alloc(client, SHADER);
	elem->elem.materialIndex = G_MaterialIndex(material);
	elem->elem.color = color;
	elem->elem.width = width;
	elem->elem.height = height;
	elem->elem.x = x;
	elem->elem.y = y;
	return elem;
}

hudElement* hudElement::SetShader(int client, int material, hudelem_color_t color, short width, short height, float x, float y)
{
	hudElement* elem = HudElem_Alloc(client, SHADER);
	elem->elem.materialIndex = material;
	elem->elem.color = color;
	elem->elem.width = width;
	elem->elem.height = height;
	elem->elem.x = x;
	elem->elem.y = y;
	return elem;
}

hudElement* hudElement::SetText(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, float x, float y)
{
	hudElement* elem = HudElem_Alloc(client, TEXT);
	elem->elem.text = G_LocalizedStringIndex(text);
	elem->elem.color = color;
	elem->elem.glowColor = glowColor;
	elem->elem.fontScale = fontScale;
	elem->elem.font = font;
	elem->elem.x = x;
	elem->elem.y = y;
	return elem;
}

hudElement* hudElement::SetText(int client, int text, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, float x, float y)
{
	hudElement* elem = HudElem_Alloc(client, TEXT);
	elem->elem.text = text;
	elem->elem.color = color;
	elem->elem.glowColor = glowColor;
	elem->elem.fontScale = fontScale;
	elem->elem.font = font;
	elem->elem.x = x;
	elem->elem.y = y;
	return elem;
}

void hudElement::ClearAll(int client)
{
	if (client == 0x3FF)
    {
        for (int index = 1008; index < 1024; index++)
        {
			memset((void*)G_HudElems(index), 0, sizeof(hudElement));
        }
    }
    else
    {
        for (int index = client * 84; index < ((client + 1) * 84); index++)
        {
			memset((void*)G_HudElems(index), 0, sizeof(hudElement));
        }
    }
}

hudElement* hudElement::SetFx(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, short letterTime, short decayStartTime, short decayDuration, int levelTime, int flags)
{
	hudElement* elem = SetText(client, text, color, glowColor, font, fontScale, 180 - strlen(text), 125);
	elem->elem.fxBirthTime = (levelTime == -1 ? level->time : levelTime);
	elem->elem.fxLetterTime = letterTime;
	elem->elem.fxDecayStartTime = decayStartTime;
	elem->elem.fxDecayDuration = decayDuration;
	elem->elem.flags |= flags;
	return elem;
}

hudElement* hudElement::SetWayPoint(const char* material, hudelem_color_t color, int team, float x, float y, float z)
{
	hudElement* elem = HudElem_Alloc(0x3FF, WAY_POINT);
	elem->elem.offscreenMaterialIdx = G_MaterialIndex(material);
	elem->elem.x = x;
	elem->elem.y = y;
	elem->elem.z = z;
	elem->team = team;
	elem->elem.color = color;
	return elem;
}

hudElement* hudElement::SetWayPoint(int material, hudelem_color_t color, int team, float x, float y, float z)
{
	hudElement* elem = HudElem_Alloc(0x3FF, WAY_POINT);
	elem->elem.offscreenMaterialIdx = material;
	elem->elem.x = x;
	elem->elem.y = y;
	elem->elem.z = z;
	elem->team = team;
	elem->elem.color = color;
	return elem;
}

hudElement* hudElement::SetValue(int client, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, float value, float x, float y)
{
	hudElement* elem = HudElem_Alloc(client, VALUE);
	elem->elem.color = color;
    elem->elem.glowColor = glowColor;
    elem->elem.value = value;
    elem->elem.font = font;
    elem->elem.fontScale = fontScale;
    elem->elem.x = x;
	elem->elem.y = y;
    return elem;
}

hudElement* hudElement::SetPlayerName(int client, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, int playerIndex, float x, float y)
{
	hudElement* elem = HudElem_Alloc(client, PLAYER_NAME);
	elem->elem.color = color;
	elem->elem.glowColor = glowColor;
	elem->elem.value = playerIndex;
	elem->elem.font = font;
	elem->elem.fontScale = fontScale;
	elem->elem.x = x;
	elem->elem.y = y;
	return elem;
}

hudElement* hudElement::SetTypeWriter(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, short letterTime, short decayStartTime, short decayDuration, int levelTime)
{
	hudElement* elem = SetFx(client, text, color, glowColor, font, fontScale, letterTime, decayStartTime, decayDuration, levelTime, 0x4000);
	return elem;
}

hudElement* hudElement::SetDecodeFx(int client, const char* text, hudelem_color_t color, hudelem_color_t glowColor, font_t font, float fontScale, short letterTime, short decayStartTime, short decayDuration, int levelTime)
{
	hudElement* elem = SetFx(client, text, color, glowColor, font, fontScale, letterTime, decayStartTime, decayDuration, levelTime, 0x4000);
	return elem;
}

void hudElement::FontScaleOverTime(short fontScaleDuration, float fontScale, int levelTime)
{
	this->elem.fromFontScale = this->elem.fontScale;
	this->elem.fontScale = fontScale;
	this->elem.fontScaleTime = fontScaleDuration;
	this->elem.fontScaleStartTime = (levelTime == -1 ? level->time : levelTime);
	this->elem.fromAlignOrg = this->elem.alignOrg;
	this->elem.fromAlignScreen = this->elem.alignScreen;
}

void hudElement::ScaleOverTime(short scaleDuration, short width, short height, int levelTime)
{
	this->elem.fromWidth = this->elem.width;
	this->elem.fromHeight = this->elem.height;
	this->elem.width = width;
	this->elem.height = height;
	this->elem.scaleTime = scaleDuration;
	this->elem.scaleStartTime = (levelTime == -1 ? level->time : levelTime);
	this->elem.fromAlignOrg = this->elem.alignOrg;
	this->elem.fromAlignScreen = this->elem.alignScreen;
}

void hudElement::MoveOverTime(short moveDuration, float x, float y, int levelTime)
{
	this->elem.fromX = this->elem.x;
	this->elem.fromY = this->elem.y;
	this->elem.x = x;
	this->elem.y = y;
	this->elem.moveTime = moveDuration;
	this->elem.fromAlignScreen = this->elem.alignScreen;
	this->elem.fromAlignOrg = this->elem.fromAlignOrg;
	this->elem.moveStartTime = (levelTime == -1 ? level->time : levelTime);
	this->elem.fromAlignOrg = this->elem.alignOrg;
	this->elem.fromAlignScreen = this->elem.alignScreen;
}

void hudElement::FadeOverTime(short fadeDuration, hudelem_color_t color, int levelTime)
{
	this->elem.fromColor = this->elem.color;
	this->elem.color = color;
	this->elem.fadeTime = fadeDuration;
	this->elem.fadeStartTime = (levelTime == -1 ? level->time : levelTime);
}

void hudElement::NullFx()
{
	this->elem.fadeStartTime = 0;
	this->elem.fadeTime = 0;
	this->elem.fontScaleStartTime = 0;
	this->elem.fontScaleTime = 0;
	this->elem.fxBirthTime = 0;
	this->elem.fxDecayDuration = 0;
	this->elem.fxDecayStartTime = 0;
	this->elem.fxLetterTime = 0;
	this->elem.fxRedactDecayDuration = 0;
	this->elem.fxRedactDecayStartTime = 0;
	this->elem.moveStartTime = 0;
	this->elem.moveTime = 0;
	this->elem.scaleStartTime = 0;
	this->elem.scaleTime = 0;
	this->elem.time = 0;
	this->elem.duration = 0;
}

void hudElement::Clear()
{
	memset(this, 0, sizeof(game_hudelem_s));
}

void hudElement::FadeWhenTargeted(bool value)
{
	if (value)
		elem.flags |= 0x100;
	else
		elem.flags &= ~0x100;
}

void hudElement::HideWhenDead(bool value)
{
	if (value)
		elem.flags |= 0x2;
	else
		elem.flags &= ~0x2;
}

void hudElement::HideWhenInKillcam(bool value)
{
	if (value)
		elem.flags |= 0x40;
	else
		elem.flags &= ~0x40;
}

void hudElement::HideWhenInDemo(bool value)
{
	if (value)
		elem.flags |= 0x200;
	else
		elem.flags &= ~0x200;
}

void hudElement::HideWhenInMenu(bool value)
{
	if (value)
		elem.flags |= 0x4;
	else
		elem.flags &= ~0x4;
}

void hudElement::HideWhenInScope(bool value)
{
	if (value)
		elem.flags |= 0x2000;
	else
		elem.flags &= ~0x2000;
}

void hudElement::HideWhileRemoteControlling(bool value)
{
	if (value)
		elem.flags |= 0x400;
	else
		elem.flags &= ~0x400;
}

void hudElement::ForeGround(bool value)
{
	if (value)
		elem.flags |= 0x1;
	else
		elem.flags &= ~0x1;
}

void hudElement::Font(font_t font)
{
	this->elem.font = font;
}

void hudElement::AlignX(alignX_t align)
{
	this->elem.alignOrg &= ~0xC;
	this->elem.alignOrg |= (align << 2);
}

void hudElement::AlignY(alignY_t align)
{
	this->elem.alignOrg &= ~0x3;
	this->elem.alignOrg |= (align);
}

void hudElement::HorzAlign(horzAlign_t align)
{
	this->elem.alignScreen &= 0x0F;
	this->elem.alignScreen |= (align << 4);
}

void hudElement::VertAlign(vertAlign_t align)
{
	this->elem.alignScreen &= 0xF0;
	this->elem.alignScreen |= (align);
}

bool hudElement::FadeWhenTargeted()
{
	return (elem.flags & 0x100);
}

bool hudElement::HideWhenDead()
{
	return (elem.flags & 0x2);
}

bool hudElement::HideWhenInKillcam()
{
	return (elem.flags & 0x40);
}

bool hudElement::HideWhenInDemo()
{
	return (elem.flags & 0x200);
}

bool hudElement::HideWhenInMenu()
{
	return (elem.flags & 0x4);
}

bool hudElement::HideWhenInScope()
{
	return (elem.flags & 0x2000);
}

bool hudElement::HideWhileRemoteControlling()
{
	return (elem.flags & 0x400);
}

bool hudElement::ForeGround()
{
	return (elem.flags & 0x1);
}
