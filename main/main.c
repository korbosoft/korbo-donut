#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <unistd.h>
#include <gccore.h>

#ifdef HW_RVL
#include <wiiuse/wpad.h>
#endif

#include <aesndlib.h>
#include <gcmodplay.h>
#include <grrlib.h>

#include "donut.h"
#include "file.h"
#include "grrproxy.h"
#include "input.h"
#include "font.h"
#include "strings.h"
#include "text.h"
#include "flavors.h"
#include "music.h"
#include "tui.h"
#include "time.h"

static void *consoleXfb = NULL;
static void *displayXfb = NULL; // needed so it all draws at once

static f64 A = 0, B = 0;

void rotate_donut(void) {
	f64 delta = delta_secs();

	A += 2.8 * delta;
	B += 0.8 * delta;
	// TODO: Manual donut speed

	A = fmod(A, M_PI * 2);
	B = fmod(B, M_PI * 2);
}

int main(int argc, char **argv) {
	bool stop = false;
	u8 selected = 0;
	Menu currentMenu = NOMENU;
	MenuResult menuResult;

	char splash[44], title[83], flavorName[83];
	guVector lpos = {0.0f, 1.0f, 0.0f};
	GXLightObj lobj;

	srand(SYS_Time());

	GRRLIB_Init();

	input_init();

	// Initialize the file... thing. I don't know what to call it
	file_init();

	// Allocate memory for the display in the uncached region
	size_t fbSize = rmode->fbWidth * rmode->xfbHeight * VI_DISPLAY_PIX_SZ;

	consoleXfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	displayXfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	GRRLIB_FillScreen(0x000000FF);

	// Initialize the console, required for printf
	CON_Init(consoleXfb, 8,16, rmode->fbWidth - 8,rmode->xfbHeight, rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	consoleSetFont(NULL, &(ConsoleFont){(u8*)donut_font_8x16, 0, 256});
	//SYS_STDIO_Report(true);
	VIDEO_SetNextFramebuffer(displayXfb);

	// setup our projection matrix
	// f32 aspect = VIDEO_GetAspectRatio();
// #ifndef HW_RVL
	const f32 aspect = 4.0 / 3.0f;
// #else
// 	const f32 aspect = (CONF_GetAspectRatio() == CONF_ASPECT_16_9) ?
// 				   16.0f/9.0f : 4.0f/3.0f;
// #endif
	f32 donAspect = aspect;
	donAspect *= (f32)DONUT_WIDTH / (f32)(DONUT_HEIGHT * 2.0f);

	music_init(title);

	PROXY_3dMode(0.1F, 300.0F, 45, true, true, donAspect);
	PROXY_Camera3dSettings(0.0f,0.0f,0.0f, 0,1,0, 0,0,0);

	donut_init();

	#define SPLASH_COUNT 14

	const char *splashMessages[SPLASH_COUNT] = {
		[0] = "Also try DS Donut!",
		[1] = "Also try 3DS Donut!",
		[2] = "Also try Lily Skate!",
		[3] = "Better than Wii Donut!",
		[4] = "(\"Doughnut\" if you're british)",
		[5] = "Korbo loves you <3",
		[6] = "Did you know you can change the music?",
		[7] = "GBA Donut: Gone but not forgotten </3",
		[8] = "You best not forget The Bob Incident.",
		[9] = "so there's this series called HLVRAI and it",
		[10] = "You LOVE Shit River. You LOVE it.",
		[11] = "The most over-engineered Wii/GC homebrew!",
		[12] = "As seen on Jiga Tech!",
		[13] = "BlueSheep was here!"
	};

	const char *eggSplashMessages[4] = {
		[0] = "oh man please to help i am not good with co", // ooer
		[1] = "Hop on Hell Diver!", // banana cookie pie
		[2] = "Something something echolalia!", // timcord
		// TODO: "how do I leave", "i can smell you pronouns"
		[3] = "Sponsored by Dickass Auto!" // wiilink
	};

	if (rand() % 50) {
		format_splash(splashMessages[rand() % SPLASH_COUNT], splash);
	} else {
		format_splash("This splash has a 1/50 chance of appearing!", splash);
	}

	GX_SetCullMode(GX_CULL_FRONT);
	VIDEO_ClearFrameBuffer(rmode, consoleXfb, COLOR_BLACK);
	VIDEO_ClearFrameBuffer(rmode, displayXfb, COLOR_BLACK);

	donutOptions.flavor = 0;
	donutOptions.renderingType = false;
	donutOptions.manual = false;
	donutOptions.doSprinkles = false;

	menuResult.ooer = false;
	menuResult.bcp = false;
	menuResult.tim = false;
	menuResult.wiilink = false;
	menuResult.exit = false;

	u8 showFlavor = 0;

	time_init();

	while (SYS_MainLoop()) {
		GX_SetNumChans(1);
		guVecMultiply(view, &lpos, &lpos);

		GX_InitLightPos(&lobj,lpos.x,lpos.y,lpos.z);
		GX_InitLightColor(&lobj, DONUT_LIGHT);
		GX_InitLightDistAttn(&lobj, 0.5f, 0.5f, GX_DA_MEDIUM);
		GX_LoadLightObj(&lobj,GX_LIGHT0);
		GX_SetChanAmbColor(GX_COLOR0A0, donutOptions.renderingType ? DONUT_CLASSIC_AMBIENT : DONUT_AMBIENT);
		GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);

		input_scan();
		input_down(0, 0);

		music_tick();
		donut_tick();

		render_frame(
			A,
			B,
			flavors[donutOptions.flavor],
			donutOptions.renderingType,
			donutOptions.manual,
			donutOptions.doSprinkles
		);

		render_info_menu(&currentMenu, splash);

		if (showFlavor)
			showFlavor--;

		print("\x1b[H");
		print(showFlavor ? flavorName : title);
		print("\x1b[0;0;0m");

		if (menuResult.ooer)
			format_splash(eggSplashMessages[0], splash);

		if (menuResult.bcp)
			format_splash(eggSplashMessages[1], splash);

		if (menuResult.tim)
			format_splash(eggSplashMessages[2], splash);

		if (menuResult.wiilink)
			format_splash(eggSplashMessages[3], splash);

		if (menuResult.exit)
			stop = true;

		switch (currentMenu) {
			case NOMENU:
				if (BUTTON_EXIT) {
					stop = true;
				} else if (BUTTON_START) {
					currentMenu = MAIN;
					selected = menuSettings[MAIN].defaultItem;
				} else if (BUTTON_A) {
					donutOptions.manual = !donutOptions.manual;
				}
				break;
			case OPTIONS:
				render_options_menu(selected);
				menuResult = handle_general_menu_buttons(&currentMenu, &selected);
				break;
			default:
				render_general_menu(currentMenu, selected);
				menuResult = handle_general_menu_buttons(&currentMenu, &selected);
		}

		if (stop) break;

		memcpy(displayXfb, consoleXfb, fbSize);
		VIDEO_Flush();
		VIDEO_WaitVSync();

		rotate_donut();
	}

	VIDEO_SetBlack(true);
	GRRLIB_2dMode();
	donut_free();
	GRRLIB_Exit();
	music_free();
	exit(0);
}
