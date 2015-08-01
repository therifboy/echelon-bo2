#ifndef __MODMENU_H__
#define __MODMENU_H__

#include "addresses.h"
#include "hud.h"

#include <types.h>

/*Verifications*/
#define VERIF_UNVERIFIED		(0)
#define VERIF_PLAYER			(1)
#define VERIF_VIP				(2)
#define VERIF_ADMIN				(3)
#define VERIF_HOST				(4)

/*Menu types*/
#define TYPE_ERROR				(0)
#define	TYPE_MENU				(1)
#define TYPE_FUNCTION			(2)

/*Menu flags*/
#define __enabled		(1<<0)
#define __stored		(1<<1)
#define __open			(1<<2)
#define __frozen		(1<<4)
#define __destroy		(1<<5)

typedef int8_t verif_t;

typedef int8_t menu_t;

typedef struct modmenu_s modmenu_s;

typedef void (*menuCallback)(modmenu_s* menu, int32_t client, int32_t scroll);

struct echelon_material_t
{
	int32_t backGround;
};

struct menu_s
{
	menu_t type;
	verif_t verification;
	int16_t stringIndex;
	int16_t titleIndex;
	int16_t stringCount;
	const char** string;
	menu_s* parent;
	union
	{
		menuCallback function;
		menu_s** menu;
	}child;

	void setup();
};

struct title_s
{
	int16_t titleIndex;
	int16_t titleSize;
	const char** title;
};

extern int32_t material_white;

class modmenu_s
{
private:
	int8_t client;
	verif_t verification;
	int8_t flags;
	int8_t scrollID;
	hudelem_color_t backColor;
	hudelem_color_t edgeColor;
	hudelem_color_t darkEdgeColor;
	game_hudelem_s* background;
	game_hudelem_s* scrollbar;
	game_hudelem_s* edge;
	game_hudelem_s* text;
	game_hudelem_s* title;
	game_hudelem_s* name;
	game_hudelem_s* nameBackground;
	game_hudelem_s* nameEdge;
	menu_s* currentMenu;
	uint32_t thread;
	int16_t safeWidth;
	int16_t safeHeight;

public:
	void setClient(int8_t client);
	void enable();
	void verify(verif_t type);
	void disable();
	void destroy();
	menu_s* getMenu();
	void freeze();
	void unfreeze();
	void setSafeArea();
	void setBackColor(hudelem_color_t color);
	void setEdgeColor(hudelem_color_t color, hudelem_color_t darkColor);
	
	static void initialize(title_s* menu_title, menu_s* main_menu, const char* main_title);

private:
	void store();
	void show();
	void hide();

	void enter();
	void exit();

	void scroll(int32_t steps, int32_t levelTime = -1, int16_t duration = 50);
	void darken();
	void lighten();
	void push(int16_t width, int16_t height);
	void move(float x, float y);
	void update();

	static void run(modmenu_s* modmenu);
};

#endif /*__MODMENU_H__*/
