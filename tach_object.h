#pragma once

#include "tach.h"

typedef tach_object (*func_t)(vm *, uint32_t, tach_object *);

struct tach_proc_t {
    tach_object *argn;
    uint32_t argc;
    uint32_t gotoval;
};


struct tach_string {
    char *str;
    uint32_t count;
};

struct tach_vector {
    tach_object *children;
    uint32_t count;
    uint32_t alloc;
};


struct tach_object {
    enum {
        TACH_OBJECT_TYPE_NULL,
        TACH_OBJECT_TYPE_NIL,
        TACH_OBJECT_TYPE_NUMBER,
        TACH_OBJECT_TYPE_TACH_STRING,
        TACH_OBJECT_TYPE_VECTOR,
        TACH_OBJECT_TYPE_FUNC,
        TACH_OBJECT_TYPE_PROC,
        TACH_OBJECT_TYPE_BOOL,
    } type;
    union {
        tach_string str;
        tach_vector vec;
        tach_proc_t *proc;
        func_t func;
        double num;
        bool boolval;
    } value;
};

struct tach_mapping {
    tach_object key;
    tach_object val;
    tach_mapping *left;
    tach_mapping *right;
};

tach_mapping *tach_create_tach_mapping();
tach_object tach_create_tach_object_nil();
tach_object tach_create_tach_object_tach_string(tach_string str);
tach_object tach_create_tach_object_number(double d);
tach_object tach_create_tach_object_tach_vector(tach_vector *vec);
tach_object tach_create_tach_object_func(func_t func);
tach_object tach_create_tach_object_proc(uint32_t val);
tach_string tach_create_tach_string(char *);
bool tach_set_tach_mapping(tach_mapping *t, tach_object k, tach_object v);
tach_object tach_get_tach_mapping(tach_mapping *t, tach_object k);