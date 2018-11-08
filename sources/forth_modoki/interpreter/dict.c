#include "clesson.h"

typedef struct Node {
   char*        key;
   Token_t      value;
   struct Node* next;
} Node_t;

#define TABLE_SIZE 16

static Node_t* array[TABLE_SIZE];

static int hash(char *str) {
  unsigned int val = 0;
  while (*str) {
    val += *str++;
  }
  return (int)(val % TABLE_SIZE);
}

/* static void dict_clear() { */
/*    dict_pos = 0; */
/* } */

static Node_t* dict_new(char* key, Token_t* value) {
   Node_t* head = malloc(sizeof(Node_t));
   head->next = NULL;
   head->key = key;
   head->value = *value;
   return head;
}

static void dict_update(Node_t* head, char* key, Token_t* value) {
   Node_t* prev;
   for (;;) {
      if (head == NULL) { // end of list, insert
         head = dict_new(key, value);
         prev->next = head;
         return;
      } else if (strcmp(head->key, key) == 0) { // match, set value to this node
         head->value = *value;
         return;
      } else { // key missmatch go to next node
         prev = head;
         head = head->next;
      }
   }
}

void dict_put(char* key, Token_t* value) {
   int idx = hash(key);
   Node_t* head = array[idx];
   if (head == NULL) {
      head = dict_new(key, value);
      array[idx] = head;
   } else {
      dict_update(head, key, value);
   }
}

int dict_get(char* key, Token_t* out_value) {
   int idx = hash(key);
   Node_t* head = array[idx];
   for (;;) {
      if (head == NULL) {
         return 0;
      } else if (strcmp(head->key, key) == 0) {
         *out_value = head->value;
         return 1;
      } else {
         head = head->next;
      }
   }
}

void dict_print_all() {
   printf("---print_all---\n");
   int i;
   for (i = 0; i < TABLE_SIZE; i++) {
      Node_t* head = array[i];
      printf("%2d  %p:", i, head);
      while (head != NULL) {
         printf("%s -> %p ", head->key, head->next);
         head = head->next;
      }
      printf("\n");
   }
   printf("-------------\n");
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

static char* key_pool[] = {"foo", "bar", "toto", "ppu", "shaa",
                           "122", "01", "a1b2", "cccc", "dddd"};

static void kv_init(Node_t* kv, int num) {
   int i;

   kv[0].key = key_pool[0];
   kv[0].value.ltype = NUMBER;
   kv[0].value.u.number = 123;
   kv[1].key = key_pool[1];
   kv[1].value.ltype = NUMBER;
   kv[1].value.u.number = 65;
   kv[2].key = key_pool[2];
   kv[2].value.ltype = NUMBER;
   kv[2].value.u.number = 1000;

   char* name3 = malloc(strlen("compile") + 1);
   strcpy(name3, "compile");
   char* name4 = malloc(strlen("run") + 1);
   strcpy(name4, "run");

   kv[3].key = key_pool[3];
   kv[3].value.ltype = LITERAL_NAME;
   kv[3].value.u.name = name3;
   kv[4].key = key_pool[4];
   kv[4].value.ltype = LITERAL_NAME;
   kv[4].value.u.name = name4;

   for (i = 5; i < num; i++) {
      kv[i].key = malloc(2);
      kv[i].key[0] = '0' + (i % 10);
      kv[i].key[1] = '\0';
      kv[i].value.ltype = NUMBER;
      kv[i].value.u.number = i;
   }

   /* for (i = 0; i < num; i++) { */
   /*    printf("%2d\t%s\n", i, kv[i].key); */
   /* } */
}

static void test_dict_mult() {
   const int max = 10;
   Node_t input[max];
   int i;

   kv_init(input, max);
   for (i = 0; i < max; i++) {
      dict_put(input[i].key, &input[i].value);
   }

   /* dict_print_all(); */

   Node_t actual;
   for (i = 0; i < max; i++) {
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
