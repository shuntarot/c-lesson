#include "clesson.h"

static const char* input = "123 456";
static int         pos   = 0;

void cl_getc_set_src(char* str)
{
    input = str;
    pos   = 0;
}

int cl_getc()
{
    if (strlen(input) == pos)
        return EOF;
    return input[pos++];
}

size_t cl_strlen(const char* s)
{
    size_t i = 0;
    while (s[i])
        i++;
    return i;
}

char* cl_strncpy(char* dest, const char* src, size_t n)
{
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    //   printf("cl_strncpy %s size %d\n", dest, (int)n);
    return dest;
}

int cl_strcmp(const char* s1, const char* s2)
{
    int i = 0;
    while (s1[i] != 0 && s2[i] != 0) {
        if (s1[i] > s2[i])
            return 1;
        else if (s1[i] < s2[i])
            return -1;
        else
            i++;
    }
    return 0;
}
