#include "scr_function.h"
#include <assert.h>
#include "../qcommon.h"
#include "../scr_vm.h"
#include "../cscr_variable.h"
#include "../cscr_stringlist.h"
#include "../sys_thread.h"

extern struct scrVarGlob_t gScrVarGlob;
extern VariableValue Scr_GetArrayIndexValue(unsigned int name);
extern void IncInParam();
extern unsigned int Scr_AllocString(const char *s);
extern void Scr_AddIString(const char *value);
extern unsigned int GetNewArrayVariableIndex(unsigned int parentId, unsigned int unsignedValue);

void Scr_FreeArray(VariableValue **array, int length)
{
    for (int i = 0; i < length; i++)
        free(array[i]);
    free(array);
}

VariableValue **Scr_GetArray(unsigned int paramnum)
{
    int parentId = Scr_GetObject(paramnum);
    assert(parentId != 0);
    assert(Scr_GetObjectType(parentId) == VAR_ARRAY);

    int length = GetArraySize(parentId);
    assert(length > 0);

    VariableValueInternal *entryValue;
    unsigned int id;
    int index = length - 1;
    unsigned int nextSibling;
    
    VariableValue **array = (VariableValue **)malloc(length * sizeof(VariableValue *));
    id = gScrVarGlob.variableList[parentId + VARIABLELIST_PARENT_BEGIN].nextSibling;
    if (id)
    {
        while (true)
        {
            entryValue = &gScrVarGlob.variableList[id + VARIABLELIST_CHILD_BEGIN];
            assert((entryValue->w.status & VAR_STAT_MASK) != VAR_STAT_FREE);
            assert(!IsObject( entryValue ));

            array[index] = (VariableValue *)malloc(sizeof(VariableValue));
            array[index]->type = entryValue->w.type & VAR_MASK;
            array[index]->u = entryValue->u.u;

            nextSibling = gScrVarGlob.variableList[id + VARIABLELIST_CHILD_BEGIN].nextSibling;
            if (!nextSibling)
                break;

            id = gScrVarGlob.variableList[nextSibling + VARIABLELIST_CHILD_BEGIN].hash.id;
            assert(id != 0);
            index--;
        }
    }
    return array;
}

VariableValue *Scr_AllocVariable(VariableValue *varRef)
{
    VariableValue *var = (VariableValue *)malloc(sizeof(VariableValue));
    var->type = varRef->type;
    var->u = varRef->u;
    return var;
}

VariableValue *Scr_SelectParam(unsigned int paramnum)
{
    VariableValue *var;

    if (paramnum >= gScrVmPub.outparamcount)
    {
        Scr_Error(va("parameter %d does not exist\n", paramnum + 1));
        return NULL;
    }

    var = &gScrVmPub.top[-paramnum];
    return var;
}

VariableValue *Scr_SelectParamOrDefault(unsigned int paramnum)
{
    VariableValue *var;

    if (paramnum >= gScrVmPub.outparamcount) // alloc param if undefined
    {
        gScrVmPub.top++;
        gScrVmPub.outparamcount++;
    }

    var = &gScrVmPub.top[-paramnum];
    return var;
}

qboolean Scr_SetParamFloat(unsigned int paramnum, float value)
{
    VariableValue *funcParam = Scr_SelectParamOrDefault(paramnum);
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
    VariableValue *funcParam = Scr_SelectParamOrDefault(paramnum);
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
    VariableValue *funcParam = Scr_SelectParamOrDefault(paramnum);
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

qboolean Scr_SetParamEntity(unsigned int paramnum, int entPointer)
{
    VariableValue *funcParam = Scr_SelectParamOrDefault(paramnum);
    if (funcParam == NULL)
        return qfalse;
    else
    {
        funcParam->type = VAR_POINTER;
        funcParam->u.pointerValue = entPointer;
        __callArgNumber++;
        return qtrue;
    }
}

qboolean Scr_SetParamString(unsigned int paramnum, const char *string)
{
    VariableValue *funcParam = Scr_SelectParamOrDefault(paramnum);
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

qboolean Scr_SetParamIString(unsigned int paramnum, const char *string)
{
    VariableValue *funcParam = Scr_SelectParamOrDefault(paramnum);
    if (funcParam == NULL)
        return qfalse;
    else
    {
        funcParam->type = VAR_ISTRING;
        funcParam->u.stringValue = Scr_AllocString(string);
        __callArgNumber++;
        return qtrue;
    }
}

qboolean Scr_SetParamFunc(unsigned int paramnum, const char *codePos)
{
    VariableValue *funcParam = Scr_SelectParamOrDefault(paramnum);
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
    VariableValue *funcParam = Scr_SelectParamOrDefault(paramnum);
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
    VariableValue *funcParam = Scr_SelectParamOrDefault(paramnum);
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

qboolean Scr_SetParamUndefined(unsigned int paramnum)
{
    VariableValue *funcParam = Scr_SelectParamOrDefault(paramnum);
    if (funcParam == NULL)
        return qfalse;
    else
    {
        funcParam->type = VAR_UNDEFINED;
        __callArgNumber++;
        return qtrue;
    }
}

void Scr_AddFunc(const char *codePosValue)
{
    IncInParam();
    gScrVmPub.top->type = VAR_FUNCTION;
    gScrVmPub.top->u.codePosValue = codePosValue;
}

void Scr_DebugVariable(VariableValue *var)
{
    Com_Printf(0, "type: %s\nintValue: %d\nfloatValue:%f\ncodePosValue:%d\npointerValue:%d\nentityOffset:%d\n", 
        var_typename[var->type], var->u.intValue, var->u.floatValue, 
        (int)var->u.codePosValue, var->u.pointerValue, var->u.entityOffset);
}

void Scr_AddVariable(VariableValue *var)
{
    switch (var->type)
    {
        case VAR_POINTER:
            Scr_AddObject(var->u.pointerValue);
            break;
        case VAR_FLOAT:
            Scr_AddFloat(var->u.floatValue);
            break;
        case VAR_INTEGER:
            Scr_AddInt(var->u.intValue);
            break;
        case VAR_STRING:
            Scr_AddString(SL_ConvertToString(var->u.stringValue));
            break;
        case VAR_ISTRING:
            Scr_AddIString(SL_ConvertToString(var->u.stringValue));
            break;
        case VAR_VECTOR:
            Scr_AddVector(var->u.vectorValue);
            break;
        case VAR_ENTITY:
            Scr_AddEntity(&g_entities[157 * var->u.entityOffset]);
            break;
        case VAR_FUNCTION:
            Scr_AddFunc(var->u.codePosValue);
            break;
        case VAR_UNDEFINED:
        default:
            Scr_AddUndefined();
            break;
    }
}

void Scr_CallFunction(void (*function)(void), ...)
{
    function();
    __callArgNumber = 0;
}

void Scr_CallMethod(void (*function)(scr_entref_t), scr_entref_t ent, ...)
{
    function(ent);
    __callArgNumber = 0;
}
