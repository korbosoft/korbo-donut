#ifndef TUI_H
#define TUI_H

#include <gccore.h>

typedef enum {
	NOMENU,
	MAIN,
	GREETZ
} Menu;

extern void render_menu_info(char *splash);
extern void draw_tui_window(u8 start_x, u8 start_y, u8 width, u8 height, const char* title);

#endif
