#ifndef TEX_H
#define TEX_H

#include <gccore.h>
#include <grrlib.h>

#include "flavors.h"

extern void tex_init(void);
extern void tex_free(void);
extern void set_tex(Donut flavor, bool doSprinkles);

#endif
