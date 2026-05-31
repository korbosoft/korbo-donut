#ifndef DONUT_H
#define DONUT_H

#include "flavors.h"
#include "colors.h"

#define DONUT_WIDTH 78
#define DONUT_HEIGHT 22
#define DONUT_FOV 45

#define DONUT_MINOR 1.0f
#define DONUT_MAJOR 2.0f
#define DONUT_SIDES 64
#define DONUT_RINGS 32

#define DONUT_AMBIENT LC_DARKER
#define DONUT_LIGHT LC_WHITE

extern void donut_init(void);
extern void donut_free(void);
extern void render_frame(float A, float B, Donut flavor, bool renderingType);

#endif
