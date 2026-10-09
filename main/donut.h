#ifndef DONUT_H
#define DONUT_H

#include "flavors.h"
#include "colors.h"

#define DONUT_WIDTH 78
#define DONUT_HEIGHT 20
#define DONUT_FOV 45

#define DONUT_MINOR 1.0f
#define DONUT_MAJOR 2.0f
#define DONUT_SIDES 64
#define DONUT_RINGS 32

#define DONUT_CLASSIC_AMBIENT LC_BLACK
#define DONUT_AMBIENT LC_DARKER
#define DONUT_LIGHT LC_WHITE

#define DONUT_ROTATION_SPEED 2.0f

typedef struct {
	f32 minor;
	f32 major;
	u32 col;
} donut_model_options_t;

typedef struct {
	donut_t flavor;
	bool renderingType;
	bool manual;
	bool doSprinkles;
} donut_options_t;

extern void donut_init(void);
extern void donut_free(void);
extern void render_frame(f32 A, f32 B, donut_t flavor, bool renderingType, bool manual, bool doSprinkles);

#endif
