#include "clesson.h"
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



struct Token {
    enum LexicalType ltype;
    union {
        int number;
        char onechar;
        char *name;
    } u;
};

#define NAME_SIZE 256

static int is_number(const int ch) {
   if ('0' <= ch && ch <= '9')
      return 1;
   else
      return 0;
}

int parse_one(int prev_ch, struct Token *out_token) {
   
   // TODO: Implement here!
   int ch = 0;
      
   if (prev_ch == EOF) {
      prev_ch = cl_getc();
   }

   if (prev_ch == ' ') {
      do {
	 ch = cl_getc();
      } while (ch == ' ');
      
      out_token->ltype = SPACE;
      return ch;
   }
   else if (is_number(prev_ch)) {
      int val = prev_ch - '0';
      do {
	 ch = cl_getc();
	 if (is_number(ch)) {
	    val *= 10;
	    val += ch - '0';
	 }
      } while (is_number(ch));
      
      out_token->ltype = NUMBER;
      out_token->u.number = val;
      return ch;
   }
   else if (ch == '\0') {
      out_token->ltype = END_OF_FILE;
      return EOF;
   }
   
   out_token->ltype = UNKNOWN;
   return EOF;
}


void parser_print_all() {
    int ch = EOF;
    struct Token token = {
        UNKNOWN,
        {0}
    };

    do {
        ch = parse_one(ch, &token);
        if(token.ltype != UNKNOWN) {
            switch(token.ltype) {
                case NUMBER:
                    printf("num: %d\n", token.u.number);
                    break;
                case SPACE:
                    printf("space!\n");
                    break;
                case OPEN_CURLY:
                    printf("Open curly brace '%c'\n", token.u.onechar);
                    break;
                case CLOSE_CURLY:
                    printf("Close curly brace '%c'\n", token.u.onechar);
                    break;
                case EXECUTABLE_NAME:
                    printf("EXECUTABLE_NAME: %s\n", token.u.name);
                    break;
                case LITERAL_NAME:
                    printf("LITERAL_NAME: %s\n", token.u.name);
                    break;

                default:
                    printf("Unknown type %d\n", token.ltype);
                    break;
            }
        }
    }while(ch != EOF);
}





static void test_parse_one_number() {
    char *input = "123";
    int expect = 123;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);

    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == NUMBER);
    assert(expect == token.u.number);
}

static void test_parse_one_empty_should_return_END_OF_FILE() {
    char *input = "";
    int expect = END_OF_FILE;

    struct Token token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect);
}


static void unit_tests() {
    test_parse_one_empty_should_return_END_OF_FILE();
    test_parse_one_number();
}

int main() {
    unit_tests();
    cl_getc_set_src("123 45 add /some { 2 3 add } def");
//    parser_print_all();
    return 1;
}
