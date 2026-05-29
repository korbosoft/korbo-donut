#ifndef INPUT_H
#define INPUT_H

#include <ogcsys.h>

#ifndef GC
#include <wiiuse/wpad.h>
#else
#include <ogc/pad.h>
#endif

extern u32 wiiPressed;
extern u16 GCPressed;

extern void input_init(void);

extern void input_scan(void);

extern void input_down(u8 wii, u8 gamecube);

#endif
