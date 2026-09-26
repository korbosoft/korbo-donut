#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gccore.h>

#include "tui.h"
#include "text.h"
#include "strings.h"
#include "input.h"

#define CLS()				print("\x1b[2J")
#define RESET_COLOR()		print("\x1b[0;0;0m")
#define COLOR_MENU_BOX()	print("\x1b[0;93;104m")
#define COLOR_SELECTED()	print("\x1b[7;93;104m")
#define COLOR_TEXT()		print("\x1b[0;97;104m")
#define COLOR_TITLE()		print("\x1b[4;97;104m")
#define COLOR_SPLASH()		print("\x1b[0;93;104m")

MenuSetting menuSettings[MENU_COUNT] = {
	{0, 0, NOMENU}, // NOMENU (here for spacing)
	{4, 1, NOMENU}, // MAIN
	{5, 5, GREETZ}, // GREETZ
};

void handle_menu_specific_buttons(Menu *currentMenu, u8 *selected) {
	switch (*currentMenu) {
		case NOMENU:
		case MAIN:
		case GREETZ:
	}
}

void handle_general_menu_buttons(Menu *currentMenu, u8 *selected) {
	u8 itemCount = menuSettings[*currentMenu].itemCount;

	if (BUTTON_UP) {
		if (*selected > 1) {
			(*selected)--;
		} else {
			*selected = itemCount;
		}
	} else if (BUTTON_DOWN) {
		if (*selected >= 1 && *selected < itemCount) {
			(*selected)++;
		} else {
			*selected = 1;
		}
	} else if (BUTTON_A) {
		handle_menu_specific_buttons(currentMenu, selected);
	} else if (BUTTON_B) {
		*currentMenu = menuSettings[*currentMenu].prevMenu;
	}
}

inline void gotoxy(u8 x, u8 y) {
	printf("\x1b[%d;%dH", (y) < 1 ? 1 : (y), (x) < 1 ? 1 : (x));
}

static char *menu_greetz[] = {
	"WiiLink",
	"/r/Ooer",
	"Timcord",
	"BCP",
	"Back"
};

static char *menu_main[] = {
	"Controls",
	"Options",
	"Greetings",
	"Exit"
};

static char *menu_info[] = {
	"Originally based off \"Wii Donut\" by emilydaemon",
	"Written, and otherwise created by Korbo Q. Lamp (Korbosoft)",
	"Press A for manual mode (controlled by sticks)"
};

void draw_tui_window(u8 startX, u8 startY, u8 width, u8 height, const char* title) {
	COLOR_MENU_BOX();

	gotoxy(startX, startY);
	print("╔");
	for (u8 i = 0; i < width - 2; i++) print("═");
	print("╗");

	for (u8 i = 1; i < height - 1; i++) {
		gotoxy(startX, startY + i);
		print("║");
		COLOR_TEXT();
		for (u8 j = 0; j < width - 2; j++) print(" ");
		COLOR_MENU_BOX();
		print("║");
	}

	gotoxy(startX, startY + height - 1);
	print("╚");
	for (u8 i = 0; i < width - 2; i++) print("═");
	print("╝");

	if (title) {
		gotoxy(startX + 2, startY + 1);
		COLOR_TITLE();
		print(title);
	}

	RESET_COLOR();
}

void render_menu_info(char *splash) {
	draw_tui_window(1, 22, 78, 6, "Korbo's Donut Shop v"VERSION" :3");
	gotoxy(34, 23);
	COLOR_SPLASH();
	print(splash);
	COLOR_TEXT();
	for (u8 i = 0; i < 3; i++) {
		gotoxy(3, 24 + i);
		print(menu_info[i]);
	}
	gotoxy(54, 26);
	print(STRING_CONTROLS);
	RESET_COLOR();
}

void render_menu(Menu menuType, u8 selected) {
	u8 startX, startY, width, height;
	u8 itemCount;
	char **items;
	char title[77];

	switch (menuType) {
		default:
			width = 12;
			height = 8;
			strcpy(title, "Placeholder");
			items = menu_main;
			itemCount = 4;
			break;
		case MAIN:
			width = 14;
			height = 8;
			strcpy(title, "Main Menu");
			items = menu_main;
			itemCount = 4;
			break;
		case GREETZ:
			width = 16;
			height = 10;
			strcpy(title, "Greetings To...");
			items = menu_greetz;
			itemCount = 5;
			break;
	}

	startX = (78 - width) / 2;
	startY = (28 - height) / 2;
	draw_tui_window(startX, startY, width, height, title);
	for (u8 i = 0; i < itemCount; i++) {
		if (i == selected - 1) {
			COLOR_SELECTED();
		} else {
			COLOR_TEXT();
		}
		gotoxy(startX + 2, startY + 2 + i);
		print(items[i]);
	}
	RESET_COLOR();
}
