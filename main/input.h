#ifndef INPUT_H
#define INPUT_H

#define DEADZONE (f32)0.20f

#define GC_DEADZONE (u8)(DEADZONE * 100)

#include <ogcsys.h>

#ifdef HW_RVL
#include <wiiuse/wpad.h>

#define BUTTON_UP 		(wiiPressed & (WPAD_BUTTON_UP | WPAD_CLASSIC_BUTTON_UP))		|| (GCPressed & PAD_BUTTON_UP)
#define BUTTON_DOWN 	(wiiPressed & (WPAD_BUTTON_DOWN | WPAD_CLASSIC_BUTTON_DOWN))	|| (GCPressed & PAD_BUTTON_DOWN)
#define BUTTON_LEFT 	(wiiPressed & (WPAD_BUTTON_LEFT | WPAD_CLASSIC_BUTTON_LEFT))	|| (GCPressed & PAD_BUTTON_LEFT)
#define BUTTON_RIGHT 	(wiiPressed & (WPAD_BUTTON_RIGHT | WPAD_CLASSIC_BUTTON_RIGHT))	|| (GCPressed & PAD_BUTTON_RIGHT)
#define BUTTON_A 		(wiiPressed & (WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A))			|| (GCPressed & PAD_BUTTON_A)
#define BUTTON_B 		(wiiPressed & (WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B))			|| (GCPressed & PAD_BUTTON_B)
#define BUTTON_START 	(wiiPressed & (WPAD_BUTTON_PLUS | WPAD_CLASSIC_BUTTON_PLUS))	|| (GCPressed & PAD_BUTTON_START)
#define BUTTON_EXIT		wiiPressed & (WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME)

extern u32 wiiPressed;
#else
#define BUTTON_UP		GCPressed & PAD_BUTTON_UP
#define BUTTON_DOWN		GCPressed & PAD_BUTTON_DOWN
#define BUTTON_LEFT		GCPressed & PAD_BUTTON_LEFT
#define BUTTON_RIGHT	GCPressed & PAD_BUTTON_RIGHT
#define BUTTON_A		GCPressed & PAD_BUTTON_A
#define BUTTON_B		GCPressed & PAD_BUTTON_B
#define BUTTON_START	GCPressed & PAD_BUTTON_START
#define BUTTON_EXIT		0
#endif

extern u16 GCPressed;

extern void input_init(void);

extern void input_scan(void);

extern void input_down(u8 wii, u8 gamecube);

#endif
