#pragma once

#include <gc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct vm;
typedef struct vm vm;
struct program;
typedef struct program program;

enum opcode {
    OPCODE_ERR,
    OPCODE_STR,
    OPCODE_NAME,
    OPCODE_NUM,
    OPCODE_CALL,
    OPCODE_RET,
    OPCODE_POP,
    OPCODE_PROC
};
typedef enum opcode opcode;

struct tach_object;
typedef struct tach_object tach_object;
struct tach_string;
typedef struct tach_string tach_string;
struct tach_vector;
typedef struct tach_vector tach_vector;
struct tach_mapping;
typedef struct tach_mapping tach_mapping;
struct tach_proc_t;
typedef struct tach_proc_t tach_proc_t;

#include "tach_clib.h"
#include "tach_lib.h"
#include "tach_object.h"
#include "tach_memory.h"
#include "tach_read.h"
#include "tach_vm.h"