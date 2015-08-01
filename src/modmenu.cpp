#include "modmenu.h"
#include "functions.h"
#include "entity.h"
#include "handle.h"
#include "utils.h"
#include "handle_functions.h"
#include "echelon.h"

#include "stdio.h"
#include "client_mp/client_mp.h"
#include "server/server.h"

#include <sys/ppu_thread.h>
#include <string.h>
#include <ctype.h>

#define SCROLL_HEIGHT	(22.846153846153846153846153846154)

#define ToUpper(buffer, str, len)		for (size_t i = 0; i < len; i++)	\
										buffer[i] = toupper(str[i]);	\
										buffer[len] = '\0';

int32_t material_white;

const int16_t minWidth = 0x200, maxWidth = 0x3FF;
const int16_t minHeight = 0x120, maxHeight = 0x200;

title_s* menu_title;
menu_s* main_menu;

/*
scrollbar = menu_mp_lobby_scrollbar_main

*/

void menu_s::setup()
{
	menu_s* child;
	static char buffer[0x400];
	for (int16_t i = 0; i < this->stringCount; i++)
	{
		child = this->child.menu[i];
		const char* title = this->string[i];
		ToUpper(buffer, title, strlen(title));
		child->titleIndex = G_LocalizedStringIndex(buffer);
		stringJoin(buffer, sizeof(buffer), '\n', child->string, child->stringCount);
		child->stringIndex = G_LocalizedStringIndex(buffer);
		
		if (child->type == TYPE_MENU)
		{
			child->setup();
		}
	}
}

void modmenu_s::setClient(int8_t client)
{
	this->client = client;
}

void modmenu_s::enable()
{
	if (!(flags & __enabled))
	{
		flags &= ~__destroy;
		if (!this->thread)
		{
			this->safeHeight = minHeight;
			this->safeWidth = minWidth;
			int ret;
			sys_ppu_thread_t tid;
			if ((ret = sys_ppu_thread_create(&tid, (void(*)(uint64_t))(modmenu_s::run), (uint64_t)this, 1020, 0x2000, SYS_PPU_THREAD_CREATE_JOINABLE, "Echelon")) != CELL_OK)
			{
				DBGPRINTF("sys_ppu_thread_create() fail: %08x\n", ret);
				tid = 0;
			}
			else
			{
				flags |= __enabled;
			}
			this->thread = (uint32_t)tid;
		}
		else
		{
			flags |= __enabled;
		}
	}
}

void modmenu_s::disable()
{
	hide();
	flags &= ~__enabled;
	flags &= ~__stored;
	flags &= ~__open;
	flags &= ~__frozen;
}

void modmenu_s::destroy()
{
	flags |= __destroy;
}

void modmenu_s::verify(verif_t type)
{
	this->verification = type;
}

menu_s* modmenu_s::getMenu()
{
	return this->currentMenu;
}

void modmenu_s::freeze()
{
	flags |= __frozen;
	hide();
}

void modmenu_s::unfreeze()
{
	flags &= ~__frozen;
	show();
}

