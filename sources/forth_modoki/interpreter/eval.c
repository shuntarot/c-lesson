#include "clesson.h"

//
// c function
//

static void def_op() {
   Token_t rs;
   Token_t rt;

   stack_pop(&rs);
   stack_pop(&rt);
   dict_put(rt.u.name, &rs);
}

#define two_op(op)                              \
   Token_t rs;                                  \
   Token_t rt;                                  \
   Token_t rd;                                  \
   stack_pop(&rs);                              \
   stack_pop(&rt);                              \
   rd.ltype = NUMBER;                           \
   rd.u.number = rt.u.number op rs.u.number;    \
   stack_push(&rd)

static void add_op() {
   two_op( + );
}

static void sub_op() {
   two_op( - );
}

static void mul_op() {
   two_op( * );
}

static void div_op() {
   two_op( / );
}

static void register_primitives() {
   Token_t def = {.ltype=ELEMENT_C_FUNC, .u.cfunc=def_op};
   dict_put("def", &def);

   Token_t add = {.ltype=ELEMENT_C_FUNC, .u.cfunc=add_op};
   dict_put("add", &add);

   Token_t sub = {.ltype=ELEMENT_C_FUNC, .u.cfunc=sub_op};
   dict_put("sub", &sub);

   Token_t mul = {.ltype=ELEMENT_C_FUNC, .u.cfunc=mul_op};
   dict_put("mul", &mul);

   Token_t div = {.ltype=ELEMENT_C_FUNC, .u.cfunc=div_op};
   dict_put("div", &div);
}

#define MAX_NAME_OP_NUMBERS 256
static struct TokenArray* compile_exec_array(int prev_ch) {
   int ch = prev_ch;
   int num_token = 0;
   Token_t token = {UNKNOWN, {0}};
   Token_t arr[MAX_NAME_OP_NUMBERS];
   struct TokenArray* ta;

   for (int i = 0; i < MAX_NAME_OP_NUMBERS; i++) {
      ch = parse_one(ch, &token);
      if (token.ltype == CLOSE_CURLY) {
         num_token = i;
         break;
      }
      arr[i] = token;
   }
   ta = malloc(sizeof(struct TokenArray) + sizeof(Token_t) * num_token);
   ta->len = num_token;
   for (int i = 0; i < num_token; i++) {
      ta->token[i] = arr[i];
   }
   return ta;
}

//
// eval
//

void eval() {
   int ch = EOF;
   Token_t token = {UNKNOWN, {0}};
   struct TokenArray* exec_arr;

   do {
      ch = parse_one(ch, &token);
      if (token.ltype != UNKNOWN) {

         switch (token.ltype) {
            case NUMBER:
               /* printf("NUMBER: %d\n", token.u.number); */
               stack_push(&token);
               break;
            case ELEMENT_C_FUNC:
               /* printf("ELEMENT_C_FUNC\n"); */
               token.u.cfunc();
               break;
            case EXECUTABLE_NAME:
               /* printf("EXECUTABLE_NAME: %s\n", token.u.name); */
               stack_push(&token);
               break;
            case LITERAL_NAME:
               /* printf("LITERAL_NAME: %s\n", token.u.name); */
               stack_push(&token);
               break;
            case OPEN_CURLY:
               exec_arr = compile_exec_array(ch);
               token.ltype = EXEC_ARRAY;
               token.u.bytecodes = exec_arr;
               stack_push(&token);
               ch = CLOSE_CURLY;
               break;
            default:
               /* printf("Unknown type %d\n", token.ltype); */
               break;
         }
      }
   } while(ch != EOF);
}

//
// Unit test
//

static void assert_token_number(int expect, Token_t* actual) {
   assert(actual->ltype == NUMBER);
   assert(actual->u.number == expect);
}

