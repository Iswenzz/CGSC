#pragma once
#include "../scr_vm_functions.h"

#define VAR_STAT_MASK 0x60
#define VAR_MASK 0x1F
#define VAR_STAT_FREE 0
#define VAR_STAT_MOVABLE 32
#define VAR_STAT_HEAD 64
#define IsObject(var) ((var->w.type & VAR_MASK) >= VAR_THREAD)
#define IsObjectVal(var) ((var->type & VAR_MASK) >= VAR_THREAD)
#define VAR_TYPE(var) (var->w.type & VAR_MASK)
#define VARIABLELIST_CHILD_BEGIN 0x8002
#define VARIABLELIST_PARENT_BEGIN 0x1
#define VAR_STAT_EXTERNAL 96
#define VAR_NAME_HIGH_MASK 0xFFFFFF00
#define VARIABLELIST_CHILD_SIZE 0xFFFE
#define VARIABLELIST_PARENT_SIZE 0x8000
#define CLASS_NUM_COUNT sizeof(gScrClassMap) / sizeof(gScrClassMap[0])
#define UNK_ENTNUM_MASK 0x3FFF
#define SL_MAX_STRING_INDEX 0x10000
#define MAX_ARRAYINDEX 0x800000
#define VAR_ARRAYINDEXSTART 0x800000
#define VAR_NAME_BITS 8
#define SCR_GET_ENTITY_FROM_ENTCLIENT(entcl) (entcl & UNK_ENTNUM_MASK)
#define MAX_LOCAL_CENTITIES 1536
#define OBJECT_STACK 0x18001u

__attribute__((unused)) static int __callArgNumber = 0;

#define FLOAT(val) Scr_SetParamFloat(__callArgNumber, val)
#define INT(val) Scr_SetParamInt(__callArgNumber, val)
#define VECTOR(val) Scr_SetParamVector(__callArgNumber, val)
#define OBJECT(val) Scr_SetParamObject(__callArgNumber, val)
#define ENTITY(val) Scr_SetParamEntity(__callArgNumber, val)
#define STRING(val) Scr_SetParamString(__callArgNumber, val)
#define ISTRING(val) Scr_SetParamIString(__callArgNumber, val)
#define FUNC(val) Scr_SetParamFunc(__callArgNumber, val)
#define UNDEFINED() Scr_SetParamUndefined(__callArgNumber)

struct __attribute__((aligned(64))) scrVarGlob_t
{
    VariableValueInternal *variableList;
};

#define DEBUG_REFCOUNT_SIZE 65536
struct scrStringDebugGlob_t
{
    volatile int refCount[DEBUG_REFCOUNT_SIZE];
    volatile int totalRefCount;
    int ignoreLeaks;
};

void Scr_FreeArray(VariableValue **array, int length);
VariableValue **Scr_GetArray(unsigned int paramnum);
VariableValue *Scr_SelectParamOrDefault(unsigned int paramnum);
VariableValue *Scr_SelectParam(unsigned int paramnum);
qboolean Scr_SetParamFloat(unsigned int paramnum, float value);
qboolean Scr_SetParamInt(unsigned int paramnum, int value);
qboolean Scr_SetParamObject(unsigned int paramnum, int structPointer);
qboolean Scr_SetParamEntity(unsigned int paramnum, int entID);
qboolean Scr_SetParamIString(unsigned int paramnum, const char *string);
qboolean Scr_SetParamString(unsigned int paramnum, const char *string);
qboolean Scr_SetParamFunc(unsigned int paramnum, const char *codePos);
qboolean Scr_SetParamStack(unsigned int paramnum, struct VariableStackBuffer *stack);
qboolean Scr_SetParamVector(unsigned int paramnum, const float *value);
qboolean Scr_SetParamUndefined(unsigned int paramnum);
void Scr_AddFunc(const char *codePosValue);
void Scr_AddVariable(VariableValue *var);
void Scr_CallFunction(void (*function)(void), ...);
void Scr_CallMethod(void (*function)(scr_entref_t), scr_entref_t ent, ...);