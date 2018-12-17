#include "clesson.h"

static int is_number(const int ch) {
   if ('0' <= ch && ch <= '9')
      return 1;
   else
      return 0;
}

static int is_alpha(const int ch) {
   if (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'))
      return 1;
   else
      return 0;
}

static int is_alnum(const int ch) {
   return is_alpha(ch) || is_number(ch);
}

int parse_one(int prev_ch, Token_t *out_token) {

   // TODO: Implement here!
   int ch = 0;
   char name[NAME_SIZE];

   if (prev_ch == EOF) {
      prev_ch = cl_getc();
   }

   if (is_number(prev_ch)) {
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

   if (prev_ch == ' ') {
      do {
         ch = cl_getc();
      } while (ch == ' ');

      out_token->ltype = SPACE;
      return ch;
   }

   if (is_alpha(prev_ch)) {
      int i = 0;
      name[i++] = prev_ch;
      while (is_alnum(ch = cl_getc())) {
         name[i++] = ch;
      }
      name[i] = '\0';
      if (dict_get(name, out_token)) {
	      ; // set ELEMENT_C_FUNC
      } else {
	      out_token->ltype = EXECUTABLE_NAME;
	      out_token->u.name = malloc(i);
	      cl_strncpy(out_token->u.name, name, i);
      }
      return ch;
   }

   if (prev_ch == '/') {
      int i = 0;
      while (is_alnum(ch = cl_getc())) {
         name[i++] = ch;
      }
      name[i] = '\0';
      out_token->ltype = LITERAL_NAME;
      out_token->u.name = malloc(i);
      cl_strncpy(out_token->u.name, name, i);
      return ch;
   }

   if (prev_ch == '{') {
      out_token->ltype = OPEN_CURLY;
      out_token->u.onechar = '{';
      return cl_getc();
   }

   if (prev_ch == '}') {
      out_token->ltype = CLOSE_CURLY;
      out_token->u.onechar = '}';
      return cl_getc();
   }

   if (prev_ch == EOF) {
      out_token->ltype = END_OF_FILE;
      return EOF;
   }

   out_token->ltype = UNKNOWN;
   return EOF;
}


void parser_print_all() {
    int ch = EOF;
    Token_t token = {
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


//
// test
//
#ifdef TEST_PARSER

static void local_stack_clear() {
   char dummy[1024] = {' '};
   /* printf("clear stack %x\n", dummy[1023]); */
}

int streq(const char *s1, const char *s2) {
   return (cl_strcmp(s1, s2) == 0);
}

static void assert_streq(const char *s1, const char *s2) {
   assert(streq(s1, s2));
}

static void test_parse_one_number() {
    char *input = "123";
    int expect = 123;

    Token_t token = {UNKNOWN, {0}};
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

    Token_t token = {UNKNOWN, {0}};
    int ch;

    cl_getc_set_src(input);
    ch = parse_one(EOF, &token);

    assert(ch == EOF);
    assert(token.ltype == expect);
}

static void test_parse_one_executable_name() {
   char* input = "add";
   char* expect_name = "add";
   int expect_type = EXECUTABLE_NAME;

   Token_t token = {UNKNOWN, {0}};
   int ch;

   cl_getc_set_src(input);
   ch = parse_one(EOF, &token);

   assert(ch == EOF);
   assert(token.ltype == expect_type);
   assert_streq(token.u.name, expect_name);
}

static void test_parse_one_literal_name() {
   char* input = "/add";
   char* expect_name = "add";
   int expect_type = LITERAL_NAME;

   Token_t token = {UNKNOWN, {0}};
   int ch;

   cl_getc_set_src(input);
   ch = parse_one(EOF, &token);

   assert(ch == EOF);
   assert(token.ltype == expect_type);
   assert_streq(token.u.name, expect_name);
}

static void test_cl_strcmp() {
   assert(cl_strcmp("abc", "z"  ) == strcmp("abc", "z"  ));
   assert(cl_strcmp("abc", "abc") == strcmp("abc", "abc"));
   assert(cl_strcmp("z"  , "abc") == strcmp("z"  , "abc"));
}

static void unit_tests() {
   local_stack_clear();
   test_parse_one_empty_should_return_END_OF_FILE();
   test_parse_one_number();
   test_parse_one_executable_name();
   test_parse_one_literal_name();
   test_cl_strcmp();
}

int main() {
   unit_tests();
   cl_getc_set_src("123 45 add /some { 2 3 add } def");
   /* parser_print_all(); */
   return 1;
}
#endif
