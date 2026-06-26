#include <gccore.h>
#include <math.h>
#include <grrlib.h>
#include <stdlib.h>

#include "colors.h"
#include "donut.h"
#include "grrproxy.h"
#include "input.h"
#include "strings.h"
#include "tex.h"
#include "text.h"

#include "shape_lut_bin.h"

static GRRLIB_texImg *shapeBuffer;
static GRRLIB_texImg *donutBuffer;

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
static DonutModel donutModel[DONUT_RINGS][DONUT_SIDES + 1];

static void draw_donut(DonutOptions options, bool filled) {
	if ((donutOptions.major != options.major) ||
		(donutOptions.minor != options.minor) ||
		(donutOptions.col != options.col)) {
		const f32 ringDelta = 2.0 * M_PI / DONUT_RINGS;
		const f32 sideDelta = 2.0 * M_PI / DONUT_SIDES;

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

void donut_init(void) {
	shapeBuffer = GRRLIB_CreateEmptyTexture(DONUT_WIDTH*2 + DONUT_WIDTH*2 % 4, DONUT_HEIGHT*4);
	donutBuffer = GRRLIB_CreateEmptyTexture(DONUT_WIDTH + DONUT_WIDTH % 4, DONUT_HEIGHT + DONUT_HEIGHT % 4);
	tex_init();
}

void donut_free(void) {
	GRRLIB_FreeTexture(shapeBuffer);
	GRRLIB_FreeTexture(donutBuffer);
	tex_free();
}
void render_frame(f32 A, f32 B, donut_t flavor, bool renderingType, bool manual) {
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

	const f32 GC_MAX = 100.0f;

	if (stick_x > DEADZONE)       norm_x = (f32)(stick_x - DEADZONE) / (GC_MAX - DEADZONE);
	else if (stick_x < -DEADZONE) norm_x = (f32)(stick_x + DEADZONE) / (GC_MAX - DEADZONE);

	if (stick_y > DEADZONE)       norm_y = (f32)(stick_y - DEADZONE) / (GC_MAX - DEADZONE);
	else if (stick_y < -DEADZONE) norm_y = (f32)(stick_y + DEADZONE) / (GC_MAX - DEADZONE);

	if (norm_x > 1.0f) norm_x = 1.0f; else if (norm_x < -1.0f) norm_x = -1.0f;
	if (norm_y > 1.0f) norm_y = 1.0f; else if (norm_y < -1.0f) norm_y = -1.0f;

#ifdef HW_RVL
	expansion_t exp;
	WPAD_Expansion(0, &exp);

	if (exp.type == WPAD_EXP_CLASSIC) {
		f32 l_mag = exp.classic.ljs.mag;
		f32 l_ang = exp.classic.ljs.ang;

		if (l_mag > 0.18f) {
			f32 rescaled_l_mag = (l_mag - 0.18f) / (1.0f - 0.18f);
			norm_x = rescaled_l_mag * sinf(l_ang * (M_PI / 180.0f));
			norm_y = rescaled_l_mag * cosf(l_ang * (M_PI / 180.0f));
		}

		if (exp.classic.rjs.mag > 0.20f) {
			c_magnitude = (exp.classic.rjs.mag - 0.20f) / (1.0f - 0.20f);
			c_direction_x = sinf(exp.classic.rjs.ang * (M_PI / 180.0f)) * c_magnitude;
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
		f32 gc_c_mag = sqrtf((fx * fx) + (fy * fy));

		if (gc_c_mag > (f32)DEADZONE) {
			c_magnitude = (gc_c_mag - (f32)DEADZONE) / (GC_MAX - (f32)DEADZONE);
			c_direction_x = (fx / gc_c_mag) * c_magnitude;
		}
	}

	if (c_magnitude > 0.0f) {
		z_rad += c_direction_x * (DONUT_ROTATION_SPEED * (M_PI / 180.0f));
	}

	if (manual) {
		Mtx rot_x, rot_y, incremental_rot;

		guMtxRotDeg(rot_x, 'X', norm_y * DONUT_ROTATION_SPEED);
		guMtxRotDeg(rot_y, 'Y', norm_x * DONUT_ROTATION_SPEED);

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

	guMtxTransApply(model2, model2, 0.0f, 0.0f, -(3.0f / sinf(DegToRad(DONUT_FOV) / 2.0f)));
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

	set_tex(flavors[0]);

	if (renderingType)
		GX_SetChanAmbColor(GX_COLOR0A0, LC_DARKDARKDARK);

	u32 vertex = RGBA(flavor.vertex.r, flavor.vertex.g, flavor.vertex.b, flavor.vertex.a);

	GX_SetViewport(0,0, DONUT_WIDTH*2, DONUT_HEIGHT*4, 0, 1);
	GX_SetScissor(0,0, DONUT_WIDTH*2, DONUT_HEIGHT*4);

	draw_donut((DonutOptions){DONUT_MINOR, DONUT_MAJOR, 0xFFFFFFFF}, true);

	GRRLIB_Screen2Texture(0, 0, shapeBuffer, true);

	if (renderingType)
		GX_SetChanAmbColor(GX_COLOR0A0, DONUT_LIGHT);

	munch_timer = (munch_timer + 1) % (256*6);
	rainbow_timer = (rainbow_timer + 1) % 48;
	genMunchTex(munchTex, munch_timer);
	genRainbowTex(rainbowTex, rainbow_timer, false);
	genRainbowTex(pastelTex, rainbow_timer, true);

	GX_SetViewport(0,0, DONUT_WIDTH, DONUT_HEIGHT, 0, 1);
	GX_SetScissor(0,0, DONUT_WIDTH, DONUT_HEIGHT);

	set_tex(flavor);

	draw_donut((DonutOptions){DONUT_MINOR, DONUT_MAJOR, vertex}, true);

	GRRLIB_Screen2Texture(0, 0, donutBuffer, true);

	char frameBuffer[DONUT_WIDTH*DONUT_HEIGHT*20 + 1];
	char *ptr = frameBuffer;
	s16 last_r = -1, last_g = -1, last_b = -1;
	const char ramp[] = " -:=+<)%}Ics7fnCo3wmSd6VAXUK8R@Q"; // generated with tools/gen.py

	print("\x1b[2H");
	bool nonSpaceCharsPrinted = false;
	for(u8 j = 0; j < DONUT_HEIGHT; j++) {
		for(u8 i = 0; i < DONUT_WIDTH; i++) {
			u16 lutIndex = 0;
			u16 l_avg = 0;
			u32 col = GRRLIB_GetPixelFromtexImg(i, j, donutBuffer);
			u8 r = R(col), g = G(col), b = B(col);

			for(u8 py = 0; py < 4; py++) {
				for(u8 px = 0; px < 2; px++) {
					u8 img_x = (i*2) + px;
					u8 img_y = (j*4) + py;

					u32 shape = GRRLIB_GetPixelFromtexImg(img_x, img_y, shapeBuffer);

					u8 l = G(shape);
					// no luminance check needed because shape buffer should always be greyscale
					u8 val = (l >> 6) & 0x03;
					u8 shift = (py*2 + px)*2;
					lutIndex |= (val << shift);

					l_avg += l;
				}
			}
			// average
			l_avg >>= 6;
			if (r | g | b) {
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
