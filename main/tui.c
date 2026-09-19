#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>

#include "text.h"
#include "strings.h"

#define CLS()					print("\x1b[2J")
#define GOTOXY(x,y)				printf("\x1b[%d;%dH", (y), (x))
#define RESET_COLOR()			print("\x1b[0;0;0m")
#define COLOR_MENU_BOX()		print("\x1b[0;93;104m")
#define COLOR_SELECTED()		print("\x1b[7;93;104m")
#define COLOR_TEXT()			print("\x1b[0;97;104m")
#define COLOR_TITLE()			print("\x1b[4;97;104m")
#define COLOR_SPLASH()			print("\x1b[0;93;104m")

const char* menu_main[] = {
	"Controls",
	"Options",
	"Greetings",
	"Exit"
};

const char* menu_greetz[] = {
	"WiiLink",
	"/r/Ooer",
	"Timcord"
	"BCP"
	"Exit"
};

const char* menu_info[] = {
	"Originally based off \"Wii Donut\" by emilydaemon",
	"Written, and otherwise created by Korbo Q. Lamp (Korbosoft)",
};

#define MAIN_ITEMS 4
#define INFO_ITEMS 2

void draw_tui_window(u8 start_x, u8 start_y, u8 width, u8 height, const char* title) {
	COLOR_MENU_BOX();

	GOTOXY(start_x, start_y);
	print("╔");
	for (u8 i = 0; i < width - 2; i++) print("═");
	print("╗");

	for (u8 i = 1; i < height - 1; i++) {
		GOTOXY(start_x, start_y + i);
		print("║");
		COLOR_TEXT();
		for (u8 i = 0; i < start_x + width - 2; i++) print(" ");
		COLOR_MENU_BOX();
		print("║");
	}

	GOTOXY(start_x, start_y + height - 1);
	print("╚");
	for (u8 i = 0; i < width - 2; i++) print("═");
	print("╝");

	if (title) {
		GOTOXY(start_x + 3, start_y + 1);
		COLOR_TITLE();
		print(title);
	}

	RESET_COLOR();
}

void render_menu_info(char *splash) {
	draw_tui_window(0, 23, 78, 6, "Korbo's Donut Shop v"VERSION" :3");
	GOTOXY(34, 24);
	COLOR_SPLASH();
	print(splash);
	COLOR_TEXT();
	for(u8 i = 0; i < INFO_ITEMS; i++) {
		GOTOXY(3, 25 + i);
		print(menu_info[i]);
	}
	GOTOXY(54, 27);
	print(STRING_CONTROLS);
	RESET_COLOR();
}

void render_menu_main(u8 selected_index) {

	draw_tui_window(0, 23, 78, 6, "Korbo's Donut Shop v"VERSION" :3");
	GOTOXY(34, 24);
	COLOR_SPLASH();
	print(splash);
	COLOR_TEXT();
	for(u8 i = 0; i < INFO_ITEMS; i++) {
		GOTOXY(3, 25 + i);
		print(menu_info[i]);
	}
	GOTOXY(54, 27);
	print(STRING_CONTROLS);
	RESET_COLOR();

	COLOR_MENU_BOX();
	RESET_COLOR();
}

void render_menu(u8 selected_index) {

	// for(u8 i = 0; i < NUM_ITEMS; i++) {
	// 	GOTOXY(2, 7 + i);
	// 	if(i == selected_index) {
	// 		print(" > %-40s ", menu_items[i]);
	// 		RESET_COLOR();
	// 	} else {
	// 		print("   %-40s ", menu_items[i]);
	// 		RESET_COLOR();
	// 	}
	// }

	COLOR_MENU_BOX();
	RESET_COLOR();
}
