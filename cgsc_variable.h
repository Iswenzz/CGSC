#pragma once
#include "cgsc.h"

void Scr_FreeArray(VariableValueArray *array);
VariableValueArray *Scr_GetArray(unsigned int paramnum);
VariableValue *Scr_AllocVariable(VariableValue *varRef);
VariableValue *Scr_SelectParamOrDefault(unsigned int paramnum);
VariableValue *Scr_SelectParam(unsigned int paramnum);
VariableValue *Scr_GetTop(unsigned int paramnum);
VariableValue *Scr_AllocReturnResult();
short Scr_ExecThreadResult(int callbackHook, unsigned int numArgs);
void Scr_AddFunc(const char *codePosValue);
void Scr_AddVariable(VariableValue *var);
void Scr_DebugVariable(VariableValue *var);