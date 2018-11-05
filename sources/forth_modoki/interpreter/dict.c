#include "clesson.h"

typedef struct Node {
   char*        key;
   Token_t      value;
   struct Node* next;
} Node_t;

#define TABLE_SIZE 1024

static Node_t* array[TABLE_SIZE];

static int hash(char *str) {
  unsigned int val = 0;
  while (*str) {
    val += *str++;
  }
  return (int)(val % 1024);
}

/* static int dict_find(char* key, int* out_pos) { */
/*    int i; */
/*    for (i = 0; i < dict_pos; i++) { */
/*       if (streq(key, dict_array[i].key)) { */
/*          *out_pos = i; */
/*          return 1; */
/*       } */
/*    } */
/*    return 0; */
/* } */

/* static void dict_clear() { */
/*    dict_pos = 0; */
/* } */

static void dict_update(Node_t* head, char* key, Token_t* value) {
   head->next = NULL;
   head->key = key;
   head->value = *value;
}

void dict_put(char* key, Token_t* elem) {
   int idx = hash(key);
   Node_t* head = array[idx];
   if (head == NULL) {
      head = malloc(sizeof(Node_t));
      head->next = NULL;
      head->key = key;
      head->value = *elem;
      array[idx] = head;
   } else {
      dict_update(head, key, elem);
   }
}

int dict_get(char* key, Token_t* out_elem) {
   int idx = hash(key);
   Node_t* head = array[idx];
   if (head != NULL) {
      *out_elem = head->value;
      return 1;
   }
   return 0;
}

void dict_print_all() {
   printf("---\n");
   int i = 0;
   for (i = 0; i < TABLE_SIZE; i++) {
      Node_t* head = array[i];
      if (head == NULL) {
	 ;
      } else {
	 printf("key: %s\n", head->key);
      }
   }
}

//
// test
//

static int token_eq(const Token_t* a, const Token_t* b) {
   // only support NUMBER and LITERAL_NAME
   if (a->ltype == b->ltype) {
      if (a->ltype == NUMBER) {
         return (a->u.number == b->u.number);
      } else if (a->ltype == LITERAL_NAME) {
         return (strcmp(a->u.name, b->u.name) == 0);
      }
   }
   return 0;
}

static int dict_eq(const Node_t* a, const Node_t* b) {
   if (strcmp(a->key, b->key) == 0) {
      return token_eq(&a->value, &b->value);
   }
   return 0;
}

static void assert_dict_eq(const Node_t* actual, const Node_t* expect) {
   assert(dict_eq(actual, expect));
}

static void test_dict_put() {
   char* key = "foo";
   Node_t input  = {.key=key, {.ltype=LITERAL_NAME, .u.name=key}};
   Node_t expect = {.key=key, {.ltype=LITERAL_NAME, .u.name=key}};

   dict_put(input.key, &input.value);
   int idx = hash(key);
   Node_t* actual = array[idx];
   assert_dict_eq(actual, &expect);
}

static void test_dict_get() {
   char* key = "foo";
   Node_t input  = {.key=key, {.ltype=LITERAL_NAME, .u.name=key}};
   Node_t expect = {.key=key, {.ltype=LITERAL_NAME, .u.name=key}};

   dict_put(input.key, &input.value);
   Node_t actual;
   dict_get(key, &actual.value);
   actual.key = key;
   assert_dict_eq(&actual, &expect);
}

static int kv_init(Node_t* array, int num) {
   int i;
			
   array[0].key = "foo";
   array[0].value.ltype = NUMBER;
   array[0].value.u.number = 123;
   array[1].key = "bar";
   array[1].value.ltype = NUMBER;
   array[1].value.u.number = 65;
   array[2].key = "toto";
   array[2].value.ltype = NUMBER;
   array[2].value.u.number = 1000;

   char* name3 = malloc(strlen("compile") + 1);
   strcpy(name3, "compile");
   char* name4 = malloc(strlen("run") + 1);
   strcpy(name4, "run");
   
   array[3].key = "puda";
   array[3].value.ltype = LITERAL_NAME;
   array[3].value.u.name = name3;
   array[4].key = "shaa";
   array[4].value.ltype = LITERAL_NAME;
   array[4].value.u.name = name4;
   return 5;
}

static void test_dict_mult() {
   Node_t input[10];
   int i;
   int x;
   int uniq_num;
   uniq_num = kv_init(input, 10);
   for (i = 0; i < 5; i++) {
      x = i % uniq_num;
      dict_put(input[x].key, &input[x].value);
   }

   dict_print_all();

   Node_t actual;
   for (i = 0; i < uniq_num; i++) {
      actual.key = input[i].key;
      dict_get(actual.key, &actual.value);
      assert_dict_eq(&actual, &input[i]);
   }
}

#ifdef TEST_DICT
int main() {
   test_dict_put();
   test_dict_get();
   test_dict_mult();
   return 1;
}
#endif
