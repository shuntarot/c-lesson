#include "clesson.h"
#include <assert.h>

/*
cc cl_getc.c int_parser_getc.c
*/
typedef enum token_type {NUMBER, SPACE} token_type_t;

int is_number(const int a) {
   if ('0' <= a && a <= '9')
      return 1;
   else
      return 0;
}

int parse_one(token_type_t* out_type, int* out_val, const int prev) {
   int a;

   if (prev == ' ') {
      do {
	 a = cl_getc();
      } while (a == ' ');
      
      *out_type = SPACE;
      *out_val  = -1;
      return a;
      
   } else if (is_number(prev)) {
      int val = prev - '0';
      
      do {
	 a = cl_getc();
	 if (is_number(a)) {
	    val *= 10;
	    val += a - '0';
	 }
      } while (is_number(a));
      
      *out_type = NUMBER;
      *out_val  = val;
      return a;
   }

   return prev;
}

void test_parse_one_123() {
   cl_getc_set_src("123");
   token_type_t t;
   int v = 0;
   
   parse_one(&t, &v, cl_getc());
   assert(t == NUMBER);
   assert(v == 123);
}

int main() {
    int answer1 = 0;
    int answer2 = 0;

    // test
    test_parse_one_123();
    cl_getc_set_src("123 456");

    // write something here.
    token_type_t t;
    int i = 0;
    int v[2] = {0};
    int next;
    
    next = cl_getc();

    do {
       next = parse_one(&t, &v[i], next);
       if (t == NUMBER) {
	  /* printf("%d %d %d\n", t, v[i], next); */
	  i++;
       }
    } while (next != EOF);

    answer1 = v[0];
    answer2 = v[1];
    
    // sample for cl_getc() usage.
    int c;

    while((c = cl_getc()) != EOF) {
        printf("%c\n",c );
    }

    // verity result.
    assert(answer1 == 123);
    assert(answer2 == 456);

    return 1;


}
