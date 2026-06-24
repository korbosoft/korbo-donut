#ifndef FLAVORS_H
#define FLAVORS_H

#define FROSTING_FLAVORS 23

enum DonutSpecial {
	MAPPED,
	GHOSTLY,
	REFLECTIVE
};

enum DonutTexture {
	NONE,
	RAINBOW,
	METAL,
	SPONGE,
	MUNCH
};

typedef struct {
	char name[17];
	GXColor top;
	GXColor bottom;
	enum DonutSpecial special;
	enum DonutTexture tex;
} donut_t;

static const donut_t flavors[FROSTING_FLAVORS] = {
	{
		"Powdered",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		MAPPED,
		NONE
	},
	{
		"Lard Lad",
		{245, 128, 195, 255},
		{206, 137, 55, 255},
		MAPPED,
		NONE
	},
	{
		"Chocolate",
		{112, 58, 42, 255},
		{255, 187, 98, 255},
		MAPPED,
		NONE
	},
	{
		"Purple",
		{138, 110, 210, 255},
		{255, 187, 98, 255},
		MAPPED,
		NONE
	},
	{
		"Lemon",
		{242, 255, 122, 255},
		{255, 187, 98, 255},
		MAPPED,
		NONE
	},
	{
		"Strawberry",
		{255, 113, 107, 255},
		{255, 187, 98, 255},
		MAPPED,
		NONE
	},
	{
		"Pumpkin",
		{255, 144, 0, 255},
		{255, 187, 98, 255},
		MAPPED,
		NONE
	},
	{
		"Key Lime",
		{189, 255, 130, 255},
		{255, 187, 98, 255},
		MAPPED,
		NONE
	},
	{
		"Blueberry",
		{105, 110, 255, 255},
		{255, 187, 98, 255},
		MAPPED,
		NONE
	},
	{
		"Caramel",
		{203, 111, 57, 255},
		{255, 187, 98, 255},
		MAPPED,
		NONE
	},
	{
		"Pistachio",
		{156, 255, 110, 255},
		{255, 187, 98, 255},
		MAPPED,
		NONE
	},
	{
		"Toxic Waste",
		{0, 255, 0, 255},
		{0, 255, 0, 255},
		MAPPED,
		NONE
	},
	{
		"Glazed",
		{255, 226, 187, 255},
		{255, 226, 187, 255},
		MAPPED,
		NONE
	},
	{
		"Bagel",
		{255, 187, 98, 255},
		{255, 187, 98, 255},
		MAPPED,
		NONE
	},
	{
		"Ghost",
		{255, 255, 255, 192},
		{255, 255, 255, 192},
		GHOSTLY,
		NONE
	},
	{
		"Uranium-235",
		{0, 255, 0, 255},
		{0, 255, 0, 255},
		REFLECTIVE,
		METAL
	},
	{
		"Rainbow",
		{128, 128, 128, 255},
		{128, 128, 128, 255},
		REFLECTIVE,
		RAINBOW
	},
	{
		"Pastel",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		REFLECTIVE,
		RAINBOW
	},
	{
		"Chrome",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		REFLECTIVE,
		METAL
	},
	{
		"Gold",
		{255, 224, 0, 255},
		{255, 224, 0, 255},
		REFLECTIVE,
		METAL
	},
	{
		"Aluminum",
		{128, 128, 144, 255},
		{128, 128, 144, 255},
		REFLECTIVE,
		METAL
	},
	{
		"Sponge",
		{128, 128, 128, 255},
		{128, 128, 128, 255},
		MAPPED,
		SPONGE
	},
	{
		"Holographic",
		{128, 128, 128, 255},
		{128, 128, 128, 255},
		REFLECTIVE,
		MUNCH
	}
};

#endif
