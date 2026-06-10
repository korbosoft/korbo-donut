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

#include "music_mod.h"

#define DEFAULT_FIFO_SIZE	(256*1024)

// static GXRModeObj *rmode = NULL;
static void *cxfb = NULL;

static bool paused = true;
static bool renderingType = false;
static u8 frostingFlavor = 0;

int main(int argc, char **argv) {
	char splash[44], title[83], frostingName[83], doughName[83];
	bool showControls = false;
	guVector lpos = {2.0f, 2.0f, 2.0f};
	GXLightObj lobj;

	srand(time(NULL));

	GRRLIB_Init();

	input_init();
	// WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC);

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
	float aspect = 4.0 / 3.0f;
	// float aspect = VIDEO_GetAspectRatio();
	// if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
	// 	aspect = 16.0f/9.0f;
	// } else {
	// 	aspect = 4.0f/3.0f;
	// }

	float donAspect = aspect;

	donAspect *= 78.0f / 44.0f; // effectively halves the width to match the character aspect

	int titleLength = music_init(title);
	int frostingLength;

	PROXY_3dMode(0.1F, 300.0F, 45, true, true, donAspect);
	PROXY_Camera3dSettings(0.0f,0.0f,0.0f, 0,1,0, 0,0,0);

	donut_init();

	float A = 1, B = 1;

	if (rand() % 49) {
		format_splash(splashMessages[rand() % SPLASH_COUNT] ?: "FLAGRANT SPLASH ERROR", splash);
	} else {
		format_splash("This splash has a 1/50 chance of appearing", splash);
	}

	GX_SetCullMode(GX_CULL_FRONT);
	VIDEO_ClearFrameBuffer(rmode, cxfb, 0x00800080);

	u8 showFrosting = 0;
	while(SYS_MainLoop()) {
		GX_SetNumChans(1);
		guVecMultiply(view, &lpos, &lpos);

		GX_InitLightPos(&lobj,lpos.x,lpos.y,lpos.z);
		GX_InitLightColor(&lobj, DONUT_LIGHT);
		GX_InitLightDistAttn(&lobj, 0.5f, 0.5f, GX_DA_MEDIUM);
		GX_LoadLightObj(&lobj,GX_LIGHT0);
		GX_SetChanAmbColor(GX_COLOR0A0, renderingType ? LC_DARKDARKDARK : LC_DARKER);
		GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);

		render_frame(A, B, frosting[frostingFlavor], renderingType);

		input_scan();
		input_down(0, 0);

		if (showControls) {
			print("\x1b[23H" "\x1b[104;37m" STRING_CONTROLS_BOX "\x1b[40m");
			// } else if (showFrosting) {

		} else {
			printf("\x1b[23H" "\x1b[104;37m" STRING_MAIN_BOX "\x1b[40m", VERSION, splash);
			// printf("cwd: %s\n", getcwd(NULL, 0));
		}

		if (showFrosting)
			showFrosting--;

		frostingLength = format_info("Flavor: ", frosting[frostingFlavor].name, frostingName, false);
		printf("\x1b[0;%iH" "%s" "\x1b[0;0m", 83 - (showFrosting ? frostingLength : titleLength), showFrosting ? frostingName : title);

		VIDEO_Flush();
		VIDEO_WaitVSync();
		if ((wiiPressed & WPAD_BUTTON_1) | (GCPressed & PAD_TRIGGER_Z)) {
			// send_donut();
		} else if ((wiiPressed & WPAD_BUTTON_2) | (GCPressed & PAD_BUTTON_B)) {
			showControls = !showControls;
		} else if ((wiiPressed & WPAD_BUTTON_MINUS) | (GCPressed & PAD_BUTTON_X)) {
			// bgColor++;
			// bgColor %= 7;
			renderingType = !renderingType;
		} else if ((wiiPressed & WPAD_BUTTON_PLUS) | (GCPressed & PAD_BUTTON_Y)) {
			frostingFlavor++;
			frostingFlavor %= FROSTING_FLAVORS;
			showFrosting = 100;
		} else if ((wiiPressed & WPAD_BUTTON_A) | (GCPressed & PAD_BUTTON_A)) {
			music_pause(paused);
			paused = !paused;
		} else if ((wiiPressed & WPAD_BUTTON_HOME) || (GCPressed & PAD_BUTTON_START))
			break;

		A += 0.035f;
		B += 0.01f;
	}

	GRRLIB_2dMode();
	donut_free();
	GRRLIB_Exit();
	music_free();
	exit(0);
}
