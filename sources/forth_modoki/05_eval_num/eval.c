#include "clesson.h"

void eval() {
   int ch = EOF;
   Token_t token = {UNKNOWN, {0}};

   do {
      ch = parse_one(ch, &token);
      if (token.ltype != UNKNOWN) {

         switch (token.ltype) {
            case NUMBER:
               /* printf("num: %d\n", token.u.number); */
               stack_push(&token);
               break;

            default:
               /* printf("Unknown type %d\n", token.ltype); */
               break;
         }
      }
   } while(ch != EOF);
}

static void test_eval_num_one() {
    char *input = "123";
    int expect = 123;

    cl_getc_set_src(input);

    eval();

    /* TODO: write code to pop stack top element */
    Token_t token = {UNKNOWN, {0}};
    assert(stack_pop(&token) == 1);
    int actual = token.u.number;

    assert(expect == actual);
    stack_clear();
}

static void test_eval_num_two() {
    char *input = "123 456";
    int expect1 = 456;
    int expect2 = 123;

    cl_getc_set_src(input);

    eval();

    /* TODO: write code to pop stack top and second top element */
    Token_t token = {UNKNOWN, {0}};

    assert(stack_pop(&token) == 1);
    int actual1 = token.u.number;
    assert(stack_pop(&token) == 1);
    int actual2 = token.u.number;

    assert(expect1 == actual1);
    assert(expect2 == actual2);
    stack_clear();
}


static void test_eval_num_add() {
    char *input = "1 2 add";
    int expect = 3;

    cl_getc_set_src(input);

    eval();

    /* TODO: write code to pop stack top element */
    int actual = 0;
    assert(expect == actual);
}


int main() {
    test_eval_num_one();
    test_eval_num_two();
    /* test_eval_num_add(); */
    return 1;
}
