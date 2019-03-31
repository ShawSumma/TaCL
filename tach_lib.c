#include "tach.h"


tach_object *tach_lib_print(vm *state, uint32_t argc, tach_object **objs) {
    for (int i = 0; i < argc; i++) {
        if (i != 0) {
            fprintf(stdout, " ");
        }
        tach_clib_print(stdout, objs[i]);
    }
    fprintf(stdout, "\n");
    return tach_create_tach_object_nil();
}

tach_object *tach_lib_set(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 2) {
        tach_vm_error(state, "set takes 2 arguments");
        return NULL;
    }
    tach_set_tach_mapping(state->world[state->callc-state->backlevel], objs[0], objs[1]);
    return tach_create_tach_object_nil();
}

tach_object *tach_lib_get(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 1) {
        tach_vm_error(state, "get takes 1 argument");
        return NULL;
    }
    tach_object *obj = tach_get_tach_mapping(state->world[0], objs[0]);
    for (uint32_t i = state->callc; i > 0 && obj == NULL; i--) {
        obj = tach_get_tach_mapping(state->world[i], objs[0]);
    }
    if (obj == NULL) {
        tach_vm_error(state, "get could not find that");
        return NULL;
    }
    return obj;
}

tach_object *tach_lib_add(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 2) {
        tach_vm_error(state, "add takes 2 numbers");
        return NULL;
    }
    if (objs[0]->type != TACH_OBJECT_TYPE_NUMBER || objs[1]->type != TACH_OBJECT_TYPE_NUMBER) {
        tach_vm_error(state, "add takes 2 numbers");
        return NULL;
    }
    return tach_create_tach_object_number(objs[0]->value.num + objs[1]->value.num);
}

tach_object *tach_lib_sub(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 2) {
        tach_vm_error(state, "sub takes 2 numbers");
        return NULL;
    }
    if (objs[0]->type != TACH_OBJECT_TYPE_NUMBER || objs[1]->type != TACH_OBJECT_TYPE_NUMBER) {
        tach_vm_error(state, "sub takes 2 numbers");
        return NULL;
    }
    return tach_create_tach_object_number(objs[0]->value.num - objs[1]->value.num);
}

tach_object *tach_lib_mul(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 2) {
        tach_vm_error(state, "mul takes 2 numbers");
        return NULL;
    }
    if (objs[0]->type != TACH_OBJECT_TYPE_NUMBER || objs[1]->type != TACH_OBJECT_TYPE_NUMBER) {
        tach_vm_error(state, "mul takes 2 numbers");
        return NULL;
    }
    return tach_create_tach_object_number(objs[0]->value.num * objs[1]->value.num);
}

tach_object *tach_lib_div(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 2) {
        tach_vm_error(state, "div takes 2 numbers");
        return NULL;
    }
    if (objs[0]->type != TACH_OBJECT_TYPE_NUMBER || objs[1]->type != TACH_OBJECT_TYPE_NUMBER) {
        tach_vm_error(state, "div takes 2 numbers");
        return NULL;
    }
    return tach_create_tach_object_number(objs[0]->value.num / objs[1]->value.num);
}

tach_object *tach_lib_cmp(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 2) {
        tach_vm_error(state, "cmp takes 2 arguments");
        return NULL;
    }
    return tach_create_tach_object_number(tach_clib_cmp(objs[0], objs[1]));
}

tach_object *tach_lib_lt(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 2) {
        tach_vm_error(state, "lt takes 2 arguments");
        return NULL;
    }
    tach_object *ret = tach_malloc(sizeof(tach_object));
    ret->type = TACH_OBJECT_TYPE_BOOL;
    ret->value.boolval = tach_clib_cmp(objs[0], objs[1]) == -1;
    return ret;
}

tach_object *tach_lib_gt(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 2) {
        tach_vm_error(state, "gt takes 2 arguments");
        return NULL;
    }
    tach_object *ret = tach_malloc(sizeof(tach_object));
    ret->type = TACH_OBJECT_TYPE_BOOL;
    ret->value.boolval = tach_clib_cmp(objs[0], objs[1]) == 1;
    return ret;
}

tach_object *tach_lib_eq(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 2) {
        tach_vm_error(state, "eq takes 2 arguments");
        return NULL;
    }
    tach_object *ret = tach_malloc(sizeof(tach_object));
    ret->type = TACH_OBJECT_TYPE_BOOL;
    ret->value.boolval = tach_clib_cmp(objs[0], objs[1]) == 0;
    return ret;
}

tach_object *tach_lib_neq(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 2) {
        tach_vm_error(state, "neq takes 2 arguments");
        return NULL;
    }
    tach_object *ret = tach_malloc(sizeof(tach_object));
    ret->type = TACH_OBJECT_TYPE_BOOL;
    ret->value.boolval = tach_clib_cmp(objs[0], objs[1]) != 0;
    return ret;
}

tach_object *tach_lib_copy(vm *state, uint32_t argc, tach_object **objs) {
    return objs[argc-1];
}

tach_object *tach_lib_proc(vm *state, uint32_t argc, tach_object **objs) {
    tach_object *obj = tach_malloc(sizeof(tach_object));
    obj->type = TACH_OBJECT_TYPE_PROC;
    obj->value.proc = tach_malloc(sizeof(tach_proc_t));
    obj->value.proc->argc = argc-2;
    obj->value.proc->argn = objs+1;
    obj->value.proc->gotoval = objs[argc-1]->value.proc->gotoval;
    tach_set_tach_mapping(state->world[state->callc-state->backlevel], objs[0], obj);
    return tach_create_tach_object_nil();
}

tach_object *tach_lib_if(vm *state, uint32_t argc, tach_object **argv) {
    if (argc < 2 || argc > 3) {
        tach_vm_error(state, "if takes 2 or 3 arguments");
        return NULL;
    }
    if (tach_clib_tobool(argv[0])) {
        tach_vm_call(state, argv[1], 0, NULL);
        return NULL;
    }
    else if (argc == 3) {
        tach_vm_call(state, argv[2], 0, NULL);
        return NULL;
    }
    return tach_create_tach_object_nil();
}

tach_object *tach_lib_depth(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 0) {
        tach_vm_error(state, "level takes no arguments");
    }
    return tach_create_tach_object_number(state->callc - state->backlevel);
}

tach_object *tach_lib_child(vm *state, uint32_t argc, tach_object **objs) {
    if (argc >= 2) {
        tach_vm_error(state, "child takes no arguments");
    }
    uint32_t count = 1;
    if (argc == 1) {
        count = objs[0]->value.num;
    }
    state->backlevel -= count;
    return tach_create_tach_object_nil();
}

tach_object *tach_lib_parent(vm *state, uint32_t argc, tach_object **objs) {
    if (argc >= 2) {
        tach_vm_error(state, "parent takes no arguments");
    }
    uint32_t count = 1;
    if (argc == 1) {
        count = objs[0]->value.num;
    }
    state->backlevel += count;
    return tach_create_tach_object_nil();
}

tach_object *tach_lib_eval(vm *state, uint32_t argc, tach_object **objs) {
    if (argc != 1) {
        tach_vm_error(state, "eval takes 1 argument");
    }
    tach_vm_call(state, objs[0], 0, NULL);
    return tach_create_tach_object_nil();
}