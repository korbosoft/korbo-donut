#ifndef TEX_H
#define TEX_H

#include <gccore.h>
#include <grrlib.h>

#include "flavors.h"

extern GRRLIB_texImg *rainbowTex;
extern GRRLIB_texImg *greyPixel;
extern GRRLIB_texImg *metalTex;
extern GRRLIB_texImg *tintedMetalTex;
extern GRRLIB_texImg *spongeTex;
extern GRRLIB_texImg *munchTex;

extern void tex_init(void);
extern void tex_free(void);
extern void genRainbowTex(GRRLIB_texImg *tex, u16 t);
extern void genMunchTex(GRRLIB_texImg *tex, u16 t);
extern void set_tex(donut_t flavor);

#endif
