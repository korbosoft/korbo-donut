#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <fnmatch.h>

typedef struct {
	char **filenames;
	int count;
	size_t capacity;
} FileList;

extern FileList* create_file_list(void);
extern void add_file_to_list(FileList *list, const char *filename);
extern void free_file_list(FileList *list);
extern void file_init(void);
extern int file_find(FileList *list, const char *path, const char *pattern, const int flags);
extern FILE *file_open(const char *filename, const char *mode);
extern int file_close(FILE *stream);
extern bool file_exists(const char *filename);

#endif
