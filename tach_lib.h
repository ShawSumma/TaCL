#pragma once

#include "tach_object.h"
#include "tach_vm.h"

object *tach_lib_print(vm *state, uint32_t argc, object **objs);
object *tach_lib_set(vm *state, uint32_t argc, object **objs);
object *tach_lib_proc(vm *state, uint32_t argc, object **objs);
object *tach_lib_add(vm *state, uint32_t argc, object **objs);
object *tach_lib_sub(vm *state, uint32_t argc, object **objs);
object *tach_lib_lt(vm *state, uint32_t argc, object **objs);
object *tach_lib_eq(vm *state, uint32_t argc, object **objs);
object *tach_lib_if(vm *state, uint32_t argc, object **objs);
object *tach_lib_copy(vm *state, uint32_t argc, object **objs);