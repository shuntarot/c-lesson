#include "clesson.h"

//
// c function
//

void eval_exec_array(struct TokenArray* exec_array);

static void def_op()
{
    Token_t rs;
    Token_t rt;

    stack_pop(&rs);
    stack_pop(&rt);
    dict_put(rt.u.name, &rs);
}

#define two_op(op)                            \
    Token_t rs;                               \
    Token_t rt;                               \
    Token_t rd;                               \
    stack_pop(&rs);                           \
    stack_pop(&rt);                           \
    rd.ltype    = NUMBER;                     \
    rd.u.number = rt.u.number op rs.u.number; \
    stack_push(&rd)

static void add_op()
{
    two_op(+);
}

static void sub_op()
{
    two_op(-);
}

static void mul_op()
{
    two_op(*);
}

static void div_op()
{
    two_op(/);
}

static void ifelse_op()
{
    Token_t rs;
    Token_t rt;
    Token_t ifelse;
    stack_pop(&rs);
    stack_pop(&rt);
    stack_pop(&ifelse);
    if (ifelse.u.number) {
        stack_push(&rt);
    } else {
        stack_push(&rs);
    }
}

static void repeat_op()
{
    Token_t rs;
    Token_t rt;
    stack_pop(&rs);
    stack_pop(&rt);
    //printf("rt.u.number %d\n", rt.u.number);
    for (int i = 0; i < rt.u.number; i++) {
        eval_exec_array(rs.u.bytecodes);
    }
}

static void exec_op()
{
    Token_t rs;
    stack_pop(&rs);
    eval_exec_array(rs.u.bytecodes);
}

static void if_op()
{
    Token_t rs;
    Token_t rt;
    stack_pop(&rs);
    stack_pop(&rt);
    if (rt.u.number) {
        eval_exec_array(rs.u.bytecodes);
    }
}

static void while_op()
{
    Token_t rexe;
    Token_t rcon;
    stack_pop(&rexe);
    stack_pop(&rcon);

    Token_t rs;
    eval_exec_array(rcon.u.bytecodes);
    stack_pop(&rs);

    while (rs.u.number) {
        eval_exec_array(rexe.u.bytecodes);
        eval_exec_array(rcon.u.bytecodes);
        stack_pop(&rs);
    }
}

static void pop_op()
{
    Token_t rs;
    stack_pop(&rs);
}

static void exch_op()
{
    Token_t rs;
    Token_t rt;
    stack_pop(&rs);
    stack_pop(&rt);
    stack_push(&rs);
    stack_push(&rt);
}

static void dup_op()
{
    Token_t rs;
    stack_pop(&rs);
    stack_push(&rs);
    stack_push(&rs);
}

static void index_op()
{
    Token_t rs;
    Token_t rt[STACK_SIZE];
    stack_pop(&rs);
    for (int i = 0; i <= rs.u.number; i++) {
        stack_pop(&rt[i]);
    }
    for (int i = rs.u.number; i >= 0; i--) {
        stack_push(&rt[i]);
    }
    stack_push(&rt[rs.u.number]);
}

static void roll_op()
{
    Token_t rs;
    Token_t rt[STACK_SIZE];
    int     range;
    int     rot;

    stack_pop(&rs);
    rot = rs.u.number;
    stack_pop(&rs);
    range = rs.u.number;

    if (range < 2)
        return;

    for (int i = 0; i < range; i++) {
        stack_pop(&rt[i]);
    }

    for (int i = 0; i < range; i++) {
        int idx = (rot + range - i - 1) % range;
        stack_push(&rt[idx]);
    }
}

static void eq_op()
{
    two_op(==);
}

static void neq_op()
{
    two_op(!=);
}

static void gt_op()
{
    two_op(>);
}

static void ge_op()
{
    two_op(>=);
}

static void lt_op()
{
    two_op(<);
}

static void le_op()
{
    two_op(<=);
}

