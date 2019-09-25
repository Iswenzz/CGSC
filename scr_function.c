#include "scr_function.h"
#include <assert.h>
#include "../qcommon.h"
#include "../scr_vm.h"
#include "../cscr_variable.h"

static int __callArgNumber = 0;

struct __attribute__((aligned(64))) scrVarGlob_t
{
    VariableValueInternal *variableList;
};

extern struct scrVarGlob_t gScrVarGlob;
extern VariableValue Scr_GetArrayIndexValue(unsigned int name);

void Scr_FreeArray(VariableValue **array, int length)
{
    for (int i = 0; i < length; i++)
        free(array[i]);
    free(array);
}

VariableValue **Scr_GetArray(unsigned int paramnum)
{
    VariableValue *var;
    VariableValueInternal *entryValue;
    int parentId = Scr_GetObject(paramnum);

    assert(parentId != 0);
    assert(Scr_GetObjectType(parentId) == VAR_ARRAY);

    int length = GetArraySize(parentId);
    assert(length > 0);

    VariableValue **array = (VariableValue **)malloc(length * sizeof(VariableValue *));
    VariableValue value;

    int id;
    for (int i = 0, id = FindLastSibling(parentId); id; id = FindPrevSibling(id), i++)
    {
        entryValue = &gScrVarGlob.variableList[id + VARIABLELIST_CHILD_BEGIN];

        assert((entryValue->w.status & VAR_STAT_MASK) != VAR_STAT_FREE 
            && (entryValue->w.status & VAR_STAT_MASK) != VAR_STAT_EXTERNAL);
        assert(!IsObject(entryValue));

        value = Scr_GetArrayIndexValue(entryValue->w.name >> VAR_NAME_BITS);
        array[i] = (VariableValue *)malloc(sizeof(VariableValue));
        array[i]->type = value.type;
        array[i]->u = value.u;
    }
    return array;
}

VariableValue *Scr_SelectParam(unsigned int paramnum)
{
    VariableValue *var;

    if (paramnum >= gScrVmPub.outparamcount)
    {
        Scr_Error(va("parameter %d does not exist", paramnum + 1));
        return NULL;
    }

    var = &gScrVmPub.top[-paramnum];
    return var;
}

qboolean Scr_SetParamFloat(unsigned int paramnum, float value)
{
    VariableValue *funcParam = Scr_SelectParam(paramnum);
    if (funcParam == NULL)
        return qfalse;
    else
    {
        funcParam->type = VAR_FLOAT;
        funcParam->u.floatValue = value;
        __callArgNumber++;
        return qtrue;
    }
}

qboolean Scr_SetParamInt(unsigned int paramnum, int value)
{
    VariableValue *funcParam = Scr_SelectParam(paramnum);
    if (funcParam == NULL)
        return qfalse;
    else
    {
        funcParam->type = VAR_INTEGER;
        funcParam->u.intValue = value;
        __callArgNumber++;
        return qtrue;
    }
}

qboolean Scr_SetParamObject(unsigned int paramnum, int structPointer)
{
    VariableValue *funcParam = Scr_SelectParam(paramnum);
    if (funcParam == NULL)
        return qfalse;
    else
    {
        funcParam->type = VAR_OBJECT;
        funcParam->u.pointerValue = structPointer;
        __callArgNumber++;
        return qtrue;
    }
}

qboolean Scr_SetParamEntity(unsigned int paramnum, int entID)
{
    VariableValue *funcParam = Scr_SelectParam(paramnum);
    if (funcParam == NULL)
        return qfalse;
    else
    {
        funcParam->type = VAR_ENTITY;
        funcParam->u.entityOffset = entID;
        __callArgNumber++;
        return qtrue;
    }
}

qboolean Scr_SetParamString(unsigned int paramnum, const char *string)
{
    VariableValue *funcParam = Scr_SelectParam(paramnum);
    if (funcParam == NULL)
        return qfalse;
    else
    {
        funcParam->type = VAR_STRING;
        funcParam->u.stringValue = Scr_AllocString(string);
        __callArgNumber++;
        return qtrue;
    }
}

qboolean Scr_SetParamFunc(unsigned int paramnum, const char *codePos)
{
    VariableValue *funcParam = Scr_SelectParam(paramnum);
    if (funcParam == NULL)
        return qfalse;
    else
    {
        funcParam->type = VAR_FUNCTION;
        funcParam->u.codePosValue = codePos;
        __callArgNumber++;
        return qtrue;
    }
}

qboolean Scr_SetParamStack(unsigned int paramnum, struct VariableStackBuffer *stack)
{
    VariableValue *funcParam = Scr_SelectParam(paramnum);
    if (funcParam == NULL)
        return qfalse;
    else
    {
        funcParam->type = VAR_STACK;
        funcParam->u.stackValue = stack;
        __callArgNumber++;
        return qtrue;
    }
}

qboolean Scr_SetParamVector(unsigned int paramnum, const float *value)
{
    VariableValue *funcParam = Scr_SelectParam(paramnum);
    if (funcParam == NULL)
        return qfalse;
    else
    {
        funcParam->type = VAR_VECTOR;
        funcParam->u.vectorValue = value;
        __callArgNumber++;
        return qtrue;
    }
}

void Scr_AddVariable(VariableValue *var)
{
    // Com_Printf(0, "entity type: %s\n", var_typename[var.type]);
    Com_Printf(0, "entity value: %d\n", var->u.intValue);

    switch (var->type)
    {
        case VAR_FLOAT:
            Scr_AddFloat(var->u.floatValue);
            break;
        case VAR_INTEGER:
            Scr_AddInt(var->u.intValue);
            break;
        case VAR_STRING:
            Scr_AddString("mdr");
            break;
    }
}

void Scr_CallFunction(void (*function)(void), ...)
{
    function();
    __callArgNumber = 0;
}
