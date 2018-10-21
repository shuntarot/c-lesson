#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static stack_data_t* stack[STACK_SIZE];
static int sp = 0;

void stack_push(stack_data_t* data) {
   if (sp == STACK_SIZE) {
      printf("Stack is full!\n");
   } else {
      stack[sp++] = data;
   }
}

void stack_pop(stack_data_t* out_data) {
   if (sp == 0) {
      printf("Stack is empty!\n");
   } else {
      out_data = stack[sp--];
   }
}

void stack_print(stack_data_t* data) {
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
      stack_print(stack[i]);
   }
}

static void assert_streq(const char* a, const char* b) {
   assert(strcmp(a, b) == 0);
}

static void test_pop_empty() {
   stack_pop(NULL);
   assert(sp == 0);
}

static void test_push() {
   stack_data_t* a = malloc(sizeof(stack_data_t));
   stack_data_t* b = malloc(sizeof(stack_data_t));

   a->ltype = NUMBER;
   a->u.number = 1;
   b->ltype = NUMBER;
   b->u.number = 321;

   stack_push(a);
   stack_push(b);
   assert(sp == 2);
   assert(stack[0]->ltype == NUMBER);
   assert(stack[1]->ltype == NUMBER);
}

static void test_pop_one() {
   stack_data_t* a;
   int old_sp = sp;
   stack_pop(a);
   if (old_sp == 0) {
      assert(old_sp == sp);
   } else {
      assert(old_sp == (sp + 1));
   }
}

static void test_push_name() {
   stack_data_t* a = malloc(sizeof(stack_data_t));
   char* name = malloc(sizeof("add"));
   name = "add";
   a->ltype = LITERAL_NAME;
   a->u.name = name;
   stack_push(a);
}

static void test_push_mult_name() {
   stack_data_t* a = malloc(sizeof(stack_data_t));
   stack_data_t* b = malloc(sizeof(stack_data_t));
   stack_data_t* c = malloc(sizeof(stack_data_t));

   char* name_a = malloc(sizeof("sub"));
   char* name_b = malloc(sizeof("Hello-World"));
   name_a = "sub";
   name_b = "Hello-World";

   a->ltype = LITERAL_NAME;
   a->u.name = name_a;
   b->ltype = LITERAL_NAME;
   b->u.name = name_b;
   c->ltype = NUMBER;
   c->u.number = 5555;

   stack_push(a);
   stack_push(b);
   stack_push(c);

   assert_streq(stack[sp-2]->u.name, name_b);
   assert_streq(stack[sp-3]->u.name, name_a);
}

static void test_push_at_full() {
   int i;
   stack_data_t* a = malloc(sizeof(stack_data_t));
   a->ltype = NUMBER;
   a->u.number = 999;

   for (i = sp; i < STACK_SIZE; i++) {
      stack_push(a);
   }

   assert(sp == STACK_SIZE);
   stack_push(a);
   assert(sp == STACK_SIZE);
}

static void unit_tests() {
   test_pop_empty();
   test_push();
   stack_print_all();
   test_pop_one();
   stack_print_all();
   test_push_name();
   stack_print_all();
   test_pop_one();
   stack_print_all();
   test_pop_one();
   test_push_mult_name();
   stack_print_all();
   test_pop_one();
   test_pop_one();
   test_pop_one();
   stack_print_all();
   test_push_at_full();
}

int main() {
   unit_tests();
   return 1;
}