static void register_primitives()
{
    Token_t def = {.ltype = ELEMENT_C_FUNC, .u.cfunc = def_op };
    dict_put("def", &def);

    Token_t add = {.ltype = ELEMENT_C_FUNC, .u.cfunc = add_op };
    dict_put("add", &add);

    Token_t sub = {.ltype = ELEMENT_C_FUNC, .u.cfunc = sub_op };
    dict_put("sub", &sub);

    Token_t mul = {.ltype = ELEMENT_C_FUNC, .u.cfunc = mul_op };
    dict_put("mul", &mul);

    Token_t div = {.ltype = ELEMENT_C_FUNC, .u.cfunc = div_op };
    dict_put("div", &div);

    Token_t ifelse = {.ltype = ELEMENT_C_FUNC, .u.cfunc = ifelse_op };
    dict_put("ifelse", &ifelse);

    Token_t repeat = {.ltype = ELEMENT_C_FUNC, .u.cfunc = repeat_op };
    dict_put("repeat", &repeat);

    Token_t exec = {.ltype = ELEMENT_C_FUNC, .u.cfunc = exec_op };
    dict_put("exec", &exec);

    Token_t iff = {.ltype = ELEMENT_C_FUNC, .u.cfunc = if_op };
    dict_put("if", &iff);

    Token_t whilee = {.ltype = ELEMENT_C_FUNC, .u.cfunc = while_op };
    dict_put("while", &whilee);

    Token_t eq = {.ltype = ELEMENT_C_FUNC, .u.cfunc = eq_op };
    dict_put("eq", &eq);

    Token_t neq = {.ltype = ELEMENT_C_FUNC, .u.cfunc = neq_op };
    dict_put("neq", &neq);

    Token_t gt = {.ltype = ELEMENT_C_FUNC, .u.cfunc = gt_op };
    dict_put("gt", &gt);

    Token_t ge = {.ltype = ELEMENT_C_FUNC, .u.cfunc = ge_op };
    dict_put("ge", &ge);

    Token_t lt = {.ltype = ELEMENT_C_FUNC, .u.cfunc = lt_op };
    dict_put("lt", &lt);

    Token_t le = {.ltype = ELEMENT_C_FUNC, .u.cfunc = le_op };
    dict_put("le", &le);

    Token_t pop = {.ltype = ELEMENT_C_FUNC, .u.cfunc = pop_op };
    dict_put("pop", &pop);

    Token_t exch = {.ltype = ELEMENT_C_FUNC, .u.cfunc = exch_op };
    dict_put("exch", &exch);

    Token_t dup = {.ltype = ELEMENT_C_FUNC, .u.cfunc = dup_op };
    dict_put("dup", &dup);

    Token_t index = {.ltype = ELEMENT_C_FUNC, .u.cfunc = index_op };
    dict_put("index", &index);

    Token_t roll = {.ltype = ELEMENT_C_FUNC, .u.cfunc = roll_op };
    dict_put("roll", &roll);
}

#define MAX_NAME_OP_NUMBERS 256
static struct TokenArray* compile_exec_array(int prev_ch)
{
    int                ch        = prev_ch;
    int                num_token = 0;
    Token_t            token     = { UNKNOWN, { 0 } };
    Token_t            arr[MAX_NAME_OP_NUMBERS];
    struct TokenArray* exec_array;
    struct TokenArray* ret;

    for (int i = 0; i < MAX_NAME_OP_NUMBERS;) {
        ch = parse_one(ch, &token);
        if (token.ltype == CLOSE_CURLY) {
            num_token = i;
            break;
        } else if (token.ltype == OPEN_CURLY) {
            exec_array        = compile_exec_array(ch);
            token.ltype       = EXEC_ARRAY;
            token.u.bytecodes = exec_array;
            ch                = ' ';
        }

        if (token.ltype != SPACE) {
            arr[i++] = token;
        }
    }
    ret      = malloc(sizeof(struct TokenArray) + sizeof(Token_t) * num_token);
    ret->len = num_token;
    memcpy(ret->token, arr, sizeof(Token_t) * num_token);
    return ret;
}

//
// eval
//

