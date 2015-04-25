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
				printf("sys_ppu_thread_create() fail: %08x\n", ret);
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
	background = hudElement::SetShader(client, material_white, black_inv, 250, 500, 88, 0);
	background->VertAlign(VERTALIGN_MIDDLE);
	background->HorzAlign(HORZALIGN_RIGHT);
	background->AlignX(ALIGN_X_RIGHT);
	background->AlignY(ALIGN_Y_MIDDLE);
	edge = hudElement::SetShader(client, material_white, black_inv, 2, 500, 88, 0);
	edge->VertAlign(VERTALIGN_MIDDLE);
	edge->HorzAlign(HORZALIGN_RIGHT);
	edge->AlignX(ALIGN_X_RIGHT);
	edge->AlignY(ALIGN_Y_MIDDLE);
	scrollbar = hudElement::SetShader(client, material_white, black_inv, 248, 21, 90, -140);
	scrollbar->VertAlign(VERTALIGN_MIDDLE);
	scrollbar->HorzAlign(HORZALIGN_RIGHT);
	scrollbar->AlignX(ALIGN_X_RIGHT);
	scrollbar->AlignY(ALIGN_Y_MIDDLE);
	text = hudElement::SetText(client, main_menu->stringIndex, black_inv, black_inv, FONT_OBJECTIVE, 1.875f, 110, -140);
	text->VertAlign(VERTALIGN_MIDDLE);
	text->HorzAlign(HORZALIGN_RIGHT);
	text->AlignX(ALIGN_X_RIGHT);
	text->AlignY(ALIGN_Y_MIDDLE);
	title = hudElement::SetText(client, main_menu->titleIndex, black_inv, black_inv, FONT_OBJECTIVE, 2.5f, 110, -170);
	title->VertAlign(VERTALIGN_MIDDLE);
	title->HorzAlign(HORZALIGN_RIGHT);
	title->AlignX(ALIGN_X_RIGHT);
	title->AlignY(ALIGN_Y_MIDDLE);
	nameBackground = hudElement::SetShader(client, material_white, black_inv, 60, 500, -60, 0);
	nameBackground->VertAlign(VERTALIGN_MIDDLE);
	nameBackground->HorzAlign(HORZALIGN_LEFT);
	nameBackground->AlignX(ALIGN_X_LEFT);
	nameBackground->AlignY(ALIGN_Y_MIDDLE);
	nameEdge = hudElement::SetShader(client, material_white, black_inv, 2, 500, 0, 0);
	nameEdge->VertAlign(VERTALIGN_MIDDLE);
	nameEdge->HorzAlign(HORZALIGN_LEFT);
	nameEdge->AlignX(ALIGN_X_LEFT);
	nameEdge->AlignY(ALIGN_Y_MIDDLE);
	name = hudElement::SetText(client, menu_title->titleIndex, black_inv, black_inv, FONT_BIG, 4, -45, -128);
	name->VertAlign(VERTALIGN_MIDDLE);
	name->HorzAlign(HORZALIGN_LEFT);
	name->AlignX(ALIGN_X_LEFT);
	name->AlignY(ALIGN_Y_MIDDLE);
#pragma region
    background->ForeGround(true);
	edge->ForeGround(true);
	scrollbar->ForeGround(true);
	text->ForeGround(true);
	title->ForeGround(true);
	name->ForeGround(true);
	nameBackground->ForeGround(true);
	nameEdge->ForeGround(true);
#pragma endregion ForeGround
#pragma region
    background->HideWhenDead(true);
	edge->HideWhenDead(true);
	scrollbar->HideWhenDead(true);
	text->HideWhenDead(true);
	title->HideWhenDead(true);
	name->HideWhenDead(true);
	nameBackground->HideWhenDead(true);
	nameEdge->HideWhenDead(true);
#pragma endregion HideWhenDead
#pragma region
    background->HideWhenInMenu(true);
	edge->HideWhenInMenu(true);
	scrollbar->HideWhenInMenu(true);
	text->HideWhenInMenu(true);
	title->HideWhenInMenu(true);
	name->HideWhenInMenu(true);
	nameBackground->HideWhenInMenu(true);
	nameEdge->HideWhenInMenu(true);