static void assert_token_array(struct TokenArray* expect, struct TokenArray* actual);
static void assert_token(Token_t expect, Token_t actual) {
   assert(expect.ltype == actual.ltype);
   if (actual.ltype == NUMBER) {
      assert(expect.u.number == actual.u.number);
   } else if (actual.ltype == EXECUTABLE_NAME || actual.ltype == LITERAL_NAME) {
      assert(strcmp(expect.u.name, actual.u.name) == 0);
   } else if (actual.ltype == EXEC_ARRAY) {
      assert_token_array(expect.u.bytecodes, actual.u.bytecodes);
   }
}

static void assert_token_array(struct TokenArray* expect, struct TokenArray* actual) {
   for (int i = 0; i < actual->len; i++) {
      assert_token(expect->token[i], actual->token[i]);
   }
}

static void test_eval_num_one() {
   char *input = "123";
   int expect = 123;

   cl_getc_set_src(input);

   eval();

   Token_t actual = {UNKNOWN, {0}};
   stack_pop(&actual);
   assert_token_number(expect, &actual);

   stack_clear();
}

static void test_eval_num_two() {
   char *input = "123 456";
   int expect1 = 456;
   int expect2 = 123;

   cl_getc_set_src(input);

   eval();

   Token_t actual1 = {UNKNOWN, {0}};
   Token_t actual2 = {UNKNOWN, {0}};
   stack_pop(&actual1);
   stack_pop(&actual2);
   assert_token_number(expect1, &actual1);
   assert_token_number(expect2, &actual2);

   stack_clear();
}


static void test_eval_num_add() {
   char *input = "1 2 add";
   int expect = 3;

   cl_getc_set_src(input);

   eval();

   Token_t actual = {UNKNOWN, {0}};
   stack_pop(&actual);
   assert_token_number(expect, &actual);

   stack_clear();
}

static void test_eval_literal() {
   char *input = "/hoge 432 def hoge";
   int expect = 432;

   cl_getc_set_src(input);

   eval();

   Token_t actual = {UNKNOWN, {0}};
   stack_pop(&actual);
   assert_token_number(expect, &actual);

   stack_clear();
}

static void test_eval_num_sub() {
   char *input = "5 3 sub";
   int expect = 2;

   cl_getc_set_src(input);

   eval();

   Token_t actual = {UNKNOWN, {0}};
   stack_pop(&actual);
   assert_token_number(expect, &actual);

   stack_clear();
}

static void test_eval_num_mul() {
   char *input = "3 8 mul";
   int expect = 24;

   cl_getc_set_src(input);

   eval();

   Token_t actual = {UNKNOWN, {0}};
   stack_pop(&actual);
   assert_token_number(expect, &actual);

   stack_clear();
}

static void test_eval_num_div() {
   char *input = "8 3 div";
   int expect = 2;

   cl_getc_set_src(input);

   eval();

   Token_t actual = {UNKNOWN, {0}};
   stack_pop(&actual);
   assert_token_number(expect, &actual);

   stack_clear();
}

static void test_eval_exec_array() {
   char *input = "{1}";
   struct TokenArray* bc = malloc(sizeof(struct TokenArray) + sizeof(Token_t) * 1);
   bc->len = 1;
   Token_t t0 = {NUMBER, {1}};
   bc->token[0] = t0;
   Token_t expect = {.ltype=EXEC_ARRAY, .u.bytecodes=bc};

   cl_getc_set_src(input);

   eval();

   Token_t actual = {UNKNOWN, {0}};
   stack_pop(&actual);
   assert_token(expect, actual);

   stack_clear();
}

//
// main
//

int main() {
   register_primitives();
   test_eval_num_one();
   test_eval_num_two();
   test_eval_num_add();
   test_eval_literal();
   test_eval_num_sub();
   test_eval_num_mul();
   test_eval_num_div();
   test_eval_exec_array();

   /* cl_getc_set_src("1 2 3 add add 4 5 6 7 8 9 add add add add add add"); */
   cl_getc_set_src("/foo 55 def /bar 11 def 1 foo add bar add 1 sub 11 div");

   eval();
   Token_t token = {UNKNOWN, {0}};
   stack_pop(&token);
   printf("result: %d\n", token.u.number);

   return 1;
}