void eval_exec_array(struct TokenArray* exec_array)
{
    Token_t token;
    for (int i = 0; i < exec_array->len; i++) {
        token = exec_array->token[i];
        switch (token.ltype) {
        case NUMBER:
            stack_push(&token);
            break;
        case ELEMENT_C_FUNC:
            token.u.cfunc();
            break;
        case EXEC_ARRAY:
            eval_exec_array(token.u.bytecodes);
            break;
        case EXECUTABLE_NAME:
            stack_push(&token);
            break;
        case LITERAL_NAME:
            stack_push(&token);
            break;
        case SPACE:
            break;
        default:
            printf("Unsuppoted type %d\n", token.ltype);
            break;
        }
    }
}

void eval()
{
    int                ch    = EOF;
    Token_t            token = { UNKNOWN, { 0 } };
    struct TokenArray* exec_arr;

    do {
        ch = parse_one(ch, &token);
        if (token.ltype != UNKNOWN) {

            switch (token.ltype) {
            case NUMBER:
                /* printf("NUMBER: %d\n", token.u.number); */
                stack_push(&token);
                break;
            case ELEMENT_C_FUNC:
                /* printf("ELEMENT_C_FUNC\n"); */
                token.u.cfunc();
                break;
            case EXEC_ARRAY:
                // printf("EXEC_ARRAY\n");
                eval_exec_array(token.u.bytecodes);
                break;
            case EXECUTABLE_NAME:
                /* printf("EXECUTABLE_NAME: %s\n", token.u.name); */
                stack_push(&token);
                break;
            case LITERAL_NAME:
                /* printf("LITERAL_NAME: %s\n", token.u.name); */
                stack_push(&token);
                break;
            case OPEN_CURLY:
                exec_arr          = compile_exec_array(ch);
                token.ltype       = EXEC_ARRAY;
                token.u.bytecodes = exec_arr;
                stack_push(&token);
                ch = ' ';
                break;
            default:
                /* printf("Unknown type %d\n", token.ltype); */
                break;
            }
        }
    } while (ch != EOF);
}

//
// Unit test
//

static void assert_token_number(int expect, Token_t* actual)
{
    assert(actual->ltype == NUMBER);
    assert(actual->u.number == expect);
}

static void assert_token_array(struct TokenArray* expect, struct TokenArray* actual);
static void assert_token(Token_t expect, Token_t actual)
{
    assert(expect.ltype == actual.ltype);
    if (actual.ltype == NUMBER) {
        assert(expect.u.number == actual.u.number);
    } else if (actual.ltype == EXECUTABLE_NAME || actual.ltype == LITERAL_NAME) {
        assert(strcmp(expect.u.name, actual.u.name) == 0);
    } else if (actual.ltype == EXEC_ARRAY) {
        assert_token_array(expect.u.bytecodes, actual.u.bytecodes);
    }
}

static void assert_token_array(struct TokenArray* expect, struct TokenArray* actual)
{
    for (int i = 0; i < actual->len; i++) {
        assert_token(expect->token[i], actual->token[i]);
    }
}

