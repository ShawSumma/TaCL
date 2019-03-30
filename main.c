#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <gc.h>


struct vm;
typedef struct vm vm;
struct program;
typedef struct program program;
struct object;
typedef struct object object;
struct string;
typedef struct string string;
struct vector;
typedef struct vector vector;
struct table;
typedef struct table table;
struct proc_t;
typedef struct proc_t proc_t;

typedef object *(*func_t)(vm *, uint32_t, object **);

enum opcode {
    OPCODE_ERR,
    OPCODE_STR,
    OPCODE_NAME,
    OPCODE_NUM,
    OPCODE_CALL,
    OPCODE_RET,
    OPCODE_POP,
    OPCODE_PROC
};
typedef enum opcode opcode;

struct proc_t {
    object **argn;
    uint32_t argc;
    uint32_t gotoval;
};

struct table {
    object **key;
    object **val;
    uint32_t size;
    uint32_t alloc;
};

struct vm {
    object **stack;
    table **world;
    uint32_t *calls;
    uint32_t calla;
    uint32_t callc;
    uint32_t stacka;
    uint32_t stackc;
    uint32_t place;
};

struct program {
    uint32_t opcount;
    opcode *opcodes;
    uint32_t *opvalues;
    double *numbers;
    char **strings;
};

struct vector {
    object *children;
    uint32_t count;
};

struct string {
    char *str;
    uint32_t count;
};

struct object {
    enum {
        TYPE_NIL,
        TYPE_NUMBER,
        TYPE_STRING,
        TYPE_VECTOR,
        TYPE_FUNC,
        TYPE_PROC,
        TYPE_BOOL,
    } type;
    union {
        string *str;
        vector *vec;
        proc_t *proc;
        func_t func;
        double num;
        bool boolval;
    } value;
};

void tach_reg(void *vp) {
    GC_add_roots(vp, vp+1);
}

void *tach_malloc(size_t size) {
    void *ret = GC_malloc(size);
    return ret;
}

void *tach_realloc(void *ptr, size_t size) {
    void *ret = GC_realloc(ptr, size);
    return ret;
}

void tach_free(void *ptr) {
    free(ptr);
}

program *tach_read(FILE *f);
char *tach_opcode_name(opcode op);
void tach_interp(program *prog);
table *tach_create_table();
object *tach_create_object_nil();
object *tach_create_object_string(string *str);
object *tach_create_object_number(double d);
object *tach_create_object_vector(vector *vec);
object *tach_create_object_func(func_t func);
string *tach_create_string(char *);
table *tach_create_world_base();
vm *tach_create_state();

void tach_vm_call(vm *state, object *func, uint32_t argc, object **argv);

bool tach_set_table(table *t, object *k, object *v);
void tach_insert_table(table *t, object *k, object *v);

bool tach_clib_equal(object *a, object *b);
bool tach_clib_tobool(object *obj);
void tach_clib_print(FILE *f, object *obj);
void tach_clib_println(FILE *f, object *obj);

object *tach_lib_print(vm *state, uint32_t argc, object **objs);
object *tach_lib_set(vm *state, uint32_t argc, object **objs);
object *tach_lib_proc(vm *state, uint32_t argc, object **objs);
object *tach_lib_add(vm *state, uint32_t argc, object **objs);
object *tach_lib_sub(vm *state, uint32_t argc, object **objs);
object *tach_lib_lt(vm *state, uint32_t argc, object **objs);
object *tach_lib_eq(vm *state, uint32_t argc, object **objs);
object *tach_lib_if(vm *state, uint32_t argc, object **objs);
object *tach_lib_copy(vm *state, uint32_t argc, object **objs);

char *tach_opcode_name(opcode op);

string *tach_create_string(char *si) {
    string *str = tach_malloc(sizeof(str));
    str->count = strlen(si);
    str->str = si;
    return str;
}

object *tach_create_object_nil() {
    object *ret = tach_malloc(sizeof(object));
    ret->type = TYPE_NIL;
    return ret;
}

