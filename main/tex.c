#include <gccore.h>
#include <math.h>
#include <grrlib.h>
#include <ogc/lwp_watchdog.h>

#include "tex.h"
#include "time.h"

#define GXColor2RGBA(gx) ( (u32)( ( ((u32)(gx.r))        <<24) |  \
								  ((((u32)(gx.g)) &0xFF) <<16) |  \
								  ((((u32)(gx.b)) &0xFF) << 8) |  \
								  ( ((u32)(gx.a)) &0xFF      ) ) )

#include "frosting_png.h"
#include "fur_png.h"
#include "metal_png.h"
#include "flash_png.h"
#include "sponge_png.h"
#include "lsponge_png.h"
#include "sprinkles_png.h"

GRRLIB_texImg *whitePixel;
GRRLIB_texImg *frostingTex;
GRRLIB_texImg *rainbowTex[60];
GRRLIB_texImg *pastelTex[60];
GRRLIB_texImg *metalTex;
GRRLIB_texImg *flashTex;
GRRLIB_texImg *tintedMetalTex;
GRRLIB_texImg *spongeTex;
GRRLIB_texImg *lSpongeTex;
GRRLIB_texImg *holoTex;
GRRLIB_texImg *sprinklesTex;
GRRLIB_texImg *furTex;

GXTexObj sprinklesTexObj;

GXTexObj init_tex_obj(GRRLIB_texImg *tex) {
	GXTexObj texObj;

	GX_InitTexObj(&texObj, tex->data, tex->w, tex->h, tex->format, GX_CLAMP, GX_CLAMP, GX_FALSE);
	if (GRRLIB_Settings.antialias == false) {
		GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
		GX_SetCopyFilter(GX_FALSE, rmode->sample_pattern, GX_FALSE, rmode->vfilter);
	} else {
		GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
	}
	return texObj;
}

void set_tex(Donut flavor, bool doSprinkles) {
	GXTexObj mainTexObj;
	GRRLIB_texImg *mainTex;

	u64 current_sixtieth = (u64)(((f64)ticks_since_start()/(f64)PPC_TIMER_CLOCK)*60.0) % 60;

	if (flavor.special == FROSTED) {
		mainTex = frostingTex;
	} else {
		switch (flavor.tex) {
			case RAINBOW:
				mainTex = rainbowTex[current_sixtieth];
				break;
			case PASTEL:
				mainTex = pastelTex[current_sixtieth];
				break;
			case METAL:
				if (GXColor2RGBA(flavor.vertex) == 0xFFFFFFFF) {
					mainTex = metalTex;
				} else {
					mainTex = tintedMetalTex;
				}
				break;
			case SPONGE:
				mainTex = spongeTex;
				break;
			case LSPONGE:
				mainTex = lSpongeTex;
				break;
			case HOLO:
				mainTex = holoTex;
				break;
			case FUR:
				mainTex = furTex;
				break;
			case FLASH:
				mainTex = flashTex;
				break;
			default:
				mainTex = whitePixel;
		}
	}
	GX_SetNumTexGens(1);
	if (flavor.special == REFLECTIVE) {
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_NRM, GX_TEXMTX0);
	} else {
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	}

	mainTexObj = init_tex_obj(mainTex);
	sprinklesTexObj = init_tex_obj(sprinklesTex);

	switch (flavor.special) {
		case FROSTED:
			if (doSprinkles) {

				GX_SetNumTevStages(3);

				GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

				GX_SetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0A0);

				GX_SetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0A0);

				GX_SetTevColor(GX_TEVREG0, flavor.bottom);
				GX_SetTevColor(GX_TEVREG1, flavor.top);

				GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
				GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);

				GX_SetTevOp(GX_TEVSTAGE1, GX_DECAL);

				GX_SetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_RASC, GX_CC_CPREV, GX_CC_ZERO);
				GX_SetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
			} else {
				GX_SetNumTevStages(2);

				GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
				GX_SetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

				GX_SetTevColor(GX_TEVREG0, flavor.bottom);
				GX_SetTevColor(GX_TEVREG1, flavor.top);

				GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

				GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
				GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);

				GX_SetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_RASC, GX_CC_CPREV, GX_CC_ZERO);
				GX_SetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
			}
			break;
		default:
			GX_SetNumTevStages(1);
			GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
	}
	GX_LoadTexObj(&mainTexObj, GX_TEXMAP0);
	GX_LoadTexObj(&sprinklesTexObj, GX_TEXMAP1);
}