static void test_eval_num_one()
{
    char* input  = "123";
    int   expect = 123;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_eval_num_two()
{
    char* input   = "123 456";
    int   expect1 = 456;
    int   expect2 = 123;

    cl_getc_set_src(input);

    eval();

    Token_t actual1 = { UNKNOWN, { 0 } };
    Token_t actual2 = { UNKNOWN, { 0 } };
    stack_pop(&actual1);
    stack_pop(&actual2);
    assert_token_number(expect1, &actual1);
    assert_token_number(expect2, &actual2);

    stack_clear();
}

static void test_eval_num_add()
{
    char* input  = "1 2 add";
    int   expect = 3;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_eval_literal()
{
    char* input  = "/hoge 432 def hoge";
    int   expect = 432;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_eval_num_sub()
{
    char* input  = "5 3 sub";
    int   expect = 2;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_eval_num_mul()
{
    char* input  = "3 8 mul";
    int   expect = 24;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_eval_num_div()
{
    char* input  = "8 3 div";
    int   expect = 2;

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_eval_exec_array1()
{
    char*              input = "{1}";
    struct TokenArray* bc    = malloc(sizeof(struct TokenArray) + sizeof(Token_t) * 1);
    bc->len                  = 1;
    Token_t t0               = { NUMBER, { 1 } };
    bc->token[0]             = t0;
    Token_t expect = {.ltype = EXEC_ARRAY, .u.bytecodes = bc };

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token(expect, actual);

    stack_clear();
}

static void test_eval_exec_array2()
{
    char*              input = "{/abc}";
    struct TokenArray* bc    = malloc(sizeof(struct TokenArray) + sizeof(Token_t) * 1);
    bc->len                  = 1;
    Token_t t0 = {.ltype = LITERAL_NAME, .u.name = "abc" };
    bc->token[0]   = t0;
    Token_t expect = {.ltype = EXEC_ARRAY, .u.bytecodes = bc };

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token(expect, actual);

    stack_clear();
}

static void test_eval_exec_array3()
{
    char*              input = "{abc}";
    struct TokenArray* bc    = malloc(sizeof(struct TokenArray) + sizeof(Token_t) * 1);
    bc->len                  = 1;
    Token_t t0 = {.ltype = EXECUTABLE_NAME, .u.name = "abc" };
    bc->token[0]   = t0;
    Token_t expect = {.ltype = EXEC_ARRAY, .u.bytecodes = bc };

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token(expect, actual);

    stack_clear();
}

static void test_eval_exec_array_mult()
{
    char* input = "{1} {2}";

    Token_t            t0  = { NUMBER, { 1 } };
    struct TokenArray* bc0 = malloc(sizeof(struct TokenArray) + sizeof(Token_t) * 1);
    bc0->len               = 1;
    bc0->token[0]          = t0;

    Token_t            t1  = { NUMBER, { 2 } };
    struct TokenArray* bc1 = malloc(sizeof(struct TokenArray) + sizeof(Token_t) * 1);
    bc1->len               = 1;
    bc1->token[0]          = t1;

    Token_t expect1 = { EXEC_ARRAY, .u.bytecodes = bc0 };
    Token_t expect2 = { EXEC_ARRAY, .u.bytecodes = bc1 };

    cl_getc_set_src(input);
    eval();

    Token_t actual1 = { UNKNOWN, { 0 } };
    Token_t actual2 = { UNKNOWN, { 0 } };
    stack_pop(&actual2);
    stack_pop(&actual1);
    assert_token(expect1, actual1);
    assert_token(expect2, actual2);

    stack_clear();
}

static void test_eval_exec_array_nest()
{
    char* input = "{1 {2} 3}";

    Token_t            tnest   = { NUMBER, { 2 } };
    struct TokenArray* bc_nest = malloc(sizeof(struct TokenArray) + sizeof(Token_t) * 1);
    bc_nest->len               = 1;
    bc_nest->token[0]          = tnest;

    Token_t t0 = { NUMBER, { 1 } };
    Token_t t1 = { EXEC_ARRAY, .u.bytecodes = bc_nest };
    Token_t t2 = { NUMBER, { 3 } };

    struct TokenArray* bc = malloc(sizeof(struct TokenArray) + sizeof(Token_t) * 3);
    bc->len               = 3;
    bc->token[0]          = t0;
    bc->token[1]          = t1;
    bc->token[2]          = t2;

    Token_t expect = { EXEC_ARRAY, .u.bytecodes = bc };

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token(expect, actual);

    stack_clear();
}

static void test_eval_exec_array_eval()
{
    char* input  = "/foo {1 2 add} def foo";
    int   expect = 3;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_eval_exec_array_eval_nest()
{
    char* input = "/ZZ {6} def /YY {4 ZZ 5} def /XX {1 2 YY 3} def XX";

    Token_t expect0 = { NUMBER, { 1 } };
    Token_t expect1 = { NUMBER, { 2 } };
    Token_t expect2 = { NUMBER, { 4 } };
    Token_t expect3 = { NUMBER, { 6 } };
    Token_t expect4 = { NUMBER, { 5 } };
    Token_t expect5 = { NUMBER, { 3 } };

    cl_getc_set_src(input);
    eval();

    Token_t actual0 = { UNKNOWN, { 0 } };
    Token_t actual1 = { UNKNOWN, { 0 } };
    Token_t actual2 = { UNKNOWN, { 0 } };
    Token_t actual3 = { UNKNOWN, { 0 } };
    Token_t actual4 = { UNKNOWN, { 0 } };
    Token_t actual5 = { UNKNOWN, { 0 } };

    stack_pop(&actual5);
    stack_pop(&actual4);
    stack_pop(&actual3);
    stack_pop(&actual2);
    stack_pop(&actual1);
    stack_pop(&actual0);
    assert_token(expect0, actual0);
    assert_token(expect1, actual1);
    assert_token(expect2, actual2);
    assert_token(expect3, actual3);
    assert_token(expect4, actual4);
    assert_token(expect5, actual5);

    stack_clear();
}

static void test_eval_ifelse_num()
{
    char* input  = "1 2 3 ifelse";
    int   expect = 2;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_eval_ifelse_exec()
{
    char* input  = "/foo 0 {1 add} {2 add} ifelse def 5 foo";
    int   expect = 7;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_eval_repeat()
{
    char* input   = "3 {1 2} repeat";
    int   expect0 = 1;
    int   expect1 = 2;

    cl_getc_set_src(input);

    eval();

    Token_t actual0 = { UNKNOWN, { 0 } };
    Token_t actual1 = { UNKNOWN, { 0 } };
    Token_t actual2 = { UNKNOWN, { 0 } };
    Token_t actual3 = { UNKNOWN, { 0 } };
    Token_t actual4 = { UNKNOWN, { 0 } };
    Token_t actual5 = { UNKNOWN, { 0 } };
    stack_pop(&actual0);
    stack_pop(&actual1);
    stack_pop(&actual2);
    stack_pop(&actual3);
    stack_pop(&actual4);
    stack_pop(&actual5);
    assert_token_number(expect1, &actual0);
    assert_token_number(expect0, &actual1);
    assert_token_number(expect1, &actual2);
    assert_token_number(expect0, &actual3);
    assert_token_number(expect1, &actual4);
    assert_token_number(expect0, &actual5);

    stack_clear();
}

static void test_op_eq_true()
{
    char* input  = "1 1 eq";
    int   expect = 1;

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_op_eq_false()
{
    char* input  = "1 2 eq";
    int   expect = 0;

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_op_neq_false()
{
    char* input  = "1 1 neq";
    int   expect = 0;

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_op_neq_true()
{
    char* input  = "1 2 neq";
    int   expect = 1;

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_op_gt_true()
{
    char* input  = "4 2 gt";
    int   expect = 1;

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_op_ge_true()
{
    char* input  = "4 4 ge";
    int   expect = 1;

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_op_gt_false()
{
    char* input  = "4 4 gt";
    int   expect = 0;

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_op_lt_true()
{
    char* input  = "1 100 lt";
    int   expect = 1;

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_op_le_true()
{
    char* input  = "100 100 le";
    int   expect = 1;

    cl_getc_set_src(input);
    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_op_pop()
{
    char* input  = "1 2 3 4 5 6 pop pop pop";
    int   expect = 3;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_op_exch()
{
    char* input   = "1 2 exch";
    int   expect1 = 1;
    int   expect2 = 2;

    cl_getc_set_src(input);

    eval();

    Token_t actual1 = { UNKNOWN, { 0 } };
    Token_t actual2 = { UNKNOWN, { 0 } };
    stack_pop(&actual1);
    stack_pop(&actual2);
    assert_token_number(expect1, &actual1);
    assert_token_number(expect2, &actual2);

    stack_clear();
}

static void test_op_dup()
{
    char* input   = "3 4 5 8 dup";
    int   expect1 = 8;
    int   expect2 = 8;

    cl_getc_set_src(input);

    eval();

    Token_t actual1 = { UNKNOWN, { 0 } };
    Token_t actual2 = { UNKNOWN, { 0 } };
    stack_pop(&actual1);
    stack_pop(&actual2);
    assert_token_number(expect1, &actual1);
    assert_token_number(expect2, &actual2);

    stack_clear();
}

static void test_op_index()
{
    char* input   = "30 20 10 0 2 index";
    int   expect1 = 20;
    int   expect2 = 0;
    int   expect3 = 10;

    cl_getc_set_src(input);

    eval();

    Token_t actual1 = { UNKNOWN, { 0 } };
    Token_t actual2 = { UNKNOWN, { 0 } };
    Token_t actual3 = { UNKNOWN, { 0 } };
    stack_pop(&actual1);
    stack_pop(&actual2);
    stack_pop(&actual3);
    assert_token_number(expect1, &actual1);
    assert_token_number(expect2, &actual2);
    assert_token_number(expect3, &actual3);

    stack_clear();
}

static void test_op_roll()
{
    char* input   = "1 2 3 4 5 6 7 4 3 roll";
    int   expect1 = 4;
    int   expect2 = 7;
    int   expect3 = 6;
    int   expect4 = 5;
    int   expect5 = 3;

    cl_getc_set_src(input);

    eval();

    Token_t actual1 = { UNKNOWN, { 0 } };
    Token_t actual2 = { UNKNOWN, { 0 } };
    Token_t actual3 = { UNKNOWN, { 0 } };
    Token_t actual4 = { UNKNOWN, { 0 } };
    Token_t actual5 = { UNKNOWN, { 0 } };
    stack_pop(&actual1);
    stack_pop(&actual2);
    stack_pop(&actual3);
    stack_pop(&actual4);
    stack_pop(&actual5);
    assert_token_number(expect1, &actual1);
    assert_token_number(expect2, &actual2);
    assert_token_number(expect3, &actual3);
    assert_token_number(expect4, &actual4);
    assert_token_number(expect5, &actual5);

    stack_clear();
}

static void test_op_exec()
{
    char* input  = "{1 2 add} exec";
    int   expect = 3;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_op_if()
{
    char* input  = "8 1 {1 2 add} if";
    int   expect = 3;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_op_while()
{
    char* input  = "4 dup {dup 1 gt} {1 sub exch 1 index mul exch} while pop";
    int   expect = 24; // 4!

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_lf()
{
    char* input  = "1 2\n3 4";
    int   expect = 4;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

static void test_comment()
{
    char* input  = "1 2\n3 4 %foo bar";
    int   expect = 4;

    cl_getc_set_src(input);

    eval();

    Token_t actual = { UNKNOWN, { 0 } };
    stack_pop(&actual);
    assert_token_number(expect, &actual);

    stack_clear();
}

//
// main
//

int main()
{
    register_primitives();

    test_eval_num_one();
    test_eval_num_two();
    test_eval_num_add();
    test_eval_literal();
    test_eval_num_sub();
    test_eval_num_mul();
    test_eval_num_div();
    test_eval_exec_array1();
    test_eval_exec_array2();
    test_eval_exec_array3();
    test_eval_exec_array_mult();
    test_eval_exec_array_nest();
    test_eval_exec_array_eval();
    test_eval_exec_array_eval_nest();
    test_eval_ifelse_num();
    test_eval_ifelse_exec();
    test_eval_repeat();
    test_op_eq_true();
    test_op_eq_false();
    test_op_neq_true();
    test_op_neq_false();
    test_op_gt_true();
    test_op_gt_false();
    test_op_ge_true();
    test_op_lt_true();
    test_op_le_true();
    test_op_pop();
    test_op_exch();
    test_op_dup();
    test_op_index();
    test_op_roll();
    test_op_exec();
    test_op_if();
    test_op_while();
    test_lf();
    test_comment();

    cl_getc_set_src("/foo 55 def /bar 11 def 1 foo add bar add 1 sub 11 div");

    eval();
    Token_t token = { UNKNOWN, { 0 } };
    stack_pop(&token);
    printf("result: %d\n", token.u.number);

    return 1;
}
