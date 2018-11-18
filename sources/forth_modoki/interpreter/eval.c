#include "clesson.h"

static void add_op() {
   Token_t rs;
   Token_t rt;
   Token_t rd;

   stack_pop(&rs);
   stack_pop(&rt);
   rd.ltype = NUMBER;
   rd.u.number = rs.u.number + rt.u.number;
   stack_push(&rd);
}

void eval() {
   int ch = EOF;
   Token_t token = {UNKNOWN, {0}};
   Token_t token_t1 = {UNKNOWN, {0}};
   Token_t token_t2 = {UNKNOWN, {0}};

   do {
      ch = parse_one(ch, &token);
      if (token.ltype != UNKNOWN) {

         switch (token.ltype) {
            case NUMBER:
               /* printf("NUMBER: %d\n", token.u.number); */
               stack_push(&token);
               break;
            case EXECUTABLE_NAME:
               /* printf("EXECUTABLE_NAME: %s\n", token.u.name); */
               if (streq(token.u.name, "add")) {
                  add_op();
                  /* stack_pop(&token_t1); */
                  /* stack_pop(&token_t2); */
                  /* token.ltype = NUMBER; */
                  /* token.u.number = token_t1.u.number + token_t2.u.number; */
                  /* stack_push(&token); */
               } else if (streq(token.u.name, "def")) {
                  stack_pop(&token_t1);
                  stack_pop(&token_t2);
                  dict_put(token_t2.u.name, &token_t1);
               } else if (dict_get(token.u.name, &token)) {
                  stack_push(&token);
               } else {
                  stack_push(&token);
               }
               break;
            case LITERAL_NAME:
               /* printf("LITERAL_NAME: %s\n", token.u.name); */
               stack_push(&token);
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

static void assert_token_number(int expect, Token_t *actual) {
   assert(actual->ltype == NUMBER);
   assert(actual->u.number == expect);
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

//
// main
//

int main() {
    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_literal();

    /* cl_getc_set_src("1 2 3 add add 4 5 6 7 8 9 add add add add add add"); */
    cl_getc_set_src("/foo 55 def /bar 11 def 1 foo add bar add");

    eval();
    Token_t token = {UNKNOWN, {0}};
    stack_pop(&token);
    printf("result: %d\n", token.u.number);

    return 1;
}
