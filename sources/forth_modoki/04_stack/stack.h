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

int stack_push(const stack_data_t* data);

int stack_pop(stack_data_t* out_data);

void stack_print(const stack_data_t* const data);

void stack_print_all();
