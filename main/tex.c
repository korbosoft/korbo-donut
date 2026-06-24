#include <gccore.h>
#include <math.h>
#include <grrlib.h>

#include "tex.h"

#define GXColor2RGBA(gx) RGBA(gx.r, gx.g, gx.b, gx.a)

#include "metal_png.h"
// #include "tintedMetal_png.h"
#include "sponge_png.h"
// #include "sprinkles_png.h"

GRRLIB_texImg *rainbowTex;
GRRLIB_texImg *greyPixel;
GRRLIB_texImg *metalTex;
GRRLIB_texImg *tintedMetalTex;
GRRLIB_texImg *spongeTex;
GRRLIB_texImg *munchTex;
// GRRLIB_texImg *sprinklesTex;

void set_tex(donut_t flavor) {
	GXTexObj texObj;
	GRRLIB_texImg *tex;

	switch (flavor.tex) {
		case RAINBOW:
			tex = rainbowTex;
			break;
		case METAL:
			if ((GXColor2RGBA(flavor.top) ^ 0xFFFFFFFF) |
				(GXColor2RGBA(flavor.bottom) ^ 0xFFFFFFFF)) {
				tex = metalTex;
				} else {
					tex = tintedMetalTex;
				}
				break;
		case SPONGE:
			tex = spongeTex;
			break;
		case MUNCH:
			tex = munchTex;
			break;
		default:
			tex = greyPixel;
	}

	GX_SetNumTexGens(1);
	if (flavor.special == REFLECTIVE) {
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_NRM, GX_TEXMTX0);
	} else {
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	}

	GX_InitTexObj(&texObj, tex->data, tex->w, tex->h, tex->format, GX_CLAMP, GX_CLAMP, GX_FALSE);
	if (GRRLIB_Settings.antialias == false) {
		GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
		GX_SetCopyFilter(GX_FALSE, rmode->sample_pattern, GX_FALSE, rmode->vfilter);
	} else {
		GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
	}

	GX_SetNumTevStages(2);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0 );
	GX_SetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR1A1 );
	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GX_SetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV );
	GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);

	GX_LoadTexObj(&texObj, GX_TEXMAP0);
}


void genRainbowTex(GRRLIB_texImg *tex, u16 t) {
	const u16 width = tex->w;
	const u16 height = tex->h;

	const f32 center_x = (width - 1) / 2.0f;
	const f32 center_y = (height - 1) / 2.0f;
	const f32 scale = 2.0f / height;

	for (u16 j = 0; j < height; j++) {
		f32 y = j - center_y;
		f32 y2 = y*y;

		for (u16 i = 0; i < width; i++) {
			f32 x = i - center_x;

			f32 p = -(sqrtf(x*x + y2) - (f32)t/8.0f)*scale;

			f32 r = sinf(M_PI*p);
			f32 g = sinf(M_PI*(p + (1.0f/3.0f)));
			f32 b = sinf(M_PI*(p + (2.0f/3.0f)));

			GRRLIB_SetPixelTotexImg(i, j, tex,
									RGBA((u8)(r*r*255),
										 (u8)(g*g*255),
										 (u8)(b*b*255),
										 255));
		}
	}
}

void genMunchTex(GRRLIB_texImg *tex, u16 t) {
	const u8 time_1 = t;
	const u8 time_2 = t/2;
	const u8 time_3 = t/3;
	for (u16 y = 0; y < tex->h; y++) {
		for (u16 x = 0; x < tex->w; x++) {
			u8 r = x ^ (y - time_1);
			u8 g = (x ^ y) + time_2;
			u8 b = x ^ (y + time_3);
			GRRLIB_SetPixelTotexImg(x, y, tex, RGBA(r, g, b, 255));
		}
	}
}

static GRRLIB_texImg *genTintedMetalTex() {
	GRRLIB_texImg *texOut = GRRLIB_CreateEmptyTexture(metalTex->w, metalTex->h);
	GRRLIB_BMFX_Grayscale(metalTex, texOut);
	for (u16 y = 0; y < texOut->h; y++) {
		for (u16 x = 0; x < texOut->w; x++) {
			u8 v = G(GRRLIB_GetPixelFromtexImg(x, y, texOut));
			const f32 min = 48.0f;
			v = (v/255.0f)*(255.0f - min) + min;
			GRRLIB_SetPixelTotexImg(x, y, texOut, RGBA(v, v, v, 255));
		}
	}
	return texOut;
}

void tex_init(void) {
	rainbowTex = GRRLIB_CreateEmptyTexture(12, 12);
	greyPixel = GRRLIB_CreateEmptyTexture(1, 1);
	GRRLIB_SetPixelTotexImg(0, 0, greyPixel, 0x808080FF);
	metalTex = GRRLIB_LoadTexturePNG(metal_png);
	tintedMetalTex = genTintedMetalTex();
	spongeTex = GRRLIB_LoadTexturePNG(sponge_png);
	munchTex = GRRLIB_CreateEmptyTexture(128, 128);
	// sprinklesTex = GRRLIB_LoadTexturePNG(sprinkles_png);
}

void tex_free(void) {
	GRRLIB_FreeTexture(rainbowTex);
	GRRLIB_FreeTexture(greyPixel);
	GRRLIB_FreeTexture(metalTex);
	GRRLIB_FreeTexture(tintedMetalTex);
	GRRLIB_FreeTexture(spongeTex);
	GRRLIB_FreeTexture(munchTex);
	// GRRLIB_FreeTexture(sprinklesTex);
}