#pragma endregion HideWhenInMenu
#pragma region
    background->HideWhenInDemo(true);
	edge->HideWhenInDemo(true);
	scrollbar->HideWhenInDemo(true);
	text->HideWhenInDemo(true);
	title->HideWhenInDemo(true);
	name->HideWhenInDemo(true);
	nameBackground->HideWhenInDemo(true);
	nameEdge->HideWhenInDemo(true);
#pragma endregion HideWhenInDemo
#pragma region
    background->HideWhenInScope(true);
	edge->HideWhenInScope(true);
	scrollbar->HideWhenInScope(true);
	text->HideWhenInScope(true);
	title->HideWhenInScope(true);
	name->HideWhenInScope(true);
	nameBackground->HideWhenInScope(true);
	nameEdge->HideWhenInScope(true);
#pragma endregion HideWhenInScope
#pragma region
    background->HideWhenInKillcam(true);
	edge->HideWhenInKillcam(true);
	scrollbar->HideWhenInKillcam(true);
	text->HideWhenInKillcam(true);
	title->HideWhenInKillcam(true);
	name->HideWhenInKillcam(true);
	nameBackground->HideWhenInKillcam(true);
	nameEdge->HideWhenInKillcam(true);
#pragma endregion HideWhenInKillcam
#pragma region
    background->HideWhileRemoteControlling(false);
	edge->HideWhileRemoteControlling(false);
	scrollbar->HideWhileRemoteControlling(false);
	text->HideWhileRemoteControlling(false);
	title->HideWhileRemoteControlling(false);
	name->HideWhileRemoteControlling(false);
	nameBackground->HideWhileRemoteControlling(false);
	nameEdge->HideWhileRemoteControlling(false);
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

		background->MoveOverTime(500, background->elem.x - 280, background->elem.y, levelTime);
		edge->MoveOverTime(500, edge->elem.x - 280, edge->elem.y, levelTime);
		scrollbar->MoveOverTime(500, scrollbar->elem.x - 280, scrollbar->elem.y, levelTime);
		text->MoveOverTime(500, text->elem.x - 280, text->elem.y, levelTime);
		title->MoveOverTime(500, title->elem.x - 280, title->elem.y, levelTime);
		name->MoveOverTime(500, name->elem.x + 60, name->elem.y, levelTime);
		nameBackground->MoveOverTime(500, nameBackground->elem.x + 60, nameBackground->elem.y, levelTime);
		nameEdge->MoveOverTime(500, nameEdge->elem.x + 60, nameEdge->elem.y, levelTime);
	
		background->FadeOverTime(500, backColor, levelTime);
		edge->FadeOverTime(500, edgeColor, levelTime);
		scrollbar->FadeOverTime(500, edgeColor, levelTime);
		text->FadeOverTime(500, white, levelTime);
		title->FadeOverTime(500, white, levelTime);
		name->FadeOverTime(500, white, levelTime);
		nameBackground->FadeOverTime(500, backColor, levelTime);
		nameEdge->FadeOverTime(500, edgeColor, levelTime);

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

		background->MoveOverTime(500, background->elem.x + 280, background->elem.y, levelTime);
		edge->MoveOverTime(500, edge->elem.x + 280, edge->elem.y, levelTime);
		scrollbar->MoveOverTime(500, scrollbar->elem.x + 280, scrollbar->elem.y, levelTime);
		text->MoveOverTime(500, text->elem.x + 280, text->elem.y, levelTime);
		title->MoveOverTime(500, title->elem.x + 280, title->elem.y, levelTime);
		name->MoveOverTime(500, name->elem.x - 60, name->elem.y, levelTime);
		nameEdge->MoveOverTime(500, nameEdge->elem.x - 60, nameEdge->elem.y, levelTime);
		nameBackground->MoveOverTime(500, nameBackground->elem.x - 60, nameBackground->elem.y, levelTime);
	
		background->FadeOverTime(500, black_inv, levelTime);
		edge->FadeOverTime(500, black_inv, levelTime);
		scrollbar->FadeOverTime(500, black_inv, levelTime);
		text->FadeOverTime(500, black_inv, levelTime);
		title->FadeOverTime(500, black_inv, levelTime);
		name->FadeOverTime(500, black_inv, levelTime);
		nameEdge->FadeOverTime(500, black_inv, levelTime);
		nameBackground->FadeOverTime(500, black_inv, levelTime);

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
		scrollbar->MoveOverTime(duration, scrollbar->elem.x, scrollbar->elem.y + steps * SCROLL_HEIGHT, (levelTime == -1 ? level->time : levelTime));
		//Sleep(duration);
	}
}

