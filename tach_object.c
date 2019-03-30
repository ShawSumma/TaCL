#include "tach_object.h"

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
