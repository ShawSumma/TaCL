#pragma once
#include "tach.h"

struct tach_error {
    char *str;
};

tach_error *tach_error_string(char *str);