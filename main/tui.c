#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gccore.h>

#include "tui.h"
#include "text.h"
#include "strings.h"
#include "input.h"
#include "flavors.h"

#define CLS()				print("\x1b[2J")
#define RESET_COLOR()		print("\x1b[0;0;0m")
#define COLOR_MENU_BOX()	print("\x1b[0;93;104m")
#define COLOR_SELECTED()	print("\x1b[7;93;104m")
#define COLOR_TEXT()		print("\x1b[0;97;104m")
#define COLOR_TITLE()		print("\x1b[4;97;104m")
#define COLOR_SPLASH()		print("\x1b[0;93;104m")

static menu_result_t ret;
static u8 prevSelected = 0;
static u8 scroll = 0;
donut_options_t donutOptions;
static bool *toggleVar = NULL;

menu_settings_t menuSettings[MENU_COUNT] = {
	{0, 0, NOMENU}, // NOMENU (here for spacing)
	{4, 3, NOMENU}, // MAIN
	{5, 5, MAIN}, // OPTIONS
	{5, 5, MAIN}, // FLAVOR
	{5, 5, MAIN}, // TOGGLE
	{5, 5, MAIN}, // GREETZ
};

typedef void (*MenuActionFn)(Menu *currentMenu, u8 *selected);

static void select_default(u8 currentMenu, u8 *selected) {
	prevSelected = *selected;
	*selected = menuSettings[currentMenu].defaultItem;
}

static void select_toggle(u8 currentMenu, u8 *selected) {
	prevSelected = *selected;
	*selected = *toggleVar ? 2 : 1;
}

static void restore_selection(u8 *selected) { *selected = prevSelected; }

static void main_options(Menu *currentMenu, u8 *selected) {
	*currentMenu = OPTIONS;
	select_default(*currentMenu, selected);
}

static void main_greetz(Menu *currentMenu, u8 *selected) {
	*currentMenu = GREETZ;
	select_default(*currentMenu, selected);
}

static void options_flavor(Menu *currentMenu, u8 *selected) {
	*currentMenu = FLAVORS;
	select_default(*currentMenu, selected);
}

static void options_sprinkles(Menu *currentMenu, u8 *selected) {
	*currentMenu = TOGGLE;
	toggleVar = &(donutOptions.doSprinkles);
	select_toggle(*currentMenu, selected);
}

static void options_renderer(Menu *currentMenu, u8 *selected) {
	*currentMenu = TOGGLE;
	toggleVar = &(donutOptions.renderingType);
	select_toggle(*currentMenu, selected);
}

static void main_exit(Menu *currentMenu, u8 *selected) { ret.exit = true; }

static void greetz_wiilink(Menu *currentMenu, u8 *selected) { ret.wiilink = true; }
static void greetz_ooer(Menu *currentMenu, u8 *selected) { ret.ooer = true; }
static void greetz_timcord(Menu *currentMenu, u8 *selected) { ret.tim = true; }
static void greetz_bcp(Menu *currentMenu, u8 *selected) { ret.bcp = true; }

static void common_back(Menu *currentMenu, u8 *selected) {
	Menu prevMenu = menuSettings[*currentMenu].prevMenu;
	if (prevMenu != NOMENU) {
		restore_selection(selected);
	} else {
		*selected = menuSettings[prevMenu].defaultItem;
	}
	*currentMenu = prevMenu;
}

static const MenuActionFn menu_actions[MENU_COUNT][ITEMS_MAX] = {
	[MAIN] = {
		main_options,
		main_greetz,
		common_back,
		main_exit
	},
	[OPTIONS] = {
		options_flavor,
		options_sprinkles,
		options_renderer
	},
	[GREETZ] = {
		greetz_ooer,
		greetz_bcp,
		greetz_timcord,
		greetz_wiilink,
		common_back
	}
};

static void handle_menu_specific_buttons(Menu *currentMenu, u8 *selected) {
	if (*currentMenu < MENU_COUNT && *selected - 1 < ITEMS_MAX) {
		MenuActionFn action = menu_actions[*currentMenu][*selected - 1];
		if (action != NULL) {
			action(currentMenu, selected);
		}
	}
}

