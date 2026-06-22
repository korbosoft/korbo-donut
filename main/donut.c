#include <gccore.h>
#include <math.h>
#include <grrlib.h>
#include <stdlib.h>

#include "colors.h"
#include "donut.h"
#include "grrproxy.h"
#include "strings.h"
#include "text.h"
#include "input.h"

#include "shape_lut_bin.h"

#include "metal_png.h"
// #include "tintedMetal_png.h"
#include "sponge_png.h"
// #include "sprinkles_png.h"

#define DEG2RAD (f32)(M_PI / 180.0f)

static GRRLIB_texImg *shapeBuffer;
static GRRLIB_texImg *donutBuffer;
static GRRLIB_texImg *rainbowTex;
static GRRLIB_texImg *greyPixel;
static GRRLIB_texImg *metalTex;
static GRRLIB_texImg *tintedMetalTex;
static GRRLIB_texImg *spongeTex;
static GRRLIB_texImg *munchTex;
// static GRRLIB_texImg *sprinklesTex;

static u16 munch_timer;
static u16 rainbow_timer;

typedef struct {
	f32 pos0[3];
	f32 nrm0[3];
	f32 tex0[2];
	u32 col;
	f32 pos1[3];
	f32 nrm1[3];
	f32 tex1[2];
} DonutModel;

static DonutOptions donutOptions;
static DonutOptions frostingOptions;

static DonutModel donutModel[DONUT_RINGS][DONUT_SIDES + 1];
static DonutModel frostingModel[DONUT_RINGS][DONUT_SIDES + 1];
static const f32 Z_TRANSLATION = -3.0f / sinf(DegToRad(DONUT_FOV) / 2.0f);

static void draw_donut(DonutOptions options, bool filled) {
	if ((donutOptions.major != options.major) ||
		(donutOptions.minor != options.minor) ||
		(donutOptions.col != options.col)) {
		const f32 ringDelta = 2.0*M_PI / DONUT_RINGS;
		const f32 sideDelta = 2.0*M_PI / DONUT_SIDES;

		f32 theta = 0.0f;
		f32 cosTheta = 1.0f;
		f32 sinTheta = 0.0f;

		donutOptions.major = options.major;
		donutOptions.minor = options.minor;
		donutOptions.col = options.col;
		for (int i = 0; i < DONUT_RINGS; i++) {
			const f32 theta1 = theta + ringDelta;
			const f32 cosTheta1 = cosf(theta1);
			const f32 sinTheta1 = sinf(theta1);

			const f32 u0 = (f32)i / DONUT_RINGS;
			const f32 u1 = (f32)(i + 1) / DONUT_RINGS;
			GX_Begin(filled ? GX_TRIANGLESTRIP : GX_LINESTRIP, GX_VTXFMT0, 2*(DONUT_SIDES + 1));
			f32 phi = M_PI;
			for (int j = 0; j <= DONUT_SIDES; j++) {
				const f32 cosPhi = cosf(phi), sinPhi = sinf(phi);
				const f32 dist = donutOptions.major + donutOptions.minor*cosPhi;

				const f32 v = (f32)j / DONUT_SIDES;
				const f32 z = donutOptions.minor*sinPhi;

				GX_Position3f32(cosTheta1*dist, -sinTheta1*dist, z);
				donutModel[i][j].pos1[0] = cosTheta1*dist;
				donutModel[i][j].pos1[1] = -sinTheta1*dist;
				donutModel[i][j].pos1[2] = z;
				GX_Normal3f32(cosTheta1*cosPhi, -sinTheta1*cosPhi, sinPhi);
				donutModel[i][j].nrm1[0] = cosTheta1*cosPhi;
				donutModel[i][j].nrm1[1] = -sinTheta1*cosPhi;
				donutModel[i][j].nrm1[2] = sinPhi;
				GX_Color1u32(donutOptions.col);
				GX_TexCoord2f32(u1, v);
				donutModel[i][j].tex1[0] = u1;
				donutModel[i][j].tex1[1] = v;

				GX_Position3f32(cosTheta*dist, -sinTheta*dist, z);
				donutModel[i][j].pos0[0] = cosTheta*dist;
				donutModel[i][j].pos0[1] = -sinTheta*dist;
				donutModel[i][j].pos0[2] = z;
				GX_Normal3f32(cosTheta*cosPhi, -sinTheta*cosPhi, sinPhi);
				donutModel[i][j].nrm0[0] = cosTheta*cosPhi;
				donutModel[i][j].nrm0[1] = -sinTheta*cosPhi;
				donutModel[i][j].nrm0[2] = sinPhi;
				GX_Color1u32(donutOptions.col);
				GX_TexCoord2f32(u0, v);
				donutModel[i][j].tex0[0] = u0;
				donutModel[i][j].tex0[1] = v;
				phi += sideDelta;
			}
			GX_End();

			cosTheta = cosTheta1;
			sinTheta = sinTheta1;
			theta = theta1;
		}
	} else {
		for (int i = 0; i < DONUT_RINGS; i++) {
			GX_Begin(filled ? GX_TRIANGLESTRIP : GX_LINESTRIP, GX_VTXFMT0, 2*(DONUT_SIDES + 1));
			for (int j = 0; j <= DONUT_SIDES; j++) {
				GX_Position3f32(donutModel[i][j].pos1[0],
								donutModel[i][j].pos1[1],
								donutModel[i][j].pos1[2]);
				GX_Normal3f32(donutModel[i][j].nrm1[0],
							  donutModel[i][j].nrm1[1],
							  donutModel[i][j].nrm1[2]);
				GX_Color1u32(donutOptions.col);
				GX_TexCoord2f32(donutModel[i][j].tex1[0],
								donutModel[i][j].tex1[1]);

				GX_Position3f32(donutModel[i][j].pos0[0],
								donutModel[i][j].pos0[1],
								donutModel[i][j].pos0[2]);
				GX_Normal3f32(donutModel[i][j].nrm0[0],
							  donutModel[i][j].nrm0[1],
							  donutModel[i][j].nrm0[2]);
				GX_Color1u32(donutOptions.col);
				GX_TexCoord2f32(donutModel[i][j].tex0[0],
								donutModel[i][j].tex0[1]);
			}
			GX_End();
		}
	}
}

