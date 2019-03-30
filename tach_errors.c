#include "tach.h"

tach_error *tach_error_string(char *str) {
    tach_error *err = tach_malloc(sizeof(tach_error));
    err->str = str;
    return err;
}
