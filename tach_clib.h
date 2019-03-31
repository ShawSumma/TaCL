#pragma once

#include "tach.h"

bool tach_clib_equal(tach_object a, tach_object b);
bool tach_clib_less(tach_object a, tach_object b);
bool tach_clib_greater(tach_object a, tach_object b);
char tach_clib_cmp(tach_object a, tach_object b);
bool tach_clib_tobool(tach_object obj);
void tach_clib_print(FILE *f, tach_object obj);
void tach_clib_println(FILE *f, tach_object obj);

