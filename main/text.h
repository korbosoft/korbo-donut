#ifndef TEXT_H
#define TEXT_H

#include <gccore.h>

extern int format_info(const char *prefix, const char *input, char output[83], bool pad);
extern void format_splash(const char *input, char *output);
extern int print(const char *str);
extern char* u82Str(char* buf, u8 n);
#endif
