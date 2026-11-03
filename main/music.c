#include <stdlib.h>
#include <ogc/conf.h>
#include <string.h>
#include <grrmod-xmp.h>

#include <aesndlib.h>

#include "music.h"

#include "file.h"
#include "text.h"

const unsigned char music[] = {
	#embed "music.it"
};

static bool paused = false;
static u8 *module = nullptr;
static FileList *list = nullptr;
static char *title_pointer;
static int currentSong = 0;

static bool is_title_empty(const char *title) {
	if (title[0] == '\0') return true;

	const int length = strlen(title) - 1;
	bool isAllSpaces = false;
	for (int i = 0; i < length; i++) {
		isAllSpaces |= (title[i] == ' ');
	}
	return isAllSpaces;
}

void music_play(char *filename) {
	FILE *f = nullptr;
	if (filename)
		f = file_open(filename, "rb");

	if (f) {
		fseek(f, 0, SEEK_END);
		size_t moduleSize = ftell(f);
		rewind(f);

		if (module)
			free(module);

		module = (u8 *)malloc(moduleSize);

		fread(module, 1, moduleSize, f);

		GRRMOD_SetMOD(module,moduleSize);
		file_close(f);
	} else {
		GRRMOD_SetMOD(music, sizeof(music));
		currentSong = -1;
	}

	char *title = GRRMOD_GetSongTitle();
	format_info("Song: ", is_title_empty(title) ? "Untitled" : title, title_pointer, true);

	GRRMOD_Stop();
	GRRMOD_Start();
}

void music_init(char *title_display) {
	title_pointer = title_display;

	list = create_file_list();

	file_find(list, "music/", "*", FNM_PATHNAME);

	GRRMOD_Init(SYS_GetSoundMode()); // check for mono

	if (list->filenames) {
		if (list->filenames[0]) {
			music_play(list->filenames[0]);
		} else { music_play(nullptr); }
	} else { music_play(nullptr); }
}

void music_tick() {
	if (GRRMOD_GetLoopCount() > 0) {
		if (currentSong != -1) {
			if (list) {
				if (++currentSong >= list->count)
					currentSong = 0;
				if (list->filenames) {
					if (list->filenames[0])
						music_play(list->filenames[currentSong]);
				}
			}
		}
	}
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
