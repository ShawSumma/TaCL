#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "tach_object.h"

bool tach_clib_equal(object *a, object *b);
bool tach_clib_tobool(object *obj);
void tach_clib_print(FILE *f, object *obj);
void tach_clib_println(FILE *f, object *obj);