object *tach_create_object_string(string *str) {
    object *ret = tach_malloc(sizeof(object));
    ret->type = TYPE_STRING;
    ret->value.str = str;
    return ret;    
}

object *tach_create_object_number(double d) {
    object *ret = tach_malloc(sizeof(object));
    ret->type = TYPE_NUMBER;
    ret->value.num = d;
    return ret;  
}

object *tach_create_object_func(func_t func) {
    object *ret = tach_malloc(sizeof(object));
    ret->type = TYPE_FUNC;
    ret->value.func = func;
    return ret;  
}

object *tach_create_object_vector(vector *vec) {
    object *ret = tach_malloc(sizeof(object));
    ret->type = TYPE_VECTOR;
    ret->value.vec = vec;
    return ret;  
}

object *tach_create_object_proc(uint32_t val) {
    object *ret = tach_malloc(sizeof(object));
    ret->type = TYPE_PROC;
    ret->value.proc = tach_malloc(sizeof(proc_t));
    ret->value.proc->argc = 0;
    ret->value.proc->argn = tach_malloc(0);
    ret->value.proc->gotoval = val;
    return ret;  
}

table *tach_create_table() {
    table *ret = tach_malloc(sizeof(table));
    ret->alloc = 8;
    ret->size = 0;
    ret->key = tach_malloc(sizeof(object *) * ret->alloc);
    ret->val = tach_malloc(sizeof(object *) * ret->alloc);
    return ret;
}

program *tach_read(FILE *f) {
    uint32_t alloc = 8;
    uint32_t numbera = 8;
    uint32_t stringa = 8;

    uint32_t size = 0;
    uint32_t numberc = 0;
    uint32_t stringc = 0;

    opcode *bytecode = tach_malloc(sizeof(opcode) * alloc);
    uint32_t *values = tach_malloc(sizeof(uint32_t) * alloc);
    double *numbers = tach_malloc(sizeof(double) * numbera);
    char **strings = tach_malloc(sizeof(char*) * stringa);

    while (!feof(f)) {
        if (size + 4 >= alloc) {
            alloc *= 1.5;
            bytecode = tach_realloc(bytecode, sizeof(opcode) * alloc);
            values = tach_realloc(values, sizeof(uint32_t) * alloc);
        }
        if (numberc + 4 >= numbera) {
            numbera *= 1.5;
            numbers = tach_realloc(numbers, sizeof(double) * numbera);
        }
        if (stringc + 4 >= stringa) {
            stringa *= 1.5;
            strings = tach_realloc(strings, sizeof(char*) * stringa);
        }
        char opcode[20];
        int errc = fscanf(f, "%s\n", opcode);
        if (!strcmp(opcode, "str")) {
            char str[256];
            int errc = fscanf(f, "%[^\n]", str);
            char *mstr = tach_malloc(sizeof(char)*strlen(str));
            strcpy(mstr, str);
            strings[stringc] = mstr;
            values[size] = stringc;
            stringc ++;
            bytecode[size] = OPCODE_STR;
            size ++;
        }
        else if (!strcmp(opcode, "name")) {
            char str[256];
            int errc = fscanf(f, "%s\n", str);
            char *mstr = tach_malloc(sizeof(char)*strlen(str));
            strcpy(mstr, str);
            strings[stringc] = mstr;
            values[size] = stringc;
            stringc ++;
            bytecode[size] = OPCODE_NAME;
            size ++;
        }
        else if (!strcmp(opcode, "num")) {
            double num;
            int errc = fscanf(f, "%lf\n", &num);
            numbers[numberc] = num;
            values[size] = numberc;
            numberc++;
            bytecode[size] = OPCODE_NUM;
            size ++;
        }
        else if (!strcmp(opcode, "call")) {
            unsigned int num;
            int errc = fscanf(f, "%d\n", &num);            
            values[size] = num;
            bytecode[size] = OPCODE_CALL;
            size ++;
        }
        else if (!strcmp(opcode, "ret")) {
            values[size] = 0;
            bytecode[size] = OPCODE_RET;
            size ++;
        }
        else if (!strcmp(opcode, "pop")) {
            values[size] = 0;
            bytecode[size] = OPCODE_POP;
            size ++;
        }
        else if (!strcmp(opcode, "proc")) {
            unsigned int num;
            int errc = fscanf(f, "%d\n", &num);
            values[size] = num;
            bytecode[size] = OPCODE_PROC;
            size ++;
        }
        else {
            break;
            printf(".opcode %s", opcode);
            values[size] = 0;
            bytecode[size] = OPCODE_ERR;
            size ++;
            exit(1);
        }
    }
    program *prog = tach_malloc(sizeof(program));
    prog->opcodes = bytecode;
    prog->opvalues = values;
    prog->opcount = size;
    prog->strings = strings;
    prog->numbers = numbers;
    return prog;
}

