#include "tach.h"

bool tach_clib_equal(tach_object a, tach_object b) {
    return tach_clib_cmp(a, b) == 0;
}

bool tach_clib_less(tach_object a, tach_object b) {
    return tach_clib_cmp(a, b) == -1;
}

bool tach_clib_greater(tach_object a, tach_object b) {
    return tach_clib_cmp(a, b) == 1;
}

char tach_clib_cmp(tach_object a, tach_object b) {
    if (a.type < b.type) {
        return -1;
    }
    if (a.type > b.type) {
        return 1;
    }
    switch (a.type) {
         case TACH_OBJECT_TYPE_NIL:
            return 0;
        case TACH_OBJECT_TYPE_NUMBER:
            if (a.value.num < b.value.num) {
                return -1;
            }
            if (a.value.num > b.value.num) {
                return 1;
            }
            return 0;
        case TACH_OBJECT_TYPE_TACH_STRING: {
            tach_string stra = a.value.str;
            tach_string strb = b.value.str;
            if (stra.count < strb.count) {
                return -1;
            }
            if (stra.count > stra.count) {
                return 1;
            }
            for (uint32_t i = 0; i < stra.count; i++) {
                if (stra.str[i] < strb.str[i]) {
                    return -1;
                
                }
                if (stra.str[i] > strb.str[i]) {
                    return 1;
                }
            }
            return 0;
        }
        case TACH_OBJECT_TYPE_VECTOR:
            return 0;
        case TACH_OBJECT_TYPE_FUNC:
            return 0;
        case TACH_OBJECT_TYPE_PROC:
            return 0;
        case TACH_OBJECT_TYPE_BOOL:
            return a.value.boolval - b.value.boolval;
    }
}

void tach_clib_print( FILE *f, tach_object obj) {
    switch (obj.type) {
        case TACH_OBJECT_TYPE_NUMBER:
            if (fmod(obj.value.num, 1) == 0) {
                fprintf(f, "%ld", (int64_t) obj.value.num);
            }
            else {
                fprintf(f, "%lf", obj.value.num);
            }
            break;
        case TACH_OBJECT_TYPE_TACH_STRING:
            fprintf(f, "%s", obj.value.str.str);
            break;
        case TACH_OBJECT_TYPE_NIL:
            fprintf(f, "nil");
            break;
        case TACH_OBJECT_TYPE_FUNC:
            fprintf(f, "(func %p)", obj.value.func);
            break;
        case TACH_OBJECT_TYPE_VECTOR:
            fprintf(f, "[vector ");
            for (uint32_t i = 0; i < obj.value.vec.count; i++) {
                if (i != 0) {
                    fprintf(f, " ");
                }
                tach_clib_print(f,obj.value.vec.children[i]);
            }
            fprintf(f, "]");
            break;
        case TACH_OBJECT_TYPE_PROC:
            fprintf(f, "(proc %p)", obj.value.proc);      
            break;      
        case TACH_OBJECT_TYPE_BOOL:
            fprintf(f, "%s", obj.value.boolval ? "true" : "false");
            break;
    }
}

tach_object tach_clib_deepcopy(tach_object obj) {
    tach_object ret;
    ret.type = obj.type;
    if (obj.type == TACH_OBJECT_TYPE_VECTOR) {
        ret.value.vec.alloc = obj.value.vec.alloc;
        ret.value.vec.count = obj.value.vec.count;
        ret.value.vec.children = tach_malloc(sizeof(tach_object) * ret.value.vec.alloc);
        for (uint32_t i = 0; i < ret.value.vec.count; i++) {
            ret.value.vec.children[i] = tach_clib_deepcopy(obj.value.vec.children[i]);
        }
    }
    else {
        ret.value = obj.value;
    }
    return ret;
}

void tach_clib_println(FILE *f, tach_object obj) {
    tach_clib_print(f, obj);
    fprintf(f, "\n");
}

bool tach_clib_tobool(tach_object obj) {
    if (obj.type == TACH_OBJECT_TYPE_NIL) {
        return false;
    }
    if (obj.type == TACH_OBJECT_TYPE_BOOL) {
        return obj.value.boolval;
    }
    return true;
}