static void draw_frosting(DonutOptions options, bool filled) {
	const f32 major = options.major - 0.2f;
	const f32 minor = options.minor + 0.2f;
	if ((frostingOptions.major != major) ||
		(frostingOptions.minor != minor) ||
		(frostingOptions.col != options.col)) {
		const f32 ringDelta = 2.0f*M_PI/DONUT_RINGS;
		const f32 sideDelta = M_PI/DONUT_SIDES;
		const f32 waveAmp = 0.5f;
		const f32 waveFreq = 10.0f;

		f32 theta = 0.0f;
		f32 cosTheta = 1.0f;
		f32 sinTheta = 0.0f;

		frostingOptions.major = major;
		frostingOptions.minor = minor;
		frostingOptions.col = options.col;
		for (int i = 0; i < DONUT_RINGS; i++) {
			const f32 theta1 = theta + ringDelta;
			const f32 cosTheta1 = cos(theta1);
			const f32 sinTheta1 = sin(theta1);

			const f32 cutZ0 = waveAmp/2*(waveAmp*2 + sinf(theta*waveFreq));
			const f32 cutZ1 = waveAmp/2*(waveAmp*2 + sinf(theta1*waveFreq));

			const f32 u0 = (f32)i / DONUT_RINGS;
			const f32 u1 = (f32)(i + 1) / DONUT_RINGS;

			GX_Begin(filled ? GX_TRIANGLESTRIP : GX_LINESTRIP, GX_VTXFMT0, 2*(DONUT_SIDES + 1));

			f32 phi = 0.0f;
			for (int j = 0; j <= DONUT_SIDES; j++) {
				const f32 cosPhi = cosf(phi), sinPhi = sinf(phi);
				const f32 dist = frostingOptions.major + frostingOptions.minor*cosPhi;

				const f32 v = (f32)j / DONUT_SIDES;

				const f32 z = frostingOptions.minor*sinPhi;
				f32 z1 = z;
				if (z1 < cutZ1) z1 = cutZ1;

				GX_Position3f32(cosTheta1*dist, -sinTheta1*dist, z1);
				frostingModel[i][j].pos1[0] = cosTheta1*dist;
				frostingModel[i][j].pos1[1] = -sinTheta1*dist;
				frostingModel[i][j].pos1[2] = z1;
				GX_Normal3f32(cosTheta1*cosPhi, -sinTheta1*cosPhi, sinPhi);
				frostingModel[i][j].nrm1[0] = cosTheta1*cosPhi;
				frostingModel[i][j].nrm1[1] = -sinTheta1*cosPhi;
				frostingModel[i][j].nrm1[2] = sinPhi;
				GX_Color1u32(frostingOptions.col);
				GX_TexCoord2f32(u1, v);
				frostingModel[i][j].tex1[0] = u1;
				frostingModel[i][j].tex1[1] = v;

				f32 z0 = z;
				if (z0 < cutZ0) z0 = cutZ0;

				GX_Position3f32(cosTheta*dist, -sinTheta*dist, z0);
				frostingModel[i][j].pos0[0] = cosTheta*dist;
				frostingModel[i][j].pos0[1] = -sinTheta*dist;
				frostingModel[i][j].pos0[2] = z0;
				GX_Normal3f32(cosTheta*cosPhi, -sinTheta*cosPhi, sinPhi);
				frostingModel[i][j].nrm0[0] = cosTheta*cosPhi;
				frostingModel[i][j].nrm0[1] = -sinTheta*cosPhi;
				frostingModel[i][j].nrm0[2] = sinPhi;
				GX_Color1u32(frostingOptions.col);
				GX_TexCoord2f32(u0, v);
				frostingModel[i][j].tex0[0] = u0;
				frostingModel[i][j].tex0[1] = v;

				phi += sideDelta;
			}
			GX_End();

			cosTheta = cosTheta1;
			sinTheta = sinTheta1;
			theta = theta1;
		}
	} else {
		for (int i = 0; i < DONUT_RINGS; i++) {
			GX_Begin(filled ? GX_TRIANGLESTRIP : GX_LINESTRIP, GX_VTXFMT0, 2*(DONUT_SIDES + 1));
			for (int j = 0; j <= DONUT_SIDES; j++) {
				GX_Position3f32(frostingModel[i][j].pos1[0],
								frostingModel[i][j].pos1[1],
								frostingModel[i][j].pos1[2]);
				GX_Normal3f32(frostingModel[i][j].nrm1[0],
							  frostingModel[i][j].nrm1[1],
							  frostingModel[i][j].nrm1[2]);
				GX_Color1u32(frostingOptions.col);
				GX_TexCoord2f32(frostingModel[i][j].tex1[0],
								frostingModel[i][j].tex1[1]);

				GX_Position3f32(frostingModel[i][j].pos0[0],
								frostingModel[i][j].pos0[1],
								frostingModel[i][j].pos0[2]);
				GX_Normal3f32(frostingModel[i][j].nrm0[0],
							  frostingModel[i][j].nrm0[1],
							  frostingModel[i][j].nrm0[2]);
				GX_Color1u32(frostingOptions.col);
				GX_TexCoord2f32(frostingModel[i][j].tex0[0],
								frostingModel[i][j].tex0[1]);
			}
			GX_End();
		}
	}
}