char *tach_opcode_name(opcode op) {
    switch (op) {
        case OPCODE_CALL:
            return "CALL";
        case OPCODE_ERR:
            return "ERR";
        case OPCODE_NAME:
            return "NAME";
        case OPCODE_NUM:
            return "NUM";
        case OPCODE_POP:
            return "POP";
        case OPCODE_PROC:
            return "PROC";
        case OPCODE_RET:
            return "RET";
        case OPCODE_STR:
            return "STR";
    }
}

void tach_insert_table(table *t, object *k, object *v) {
    if (t->size + 4 >= t->alloc) {
        t->alloc *= 1.5;
        t->key = tach_realloc(t->key, sizeof(object *) * t->alloc);
        t->val = tach_realloc(t->val, sizeof(object *) * t->alloc);
    }
    t->key[t->size] = k;
    t->val[t->size] = v;
    t->size ++;
}

bool tach_set_table(table *t, object *k, object *v) {
    for (uint32_t i = 0; i < t->size; i++) {
        if (tach_clib_equal(k, t->key[i])) {
            t->val[i] = v;
            return false;
        }
    }
    tach_insert_table(t, k, v);
    return true;
}

object *tach_get_table(table *t, object *k) {
    for (uint32_t i = 0; i < t->size; i++) {
        if (tach_clib_equal(k, t->key[i])) {
            return t->val[i];
        }
    }
    return NULL;
}

table *tach_create_world_base() {
    table *table = tach_create_table();
    object
        *trueobj = tach_malloc(sizeof(object)),
        *falseobj = tach_malloc(sizeof(object));
    trueobj->type = TYPE_BOOL;
    falseobj->type = TYPE_BOOL;
    trueobj->value.boolval = true;
    falseobj->value.boolval = false;
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("print")),
        tach_create_object_func(tach_lib_print)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("set")),
        tach_create_object_func(tach_lib_set)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("proc")),
        tach_create_object_func(tach_lib_proc)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("add")),
        tach_create_object_func(tach_lib_add)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("sub")),
        tach_create_object_func(tach_lib_sub)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("if")),
        tach_create_object_func(tach_lib_if)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("lt")),
        tach_create_object_func(tach_lib_lt)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("eq")),
        tach_create_object_func(tach_lib_eq)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("true")),
        trueobj
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("copy")),
        tach_create_object_func(tach_lib_copy)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("false")),
        falseobj
    );
    return table;
}

void tach_vm_push(vm *state, object *o) {
    if (state->stackc + 4 >= state->stacka) {
        state->stacka *= 1.5;
        state->stack = tach_realloc(state->stack, sizeof(object *) * state->stacka);
    }
    state->stack[state->stackc] = o;
    state->stackc ++;
}

vm *tach_create_state() {
    vm *ret = tach_malloc(sizeof(vm));
    ret->calla = 256;
    ret->callc = 0;
    ret->stacka = 256;
    ret->stackc = 0;
    
    ret->world = tach_malloc(sizeof(table *) * ret->calla);
    ret->calls = tach_malloc(sizeof(uint32_t) * ret->calla);
    ret->stack = tach_malloc(sizeof(object *) * ret->stacka);

    ret->world[0] = tach_create_world_base();
    ret->place = 0;

    return ret;
}

