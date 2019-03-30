#pragma once

#include "tach.h"

struct vm {
    tach_object **stack;
    tach_mapping **world;
    tach_error *error;
    uint32_t *calls;
    uint32_t calla;
    uint32_t callc;
    uint32_t stacka;
    uint32_t stackc;
    uint32_t place;
};

struct program {
    uint32_t opcount;
    opcode *opcodes;
    uint32_t *opvalues;
    double *numbers;
    char **tach_strings;
};

char *tach_opcode_name(opcode op);
void tach_vm_call(vm *state, tach_object *func, uint32_t argc, tach_object **argv);
void tach_interp(program *prog);
tach_mapping *tach_create_world_base();
vm *tach_create_state();
void tach_vm_error(vm *state, char *err);