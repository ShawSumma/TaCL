
#include "tach.h"

char *tach_opcode_name(opcode op) {
    switch (op) {
        case OPCODE_CALL:
            return "CALL";
        case OPCODE_ERR:
            return "ERR";
        case OPCODE_NAME:
            return "NAME";
        case OPCODE_NUM:
            return "NUM";
        case OPCODE_POP:
            return "POP";
        case OPCODE_PROC:
            return "PROC";
        case OPCODE_RET:
            return "RET";
        case OPCODE_STR:
            return "STR";
    }
}

table *tach_create_world_base() {
    table *table = tach_create_table();
    object
        *trueobj = tach_malloc(sizeof(object)),
        *falseobj = tach_malloc(sizeof(object));
    trueobj->type = TYPE_BOOL;
    falseobj->type = TYPE_BOOL;
    trueobj->value.boolval = true;
    falseobj->value.boolval = false;
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("print")),
        tach_create_object_func(tach_lib_print)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("set")),
        tach_create_object_func(tach_lib_set)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("proc")),
        tach_create_object_func(tach_lib_proc)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("add")),
        tach_create_object_func(tach_lib_add)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("sub")),
        tach_create_object_func(tach_lib_sub)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("if")),
        tach_create_object_func(tach_lib_if)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("lt")),
        tach_create_object_func(tach_lib_lt)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("eq")),
        tach_create_object_func(tach_lib_eq)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("true")),
        trueobj
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("copy")),
        tach_create_object_func(tach_lib_copy)
    );
    tach_set_table(
        table,
        tach_create_object_string(tach_create_string("false")),
        falseobj
    );
    return table;
}

void tach_vm_push(vm *state, object *o) {
    if (state->stackc + 4 >= state->stacka) {
        state->stacka *= 1.5;
        state->stack = tach_realloc(state->stack, sizeof(object *) * state->stacka);
    }
    state->stack[state->stackc] = o;
    state->stackc ++;
}

vm *tach_create_state() {
    vm *ret = tach_malloc(sizeof(vm));
    ret->calla = 256;
    ret->callc = 0;
    ret->stacka = 256;
    ret->stackc = 0;
    
    ret->world = tach_malloc(sizeof(table *) * ret->calla);
    ret->calls = tach_malloc(sizeof(uint32_t) * ret->calla);
    ret->stack = tach_malloc(sizeof(object *) * ret->stacka);

    ret->world[0] = tach_create_world_base();
    ret->place = 0;

    return ret;
}

void tach_vm_call(vm *state, object *func, uint32_t argc, object **argv) {
    switch (func->type) {
        case TYPE_FUNC: {
            object *result = func->value.func(state, argc, argv);
            if (result != NULL) {
                tach_vm_push(state, result);
            }
            break;
        }
        case TYPE_PROC: {
            if (state->callc + 4 >= state->calla) {
                state->calla *= 1.5;
                state->calls = tach_realloc(state->calls, sizeof(uint32_t) * state->calla);
                state->world = tach_realloc(state->world, sizeof(table *) * state->callc);
            }
            state->calls[state->callc] = state->place;
            state->callc ++;
            state->world[state->callc] = tach_create_table();
            state->place = func->value.proc->gotoval;
            for (uint32_t i = 0; i < func->value.proc->argc; i++) {
                tach_set_table(state->world[state->callc], func->value.proc->argn[i], argv[i]);
            }
            break;
        }
        default: {
            printf("error, cannot call that!\n");
            exit(1);
        }
    }
}

void tach_interp(program *prog) {

    vm *state = tach_create_state();

    while (state->place < prog->opcount) {
        uint32_t i = state->place;
        switch (prog->opcodes[i]) {
            case OPCODE_NAME: {
                char *name = prog->strings[prog->opvalues[i]];
                object *obj = NULL;
                object *nameobj = tach_create_object_string(tach_create_string(name));
                for (uint32_t i = state->callc; i >= 0; i--) {
                    if (obj != NULL) {
                        goto done;
                    }
                    obj = tach_get_table(state->world[i], nameobj);
                }
                if (obj == NULL) {
                    printf("no name %s\n", name);
                    exit(1);
                }
                done:
                tach_vm_push(state, obj);
                break;
            }
            case OPCODE_RET: {
                state->callc --;
                state->place = state->calls[state->callc];
                break;
            }
            case OPCODE_PROC: {
                object *obj = tach_create_object_proc(i);
                tach_vm_push(state, obj);
                state->place += prog->opvalues[i] + 1;
                break;
            }
            case OPCODE_NUM: {
                tach_vm_push(state, tach_create_object_number(prog->numbers[prog->opvalues[i]]));
                break;
            }
            case OPCODE_POP: {
                state->stackc --;
                break;
            }
            case OPCODE_STR: {
                char *name = prog->strings[prog->opvalues[i]];
                tach_vm_push(state, tach_create_object_string(tach_create_string(name)));
                break;
            }
            case OPCODE_CALL: {
                uint32_t argc = prog->opvalues[i];
                object **args = tach_malloc(sizeof(object *) * argc);
                for (uint32_t i = 0; i < argc; i++) {
                    args[argc-1-i] = state->stack[state->stackc-1];
                    state->stackc --;
                }
                object *func = state->stack[state->stackc-1];
                state->stackc --;
                tach_vm_call(state, func, argc, args);
                break;
            }
            default :{
                printf("cannot handle %s\n", tach_opcode_name(prog->opcodes[i]));
                exit(1);
            }
        }
        state->place++;
    }
}