#pragma once

#include "tach.h"

tach_object tach_lib_print(vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_set(vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_proc(vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_add(vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_sub(vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_mul(vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_div(vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_lt(vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_gt(vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_eq(vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_neq(vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_if(vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_copy(vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_parent(vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_child(vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_depth(vm *state, uint32_t argc, tach_object *objs);
tach_object tach_lib_eval(vm *state, uint32_t argc, tach_object *objs);

tach_object tach_lib_get(vm *state, uint32_t argc, tach_object *objs);