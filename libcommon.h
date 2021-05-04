//
// libcommon.h - application/library common header
//
//

// requires: #include <stdarg.h>

// printf is normally defined in stdio.h
void minprintf(char *fmt, ...);

#ifndef USE_STDIO_H
#define NULL 0
#define EOF (-1)
void* memmove(void *dst, const void *src, int n);
void* memcpy(void *dst, const void *src, int n);
int putchar(int c);
int getchar();

// char *fgets(char *str, int size, FILE *stream); // XXX removed FILE* type for now
char *fgets(char *str, int size, void *stream);

#endif


#ifndef USE_STDLIB_H
long strtol(const char *s, char **ptr, int base);
int atoi(const char *s);
#endif

#ifndef USE_CTYPE_H
// normally defined by ctype.h, see below */
int isdigit(int c);
int toupper(int c);
int tolower(int c);
int isprint(int c);
int isspace(int c);
#endif
