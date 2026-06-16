#include "input.h"

u32 wiiPressed;
#ifdef HW_RVL
expansion_t wiiExp;
#endif
u16 GCPressed;

void input_init(void) {
	#ifdef HW_RVL
	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC);
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
