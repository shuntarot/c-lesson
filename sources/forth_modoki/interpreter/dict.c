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
      printf("key: %s\n", dict_array[i].key);
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

#ifdef TEST_DICT
int main() {
   test_dict_put();
   test_dict_get();
   return 1;
}
#endif
