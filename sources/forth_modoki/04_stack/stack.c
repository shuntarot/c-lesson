#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static stack_data_t stack[STACK_SIZE];
static int sp = 0;

int stack_push(const stack_data_t* data) {
   if (sp == STACK_SIZE) {
      printf("Stack is full!\n");
      return 1;
   } else {
      stack[sp++] = *data;
      return 0;
   }
}

int stack_pop(stack_data_t* out_data) {
   if (sp == 0) {
      printf("Stack is empty!\n");
      return 1;
   } else {
      *out_data = stack[--sp];
      return 0;
   }
}

void stack_print(const stack_data_t* const data) {
   if (data->ltype == NUMBER) {
      printf("%d\n", data->u.number);
   } else if (data->ltype == LITERAL_NAME) {
      printf("%s\n", data->u.name);
   }
}

void stack_print_all() {
   printf("---\n");
   for (int i = 0; i < sp; i++) {
      printf("%3d: ", i);
      stack_print(&stack[i]);
   }
}

//
// Test
//

static void stack_clear() {
   sp = 0;
}

static int stack_eq(const stack_data_t* a, const stack_data_t* b) {

   if (a->ltype == b->ltype) {
      if (a->ltype == NUMBER) {
	 return (a->u.number == b->u.number);
      } else if (a->ltype == LITERAL_NAME) {
	 return (strcmp(a->u.name, b->u.name) == 0);
      }
   }
   
   return 0;
}

static void assert_stack_eq(const stack_data_t* actual, const stack_data_t* expect) {
   assert(stack_eq(actual, expect));
}

static void assert_stack_not_eq(const stack_data_t* actual, const stack_data_t* expect) {
   assert(stack_eq(actual, expect) == 0);
}

static void assert_streq(const char* a, const char* b) {
   assert(strcmp(a, b) == 0);
}

static void test_pop_empty() {
   stack_data_t input = {UNKNOWN, {0}};
   int expect = 1;
   
   int actual = stack_pop(&input);
   assert(actual == expect);
   
   stack_clear();
}

static void test_push() {
   stack_data_t input1  = {NUMBER, {1}};
   stack_data_t input2  = {NUMBER, {321}};
   
   stack_data_t expect1 = {NUMBER, {1}};
   stack_data_t expect2 = {NUMBER, {321}};
   
   stack_push(&input1);
   stack_push(&input2);
   stack_print_all();

   stack_data_t actual1 = stack[0];
   stack_data_t actual2 = stack[1];

   assert_stack_eq(&actual1, &expect1);
   assert_stack_eq(&actual2, &expect2);

   stack_clear();
}

static void test_push_name() {
   char* input_name = malloc(sizeof("add"));
   input_name = "add";
   stack_data_t input = {.ltype = LITERAL_NAME, .u.name = input_name};

   char* expect_name = malloc(sizeof("add"));
   expect_name = "add";
   stack_data_t expect = {.ltype = LITERAL_NAME, .u.name = expect_name};

   stack_push(&input);
   stack_print_all();

   stack_data_t actual = stack[0];
   
   assert_stack_eq(&actual, &expect);
   stack_clear();
}

static void test_pop_one() {
   stack_data_t input  = {NUMBER, {555}};
   stack_data_t expect = {NUMBER, {555}};
   stack_data_t actual = {UNKNOWN, {0}};

   assert_stack_not_eq(&actual, &expect);
   
   stack_push(&input);
   stack_print_all();
   stack_pop(&actual);

   assert_stack_eq(&actual, &expect);
   
   stack_clear();
}

static void test_push_mult_name() {
   char* name[2] = {"sub", "Hello-World"};
   
   char* input_name1 = malloc(sizeof(name[0]));
   char* input_name2 = malloc(sizeof(name[1]));
   input_name1 = name[0];
   input_name2 = name[1];
   stack_data_t input1 = {.ltype = LITERAL_NAME, .u.name = input_name1};
   stack_data_t input2 = {.ltype = LITERAL_NAME, .u.name = input_name2};
   stack_data_t input3 = {.ltype = NUMBER, .u.number = 100};

   char* expect_name1 = malloc(sizeof(name[0]));
   char* expect_name2 = malloc(sizeof(name[1]));
   expect_name1 = name[0];
   expect_name2 = name[1];
   stack_data_t expect1 = {.ltype = LITERAL_NAME, .u.name = expect_name1};
   stack_data_t expect2 = {.ltype = LITERAL_NAME, .u.name = expect_name2};
   stack_data_t expect3 = {.ltype = NUMBER, .u.number = 100};
   
   stack_push(&input1);
   stack_push(&input2);
   stack_push(&input3);
   stack_print_all();

   assert_stack_eq(&stack[0], &expect1);
   assert_stack_eq(&stack[1], &expect2);

   stack_data_t actual1 = {UNKNOWN, {0}};
   stack_data_t actual2 = {UNKNOWN, {0}};
   stack_data_t actual3 = {UNKNOWN, {0}};

   assert_stack_not_eq(&actual1, &expect1);
   assert_stack_not_eq(&actual2, &expect2);
   assert_stack_not_eq(&actual3, &expect3);

   stack_pop(&actual3);
   stack_pop(&actual2);
   stack_pop(&actual1);

   assert_stack_eq(&actual1, &expect1);
   assert_stack_eq(&actual2, &expect2);
   assert_stack_eq(&actual3, &expect3);
   
   stack_clear();
}

static void test_push_at_full() {
   int i;
   stack_data_t input  = {NUMBER, {999}};
   int  expect1 = STACK_SIZE;
   int  expect2 = 1;

   for (i = sp; i < STACK_SIZE; i++) {
      stack_push(&input);
   }

   int actual1 = sp;
   assert(actual1 == expect1);
   
   int actual2 = stack_push(&input);
   assert(actual2 == expect2);
   
   stack_clear();
}

static void unit_tests() {
   test_pop_empty();
   test_push();
   test_push_name();
   test_pop_one();
   test_push_mult_name();
   test_push_at_full();
}

int main() {
   unit_tests();
   return 1;
}
