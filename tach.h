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

struct object;
typedef struct object object;
struct string;
typedef struct string string;
struct vector;
typedef struct vector vector;
struct table;
typedef struct table table;
struct proc_t;
typedef struct proc_t proc_t;

#include "tach_clib.h"
#include "tach_lib.h"
#include "tach_object.h"
#include "tach_memory.h"
#include "tach_read.h"
#include "tach_vm.h"