#ifndef TUI_H
#define TUI_H

#include <gccore.h>
#include "donut.h"

typedef enum {
	// NOMENU has to technically be a menu... for annoying reasons
	NOMENU,
	MAIN,
	OPTIONS,
	FLAVORS,
	TOGGLE,
	GREETZ,
} Menu;

typedef struct {
	bool ooer;
	bool bcp;
	bool tim;
	bool wiilink;
	bool exit;
} MenuResult;

typedef struct {
	u8 itemCount;
	u8 defaultItem;
	Menu prevMenu;
} MenuSettings;

#define MENU_COUNT 6
#define ITEMS_MAX 9

extern DonutOptions donutOptions;
extern MenuSettings menuSettings[MENU_COUNT];
extern MenuResult handle_general_menu_buttons(Menu *currentMenu, u8 *selected);
extern void render_info_menu(Menu *currentMenu, char *splash);
extern void render_general_menu(Menu menuType, u8 selected);
extern void render_options_menu(u8 selected);

#endif
