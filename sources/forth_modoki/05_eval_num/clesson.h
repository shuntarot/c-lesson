#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum LexicalType {
    NUMBER,
    SPACE,
    EXECUTABLE_NAME,
    LITERAL_NAME,
    OPEN_CURLY,
    CLOSE_CURLY,
    END_OF_FILE,
    UNKNOWN
};

typedef struct Token {
    enum LexicalType ltype;
    union {
        int number;
        char onechar;
        char *name;
    } u;
} Token_t;

#define STACK_SIZE 1024
#define NAME_SIZE  256

int stack_push(const Token_t* data);

int stack_pop(Token_t* out_data);

void stack_print(const Token_t* const data);

void stack_print_all();

/*
return one character and move cursor.
return EOF if end of file.
*/
int cl_getc();

void cl_getc_set_src(char* str);

size_t cl_strlen(const char *s);

char* cl_strncpy(char *dest, const char *src, size_t n);

int cl_strcmp(const char *s1, const char *s2);
