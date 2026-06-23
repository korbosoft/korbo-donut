#ifndef INPUT_H
#define INPUT_H

#define DEADZONE 0.20f

#include <ogcsys.h>

#ifdef HW_RVL
#include <wiiuse/wpad.h>
#endif

extern u32 wiiPressed;
extern u16 GCPressed;

extern void input_init(void);

extern void input_scan(void);

extern void input_down(u8 wii, u8 gamecube);

#endif
