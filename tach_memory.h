#pragma once

#include "tach.h"

void *tach_malloc(size_t size);
void *tach_realloc(void *ptr, size_t size);
void tach_free(void *ptr);
