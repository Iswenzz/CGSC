#pragma once
#include "../pinc.h"

#define SCRSTRUCT_ADDR 0x895bf08
#define STRINGINDEX_ADDR 0x836fe20
#define stringIndex (*((stringIndex_t *)(STRINGINDEX_ADDR)))
#define scrVarGlob (((VariableValueInternal *)(0x8a64e80)))
#define scrVarGlob_high (((VariableValueInternal *)(0x8a64e80 + 16 * 32770)))
#define scrVarPub (*((scrVarPub_t *)(0x8be4e80)))
#define scrVmPub (*((scrVmPub_t *)(0x8c06320)))
#define g_script_error_level *(int *)(0x8c0631c)
#define g_threadStartingTime *(int *)(0x8c0a678)

static int __callArgNumber;

#define FLOAT(val) Scr_SetParamFloat(__callArgNumber, val)
#define INT(val) Scr_SetParamInt(__callArgNumber, val)
#define VECTOR(val) Scr_SetParamVector(__callArgNumber, val)
#define OBJECT(val) Scr_SetParamObject(__callArgNumber, val)
#define ENTITY(val) Scr_SetParamEntity(__callArgNumber, val)
#define STRING(val) Scr_SetParamString(__callArgNumber, val)
#define FUNC(val) Scr_SetParamFunc(__callArgNumber, val)
#define STACK(val) Scr_SetParamStack(__callArgNumber, val)

typedef enum {
    GSC_UNDEFINED,
    GSC_OBJECT,
    GSC_STRING,
    GSC_LOCALIZEDSTRING,
    GSC_VECTOR,
    GSC_FLOAT,
    GSC_INT,
    GSC_CODEPOS,
    GSC_PRECODEPOS,
    GSC_FUNCTION,
    GSC_STACK,
    GSC_ANIMATION,
    GSC_DEVCODEPOS,
    GSC_INCLUDECODEPOS,
    GSC_THREAD,
    GSC_THREAD2,
    GSC_THREAD3,
    GSC_THREAD4,
    GSC_STRUCT,
    GSC_REMOVEDENTITY,
    GSC_ENTITY,
    GSC_ARRAY,
    GSC_REMOVEDTHREAD
} gsctype_t;

unsigned int Scr_GetObject(unsigned int paramnum);
VariableValue **Scr_GetArray(unsigned int paramnum, unsigned int arrLength);
int Scr_GetFunc(unsigned int paramnum);
VariableValue *Scr_SelectParam(unsigned int paramnum);
qboolean Scr_SetParamFloat(unsigned int paramnum, float value);
qboolean Scr_SetParamInt(unsigned int paramnum, int value);
qboolean Scr_SetParamObject(unsigned int paramnum, int structPointer);
qboolean Scr_SetParamEntity(unsigned int paramnum, int entID);
qboolean Scr_SetParamString(unsigned int paramnum, const char *string);
qboolean Scr_SetParamFunc(unsigned int paramnum, const char *codePos);
qboolean Scr_SetParamStack(unsigned int paramnum, struct VariableStackBuffer *stack);
qboolean Scr_SetParamVector(unsigned int paramnum, const float *value);
void Scr_FreeArray(VariableValue **array, uint32_t size);
void Scr_AddVariable(VariableValue *var);
void Scr_CallFunction(void (*function)(void), ...);