static GRRLIB_texImg *genRainbowTex(u16 w, u16 h, u8 t, bool pastel) {
	const u16 width = w;
	const u16 height = h;

	GRRLIB_texImg *texOut = GRRLIB_CreateEmptyTexture(w, h);

	const f32 center_x = (width - 1) / 2.0f;
	const f32 center_y = (height - 1) / 2.0f;
	const f32 scale = 2.0f / height;

	// Exactly 1 full color cycle (6.0 offset for height=12) spread cleanly across 60 frames
	const f32 time_offset = ((f32)t / 60.0f) * ((f32)height / 2.0f);

	for (u16 j = 0; j < height; j++) {
		const f32 y = j - center_y;
		const f32 y2 = y * y;

		for (u16 i = 0; i < width; i++) {
			const f32 x = i - center_x;
			const f32 p = -(sqrtf(x * x + y2) - time_offset) * scale;
			const u16 mult = pastel ? 510 : 255;

			f32 r = sinf(M_PI * p);
			f32 g = sinf(M_PI * (p + (1.0f / 3.0f)));
			f32 b = sinf(M_PI * (p + (2.0f / 3.0f)));

			r *= r * mult;
			g *= g * mult;
			b *= b * mult;

			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;

			GRRLIB_SetPixelTotexImg(i, j, texOut, RGBA((u8)r, (u8)g, (u8)b, 255));
		}
	}
	return texOut;
}

static GRRLIB_texImg *genHoloTex(u16 w, u16 h) {
	const f32 twoThirdsPi = (2.0f / 3.0f) * M_PI;

	GRRLIB_texImg *texOut = GRRLIB_CreateEmptyTexture(w, h);

	for (u16 y = 0; y < h; y++) {
		f32 latitude = ((f32)y / (f32)h - 0.5f) * M_PI;
		f32 cosLat = cosf(latitude);

		for (u16 x = 0; x < w; x++) {
			f32 longitude = ((f32)x / (f32)w - 0.5f) * (2.0f * M_PI);

			f32 dirX = cosLat * cosf(longitude);
			f32 dirZ = cosLat * sinf(longitude);

			f32 fresnel = powf(1.0f - fabsf(dirZ), 3.0f);

			u8 munch = (u8)((x ^ y) * 3 / 4) + 64;

			f32 basePhase = atan2f(dirZ, dirX) * 3.0f;

			f32 holoR = 0.5f + 0.5f * cosf(basePhase);
			f32 holoG = 0.5f + 0.5f * cosf(basePhase - twoThirdsPi);
			f32 holoB = 0.5f + 0.5f * cosf(basePhase - (twoThirdsPi * 2.0f));

			u8 r = (u8)(munch * holoR + fresnel);
			u8 g = (u8)(munch * holoG + fresnel);
			u8 b = (u8)(munch * holoB + fresnel);

			GRRLIB_SetPixelTotexImg(x, y, texOut, RGBA(r, g, b, 255));
		}
	}
	return texOut;
}

static GRRLIB_texImg *genTintedMetalTex() {
	u16 width = metalTex->w;
	u16 height = metalTex->h;

	GRRLIB_texImg *texOut = GRRLIB_CreateEmptyTexture(width, height);
	const f32 min = 72.0f;
	for (u16 y = 0; y < height; y++) {
		for (u16 x = 0; x < width; x++) {
			u32 col = GRRLIB_GetPixelFromtexImg(x, y, metalTex);
			u8 r = R(col);
			u8 g = G(col);
			u8 b = B(col);
			r = (r/255.0f)*(255.0f - min) + min;
			g = (g/255.0f)*(255.0f - min) + min;
			b = (b/255.0f)*(255.0f - min) + min;
			GRRLIB_SetPixelTotexImg(x, y, texOut, RGBA(r, g, b, 255));
		}
	}
	return texOut;
}

void tex_init(void) {
	whitePixel = GRRLIB_CreateEmptyTexture(1, 1);
	frostingTex = GRRLIB_LoadTexturePNG(frosting_png);
	for (u8 i = 0; i < 60; i++) {
		rainbowTex[i] = genRainbowTex(12, 12, i, false);
		pastelTex[i] = genRainbowTex(12, 12, i, true);
	}
	GRRLIB_SetPixelTotexImg(0, 0, whitePixel, 0xFFFFFFFF);
	metalTex = GRRLIB_LoadTexturePNG(metal_png);
	tintedMetalTex = genTintedMetalTex();
	spongeTex = GRRLIB_LoadTexturePNG(sponge_png);
	lSpongeTex = GRRLIB_LoadTexturePNG(lsponge_png);
	holoTex = genHoloTex(256, 256);
	sprinklesTex = GRRLIB_LoadTexturePNG(sprinkles_png);
	furTex = GRRLIB_LoadTexturePNG(fur_png);
	flashTex = GRRLIB_LoadTexturePNG(flash_png);
}

void tex_free(void) {
	GRRLIB_FreeTexture(whitePixel);
	GRRLIB_FreeTexture(frostingTex);
	for (u8 i = 0; i < 60; i++) {
		GRRLIB_FreeTexture(rainbowTex[i]);
		GRRLIB_FreeTexture(pastelTex[i]);
	}
	GRRLIB_FreeTexture(metalTex);
	GRRLIB_FreeTexture(tintedMetalTex);
	GRRLIB_FreeTexture(spongeTex);
	GRRLIB_FreeTexture(lSpongeTex);
	GRRLIB_FreeTexture(holoTex);
	GRRLIB_FreeTexture(sprinklesTex);
	GRRLIB_FreeTexture(furTex);
	GRRLIB_FreeTexture(flashTex);
}
