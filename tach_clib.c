#include "tach.h"

bool tach_clib_equal(tach_object *a, tach_object *b) {
    if (a->type != b->type) {
        return false;
    }
    switch (a->type) {
        case TACH_OBJECT_TYPE_NUMBER:
            return a->value.num == b->value.num;
        case TACH_OBJECT_TYPE_tach_string:
            if (a->value.str->count != b->value.str->count) {
                return false;
            }
            return !strcmp(a->value.str->str, b->value.str->str);
        case TACH_OBJECT_TYPE_NIL:
            return true;
        case TACH_OBJECT_TYPE_FUNC:
            return false;
        case TACH_OBJECT_TYPE_VECTOR:
            return false;
        case TACH_OBJECT_TYPE_PROC:
            return false;
        case TACH_OBJECT_TYPE_BOOL:
            return a->value.boolval == b->value.boolval;
    }
}

void tach_clib_print( FILE *f, tach_object *obj) {
    switch (obj->type) {
        case TACH_OBJECT_TYPE_NUMBER:
            fprintf(f, "%lf", obj->value.num);
            break;
        case TACH_OBJECT_TYPE_tach_string:
            fprintf(f, "%s", obj->value.str->str);
            break;
        case TACH_OBJECT_TYPE_NIL:
            fprintf(f, "nil");
            break;
        case TACH_OBJECT_TYPE_FUNC:
            fprintf(f, "(func %p)", obj->value.func);
            break;
        case TACH_OBJECT_TYPE_VECTOR:
            fprintf(f, "(list)");
            break;
        case TACH_OBJECT_TYPE_PROC:
            fprintf(f, "(proc %p)", obj->value.proc);      
            break;      
        case TACH_OBJECT_TYPE_BOOL:
            fprintf(f, "%s", obj->value.boolval ? "true" : "false");
            break;
    }
}

void tach_clib_println(FILE *f, tach_object *obj) {
    tach_clib_print(f, obj);
    fprintf(f, "\n");
}

bool tach_clib_tobool(tach_object *obj) {
    if (obj->type == TACH_OBJECT_TYPE_NIL) {
        return false;
    }
    if (obj->type == TACH_OBJECT_TYPE_BOOL) {
        return obj->value.boolval;
    }
    return true;
}