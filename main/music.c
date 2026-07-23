#include <stdlib.h>
#include <ogc/conf.h>
#include <string.h>
#include <grrmod.h>

#include <aesndlib.h>

#include "music.h"

#include "file.h"
#include "text.h"

#include "music_it.h"

#define FILETYPE_COUNT 68

static bool paused = false;
static u8 *module;

static const char *moduleTypes[FILETYPE_COUNT] = {
	[0] = "mptm",
	[1] = "mod",
	[2] = "s3m",
	[3] = "xm",
	[4] = "it",
	[5] = "667",
	[6] = "669",
	[7] = "amf",
	[8] = "ams",
	[9] = "c67",
	[10] = "cba",
	[11] = "dbm",
	[12] = "digi",
	[13] = "dmf",
	[14] = "dsm",
	[15] = "dsym",
	[16] = "dtm",
	[17] = "etx",
	[18] = "far",
	[19] = "fc",
	[20] = "fc13",
	[21] = "fc14",
	[22] = "fmt",
	[23] = "fst",
	[24] = "ftm",
	[25] = "imf",
	[26] = "ims",
	[27] = "ice",
	[28] = "j2b",
	[29] = "m15",
	[30] = "mdl",
	[31] = "med",
	[32] = "mms",
	[33] = "mt2",
	[34] = "mtm",
	[35] = "mus",
	[36] = "nst",
	[37] = "okt",
	[38] = "plm",
	[39] = "psm",
	[40] = "pt36",
	[41] = "ptm",
	[42] = "puma",
	[43] = "rtm",
	[44] = "sfx",
	[45] = "sfx2",
	[46] = "smod",
	[47] = "st26",
	[48] = "stk",
	[49] = "stm",
	[50] = "stx",
	[51] = "stp",
	[52] = "symmod",
	[53] = "tcb",
	[54] = "gmc",
	[55] = "gtk",
	[56] = "gt2",
	[57] = "ult",
	[58] = "unic",
	[59] = "wow",
	[60] = "xmf",
	[61] = "gdm",
#ifdef HW_RVL
	[62] = "mo3",
#else
	[62] = "",
#endif
	[63] = "oxm",
	[64] = "umx",
	[65] = "xpk",
	[66] = "ppm",
	[67] = "mmcmp"
};

static bool is_title_empty(const char *title) {
	if (title[0] == '\0') return true;

	const int length = strlen(title) - 1;
	int i;
	bool isAllSpaces = false;
	for (i = 0; i < length; i++) {
		isAllSpaces |= (title[i] == ' ');
	}
	return isAllSpaces;
}

static bool music_attempt(const char *type) {
	char tmp[13] = "music.";
	strcat(tmp, type);
	return file_exists(tmp);
}

int music_init(char *title_display) {
	char tmp[13] = "music.";
	u8 i;

	for (i = 0; i < FILETYPE_COUNT; i++) {
		if (music_attempt(moduleTypes[i])) {
			strcat(tmp, moduleTypes[i]);
			break;
		}
	}

	FILE *f;
	f = file_open(tmp, "rb");

	GRRMOD_Init(SYS_GetSoundMode()); // check for mono
	if (f) {
		fseek(f, 0, SEEK_END);
		long module_size = ftell(f);
		rewind(f);

		module = (u8 *)malloc(module_size);

		fread(module, 1, module_size, f);

		GRRMOD_SetMOD(module,module_size);
		file_close(f);
	} else {
		GRRMOD_SetMOD(music_it, music_it_size);
		strcat(tmp, "it"); // built-in song should always have a title but whatever
	}

	char *title = GRRMOD_GetSongTitle();
	format_info("Song: ", is_title_empty(title) ? tmp : title, title_display, true);
	// format_info("Song: ", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", title_display);

	GRRMOD_Start();
	return strlen(title_display);
}

void music_free(void) {
	if (module)
		free(module);
	GRRMOD_End();
}

void music_pause(const bool pause) {
	if (pause != paused) {
		GRRMOD_Pause();
		paused = !paused;
	}
}