void tach_vm_call(vm *state, object *func, uint32_t argc, object **argv) {
    switch (func->type) {
        case TYPE_FUNC: {
            object *result = func->value.func(state, argc, argv);
            if (result != NULL) {
                tach_vm_push(state, result);
            }
            break;
        }
        case TYPE_PROC: {
            if (state->callc + 4 >= state->calla) {
                state->calla *= 1.5;
                state->calls = tach_realloc(state->calls, sizeof(uint32_t) * state->calla);
                state->world = tach_realloc(state->world, sizeof(table *) * state->callc);
            }
            state->calls[state->callc] = state->place;
            state->callc ++;
            state->world[state->callc] = tach_create_table();
            state->place = func->value.proc->gotoval;
            for (uint32_t i = 0; i < func->value.proc->argc; i++) {
                tach_set_table(state->world[state->callc], func->value.proc->argn[i], argv[i]);
            }
            break;
        }
        default: {
            printf("error, cannot call that!\n");
            exit(1);
        }
    }
}

void tach_interp(program *prog) {

    vm *state = tach_create_state();

    while (state->place < prog->opcount) {
        uint32_t i = state->place;
        switch (prog->opcodes[i]) {
            case OPCODE_NAME: {
                char *name = prog->strings[prog->opvalues[i]];
                object *obj = NULL;
                object *nameobj = tach_create_object_string(tach_create_string(name));
                for (uint32_t i = state->callc; i >= 0; i--) {
                    if (obj != NULL) {
                        goto done;
                    }
                    obj = tach_get_table(state->world[i], nameobj);
                }
                if (obj == NULL) {
                    printf("no name %s\n", name);
                    exit(1);
                }
                done:
                tach_vm_push(state, obj);
                break;
            }
            case OPCODE_RET: {
                state->callc --;
                state->place = state->calls[state->callc];
                break;
            }
            case OPCODE_PROC: {
                object *obj = tach_create_object_proc(i);
                tach_vm_push(state, obj);
                state->place += prog->opvalues[i] + 1;
                break;
            }
            case OPCODE_NUM: {
                tach_vm_push(state, tach_create_object_number(prog->numbers[prog->opvalues[i]]));
                break;
            }
            case OPCODE_POP: {
                state->stackc --;
                break;
            }
            case OPCODE_STR: {
                char *name = prog->strings[prog->opvalues[i]];
                tach_vm_push(state, tach_create_object_string(tach_create_string(name)));
                break;
            }
            case OPCODE_CALL: {
                uint32_t argc = prog->opvalues[i];
                object **args = tach_malloc(sizeof(object *) * argc);
                for (uint32_t i = 0; i < argc; i++) {
                    args[argc-1-i] = state->stack[state->stackc-1];
                    state->stackc --;
                }
                object *func = state->stack[state->stackc-1];
                state->stackc --;
                tach_vm_call(state, func, argc, args);
                break;
            }
            default :{
                printf("cannot handle %s\n", tach_opcode_name(prog->opcodes[i]));
                exit(1);
            }
        }
        state->place++;
    }
}

bool tach_clib_equal(object *a, object *b) {
    if (a->type != b->type) {
        return false;
    }
    switch (a->type) {
        case TYPE_NUMBER:
            return a->value.num == b->value.num;
        case TYPE_STRING:
            if (a->value.str->count != b->value.str->count) {
                return false;
            }
            return !strcmp(a->value.str->str, b->value.str->str);
        case TYPE_NIL:
            return true;
        case TYPE_FUNC:
            return false;
        case TYPE_VECTOR:
            return false;
        case TYPE_PROC:
            return false;
        case TYPE_BOOL:
            return a->value.boolval == b->value.boolval;
    }
}

