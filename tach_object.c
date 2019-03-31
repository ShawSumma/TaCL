#include "tach.h"

tach_string *tach_create_tach_string(char *si) {
    tach_string *str = tach_malloc(sizeof(str));
    str->count = strlen(si);
    str->str = si;
    return str;
}

tach_object *tach_create_tach_object_nil() {
    tach_object *ret = tach_malloc(sizeof(tach_object));
    ret->type = TACH_OBJECT_TYPE_NIL;
    return ret;
}

tach_object *tach_create_tach_object_tach_string(tach_string *str) {
    tach_object *ret = tach_malloc(sizeof(tach_object));
    ret->type = TACH_OBJECT_TYPE_tach_string;
    ret->value.str = str;
    return ret;    
}

tach_object *tach_create_tach_object_number(double d) {
    tach_object *ret = tach_malloc(sizeof(tach_object));
    ret->type = TACH_OBJECT_TYPE_NUMBER;
    ret->value.num = d;
    return ret;  
}

tach_object *tach_create_tach_object_func(func_t func) {
    tach_object *ret = tach_malloc(sizeof(tach_object));
    ret->type = TACH_OBJECT_TYPE_FUNC;
    ret->value.func = func;
    return ret;  
}

tach_object *tach_create_tach_object_tach_vector(tach_vector *vec) {
    tach_object *ret = tach_malloc(sizeof(tach_object));
    ret->type = TACH_OBJECT_TYPE_VECTOR;
    ret->value.vec = vec;
    return ret;  
}

tach_object *tach_create_tach_object_proc(uint32_t val) {
    tach_object *ret = tach_malloc(sizeof(tach_object));
    ret->type = TACH_OBJECT_TYPE_PROC;
    ret->value.proc = tach_malloc(sizeof(tach_proc_t));
    ret->value.proc->argc = 0;
    ret->value.proc->argn = tach_malloc(0);
    ret->value.proc->gotoval = val;
    return ret;  
}

tach_mapping *tach_create_tach_mapping() {
    tach_mapping *ret = tach_malloc(sizeof(tach_mapping));
    ret->key = NULL;
    ret->val = NULL;
    ret->left = NULL;
    ret->right = NULL;
    return ret;
}

bool tach_set_tach_mapping(tach_mapping *t, tach_object *k, tach_object *v) {
    while (true) {
        if (t->key == NULL) {
            t->key = k;
            t->val = v;
            t->left = tach_create_tach_mapping();
            t->right = tach_create_tach_mapping();
            return true;
        }
        char cmp = tach_clib_cmp(k, t->key);
        if (cmp == 0) {
            t->val = v;
            return false;
        }
        if (cmp == -1) {
            t = t->left;
        }
        if (cmp == 1) {
            t = t->right;
        }
    }
}

tach_object *tach_get_tach_mapping(tach_mapping *t, tach_object *k) {
    while (true) {
        if (t == NULL || t->key == NULL) {
            return NULL;
        }
        char cmp = tach_clib_cmp(k, t->key);
        if (cmp == 0) {
            return t->val;
        }
        if (cmp == -1) {
            t = t->left;
        }
        if (cmp == 1) {
            t = t->right;
        }
    }
}