#include "clesson.h"

static int dict_pos = 0;
struct KeyValue {
   char*   key;
   Token_t value;
};
static struct KeyValue dict_array[1024];

int dict_find(char* key, int* out_pos) {
   int i;
   for (i = 0; i < dict_pos; i++) {
      if (streq(key, dict_array[i].key)) {
         *out_pos = i;
         return 1;
      }
   }
   return 0;
}

static void dict_clear() {
   dict_pos = 0;
}

void dict_put(char* key, Token_t* elem) {
   int pos;
   struct KeyValue kv = {key, *elem};
   if (dict_find(key, &pos)) {
      dict_array[pos] = kv;
   } else if (dict_pos < 1024){
      dict_array[dict_pos++] = kv;
   }
}

int dict_get(char* key, Token_t* out_elem) {
   int pos;
   if (dict_find(key, &pos)) {
      struct KeyValue kv = dict_array[pos];
      *out_elem = kv.value;
      return 1;
   }
   return 0;
}

void dict_print_all() {
   printf("---\n");
   int i = 0;
   for (i = 0; i < dict_pos; i++) {
     if (dict_array[i].value.ltype == NUMBER) {
       printf("key: %s\t%d\n", dict_array[i].key, dict_array[i].value.u.number);
     } else if (dict_array[i].value.ltype == LITERAL_NAME) {
       printf("key: %s\t%s\n", dict_array[i].key, dict_array[i].value.u.name);
     } else {
       printf("key: %s\n", dict_array[i].key);
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

static int dict_eq(const struct KeyValue* a, const struct KeyValue* b) {
   if (strcmp(a->key, b->key) == 0) {
      return token_eq(&a->value, &b->value);
   }
   return 0;
}

static void assert_dict_eq(const struct KeyValue* actual, const struct KeyValue* expect) {
   assert(dict_eq(actual, expect));
}

static void test_dict_put() {
   char* key = "foo";
   struct KeyValue input  = {.key=key, {.ltype=LITERAL_NAME, .u.name=key}};
   struct KeyValue expect = {.key=key, {.ltype=LITERAL_NAME, .u.name=key}};

   dict_put(input.key, &input.value);
   struct KeyValue actual = dict_array[0];
   assert_dict_eq(&actual, &expect);

   dict_clear();
}

static void test_dict_get() {
   char* key = "foo";
   struct KeyValue input  = {.key=key, {.ltype=LITERAL_NAME, .u.name=key}};
   struct KeyValue expect = {.key=key, {.ltype=LITERAL_NAME, .u.name=key}};

   dict_put(input.key, &input.value);
   struct KeyValue actual;
   dict_get(key, &actual.value);
   actual.key = key;
   assert_dict_eq(&actual, &expect);

   dict_clear();
}

static int kv_init(struct KeyValue* kv_array, int num) {
   int i;
   kv_array[0].key = "foo";
   kv_array[0].value.ltype = NUMBER;
   kv_array[0].value.u.number = 123;
   kv_array[1].key = "bar";
   kv_array[1].value.ltype = NUMBER;
   kv_array[1].value.u.number = 65;
   kv_array[2].key = "toto";
   kv_array[2].value.ltype = NUMBER;
   kv_array[2].value.u.number = 1000;
   kv_array[3].key = "puda";
   kv_array[3].value.ltype = LITERAL_NAME;
   kv_array[3].value.u.name = "compile"; // [TODO] Need malloc?
   kv_array[4].key = "shaa";
   kv_array[4].value.ltype = LITERAL_NAME;
   kv_array[4].value.u.name = "run";     // [TODO] Need malloc?
   return 5;
}

static void test_dict_mult() {
   struct KeyValue input[10];
   int i;
   int x;
   int uniq_num;
   uniq_num = kv_init(input, 10);
   for (i = 0; i < 10; i++) {
      x = i % uniq_num;
      dict_put(input[x].key, &input[x].value);
   }

   /* dict_print_all(); */

   struct KeyValue actual;
   for (i = 0; i < uniq_num; i++) {
      actual.key = input[i].key;
      dict_get(actual.key, &actual.value);
      assert_dict_eq(&actual, &input[i]);
   }

   dict_clear();
}

#ifdef TEST_DICT
int main() {
   test_dict_put();
   test_dict_get();
   test_dict_mult();
   return 1;
}
#endif
