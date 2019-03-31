#pragma once
#include "tach.h"

struct tach_error {
    tach_string str;
};

tach_error *tach_error_string(char *str);