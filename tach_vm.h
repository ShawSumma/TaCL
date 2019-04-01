#pragma once

#include "tach.h"

struct tach_vm {
    struct {
        tach_object objnil;
        tach_object objtrue;
        tach_object objfalse;
        tach_object objnull;
    } common;
    tach_object *stack;
    tach_mapping **world;
    tach_error *error;
    uint32_t *calls;
    uint32_t calla;
    uint32_t callc;
    uint32_t stacka;
    uint32_t stackc;
    uint32_t place;
    uint32_t backlevel;
};

struct tach_program {
    uint32_t opcount;
    uint32_t alloc;
    uint32_t stringc;
    uint32_t stringa;
    uint32_t numberc;
    uint32_t numbera;
    opcode *opcodes;
    uint32_t *opvalues;
    double *numbers;
    char **strings;
};

void tach_vm_push(tach_vm *state, tach_object o);
char *tach_opcode_name(opcode op);
void tach_vm_call(tach_vm *state, tach_object func, uint32_t argc, tach_object *argv);
void tach_interp(tach_program *prog);
tach_mapping *tach_create_world_base();
tach_vm *tach_create_state();
void tach_vm_error(tach_vm *state, char *err);