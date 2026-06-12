#include "input.h"

#ifdef HW_RVL
#include <wiiuse/wpad.h>
#else
#include <ogc/pad.h>
#endif

u32 wiiPressed;
u16 GCPressed;

void input_init(void) {
	#ifdef HW_RVL
	WPAD_Init();
	#endif
	PAD_Init();
}

void input_scan(void) {
	#ifdef HW_RVL
	WPAD_ScanPads();
	#endif
	PAD_ScanPads();
}
void input_down(u8 wii, u8 gamecube) {
	#ifdef HW_RVL
	wiiPressed = WPAD_ButtonsDown(wii);
	#endif
	GCPressed = PAD_ButtonsDown(gamecube);
}
