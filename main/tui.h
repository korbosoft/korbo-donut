#ifndef TUI_H
#define TUI_H

#include <gccore.h>

typedef enum {
	// NOMENU has to technically be a menu... for annoying reasons
	NOMENU,
	MAIN,
	OPTIONS,
	GREETZ
} Menu;

typedef struct {
	u8 itemCount;
	u8 defaultItem;
	Menu prevMenu;
} MenuSetting;

#define MENU_COUNT 4
#define ITEMS_MAX 5

extern MenuSetting menuSettings[MENU_COUNT];
extern bool handle_general_menu_buttons(Menu *currentMenu, u8 *selected);
extern void render_menu_info(char *splash);
extern void render_menu(Menu menuType, u8 selected);

#endif
