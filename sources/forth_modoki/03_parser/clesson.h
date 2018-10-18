#include <stdio.h>

/*
return one character and move cursor.
return EOF if end of file.
*/
int cl_getc();

void cl_getc_set_src(char* str);

size_t cl_strlen(const char *s);

char* cl_strncpy(char *dest, const char *src, size_t n);

int cl_strcmp(const char *s1, const char *s2);
