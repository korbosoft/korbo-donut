#ifndef DONUT_H
#define DONUT_H

#include "flavors.h"

#define DONUT_WIDTH 78
#define DONUT_HEIGHT 22
#define DONUT_FOV 45

extern void donut_init(void);
extern void donut_free(void);
extern void render_frame(float A, float B, Donut flavor, bool renderingType);

#endif
