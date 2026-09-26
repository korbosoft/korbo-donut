#ifndef TUI_H
#define TUI_H

#include <gccore.h>

typedef enum {
	// NOMENU has to technically be a menu... for annoying reasons
	NOMENU,
	MAIN,
	GREETZ
} Menu;

typedef struct {
	u8 itemCount;
	u8 defaultItem;
	Menu prevMenu;
} MenuSetting;

#define MENU_COUNT 3

extern MenuSetting menuSettings[MENU_COUNT];
extern void handle_general_menu_buttons(Menu *currentMenu, u8 *selected);
extern void draw_tui_window(u8 start_x, u8 start_y, u8 width, u8 height, const char* title);
extern void render_menu_info(char *splash);
extern void render_menu(Menu menuType, u8 selected);

#endif