void modmenu_s::darken()
{
	scrollbar->NullFx();
	scrollbar->elem.color = darkEdgeColor;
}

void modmenu_s::lighten()
{
	scrollbar->NullFx();
	scrollbar->elem.color = edgeColor;
}

void modmenu_s::push(int16_t width, int16_t height)
{
	int32_t levelTime = level->time + 1;
	scrollbar->ScaleOverTime(50, scrollbar->elem.width - width, scrollbar->elem.height - height, levelTime);
	//scrollbar->MoveOverTime(50, scrollbar->x + (width / 2), scrollbar->y + (height / 2), levelTime);
	/*scrollbar->fromWidth = scrollbar->width;
	scrollbar->fromHeight = scrollbar->height;
	scrollbar->fromX = scrollbar->x;
	scrollbar->fromY = scrollbar->y;
	scrollbar->width -= width;
	scrollbar->x += (width / 2);
	scrollbar->height -= height;
	scrollbar->y += (height / 2);
	scrollbar->scaleTime = scrollbar->moveTime = 50;
	scrollbar->scaleStartTime = scrollbar->moveStartTime = LevelTime;*/
	GameSleep(50);
	levelTime = level->time + 1;
	scrollbar->ScaleOverTime(50, scrollbar->elem.width + width, scrollbar->elem.height + height, levelTime);
	//scrollbar->MoveOverTime(50, scrollbar->x - (width / 2), scrollbar->y - (height / 2), levelTime);
	/*scrollbar->fromWidth = scrollbar->width;
	scrollbar->fromHeight = scrollbar->height;
	scrollbar->fromX = scrollbar->x;
	scrollbar->fromY = scrollbar->y;
	scrollbar->width += width;
	scrollbar->x -= (width / 2);
	scrollbar->height += height;
	scrollbar->y -= (height / 2);
	scrollbar->scaleTime = scrollbar->moveTime = 50;
	scrollbar->scaleStartTime = scrollbar->moveStartTime = level->time;*/
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
		background->FadeOverTime(2000, backColor, startTime);
		nameBackground->FadeOverTime(2000, backColor, startTime);

		edge->FadeOverTime(2000, edgeColor, startTime);
		scrollbar->FadeOverTime(2000, edgeColor, startTime);
		nameEdge->FadeOverTime(2000, edgeColor, startTime);
	}
}

void modmenu_s::setSafeArea()
{
	hudelem_color_t gray; gray.rgba = 0x808080C0;

	freeze();
	setBlur(client, 0, 5);
	hudElement* backGround = hudElement::SetShader(client, material_white, gray, safeWidth, safeHeight, 0, 0);
	backGround->VertAlign(VERTALIGN_MIDDLE);
	backGround->HorzAlign(HORZALIGN_CENTER);
	backGround->AlignX(ALIGN_X_CENTER);
	backGround->AlignY(ALIGN_Y_MIDDLE);
	gclient_s* gclient = g_entities[client].client;
	gclient->flags |= 4;

	for (;;)
	{
		int32_t btn = gclient->button_bits.array[0];
		if (btn & __jump)
		{
			this->safeHeight = backGround->elem.height;
			this->safeWidth = backGround->elem.width;
			backGround->Clear();
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
		background->FadeOverTime(2000, backColor, startTime);
		nameBackground->FadeOverTime(2000, backColor, startTime);
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
		edge->FadeOverTime(2000, edgeColor, startTime);
		scrollbar->FadeOverTime(2000, edgeColor, startTime);
		nameEdge->FadeOverTime(2000, edgeColor, startTime);
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
