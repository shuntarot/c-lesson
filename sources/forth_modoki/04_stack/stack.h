#define STACK_SIZE 1024

enum LexicalDataType {
    NUMBER,
    LITERAL_NAME,
    UNKNOWN
};

typedef struct stack_data {
   enum LexicalDataType ltype;
   union {
      int number;
      char *name;
   } u;
} stack_data_t;

void stack_push(stack_data_t* data);

void stack_pop(stack_data_t* out_data);

void stack_print(stack_data_t* data);

void stack_print_all();
