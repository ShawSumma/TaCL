
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

void tach_vm_error(tach_vm *state, char *err) {
    state->error = tach_error_string(err);
} 

tach_mapping *tach_create_world_base() {
    tach_mapping *tach_mapping = tach_create_tach_mapping();
    tach_object
        *trueobj = tach_malloc(sizeof(tach_object)),
        *falseobj = tach_malloc(sizeof(tach_object));
    trueobj->type = TACH_OBJECT_TYPE_BOOL;
    falseobj->type = TACH_OBJECT_TYPE_BOOL;
    trueobj->value.boolval = true;
    falseobj->value.boolval = false;
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("print")),
        tach_create_tach_object_func(tach_lib_print)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("set")),
        tach_create_tach_object_func(tach_lib_set)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("proc")),
        tach_create_tach_object_func(tach_lib_proc)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("add")),
        tach_create_tach_object_func(tach_lib_add)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("sub")),
        tach_create_tach_object_func(tach_lib_sub)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("mul")),
        tach_create_tach_object_func(tach_lib_mul)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("div")),
        tach_create_tach_object_func(tach_lib_div)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("if")),
        tach_create_tach_object_func(tach_lib_if)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("lt")),
        tach_create_tach_object_func(tach_lib_lt)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("eq")),
        tach_create_tach_object_func(tach_lib_eq)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("gt")),
        tach_create_tach_object_func(tach_lib_gt)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("neq")),
        tach_create_tach_object_func(tach_lib_neq)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("copy")),
        tach_create_tach_object_func(tach_lib_copy)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("parent")),
        tach_create_tach_object_func(tach_lib_parent)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("child")),
        tach_create_tach_object_func(tach_lib_child)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("eval")),
        tach_create_tach_object_func(tach_lib_eval)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("get")),
        tach_create_tach_object_func(tach_lib_get)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("depth")),
        tach_create_tach_object_func(tach_lib_depth)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("vector")),
        tach_create_tach_object_func(tach_lib_vector)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("append")),
        tach_create_tach_object_func(tach_lib_append)
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("true")),
        *trueobj
    );
    tach_set_tach_mapping(
        tach_mapping,
        tach_create_tach_object_tach_string(tach_create_tach_string("false")),
        *falseobj
    );
    return tach_mapping;
}

void tach_vm_push(tach_vm *state, tach_object o) {
    if (state->stackc + 4 >= state->stacka) {
        state->stacka *= 1.5;
        state->stack = tach_realloc(state->stack, sizeof(tach_object) * state->stacka);
    }
    state->stack[state->stackc] = o;
    state->stackc ++;
}

tach_vm *tach_create_state() {
    tach_vm*ret = tach_malloc(sizeof(tach_vm));
    ret->calla = 8;
    ret->callc = 0;
    ret->stacka = 8;
    ret->stackc = 0;

    ret->backlevel = 0;
    
    ret->world = tach_malloc(sizeof(tach_mapping *) * ret->calla);
    ret->calls = tach_malloc(sizeof(uint32_t) * ret->calla);
    ret->stack = tach_malloc(sizeof(tach_object) * ret->stacka);

    ret->world[0] = tach_create_world_base();
    ret->place = 0;

    ret->error = NULL;

    ret->common.objtrue.type = TACH_OBJECT_TYPE_BOOL;
    ret->common.objfalse.type = TACH_OBJECT_TYPE_BOOL;
    ret->common.objtrue.value.boolval = true;
    ret->common.objfalse.value.boolval = false;
    ret->common.objnil.type = TACH_OBJECT_TYPE_NIL;
    ret->common.objnull.type = TACH_OBJECT_TYPE_NULL;

    return ret;
}

void tach_vm_except(tach_vm *state) {
    printf("error: %s\n", state->error->str.str);
    state->error = NULL;
    exit(1);
}

void tach_vm_call(tach_vm *state, tach_object func, uint32_t argc, tach_object *argv) {
    switch (func.type) {
        case TACH_OBJECT_TYPE_FUNC: {
            tach_object result = func.value.func(state, argc, argv);
            if (result.type != TACH_OBJECT_TYPE_NULL) {
                tach_vm_push(state, result);
            }
            else if (state->error != NULL) {
                tach_vm_except(state);
            }
            break;
        }
        case TACH_OBJECT_TYPE_PROC: {
            if (state->callc + 4 >= state->calla) {
                state->calla *= 1.5;
                state->calls = tach_realloc(state->calls, sizeof(uint32_t) * state->calla);
                state->world = tach_realloc(state->world, sizeof(tach_mapping *) * state->calla);
            }
            state->calls[state->callc] = state->place;
            state->callc ++;
            state->world[state->callc] = tach_create_tach_mapping();
            state->place = func.value.proc->gotoval;
            for (uint32_t i = 0; i < func.value.proc->argc; i++) {
                tach_set_tach_mapping(state->world[state->callc], func.value.proc->argn[i], argv[i]);
            }
            break;
        }
        default: {
            tach_clib_println(stdout, func);
            printf("error, cannot call that!\n");
            exit(1);
        }
    }
}

void tach_interp(tach_program *prog, tach_vm *state) {
    while (state->place < prog->opcount) {
        uint32_t i = state->place;
        switch (prog->opcodes[i]) {
            case OPCODE_NAME: {
                char *name = prog->strings[prog->opvalues[i]];
                tach_object strobj;
                strobj.type = TACH_OBJECT_TYPE_TACH_STRING;
                strobj.value.str = tach_create_tach_string(name);
                tach_object obj = tach_get_tach_mapping(state->world[0], strobj);
                uint32_t depth = 0;
                for (uint32_t j = state->callc-state->backlevel; j > 0 && obj.type == TACH_OBJECT_TYPE_NULL; j--) {
                    obj = tach_get_tach_mapping(state->world[j], strobj);
                    depth ++;
                }
                if (obj.type == TACH_OBJECT_TYPE_NULL) {
                    printf("error: no name %s\n", name);
                    exit(1);
                }
                tach_vm_push(state, obj);
                break;
            }
            case OPCODE_RET: {
                state->callc --;
                state->place = state->calls[state->callc];
                break;
            }
            case OPCODE_PROC: {
                tach_object obj = tach_create_tach_object_proc(i);
                tach_vm_push(state, obj);
                state->place += prog->opvalues[i] + 1;
                break;
            }
            case OPCODE_NUM: {
                tach_vm_push(state, tach_create_tach_object_number(prog->numbers[prog->opvalues[i]]));
                break;
            }
            case OPCODE_POP: {
                state->stackc --;
                break;
            }
            case OPCODE_STR: {
                char *name = prog->strings[prog->opvalues[i]];
                tach_vm_push(state, tach_create_tach_object_tach_string(tach_create_tach_string(name)));
                break;
            }
            case OPCODE_CALL: {
                uint32_t argc = prog->opvalues[i];
                tach_object *args = tach_malloc(sizeof(tach_object) * argc);
                for (uint32_t i = 0; i < argc; i++) {
                    args[argc-1-i] = state->stack[state->stackc-1];
                    state->stackc --;
                }
                tach_object func = state->stack[state->stackc-1];
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