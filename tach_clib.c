#include "tach_clib.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

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