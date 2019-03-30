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

// tach_mapping *tach_create_tach_mapping() {
//     tach_mapping *ret = tach_malloc(sizeof(tach_mapping));
//     ret->alloc = 8;
//     ret->size = 0;
//     ret->key = tach_malloc(sizeof(tach_object *) * ret->alloc);
//     ret->val = tach_malloc(sizeof(tach_object *) * ret->alloc);
//     return ret;
// }

// void tach_insert_tach_mapping(tach_mapping *t, tach_object *k, tach_object *v) {
//     if (t->size + 4 >= t->alloc) {
//         t->alloc *= 1.5;
//         t->key = tach_realloc(t->key, sizeof(tach_object *) * t->alloc);
//         t->val = tach_realloc(t->val, sizeof(tach_object *) * t->alloc);
//     }
//     t->key[t->size] = k;
//     t->val[t->size] = v;
//     t->size ++;
// }

// bool tach_set_tach_mapping(tach_mapping *t, tach_object *k, tach_object *v) {
//     for (uint32_t i = 0; i < t->size; i++) {
//         if (tach_clib_equal(k, t->key[i])) {
//             t->val[i] = v;
//             return false;
//         }
//     }
//     tach_insert_tach_mapping(t, k, v);
//     return true;
// }

// tach_object *tach_get_tach_mapping(tach_mapping *t, tach_object *k) {
//     for (uint32_t i = 0; i < t->size; i++) {
//         if (tach_clib_equal(k, t->key[i])) {
//             return t->val[i];
//         }
//     }
//     return NULL;
// }

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
    printf("\nstart\n");
    while (true) {
        if (t == NULL || t->key == NULL) {
            return NULL;
        }
        tach_clib_println(stderr, t->key);
        tach_clib_println(stderr, k);
        char cmp = tach_clib_cmp(k, t->key);
        if (cmp == 0) {
            return t->val;
        }
        if (cmp == -1) {
            // printf("left\n");
            t = t->left;
        }
        if (cmp == 1) {
            // printf("right\n");
            t = t->right;
        }
    }
}