void tach_clib_print( FILE *f, object *obj) {
    switch (obj->type) {
        case TYPE_NUMBER:
            fprintf(f, "%lf", obj->value.num);
            break;
        case TYPE_STRING:
            fprintf(f, "%s", obj->value.str->str);
            break;
        case TYPE_NIL:
            fprintf(f, "nil");
            break;
        case TYPE_FUNC:
            fprintf(f, "(func %p)", obj->value.func);
            break;
        case TYPE_VECTOR:
            fprintf(f, "(list)");
            break;
        case TYPE_PROC:
            fprintf(f, "(proc %p)", obj->value.proc);      
            break;      
        case TYPE_BOOL:
            fprintf(f, "%s", obj->value.boolval ? "true" : "false");
            break;
    }
}

void tach_clib_println(FILE *f, object *obj) {
    tach_clib_print(f, obj);
    fprintf(f, "\n");
}

bool tach_clib_tobool(object *obj) {
    if (obj->type == TYPE_NIL) {
        return false;
    }
    if (obj->type == TYPE_BOOL) {
        return obj->value.boolval;
    }
    return true;
}

object *tach_lib_print(vm *state, uint32_t argc, object **objs) {
    for (int i = 0; i < argc; i++) {
        if (i != 0) {
            fprintf(stdout, " ");
        }
        tach_clib_print(stdout, objs[i]);
    }
    fprintf(stdout, "\n");
    return tach_create_object_nil();
}

object *tach_lib_set(vm *state, uint32_t argc, object **objs) {
    if (argc != 2) {
        printf("set takes 2 arguments\n");
        exit(1);
    }
    tach_set_table(state->world[state->callc], objs[0], objs[1]);
    return tach_create_object_nil();
}

object *tach_lib_add(vm *state, uint32_t argc, object **objs) {
    if (argc != 2) {
        printf("set takes 2 arguments\n");
        exit(1);
    }
    return tach_create_object_number(objs[0]->value.num + objs[1]->value.num);
}

object *tach_lib_sub(vm *state, uint32_t argc, object **objs) {
    if (argc != 2) {
        printf("set takes 2 arguments\n");
        exit(1);
    }
    return tach_create_object_number(objs[0]->value.num - objs[1]->value.num);
}

object *tach_lib_lt(vm *state, uint32_t argc, object **objs) {
    if (argc != 2) {
        printf("set takes 2 arguments\n");
        exit(1);
    }
    object *ret = tach_malloc(sizeof(object));
    ret->type = TYPE_BOOL;
    ret->value.boolval = objs[0]->value.num < objs[1]->value.num;
    return ret;
}

object *tach_lib_eq(vm *state, uint32_t argc, object **objs) {
    if (argc != 2) {
        printf("set takes 2 arguments\n");
        exit(1);
    }
    object *ret = tach_malloc(sizeof(object));
    ret->type = TYPE_BOOL;
    ret->value.boolval = tach_clib_equal(objs[0], objs[1]);
    return ret;
}

object *tach_lib_copy(vm *state, uint32_t argc, object **objs) {
    return objs[argc-1];
}

object *tach_lib_proc(vm *state, uint32_t argc, object **objs) {
    object *obj = tach_malloc(sizeof(object));
    obj->type = TYPE_PROC;
    obj->value.proc = tach_malloc(sizeof(proc_t));
    obj->value.proc->argc = argc-2;
    obj->value.proc->argn = objs+1;
    obj->value.proc->gotoval = objs[argc-1]->value.proc->gotoval;
    tach_set_table(state->world[state->callc], objs[0], obj);
    return tach_create_object_nil();
}

object *tach_lib_if(vm *state, uint32_t argc, object **argv) {
    if (argc < 2 || argc > 3) {
        printf("if takes 2 or 3 arguments not %d\n", argc);
        exit(1);
    }
    if (tach_clib_tobool(argv[0])) {
        tach_vm_call(state, argv[1], 0, NULL);
        return NULL;
    }
    else if (argc == 3) {
        tach_vm_call(state, argv[2], 0, NULL);
        return NULL;
    }
    return tach_create_object_nil();
}

int main() {
    FILE *f = fopen("out.bytecode", "r");
    program *prog = tach_read(f);
    tach_interp(prog);
    fclose(f);
}