menu_result_t handle_general_menu_buttons(Menu *currentMenu, u8 *selected) {
	u8 itemCount = menuSettings[*currentMenu].itemCount;

	ret.ooer = false;
	ret.bcp = false;
	ret.tim = false;
	ret.wiilink = false;
	ret.exit = false;

	if (BUTTON_UP) {
		if (*selected > 1) {
			(*selected)--;
		} else {
			*selected = itemCount;
		}
	} else if (BUTTON_DOWN) {
		if (*selected < itemCount) {
			(*selected)++;
		} else {
			*selected = 1;
		}
	} else if (BUTTON_A) {
		handle_menu_specific_buttons(currentMenu, selected);

	} else if (BUTTON_B) {
		common_back(currentMenu, selected);
	}
	return ret;
}

inline void gotoxy(u8 x, u8 y) {
	printf("\x1b[%d;%dH", (y) < 1 ? 1 : (y), (x) < 1 ? 1 : (x));
}

static char *menu_options[] = {
	"Flavor: ",
	"Sprinkles: ",
	"Legacy Style: "
};

static char *menu_toggle[] = {
	"On",
	"Off"
};

static char *menu_greetz[] = {
	"/r/Ooer",
	"BCP",
	"Timcord",
	"WiiLink",
	STRING_BACK
};

static char *menu_main[] = {
	"Options",
	"Greetings",
	STRING_BACK,
	"Exit"
};

static char *menu_info[] = {
	"Originally based off \"Wii Donut\" by emilydaemon",
	"Written, and otherwise created by Korbo Q. Lamp (Korbosoft)",
	"Press A to toggle manual mode (controlled by sticks)"
};

static void draw_tui_window(u8 startX, u8 startY, u8 width, u8 height, const char* title) {
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

void choose_toggle(u8 selected) {
	u8 startX, startY, width, height;
	char title[13];

	width = 16;
	height = 8;

	if (toggleVar == &(donutOptions.doSprinkles)) {
		strcpy(title, "Sprinkles");
	} else if (toggleVar == &(donutOptions.renderingType)) {
		strcpy(title, "Legacy Style");
	} else {
		strcpy(title, "Placeholder");
	}

	startX = (78 - width) / 2;
	startY = (28 - height) / 2;
	draw_tui_window(startX, startY, width, height, title);
	for (u8 i = 0; i < 2; i++) {
		if (i == selected - 1) {
			COLOR_SELECTED();
		} else {
			COLOR_TEXT();
		}
		gotoxy(startX + 2, startY + 2 + i);
		print(menu_toggle[i]);
	}
	RESET_COLOR();
}

void choose_flavor(u8 selected) {
	u8 startX, startY, width, height;

	width = 16;
	height = 12;

	startX = (78 - width) / 2;
	startY = (28 - height) / 2;
	draw_tui_window(startX, startY, width, height, "Flavors");
	for (u8 i = scroll; i < 9 + scroll; i++) {
		if (i >= FROSTING_FLAVORS) break;
		if (i == selected - 1) {
			COLOR_SELECTED();
		} else {
			COLOR_TEXT();
		}
		gotoxy(startX + 2, startY + 2 + i);
		print(flavors[i].name);

		gotoxy(startX + 2, startY);
		if ((i == scroll) && scroll) { print("\xf9\xf9\xf9\xf9\xf9\xf9\xf9\xf9\xf9\xf9\xf9\xf9"); }
		gotoxy(startX + 2, startY + 11);
		if (i == 8 + scroll) { print("\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa\xfa"); }
	}
	RESET_COLOR();
}

void render_general_menu(Menu menuType, u8 selected) {
	u8 startX, startY, width, height;
	u8 itemCount;
	char **items = NULL;
	char title[77];

	switch (menuType) {
		default:
			width = 16;
			height = 8;
			strcpy(title, "Placeholder");
			items = menu_greetz;
			break;
		case MAIN:
			width = 14;
			height = 8;
			strcpy(title, "Main Menu");
			items = menu_main;
			break;
		case OPTIONS:
			render_options_menu(selected);
			break;
		case FLAVORS:
			choose_flavor(selected);
			break;
		case TOGGLE:
			choose_toggle(selected);
			break;
		case GREETZ:
			width = 20;
			height = 10;
			strcpy(title, "Greetings To...");
			items = menu_greetz;
			break;
	}
	if (items) {
		itemCount = menuSettings[menuType].itemCount;

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
}

void render_options_menu(u8 selected) {
	draw_tui_window(1, 22, 78, 6, "Options");
	for (u8 i = 0; i < 3; i++) {
		if (i == selected - 1) {
			COLOR_SELECTED();
		} else {
			COLOR_TEXT();
		}
		gotoxy(3, 24 + i);
		print(menu_options[i]);
	}
	gotoxy(54, 26);
	print("    Press B to go back.");
	RESET_COLOR();
}
