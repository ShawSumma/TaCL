#pragma once

#include <gc.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct tach_vm;
typedef struct tach_vm tach_vm;
struct tach_program;
typedef struct tach_program tach_program;

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

enum tach_token_type {
    TACH_TOKEN_NEWLINE,

    TACH_TOKEN_STRING,
    TACH_TOKEN_DOLLAR,
    TACH_TOKEN_NUMBER,

    TACH_TOKEN_OPEN,
    TACH_TOKEN_CLOSE,

    TACH_TOKEN_BEGIN,
    TACH_TOKEN_END,
};
typedef enum tach_token_type tach_token_type;

enum tach_ast_node_type {
    TACH_AST_TYPE_ERROR,

    TACH_AST_TYPE_PROGRAM,
    TACH_AST_TYPE_EMPTY,
    TACH_AST_TYPE_BODY,

    TACH_AST_TYPE_COMMAND,
    
    TACH_AST_TYPE_STRING,
    TACH_AST_TYPE_NAME,
    TACH_AST_TYPE_NUMBER,
};
typedef enum tach_ast_node_type tach_ast_node_type;

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

struct tach_ast;
typedef struct tach_ast tach_ast;
struct tach_tokens;
typedef struct tach_tokens tach_tokens;

struct tach_error;
typedef struct tach_error tach_error;

#include "tach_clib.h"
#include "tach_lib.h"
#include "tach_object.h"
#include "tach_memory.h"
#include "tach_read.h"
#include "tach_vm.h"
#include "tach_errors.h"
#include "tach_parse.h"
#include "tach_comp.h"