void modmenu_s::store()
{
	const int16_t client = this->client;
	//hudelem_s::ClearAll(client);
	background = SetShader(client, material_white, black_inv, 250, 500, 88, 0);
	VertAlign(background, VERTALIGN_MIDDLE);
	HorzAlign(background, HORZALIGN_RIGHT);
	AlignX(background, ALIGN_X_RIGHT);
	AlignY(background, ALIGN_Y_MIDDLE);
	edge = SetShader(client, material_white, black_inv, 2, 500, 88, 0);
	VertAlign(edge, VERTALIGN_MIDDLE);
	HorzAlign(edge, HORZALIGN_RIGHT);
	AlignX(edge, ALIGN_X_RIGHT);
	AlignY(edge, ALIGN_Y_MIDDLE);
	scrollbar = SetShader(client, material_white, black_inv, 248, 21, 90, -140);
	VertAlign(scrollbar, VERTALIGN_MIDDLE);
	HorzAlign(scrollbar, HORZALIGN_RIGHT);
	AlignX(scrollbar, ALIGN_X_RIGHT);
	AlignY(scrollbar, ALIGN_Y_MIDDLE);
	text = SetText(client, main_menu->stringIndex, black_inv, black_inv, HE_FONT_OBJECTIVE, 1.875f, 110, -140);
	VertAlign(text, VERTALIGN_MIDDLE);
	HorzAlign(text, HORZALIGN_RIGHT);
	AlignX(text, ALIGN_X_RIGHT);
	AlignY(text, ALIGN_Y_MIDDLE);
	title = SetText(client, main_menu->titleIndex, black_inv, black_inv, HE_FONT_OBJECTIVE, 2.5f, 110, -170);
	VertAlign(title, VERTALIGN_MIDDLE);
	HorzAlign(title, HORZALIGN_RIGHT);
	AlignX(title, ALIGN_X_RIGHT);
	AlignY(title, ALIGN_Y_MIDDLE);
	nameBackground = SetShader(client, material_white, black_inv, 60, 500, -60, 0);
	VertAlign(nameBackground, VERTALIGN_MIDDLE);
	HorzAlign(nameBackground, HORZALIGN_LEFT);
	AlignX(nameBackground, ALIGN_X_LEFT);
	AlignY(nameBackground, ALIGN_Y_MIDDLE);
	nameEdge = SetShader(client, material_white, black_inv, 2, 500, 0, 0);
	VertAlign(nameEdge, VERTALIGN_MIDDLE);
	HorzAlign(nameEdge, HORZALIGN_LEFT);
	AlignX(nameEdge, ALIGN_X_LEFT);
	AlignY(nameEdge, ALIGN_Y_MIDDLE);
	name = SetText(client, menu_title->titleIndex, black_inv, black_inv, HE_FONT_BIG, 4, -45, -128);
	VertAlign(name, VERTALIGN_MIDDLE);
	HorzAlign(name, HORZALIGN_LEFT);
	AlignX(name, ALIGN_X_LEFT);
	AlignY(name, ALIGN_Y_MIDDLE);
#pragma region
    ForeGround(background, true);
	ForeGround(edge, true);
	ForeGround(scrollbar, true);
	ForeGround(text, true);
	ForeGround(title, true);
	ForeGround(name, true);
	ForeGround(nameBackground, true);
	ForeGround(nameEdge, true);
#pragma endregion ForeGround
#pragma region
    HideWhenDead(background, true);
	HideWhenDead(edge, true);
	HideWhenDead(scrollbar, true);
	HideWhenDead(text, true);
	HideWhenDead(title, true);
	HideWhenDead(name, true);
	HideWhenDead(nameBackground, true);
	HideWhenDead(nameEdge, true);
#pragma endregion HideWhenDead
#pragma region
    HideWhenInMenu(background, true);
	HideWhenInMenu(edge, true);
	HideWhenInMenu(scrollbar, true);
	HideWhenInMenu(text, true);
	HideWhenInMenu(title, true);
	HideWhenInMenu(name, true);
	HideWhenInMenu(nameBackground, true);
	HideWhenInMenu(nameEdge, true);
#pragma endregion HideWhenInMenu
#pragma region
    HideWhenInDemo(background, true);
	HideWhenInDemo(edge, true);
	HideWhenInDemo(scrollbar, true);
	HideWhenInDemo(text, true);
	HideWhenInDemo(title, true);
	HideWhenInDemo(name, true);
	HideWhenInDemo(nameBackground, true);
	HideWhenInDemo(nameEdge, true);
#pragma endregion HideWhenInDemo
#pragma region
    HideWhenInScope(background, true);
	HideWhenInScope(edge, true);
	HideWhenInScope(scrollbar, true);
	HideWhenInScope(text, true);
	HideWhenInScope(title, true);
	HideWhenInScope(name, true);
	HideWhenInScope(nameBackground, true);
	HideWhenInScope(nameEdge, true);
#pragma endregion HideWhenInScope
#pragma region
    HideWhenInKillcam(background, true);
	HideWhenInKillcam(edge, true);
	HideWhenInKillcam(scrollbar, true);
	HideWhenInKillcam(text, true);
	HideWhenInKillcam(title, true);
	HideWhenInKillcam(name, true);
	HideWhenInKillcam(nameBackground, true);
	HideWhenInKillcam(nameEdge, true);
#pragma endregion HideWhenInKillcam
#pragma region
    HideWhileRemoteControlling(background, false);
	HideWhileRemoteControlling(edge, false);
	HideWhileRemoteControlling(scrollbar, false);
	HideWhileRemoteControlling(text, false);
	HideWhileRemoteControlling(title, false);
	HideWhileRemoteControlling(name, false);
	HideWhileRemoteControlling(nameBackground, false);
	HideWhileRemoteControlling(nameEdge, false);
#pragma endregion HideWhileRemoteControlling
	flags |= __stored;
	currentMenu = main_menu;
}