static void makeRainbowTex(GRRLIB_texImg *tex, u16 t) {
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

static void genMunchTex(GRRLIB_texImg *tex, u16 t) {
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

void donut_init(void) {
	shapeBuffer = GRRLIB_CreateEmptyTexture(DONUT_WIDTH*2 + DONUT_WIDTH*2 % 4, DONUT_HEIGHT*4);
	donutBuffer = GRRLIB_CreateEmptyTexture(DONUT_WIDTH + DONUT_WIDTH % 4, DONUT_HEIGHT + DONUT_HEIGHT % 4);
	rainbowTex = GRRLIB_CreateEmptyTexture(12, 12);
	greyPixel = GRRLIB_CreateEmptyTexture(1, 1);
	GRRLIB_SetPixelTotexImg(0, 0, greyPixel, 0x808080FF);
	metalTex = GRRLIB_LoadTexturePNG(metal_png);
	tintedMetalTex = genTintedMetalTex();
	spongeTex = GRRLIB_LoadTexturePNG(sponge_png);
	munchTex = GRRLIB_CreateEmptyTexture(128, 128);
	// sprinklesTex = GRRLIB_LoadTexturePNG(sprinkles_png);
}

void donut_free(void) {
	GRRLIB_FreeTexture(shapeBuffer);
	GRRLIB_FreeTexture(donutBuffer);
	GRRLIB_FreeTexture(rainbowTex);
	GRRLIB_FreeTexture(greyPixel);
	GRRLIB_FreeTexture(metalTex);
	GRRLIB_FreeTexture(tintedMetalTex);
	GRRLIB_FreeTexture(spongeTex);
	GRRLIB_FreeTexture(munchTex);
	// GRRLIB_FreeTexture(sprinklesTex);
}

static void set_tex(GRRLIB_texImg *tex, bool reflective) {
	GXTexObj texObj;

	GX_SetNumTexGens(1);
	if (reflective) {
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

void render_frame(f32 A, f32 B, Donut flavor, bool renderingType, bool manual) {
	static Mtx base_orientation = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f}
	};
	static f32 z_rad = 0.0f;

	static bool last_manual = false;

	Mtx model, rot_z, model2, normal;

	s8 stick_x = PAD_StickX(0);
	s8 stick_y = PAD_StickY(0);

	f32 norm_x = 0.0f;
	f32 norm_y = 0.0f;
	f32 c_magnitude = 0.0f;
	f32 c_direction_x = 0.0f;

	if (stick_x > DEADZONE)       norm_x = (f32)(stick_x - DEADZONE) / (STICK_MAX - DEADZONE);
	else if (stick_x < -DEADZONE) norm_x = (f32)(stick_x + DEADZONE) / (STICK_MAX - DEADZONE);

	if (stick_y > DEADZONE)       norm_y = (f32)(stick_y - DEADZONE) / (STICK_MAX - DEADZONE);
	else if (stick_y < -DEADZONE) norm_y = (f32)(stick_y + DEADZONE) / (STICK_MAX - DEADZONE);

	if (norm_x > 1.0f) norm_x = 1.0f; else if (norm_x < -1.0f) norm_x = -1.0f;
	if (norm_y > 1.0f) norm_y = 1.0f; else if (norm_y < -1.0f) norm_y = -1.0f;

#ifdef HW_RVL
	expansion_t exp;
	WPAD_Expansion(0, &exp);

	if (exp.type == WPAD_EXP_CLASSIC) {
		f32 l_mag = exp.classic.ljs.mag;
		f32 l_ang = exp.classic.ljs.ang;

		if (l_mag > DEADZONE) {
			f32 rescaled_l_mag = (l_mag - DEADZONE) / (1.0f - DEADZONE);
			f32 l_ang_rad = DegToRad(l_ang);
			norm_x = rescaled_l_mag*sinf(l_ang_rad);
			norm_y = rescaled_l_mag*cosf(l_ang_rad);
		}

		if (exp.classic.rjs.mag > DEADZONE) {
			c_magnitude = (exp.classic.rjs.mag - DEADZONE) / (1.0f - DEADZONE);
			c_direction_x = sinf(DegToRad(exp.classic.rjs.ang))*c_magnitude;
		}
	}
#endif

	if (manual && !last_manual) {
		guMtxIdentity(base_orientation);
		z_rad = 0.0f;
	}
	last_manual = manual;

	if (c_magnitude == 0.0f) {
		s8 c_stick_x = PAD_SubStickX(0);
		s8 c_stick_y = PAD_SubStickY(0);

		f32 fx = (f32)c_stick_x;
		f32 fy = (f32)c_stick_y;
		f32 gc_c_mag = sqrtf((fx*fx) + (fy*fy));

		if (gc_c_mag > DEADZONE) {
			c_magnitude = (gc_c_mag - DEADZONE) / (STICK_MAX - DEADZONE);
			c_direction_x = (fx / gc_c_mag)*c_magnitude;
		}
	}

	if (c_magnitude > 0.0f) {
		z_rad += c_direction_x*(DegToRad(DONUT_ROTATION_SPEED));
	}

	if (manual) {
		Mtx rot_x, rot_y, incremental_rot;

		guMtxRotDeg(rot_x, 'X', norm_y*DONUT_ROTATION_SPEED);
		guMtxRotDeg(rot_y, 'Y', norm_x*DONUT_ROTATION_SPEED);

		guMtxConcat(rot_x, rot_y, incremental_rot);
		guMtxConcat(base_orientation, incremental_rot, base_orientation);

		guMtxCopy(base_orientation, model);
	} else {
		Mtx auto_rot_x, auto_rot_z;
		guMtxRotRad(auto_rot_x, 'X', A);
		guMtxRotRad(auto_rot_z, 'Z', B);

		guMtxConcat(auto_rot_z, auto_rot_x, model);
		z_rad = 0.0f;
	}

	guMtxIdentity(rot_z);
	guMtxRotRad(rot_z, 'Z', z_rad);
	guMtxConcat(model, rot_z, model2);

	guMtxTransApply(model2, model2, 0.0f, 0.0f, Z_TRANSLATION);
	guMtxConcat(view, model2, model2);

	GX_LoadPosMtxImm(model2, GX_PNMTX0);

	guMtxCopy(model2, normal);
	normal[0][3] = 0.0f;
	normal[1][3] = 0.0f;
	normal[2][3] = 0.0f;
	GX_LoadNrmMtxImm(normal, GX_PNMTX0);
	GX_SetCurrentMtx(GX_PNMTX0);

	guMtxCopy(normal, normal);
	guMtxScaleApply(normal, normal, 0.5f, -0.5f, 0.0f);
	guMtxTransApply(normal, normal, 0.5f, 0.5f, 1.0f);
	GX_LoadTexMtxImm(normal, GX_TEXMTX0, GX_MTX3x4);

	set_tex(greyPixel, false);

	if (renderingType)
		GX_SetChanAmbColor(GX_COLOR0A0, LC_DARKDARKDARK);

	u32 top = RGBA(flavor.top.r, flavor.top.g, flavor.top.b, flavor.top.a);
	u32 bottom = RGBA(flavor.bottom.r, flavor.bottom.g, flavor.bottom.b, flavor.bottom.a);

	GX_SetViewport(0,0, DONUT_WIDTH*2, DONUT_HEIGHT*4, 0, 1);
	GX_SetScissor(0,0, DONUT_WIDTH*2, DONUT_HEIGHT*4);

	draw_donut((DonutOptions){DONUT_MINOR, DONUT_MAJOR, 0xFFFFFFFF}, true);
	if (top != bottom)
		draw_frosting((DonutOptions){DONUT_MINOR, DONUT_MAJOR, 0xFFFFFFFF}, true);

	GRRLIB_Screen2Texture(0, 0, shapeBuffer, true);

	if (renderingType)
		GX_SetChanAmbColor(GX_COLOR0A0, DONUT_LIGHT);

	munch_timer = (munch_timer + 1) % (256*6);
	rainbow_timer = (rainbow_timer + 1) % 48;
	genMunchTex(munchTex, munch_timer);
	makeRainbowTex(rainbowTex, rainbow_timer);

	switch (flavor.texture) {
		case RAINBOW:
			set_tex(rainbowTex, true);
			break;
		case METAL:
			if ((top == 0xFFFFFFFF) && (bottom == 0xFFFFFFFF)) {
				set_tex(metalTex, true);
			} else {
				set_tex(tintedMetalTex, true);
			}
			break;
		case SPONGE:
			set_tex(spongeTex, false);
			break;
		case MUNCH:
			set_tex(munchTex, true);
			break;
		default:
			set_tex(greyPixel, false);
	}

	GX_SetViewport(0,0, DONUT_WIDTH, DONUT_HEIGHT, 0, 1);
	GX_SetScissor(0,0, DONUT_WIDTH, DONUT_HEIGHT);

	draw_donut((DonutOptions){DONUT_MINOR, DONUT_MAJOR, bottom}, true);
	if (top != bottom)
		draw_frosting((DonutOptions){DONUT_MINOR, DONUT_MAJOR, top}, true);

	GRRLIB_Screen2Texture(0, 0, donutBuffer, true);

	char frameBuffer[DONUT_WIDTH*DONUT_HEIGHT*20 + 1];
	char *ptr = frameBuffer;
	s16 last_r = -1, last_g = -1, last_b = -1;
	const char ramp[] = " -:=+<)%}Ics7fnCo3wmSd6VAXUK8R@Q"; // generated with tools/gen.py

	print("\x1b[2H");
	bool nonSpaceCharsPrinted = false;

	for(u8 j = 0; j < DONUT_HEIGHT; j++) {
		for(u8 i = 0; i < DONUT_WIDTH; i++) {
			u32 col = GRRLIB_GetPixelFromtexImg(i, j, donutBuffer);
			if (col & 0x00FFFFFF) {
				u8 r = R(col), g = G(col), b = B(col);

				u16 lutIndex = 0;
				u16 l_avg = 0;

				for(u8 py = 0; py < 4; py++) {
					for(u8 px = 0; px < 2; px++) {
						u8 img_x = (i*2) + px;
						u8 img_y = (j*4) + py;
						u32 shape = GRRLIB_GetPixelFromtexImg(img_x, img_y, shapeBuffer);
						u8 l = G(shape);
						u8 val = (l >> 6) & 0x03;
						u8 shift = (py*2 + px)*2;
						lutIndex |= (val << shift);
						l_avg += l;
					}
				}
				l_avg >>= 6;
				if ((last_r != r) || (last_g != g) || (last_b != b)) {
					ptr = stpcpy(ptr, "\x1b[38;2;");
					ptr = u82Str(ptr, r);
					*ptr++ = ';';
					ptr = u82Str(ptr, g);
					*ptr++ = ';';
					ptr = u82Str(ptr, b);
					*ptr++ = 'm';
					last_r = r;
					last_g = g;
					last_b = b;
				}
				if (renderingType) {
					*ptr++ = ramp[l_avg];
				} else {
					*ptr++ = shape_lut_bin[lutIndex];
				}
				nonSpaceCharsPrinted = true;
			} else {
				*ptr++ = ' ';
			}
		}
	}
	*ptr++ = '\0';
	print(frameBuffer);
	if (!nonSpaceCharsPrinted)
		print("\x1b[0;104;37m" STRING_DONUT_ERROR);
}
