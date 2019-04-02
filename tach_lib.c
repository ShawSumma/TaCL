#include "tach.h"


tach_object tach_lib_print(tach_vm *state, uint32_t argc, tach_object *objs) {
    for (int i = 0; i < argc; i++) {
        if (i != 0) {
            fprintf(stdout, " ");
        }
        tach_clib_print(stdout, objs[i]);
    }
    fprintf(stdout, "\n");
    return state->common.objnil;
}

tach_object tach_lib_set(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 2) {
        tach_vm_error(state, "set takes 2 arguments");
        return state->common.objnull;
    }
    tach_set_tach_mapping(state->world[state->callc-state->backlevel], objs[0], tach_clib_deepcopy(objs[1]));
    return state->common.objnil;
}

tach_object tach_lib_get(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 1) {
        tach_vm_error(state, "get takes 1 argument");
        return state->common.objnull;
    }
    tach_object obj = tach_get_tach_mapping(state->world[0], objs[0]);
    for (uint32_t i = state->callc; i > 0 && obj.type == TACH_OBJECT_TYPE_NULL; i--) {
        obj = tach_get_tach_mapping(state->world[i], objs[0]);
    }
    if (obj.type == TACH_OBJECT_TYPE_NULL) {
        tach_vm_error(state, "get could not find that");
        return state->common.objnull;
    }
    return obj;
}

tach_object tach_lib_add(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 2) {
        tach_vm_error(state, "add takes 2 numbers");
        return state->common.objnull;
    }
    if (objs[0].type != TACH_OBJECT_TYPE_NUMBER || objs[1].type != TACH_OBJECT_TYPE_NUMBER) {
        tach_vm_error(state, "add takes 2 numbers");
        return state->common.objnull;
    }
    tach_object ret;
    ret.type = TACH_OBJECT_TYPE_NUMBER;
    ret.value.num = objs[0].value.num + objs[1].value.num;
    return ret;
}

tach_object tach_lib_sub(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 2) {
        tach_vm_error(state, "sub takes 2 numbers");
        return state->common.objnull;
    }
    if (objs[0].type != TACH_OBJECT_TYPE_NUMBER || objs[1].type != TACH_OBJECT_TYPE_NUMBER) {
        tach_vm_error(state, "sub takes 2 numbers");
        return state->common.objnull;
    }
    tach_object ret;
    ret.type = TACH_OBJECT_TYPE_NUMBER;
    ret.value.num = objs[0].value.num - objs[1].value.num;
    return ret;
}

tach_object tach_lib_mul(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 2) {
        tach_vm_error(state, "mul takes 2 numbers");
        return state->common.objnull;
    }
    if (objs[0].type != TACH_OBJECT_TYPE_NUMBER || objs[1].type != TACH_OBJECT_TYPE_NUMBER) {
        tach_vm_error(state, "mul takes 2 numbers");
        return state->common.objnull;
    }
    tach_object ret;
    ret.type = TACH_OBJECT_TYPE_NUMBER;
    ret.value.num = objs[0].value.num * objs[1].value.num;
    return ret;
}

tach_object tach_lib_div(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 2) {
        tach_vm_error(state, "div takes 2 numbers");
        return state->common.objnull;
    }
    if (objs[0].type != TACH_OBJECT_TYPE_NUMBER || objs[1].type != TACH_OBJECT_TYPE_NUMBER) {
        tach_vm_error(state, "div takes 2 numbers");
        return state->common.objnull;
    }

    tach_object ret;
    ret.type = TACH_OBJECT_TYPE_NUMBER;
    ret.value.num = objs[0].value.num / objs[1].value.num;
    return ret;
}

tach_object tach_lib_cmp(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 2) {
        tach_vm_error(state, "cmp takes 2 arguments");
        return state->common.objnull;
    }
    tach_object ret;
    ret.type = TACH_OBJECT_TYPE_NUMBER;
    ret.value.num = tach_clib_cmp(objs[0], objs[1]);
    return ret;
}

tach_object tach_lib_lt(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 2) {
        tach_vm_error(state, "lt takes 2 arguments");
        return state->common.objnull;
    }
    return tach_clib_cmp(objs[0], objs[1]) == -1 ? state->common.objtrue : state->common.objfalse;
}