void modmenu_s::show()
{
	if (!(flags & __open) && cl_ingame[0]->current.boolean)
	{
		g_entities[client].client->ps.pm_type = 9;
		SV_GameSendServerCommand(client, SV_CMD_CAN_IGNORE, "B 376130938");//uin_main_pause

		int32_t levelTime = level->time + 15;

		MoveOverTime(background, 500, background->elem.x - 280, background->elem.y, levelTime);
		MoveOverTime(edge, 500, edge->elem.x - 280, edge->elem.y, levelTime);
		MoveOverTime(scrollbar, 500, scrollbar->elem.x - 280, scrollbar->elem.y, levelTime);
		MoveOverTime(text, 500, text->elem.x - 280, text->elem.y, levelTime);
		MoveOverTime(title, 500, title->elem.x - 280, title->elem.y, levelTime);
		MoveOverTime(name, 500, name->elem.x + 60, name->elem.y, levelTime);
		MoveOverTime(nameBackground, 500, nameBackground->elem.x + 60, nameBackground->elem.y, levelTime);
		MoveOverTime(nameEdge, 500, nameEdge->elem.x + 60, nameEdge->elem.y, levelTime);
	
		FadeOverTime(background, 500, backColor, levelTime);
		FadeOverTime(edge, 500, edgeColor, levelTime);
		FadeOverTime(scrollbar, 500, edgeColor, levelTime);
		FadeOverTime(text, 500, white, levelTime);
		FadeOverTime(title, 500, white, levelTime);
		FadeOverTime(name, 500, white, levelTime);
		FadeOverTime(nameBackground, 500, backColor, levelTime);
		FadeOverTime(nameEdge, 500, edgeColor, levelTime);

		GameSleep(500);
		flags |= __open;
	}
}

void modmenu_s::hide()
{
	if (flags & __open)
	{
		g_entities[client].client->ps.pm_type = 0;
		SV_GameSendServerCommand(client, SV_CMD_CAN_IGNORE, "B 2418109549");//cac_cmn_backout

		int32_t levelTime = level->time + 15;

		MoveOverTime(background, 500, background->elem.x + 280, background->elem.y, levelTime);
		MoveOverTime(edge, 500, edge->elem.x + 280, edge->elem.y, levelTime);
		MoveOverTime(scrollbar, 500, scrollbar->elem.x + 280, scrollbar->elem.y, levelTime);
		MoveOverTime(text, 500, text->elem.x + 280, text->elem.y, levelTime);
		MoveOverTime(title, 500, title->elem.x + 280, title->elem.y, levelTime);
		MoveOverTime(name, 500, name->elem.x - 60, name->elem.y, levelTime);
		MoveOverTime(nameEdge, 500, nameEdge->elem.x - 60, nameEdge->elem.y, levelTime);
		MoveOverTime(nameBackground, 500, nameBackground->elem.x - 60, nameBackground->elem.y, levelTime);
	
		FadeOverTime(background, 500, black_inv, levelTime);
		FadeOverTime(edge, 500, black_inv, levelTime);
		FadeOverTime(scrollbar, 500, black_inv, levelTime);
		FadeOverTime(text, 500, black_inv, levelTime);
		FadeOverTime(title, 500, black_inv, levelTime);
		FadeOverTime(name, 500, black_inv, levelTime);
		FadeOverTime(nameEdge, 500, black_inv, levelTime);
		FadeOverTime(nameBackground, 500, black_inv, levelTime);

		GameSleep(500);
		flags &= ~__open;
	}
}

