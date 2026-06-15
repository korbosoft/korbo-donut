#ifndef INPUT_H
#define INPUT_H

#define GC_DEADZONE 18

#include <ogcsys.h>

extern u32 wiiPressed;
extern u16 GCPressed;

extern void input_init(void);

extern void input_scan(void);

extern void input_down(u8 wii, u8 gamecube);

#endif
