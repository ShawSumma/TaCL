#pragma once

#include "tach.h"

struct vm {
    object **stack;
    table **world;
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
    char **strings;
};

char *tach_opcode_name(opcode op);
void tach_vm_call(vm *state, object *func, uint32_t argc, object **argv);
void tach_interp(program *prog);
table *tach_create_world_base();
vm *tach_create_state();