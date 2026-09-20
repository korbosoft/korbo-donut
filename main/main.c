#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
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

#define DEFAULT_FIFO_SIZE	(256*1024)

// static GXRModeObj *rmode = NULL;
static void *cxfb = NULL;

static bool doSprinkles = false;
static bool manual = false;
static bool paused = true;
static bool renderingType = false;
static bool showControls = false;
static bool stop = false;
static u8 flavor = 0;
static s16 selectedIndex = 0;
static Menu currentMenu = NOMENU;

void handle_menu_buttons() {
	if (((wiiPressed & (WPAD_BUTTON_UP | WPAD_CLASSIC_BUTTON_UP))) | (GCPressed & PAD_BUTTON_UP)) {
		if (selectedIndex) selectedIndex--;
	}
}

int main(int argc, char **argv) {
	char splash[44], title[83], flavorName[83];
	guVector lpos = {0.0f, 1.0f, 0.0f};
	GXLightObj lobj;

	srand(time(NULL));

	GRRLIB_Init();

	input_init();

	// Initialize the file... thing. I can't really call it the "file system", can I?
	file_init();

	// Allocate memory for the display in the uncached region
	cxfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	GRRLIB_FillScreen(0x000000FF);

	// Initialise the console, required for printf
	CON_Init(cxfb, 8,16, rmode->fbWidth - 8,rmode->xfbHeight, rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	consoleSetFont(NULL, &(ConsoleFont){(u8*)donut_font_8x16, 0, 256});
	//SYS_STDIO_Report(true);
	VIDEO_SetNextFramebuffer(cxfb);

	// setup our projection matrix
	// float aspect = VIDEO_GetAspectRatio();
// #ifndef HW_RVL
	const float aspect = 4.0 / 3.0f;
// #else
// 	const float aspect = (CONF_GetAspectRatio() == CONF_ASPECT_16_9) ?
// 				   16.0f/9.0f : 4.0f/3.0f;
// #endif
	float donAspect = aspect;

	donAspect *= (float)DONUT_WIDTH / (float)(DONUT_HEIGHT*2); // times 2 because text characters are 8x16

	music_init(title);

	PROXY_3dMode(0.1F, 300.0F, 45, true, true, donAspect);
	PROXY_Camera3dSettings(0.0f,0.0f,0.0f, 0,1,0, 0,0,0);

	donut_init();

	float A = 1, B = 1;

	#define SPLASH_COUNT 13

	const char *splashMessages[SPLASH_COUNT] = {
		[0] = "Also try DS Donut!",
		[1] = "Also try 3DS Donut!",
		[2] = "Also try Lily Skate!",
		[3] = "Better than Wii Donut!",
		[4] = "oh man please to help i am not good with co",
		[5] = "(\"Doughnut\" if you're british)",
		[6] = "Korbo loves you <3",
		[7] = "Did you know you can change the music?",
		[8] = "GBA Donut: Gone but not forgotten </3",
		[9] = "You best not forget the spongebob incident.",
		[10] = "so there's this series called HLVRAI and it",
		[11] = "You love Shit River.",
		[12] = "The most overengineered Wii/GC homebrew!"
	};

	if (rand() % 50) {
		format_splash(splashMessages[rand() % SPLASH_COUNT], splash);
	} else {
		format_splash("This splash has a 1/50 chance of appearing!", splash);
	}

	GX_SetCullMode(GX_CULL_FRONT);
	VIDEO_ClearFrameBuffer(rmode, cxfb, COLOR_BLACK);

	u8 showFlavor = 0;
	while(SYS_MainLoop()) {
		GX_SetNumChans(1);
		guVecMultiply(view, &lpos, &lpos);

		GX_InitLightPos(&lobj,lpos.x,lpos.y,lpos.z);
		GX_InitLightColor(&lobj, DONUT_LIGHT);
		GX_InitLightDistAttn(&lobj, 0.5f, 0.5f, GX_DA_MEDIUM);
		GX_LoadLightObj(&lobj,GX_LIGHT0);
		GX_SetChanAmbColor(GX_COLOR0A0, renderingType ? LC_DARKDARKDARK : LC_DARKER);
		GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);

		input_scan();
		input_down(0, 0);

		render_frame(A, B, flavors[flavor], renderingType, manual, doSprinkles);

		render_menu_info(splash);

		if (showFlavor)
			showFlavor--;

		print("\x1b[H");
		print(showFlavor ? flavorName : title);
		print("\x1b[0;0;0m");

		if (currentMenu == NOMENU) {
			if (wiiPressed & (WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME)) {
				stop = true;
			} else if (((wiiPressed & (WPAD_BUTTON_PLUS | WPAD_CLASSIC_BUTTON_PLUS))) | (GCPressed & PAD_BUTTON_START)) {
				currentMenu = MAIN;
				selectedIndex = 0;
			}
		} else {
			render_menu(currentMenu, selectedIndex);
			handle_menu_buttons();
		}

		if (stop) break;

		VIDEO_Flush();
		VIDEO_WaitVSync();

		A += 0.035f;
		B += 0.01f;
	}

	GRRLIB_2dMode();
	donut_free();
	GRRLIB_Exit();
	music_free();
	exit(0);
}
