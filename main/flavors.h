#ifndef FLAVORS_H
#define FLAVORS_H

#define FROSTING_FLAVORS 23

enum DonutSpecial {
	MAPPED,
	FROSTED,
	GHOSTLY,
	REFLECTIVE
};

enum DonutTexture {
	NONE,
	RAINBOW,
	PASTEL,
	METAL,
	SPONGE,
	MUNCH
};

typedef struct {
	char name[17];
	GXColor top;
	GXColor bottom;
	GXColor vertex;
	enum DonutSpecial special;
	enum DonutTexture tex;
} donut_t;

static const donut_t flavors[FROSTING_FLAVORS] = {
	{
		"Powdered",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		MAPPED,
		NONE
	},
	{
		"Lard Lad",
		{245, 128, 195, 255},
		{206, 137, 55, 255},
		{255, 255, 255, 255},
		FROSTED,
		NONE
	},
	{
		"Chocolate",
		{112, 58, 42, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NONE
	},
	{
		"Purple",
		{138, 110, 210, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NONE
	},
	{
		"Lemon",
		{242, 255, 122, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NONE
	},
	{
		"Strawberry",
		{255, 113, 107, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NONE
	},
	{
		"Pumpkin",
		{255, 144, 0, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NONE
	},
	{
		"Key Lime",
		{189, 255, 130, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NONE
	},
	{
		"Blueberry",
		{105, 110, 255, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NONE
	},
	{
		"Caramel",
		{203, 111, 57, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NONE
	},
	{
		"Pistachio",
		{156, 255, 110, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NONE
	},
	{
		"Toxic Waste",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{0, 255, 0, 255},
		MAPPED,
		NONE
	},
	{
		"Glazed",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 226, 187, 255},
		MAPPED,
		NONE
	},
	{
		"Bagel",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 187, 98, 255},
		MAPPED,
		NONE
	},
	{
		"Ghost",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 255, 255, 192},
		GHOSTLY,
		NONE
	},
	{
		"Uranium-235",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{0, 255, 0, 255},
		REFLECTIVE,
		METAL
	},
	{
		"Rainbow",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		REFLECTIVE,
		RAINBOW
	},
	{
		"Pastel",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		REFLECTIVE,
		PASTEL
	},
	{
		"Chrome",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		REFLECTIVE,
		METAL
	},
	{
		"Gold",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 224, 64, 255},
		REFLECTIVE,
		METAL
	},
	{
		"Aluminum",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{128, 128, 144, 255},
		REFLECTIVE,
		METAL
	},
	{
		"Sponge",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		MAPPED,
		SPONGE
	},
	{
		"Holographic",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		REFLECTIVE,
		MUNCH
	}
};

#endif
