#ifndef FLAVORS_H
#define FLAVORS_H

enum DonutSpecial {
	MAPPED,
	FROSTED,
	GHOSTLY,
	REFLECTIVE
};

enum DonutTexture {
	NOTEXTURE,
	RAINBOW,
	PASTEL,
	METAL,
	SPONGE,
	MUNCH,
	FUR
};

typedef struct {
	char name[13];
	GXColor top;
	GXColor bottom;
	GXColor vertex;
	enum DonutSpecial special;
	enum DonutTexture tex;
} Donut;

#define MAX_FLAVORS 35

static const Donut flavors[MAX_FLAVORS] = {
	{
		"Powdered",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		MAPPED,
		NOTEXTURE
	},
	{
		"Lard Lad",
		{245, 128, 195, 255},
		{206, 137, 55, 255},
		{255, 255, 255, 255},
		FROSTED,
		NOTEXTURE
	},
	{
		"Chocolate",
		{112, 58, 42, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NOTEXTURE
	},
	{
		"Purple",
		{138, 110, 210, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NOTEXTURE
	},
	{
		"Lemon",
		{242, 255, 122, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NOTEXTURE
	},
	{
		"Strawberry",
		{255, 113, 107, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NOTEXTURE
	},
	{
		"Pumpkin",
		{255, 144, 0, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NOTEXTURE
	},
	{
		"Key Lime",
		{189, 255, 130, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NOTEXTURE
	},
	{
		"Blueberry",
		{105, 110, 255, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NOTEXTURE
	},
	{
		"Caramel",
		{203, 111, 57, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NOTEXTURE
	},
	{
		"Pistachio",
		{156, 255, 110, 255},
		{255, 187, 98, 255},
		{255, 255, 255, 255},
		FROSTED,
		NOTEXTURE
	},
	{
		"Toxic Waste",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{0, 255, 0, 255},
		MAPPED,
		NOTEXTURE
	},
	{
		"Glazed",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 226, 187, 255},
		MAPPED,
		NOTEXTURE
	},
	{
		"Bagel",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 187, 98, 255},
		MAPPED,
		NOTEXTURE
	},
	{
		"Ghost",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 255, 255, 192},
		GHOSTLY,
		NOTEXTURE
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
		"Diamond",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{94, 239, 255, 255},
		REFLECTIVE,
		METAL
	},
	{
		"Aluminum",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{224, 224, 255, 255},
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
		"Lovey Sponge",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 128, 128, 255},
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
	},
	{
		"Fur",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		MAPPED,
		FUR
	},
	{
		"Fur (Red)",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 128, 128, 255},
		MAPPED,
		FUR
	},
	{
		"Fur (Orange)",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 192, 128, 255},
		MAPPED,
		FUR
	},
	{
		"Fur (Yellow)",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 255, 128, 255},
		MAPPED,
		FUR
	},
	{
		"Fur (Green)",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{128, 255, 128, 255},
		MAPPED,
		FUR
	},
	{
		"Fur (Blue)",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{128, 128, 255, 255},
		MAPPED,
		FUR
	},
	{
		"Fur (Pink)",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{255, 128, 255, 255},
		MAPPED,
		FUR
	},
	{
		"Fur (Purple)",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{128, 64, 128, 255},
		MAPPED,
		FUR
	},
	{
		"Fur (Gray)",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{128, 128, 128, 255},
		MAPPED,
		FUR
	},
	{
		"Fur (Brown)",
		{255, 255, 255, 255},
		{255, 255, 255, 255},
		{128, 64, 0, 255},
		MAPPED,
		FUR
	}
};

#endif