void modmenu_s::enter()
{
	if (currentMenu->type == TYPE_FUNCTION)
	{
		currentMenu->child.function(this, client, scrollID);
	}
	else
	{
		int32_t levelTime = level->time + 10;
		menu_s* menu = currentMenu->child.menu[scrollID];
		text->elem.text = menu->stringIndex;
		title->elem.text = menu->titleIndex;
		scroll(-scrollID, levelTime, 200);
		currentMenu = menu;
		scrollID = 0;
	}
}

void modmenu_s::exit()
{
	int32_t levelTime = level->time + 10;
	menu_s* menu = currentMenu->parent;
	text->elem.text = menu->stringIndex;
	title->elem.text = menu->titleIndex;
	scroll(-scrollID, levelTime, 200);
	currentMenu = menu;
	scrollID = 0;
}

void modmenu_s::scroll(int32_t steps, int32_t levelTime, int16_t duration)
{
	if (cl_ingame[0]->current.boolean)
	{
		SV_GameSendServerCommand(client, SV_CMD_CAN_IGNORE, "B 470603177");//cac_grid_nav
		MoveOverTime(scrollbar, duration, scrollbar->elem.x, scrollbar->elem.y + steps * SCROLL_HEIGHT, (levelTime == -1 ? level->time : levelTime));
		//Sleep(duration);
	}
}

void modmenu_s::darken()
{
	NullFx(scrollbar);
	scrollbar->elem.color = darkEdgeColor;
}

void modmenu_s::lighten()
{
	NullFx(scrollbar);
	scrollbar->elem.color = edgeColor;
}

void modmenu_s::push(int16_t width, int16_t height)
{
	int32_t levelTime = level->time + 1;
	ScaleOverTime(scrollbar, 50, scrollbar->elem.width - width, scrollbar->elem.height - height, levelTime);
	//scrollbar->MoveOverTime(50, scrollbar->x + (width / 2), scrollbar->y + (height / 2), levelTime);
	GameSleep(50);
	levelTime = level->time + 1;
	ScaleOverTime(scrollbar, 50, scrollbar->elem.width + width, scrollbar->elem.height + height, levelTime);
	//scrollbar->MoveOverTime(50, scrollbar->x - (width / 2), scrollbar->y - (height / 2), levelTime);
	GameSleep(50);
}

void modmenu_s::move(float x, float y)
{
	// no need for this in Echelon BO2
}

void modmenu_s::update()
{
	if (flags & __stored)
	{
		int32_t startTime = level->time;
		FadeOverTime(background, 2000, backColor, startTime);
		FadeOverTime(nameBackground, 2000, backColor, startTime);

		FadeOverTime(edge, 2000, edgeColor, startTime);
		FadeOverTime(scrollbar, 2000, edgeColor, startTime);
		FadeOverTime(nameEdge, 2000, edgeColor, startTime);
	}
}

