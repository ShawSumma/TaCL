#pragma once

#include "tach.h"

typedef object *(*func_t)(vm *, uint32_t, object **);

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

table *tach_create_table();
object *tach_create_object_nil();
object *tach_create_object_string(string *str);
object *tach_create_object_number(double d);
object *tach_create_object_vector(vector *vec);
object *tach_create_object_func(func_t func);
object *tach_create_object_proc(uint32_t val);
string *tach_create_string(char *);
bool tach_set_table(table *t, object *k, object *v);
object *tach_get_table(table *t, object *k);