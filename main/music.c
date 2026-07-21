#include <stdlib.h>
#include <ogc/conf.h>
#include <string.h>
#include <grrmod.h>

#include <aesndlib.h>

#include "music.h"

#include "file.h"
#include "text.h"

#include "music_it.h"

#define FILETYPE_COUNT 62

static bool paused = false;
static u8 *module;

static const char *moduleTypes[FILETYPE_COUNT] = {
	[0] = "s3m",
	[1] = "xm",
	[2] = "it",
	[3] = "mptm",
	[4] = "667",
	[5] = "669",
	[6] = "amf",
	[7] = "dmf",
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
	[22] = "smod",
	[23] = "fmt",
	[24] = "ftm",
	[25] = "gdm",
	[26] = "gmc",
	[27] = "gtk",
	[28] = "gt2",
	[29] = "ice",
	[30] = "st26",
	[31] = "imf",
	[32] = "ims",
	[33] = "j2b",
	[34] = "m15",
	[35] = "stk",
	[36] = "mdl",
	[37] = "med",
#ifdef HW_RVL
	[38] = "mo3",
#else
	[38] = "",
#endif
	[39] = "mod",
	[40] = "mt2",
	[41] = "mtm",
	[42] = "mus",
	[43] = "okt",
	[44] = "oxm",
	[45] = "psm",
	[46] = "plm",
	[47] = "pt36",
	[48] = "ptm",
	[49] = "puma",
	[50] = "rtm",
	[51] = "sfx",
	[52] = "sfx2",
	[53] = "mms",
	[54] = "stm",
	[55] = "stx",
	[56] = "stp",
	[57] = "symmod",
	[58] = "ult",
	[59] = "umx",
	[60] = "wow",
	[61] = "xmf"
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