void modmenu_s::setSafeArea()
{
	hudelem_color_t gray; gray.rgba = 0x808080C0;

	freeze();
	setBlur(client, 0, 5);
	game_hudelem_s* backGround = SetShader(client, material_white, gray, safeWidth, safeHeight, 0, 0);
	VertAlign(backGround, VERTALIGN_MIDDLE);
	HorzAlign(backGround, HORZALIGN_CENTER);
	AlignX(backGround, ALIGN_X_CENTER);
	AlignY(backGround, ALIGN_Y_MIDDLE);
	gclient_s* gclient = g_entities[client].client;
	gclient->flags |= 4;

	for (;;)
	{
		int32_t btn = gclient->button_bits.array[0];
		if (btn & __jump)
		{
			this->safeHeight = backGround->elem.height;
			this->safeWidth = backGround->elem.width;
			ClearHud(backGround);
			setBlur(client, 0, 0);
			update();
			unfreeze();
			gclient->flags &= ~4;
			break;
		}

		if (btn & __attack)
		{
			if (!(btn & __ads))
			{
				if (backGround->elem.width < maxWidth)
					++backGround->elem.width;
			}
		}
		else if (btn & __ads)
		{
			if (backGround->elem.width > minWidth)
				--backGround->elem.width;
		}

		if (btn & __frag)
		{
			if (!(btn & __tactical))
			{
				if (backGround->elem.height < maxHeight)
					++backGround->elem.height;
			}
		}
		else if (btn & __tactical)
		{
			if (backGround->elem.height > minHeight)
				--backGround->elem.height;
		}

		Sleep(10);
	}
}

void modmenu_s::setBackColor(hudelem_color_t color)
{
	this->backColor = color;
	//update();
	if (flags & __stored)
	{
		int32_t startTime = level->time;
		FadeOverTime(background, 2000, backColor, startTime);
		FadeOverTime(nameBackground, 2000, backColor, startTime);
	}
}

void modmenu_s::setEdgeColor(hudelem_color_t color, hudelem_color_t darkColor)
{
	this->edgeColor = color;
	this->darkEdgeColor = darkColor;
	//update();
	if (flags & __stored)
	{
		int32_t startTime = level->time;
		FadeOverTime(edge, 2000, edgeColor, startTime);
		FadeOverTime(scrollbar, 2000, edgeColor, startTime);
		FadeOverTime(nameEdge, 2000, edgeColor, startTime);
	}
}

