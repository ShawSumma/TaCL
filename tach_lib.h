#pragma once

#include "tach.h"

tach_object tach_lib_print(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_set(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_proc(tach_vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_add(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_sub(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_mul(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_div(tach_vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_lt(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_gt(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_eq(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_neq(tach_vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_if(tach_vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_copy(tach_vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_parent(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_child(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_depth(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_eval(tach_vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_get(tach_vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_vector(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_append(tach_vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_index(tach_vm *state, uint32_t argc, tach_object *objs);