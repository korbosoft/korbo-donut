#include "file.h"

#include <dvm.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int dvmInitResult;

FileList* create_file_list(void) {
	FileList *list = malloc(sizeof(FileList));
	if (!list) return NULL;
	list->capacity = 16;
	list->count = 0;
	list->filenames = malloc(list->capacity * sizeof(char *));
	if (!list->filenames) {
		free(list);
		return NULL;
	}
	return list;
}

static int sort_files(const void *a, const void *b) {
	const char *str1 = *(const char **)a;
	const char *str2 = *(const char **)b;
	return strcmp(str1, str2);
}

void add_file_to_list(FileList *list, const char *filename) {
	if (list->count >= list->capacity) {
		size_t new_capacity = list->capacity * 2;
		char **temp = realloc(list->filenames, new_capacity * sizeof(char *));
		if (!temp) return;
		list->filenames = temp;
		list->capacity = new_capacity;
	}
	list->filenames[list->count] = strdup(filename);
	list->count++;
}

void free_file_list(FileList *list) {
	if (!list) return;
	for (int i = 0; i < list->count; i++) {
		free(list->filenames[i]);
	}
	free(list->filenames);
	free(list);
}

void file_init(void) {
	dvmInitResult = dvmInitDefault();
}

int file_find(FileList *list, const char *path, const char *pattern, const int flags) {
	int count = 0;
	if (dvmInitResult) {
		DIR *dir = opendir(path);
		if (dir != NULL) {
			struct dirent *entry;
			while ((entry = readdir(dir)) != NULL) {
				if (fnmatch(pattern, entry->d_name, flags) == 0) {
					if (entry->d_type == DT_DIR)
						continue;

					char full_path[512];
					int written = snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
					if (written < 0 || (size_t)written >= sizeof(full_path)) {
						continue;
					}

					add_file_to_list(list, full_path);
					count++;
				}
			}
			closedir(dir);

			if (list->count > 0) {
				qsort(list->filenames, list->count, sizeof(char *), sort_files);
			}
		}
	}
	return count;
}

FILE *file_open(const char *filename, const char *mode) {
	if (dvmInitResult) {
		return fopen(filename, mode);
	}
	return NULL;
}

int file_close(FILE *stream) {
	if (dvmInitResult) {
		return fclose(stream);
	}
	return -1;
}

bool file_exists(const char *filename) {
	FILE *f = file_open(filename, "r");
	if (f) {
		file_close(f);
		return true;
	}
	return false;
}
