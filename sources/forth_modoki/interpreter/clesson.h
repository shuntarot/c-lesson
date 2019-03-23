#include <assert.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 1024
#define NAME_SIZE 256
#define TABLE_SIZE 16

enum LexicalType {
    NUMBER,
    SPACE,
    EXECUTABLE_NAME,
    LITERAL_NAME,
    OPEN_CURLY,
    CLOSE_CURLY,
    END_OF_FILE,
    ELEMENT_C_FUNC,
    EXEC_ARRAY,
    UNKNOWN
};

typedef struct Token {
    enum LexicalType ltype;
    union {
        int   number;
        char  onechar;
        char* name;
        void (*cfunc)();
        struct TokenArray* bytecodes;
    } u;
} Token_t;

struct TokenArray {
    int     len;
    Token_t token[0];
};

typedef struct Node {
    char*        key;
    Token_t      value;
    struct Node* next;
} Node_t;

//
// stack
//

int stack_push(const Token_t* data);

int stack_pop(Token_t* out_data);

void stack_print(const Token_t* const data);

void stack_print_all();

void stack_clear();

//
// dictionary
//

void dict_put(char* key, Token_t* elem);

int dict_get(char* key, Token_t* out_elem);

void dict_print_all();

//
// parse
//

int parse_one(int prev_ch, Token_t* out_token);

int streq(const char* s1, const char* s2);

/*
  return one character and move cursor.
  return EOF if end of file.
*/
int cl_getc();

void cl_getc_set_src(char* str);

size_t cl_strlen(const char* s);

char* cl_strncpy(char* dest, const char* src, size_t n);