tach_object tach_lib_gt(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 2) {
        tach_vm_error(state, "gt takes 2 arguments");
        return state->common.objnull;
    }
    return tach_clib_cmp(objs[0], objs[1]) == 1 ? state->common.objtrue : state->common.objfalse;
}

tach_object tach_lib_eq(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 2) {
        tach_vm_error(state, "eq takes 2 arguments");
        return state->common.objnull;
    }
    return tach_clib_cmp(objs[0], objs[1]) == 0 ? state->common.objtrue : state->common.objfalse;
}

tach_object tach_lib_neq(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 2) {
        tach_vm_error(state, "neq takes 2 arguments");
        return state->common.objnull;
    }
    return tach_clib_cmp(objs[0], objs[1]) != 0 ? state->common.objtrue : state->common.objfalse;
}

tach_object tach_lib_copy(tach_vm *state, uint32_t argc, tach_object *objs) {
    return tach_clib_deepcopy(objs[argc-1]);
}

tach_object tach_lib_proc(tach_vm *state, uint32_t argc, tach_object *objs) {
    tach_object obj;
    obj.type = TACH_OBJECT_TYPE_PROC;
    obj.value.proc = tach_malloc(sizeof(tach_proc_t));
    obj.value.proc->argc = argc-2;
    obj.value.proc->argn = objs+1;
    obj.value.proc->gotoval = objs[argc-1].value.proc->gotoval;
    tach_set_tach_mapping(state->world[state->callc-state->backlevel], objs[0], tach_clib_deepcopy(obj));
    return state->common.objnil;
}

tach_object tach_lib_if(tach_vm *state, uint32_t argc, tach_object *argv) {
    if (argc < 2 || argc > 3) {
        tach_vm_error(state, "if takes 2 or 3 arguments");
        return state->common.objnull;
    }
    if (tach_clib_tobool(argv[0])) {
        tach_vm_call(state, argv[1], 0, NULL);
        return state->common.objnull;
    }
    else if (argc == 3) {
        tach_vm_call(state, argv[2], 0, NULL);
        return state->common.objnull;
    }
    return state->common.objnil;
}

tach_object tach_lib_depth(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 0) {
        tach_vm_error(state, "level takes no arguments");
    }
    tach_object ret;
    ret.type = TACH_OBJECT_TYPE_NUMBER;
    ret.value.num = state->callc - state->backlevel;
    return ret;
}

tach_object tach_lib_child(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc >= 2) {
        tach_vm_error(state, "child takes no arguments");
    }
    uint32_t count = 1;
    if (argc == 1) {
        count = objs[0].value.num;
    }
    state->backlevel -= count;
    return state->common.objnil;
}

tach_object tach_lib_parent(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc >= 2) {
        tach_vm_error(state, "parent takes no arguments");
    }
    uint32_t count = 1;
    if (argc == 1) {
        count = objs[0].value.num;
    }
    state->backlevel += count;
    return state->common.objnil;
}

tach_object tach_lib_eval(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc != 1) {
        tach_vm_error(state, "eval takes 1 argument");
    }
    tach_vm_call(state, objs[0], 0, NULL);
    return state->common.objnil;
}

tach_object tach_lib_vector(tach_vm *state, uint32_t argc, tach_object *objs) {
    tach_object ret;
    ret.type = TACH_OBJECT_TYPE_VECTOR;
    ret.value.vec.count = argc;
    ret.value.vec.alloc = argc;
    ret.value.vec.children = objs;
    return ret;
}

tach_object tach_lib_append(tach_vm *state, uint32_t argc, tach_object *objs) {
    if (argc == 2) {
        tach_object ret = objs[0];
        if (ret.value.vec.count + 4 >  ret.value.vec.alloc) {
            ret.value.vec.alloc *= 1.5;
            ret.value.vec.alloc += 2;
            ret.value.vec.children = tach_realloc(ret.value.vec.children, sizeof(tach_object) * ret.value.vec.alloc);
        }
        ret.value.vec.children[ret.value.vec.count] = objs[1];
        ret.value.vec.count ++;
        return ret;
    }
    else {
        tach_vm_error(state, "append takes 2 arguments");
        return state->common.objnil;
    }
}