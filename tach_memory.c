#include "tach_memory.h"

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