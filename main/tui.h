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
} menu_result_t;

typedef struct {
	u8 itemCount;
	u8 defaultItem;
	Menu prevMenu;
} menu_settings_t;

#define MENU_COUNT 6
#define ITEMS_MAX 5

extern donut_options_t donutOptions;
extern menu_settings_t menuSettings[MENU_COUNT];
extern menu_result_t handle_general_menu_buttons(Menu *currentMenu, u8 *selected);
extern void render_menu_info(char *splash);
extern void render_general_menu(Menu menuType, u8 selected);
extern void render_options_menu(u8 selected);

#endif
