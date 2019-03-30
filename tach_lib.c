#include "tach_lib.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"


object *tach_lib_print(vm *state, uint32_t argc, object **objs) {
    for (int i = 0; i < argc; i++) {
        if (i != 0) {
            fprintf(stdout, " ");
        }
        tach_clib_print(stdout, objs[i]);
    }
    fprintf(stdout, "\n");
    return tach_create_object_nil();
}

object *tach_lib_set(vm *state, uint32_t argc, object **objs) {
    if (argc != 2) {
        printf("set takes 2 arguments\n");
        exit(1);
    }
    tach_set_table(state->world[state->callc], objs[0], objs[1]);
    return tach_create_object_nil();
}

object *tach_lib_add(vm *state, uint32_t argc, object **objs) {
    if (argc != 2) {
        printf("set takes 2 arguments\n");
        exit(1);
    }
    return tach_create_object_number(objs[0]->value.num + objs[1]->value.num);
}

object *tach_lib_sub(vm *state, uint32_t argc, object **objs) {
    if (argc != 2) {
        printf("set takes 2 arguments\n");
        exit(1);
    }
    return tach_create_object_number(objs[0]->value.num - objs[1]->value.num);
}

object *tach_lib_lt(vm *state, uint32_t argc, object **objs) {
    if (argc != 2) {
        printf("set takes 2 arguments\n");
        exit(1);
    }
    object *ret = tach_malloc(sizeof(object));
    ret->type = TYPE_BOOL;
    ret->value.boolval = objs[0]->value.num < objs[1]->value.num;
    return ret;
}

object *tach_lib_eq(vm *state, uint32_t argc, object **objs) {
    if (argc != 2) {
        printf("set takes 2 arguments\n");
        exit(1);
    }
    object *ret = tach_malloc(sizeof(object));
    ret->type = TYPE_BOOL;
    ret->value.boolval = tach_clib_equal(objs[0], objs[1]);
    return ret;
}

object *tach_lib_copy(vm *state, uint32_t argc, object **objs) {
    return objs[argc-1];
}

object *tach_lib_proc(vm *state, uint32_t argc, object **objs) {
    object *obj = tach_malloc(sizeof(object));
    obj->type = TYPE_PROC;
    obj->value.proc = tach_malloc(sizeof(proc_t));
    obj->value.proc->argc = argc-2;
    obj->value.proc->argn = objs+1;
    obj->value.proc->gotoval = objs[argc-1]->value.proc->gotoval;
    tach_set_table(state->world[state->callc], objs[0], obj);
    return tach_create_object_nil();
}

object *tach_lib_if(vm *state, uint32_t argc, object **argv) {
    if (argc < 2 || argc > 3) {
        printf("if takes 2 or 3 arguments not %d\n", argc);
        exit(1);
    }
    if (tach_clib_tobool(argv[0])) {
        tach_vm_call(state, argv[1], 0, NULL);
        return NULL;
    }
    else if (argc == 3) {
        tach_vm_call(state, argv[2], 0, NULL);
        return NULL;
    }
    return tach_create_object_nil();
}