void modmenu_s::run(modmenu_s* modmenu)
{
	const int32_t num = modmenu->client;
	gclient_s* client = g_entities[num].client;
	int8_t forwardmove;
	int32_t scrollSpeed = 32, scrollTimes = 31;
	int32_t tempScrollID, levelTime, steps;
	int32_t buttons, actionslots;
	bool actionslotPress = false;

	for (;;)
	{
		if (modmenu->flags & __destroy)
		{
			memset(modmenu, 0, sizeof(modmenu_s));
			threadExit(0);
		}
		if (modmenu->flags & __enabled)
		{
			if (!(modmenu->flags & __stored))
			{
				modmenu->store();
			}
			if (!(modmenu->flags & __frozen))
			{
				if (modmenu->flags & __open)
				{
					buttons = client->button_bits.array[0];
					actionslots = client->button_bitsSinceLastFrame.array[1];
					if (buttons & __frag)
					{
						while (client->button_bits.array[0] & __frag)
							Sleep(50);
						modmenu->hide();
					}
					if (buttons & __use)
					{
						modmenu->darken();
						while (client->button_bits.array[0] & __use)
							Sleep(50);
						modmenu->push(20, 20);
						modmenu->lighten();
						if (modmenu->currentMenu->type == TYPE_FUNCTION)
						{
							modmenu->currentMenu->child.function(modmenu, num, modmenu->scrollID);
						}
						else if (modmenu->currentMenu->child.menu[modmenu->scrollID]->verification <= modmenu->verification)
						{
							tempScrollID = modmenu->scrollID;
							levelTime = level->time + 10;
							modmenu->currentMenu = modmenu->currentMenu->child.menu[tempScrollID];
							modmenu->text->elem.text = modmenu->currentMenu->stringIndex;
							modmenu->title->elem.text = modmenu->currentMenu->titleIndex;
							modmenu->scroll(-tempScrollID, levelTime, 200);
							modmenu->scrollID = 0;
							run(modmenu);
							levelTime = level->time + 10;
							modmenu->currentMenu = modmenu->currentMenu->parent;
							modmenu->text->elem.text = modmenu->currentMenu->stringIndex;
							modmenu->title->elem.text = modmenu->currentMenu->titleIndex;
							modmenu->scroll(tempScrollID - modmenu->scrollID, levelTime, 200);
							modmenu->scrollID = tempScrollID;
						}
					}
					else if (buttons & __melee)
					{
						while (client->button_bits.array[0] & __melee)
							Sleep(50);
						if (modmenu->currentMenu->parent != modmenu->currentMenu)
							return;
						levelTime = level->time + 10;
						modmenu->scroll(-modmenu->scrollID, levelTime, 200);
						modmenu->scrollID = 0;
					}

					if (!actionslotPress)
					{
						if (actionslots & __dpadup)
						{
							steps = modmenu->currentMenu->stringCount - 1;
							if (modmenu->scrollID == 0)
							{
								modmenu->scrollID = steps;
							}
							else
							{
								steps = -1;
								modmenu->scrollID--;
							}
							modmenu->scroll(steps);
							actionslotPress = true;
						}
						else if (actionslots & __dpaddown)
						{
							steps = modmenu->currentMenu->stringCount - 1;
							if (modmenu->scrollID == steps)
							{
								steps *= -1;
								modmenu->scrollID = 0;
							}
							else
							{
								steps = 1;
								modmenu->scrollID++;
							}
							modmenu->scroll(steps);
							actionslotPress = true;
						}
					}
					else
					{
						actionslotPress = false;
					}

					if ((forwardmove = client->sess.cmd.forwardmove))
					{
						if (forwardmove > 80)
						{
							if (scrollTimes++ == scrollSpeed)
							{
								steps = modmenu->currentMenu->stringCount - 1;
								if (modmenu->scrollID == 0)
								{
									modmenu->scrollID = steps;
								}
								else
								{
									steps = -1;
									modmenu->scrollID--;
								}
								modmenu->scroll(steps);

								scrollTimes = 0;
								if (scrollSpeed >= 2)
									scrollSpeed /= 2;
							}
	
						}
						else if (forwardmove < -80)
						{
							if (scrollTimes++ == scrollSpeed)
							{
								steps = modmenu->currentMenu->stringCount - 1;
								if (modmenu->scrollID == steps)
								{
									steps *= -1;
									modmenu->scrollID = 0;
								}
								else
								{
									steps = 1;
									modmenu->scrollID++;
								}
								modmenu->scroll(steps);

								scrollTimes = 0;
								if (scrollSpeed >= 2)
									scrollSpeed /= 2;
							}
						}
						else
						{
							scrollSpeed = 32;
							scrollTimes = 31;
						}
					}
				}
				else
				{
					if (client->button_bits.array[0] & __frag)
					{
						while (client->button_bits.array[0] & __frag)
							Sleep(50);
						modmenu->show();
					}
				}
			}
		}
		else
		{
			if (modmenu->currentMenu->parent != modmenu->currentMenu)
				return;
			modmenu->scrollID = 0;
		}
		if (modmenu->verification <= VERIF_UNVERIFIED)
		{
			modmenu->disable();
		}
		GameSleep(20);
	}
}

void modmenu_s::initialize(title_s* title, menu_s* main, const char* main_title)
{
	char buffer[0x200];

	stringJoin(buffer, sizeof(buffer), '\n', title->title, title->titleSize);
	title->titleIndex = G_LocalizedStringIndex(buffer);

	stringJoin(buffer, sizeof(buffer), '\n', main->string, main->stringCount);
	main->stringIndex = G_LocalizedStringIndex(buffer);
	main->titleIndex = G_LocalizedStringIndex(main_title);

	main->setup();

	menu_title = title;
	main_menu = main;
}
