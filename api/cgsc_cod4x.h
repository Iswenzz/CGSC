#pragma once
#define COD4X

#include "scr_vm.h"
#include "cscr_variable.h"
#include "cscr_stringlist.h"

#ifndef PLUGIN_HANDLER_VERSION_MAJOR
	#include "plugin_declarations.h"
#endif

struct __attribute__((aligned(64))) scrVarGlob_t
{
	VariableValueInternal *variableList;
};
