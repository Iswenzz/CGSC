#include "scr_function.h"

static const char *var_typename[23] = {
    "undefined",
    "object",
    "string",
    "localized string",
    "vector",
    "float",
    "int",
    "codepos",
    "precodepos",
    "function",
    "stack",
    "animation",
    "developer codepos",
    "include codepos",
    "thread",
    "thread",
    "thread",
    "thread",
    "struct",
    "removed entity",
    "entity",
    "array",
    "removed thread"
};

static int __callArgNumber = 0;

unsigned int Scr_GetObject(unsigned int paramnum)
{
    VariableValue *var;

    if (paramnum >= scrVmPub.outparamcount)
    {
        Plugin_PrintError("parameter %d does not exist", paramnum + 1);
        return 0;
    }

    var = &scrVmPub.top[-paramnum];
    if (var->type == GSC_OBJECT)
        return var->u.pointerValue;

    scrVarPub.error_index = paramnum + 1;
    Plugin_PrintError("type %s is not an object", var_typename[var->type]);
    return 0;
}

VariableValue **Scr_GetArray(unsigned int paramnum, unsigned int arrLength)
{
    unsigned int ptr = Scr_GetObject(paramnum);
    VariableValueInternal *var;
    VariableValue **array = (VariableValue **)Plugin_Malloc(arrLength * sizeof(VariableValue *));

    unsigned int hash_id = 0;
    int i = 0;

    do
    {
        array[i] = (VariableValue *)Plugin_Malloc(sizeof(VariableValue));

        if (hash_id == 0)
        {
            hash_id = scrVarGlob[ptr + 1].hash.u.prevSibling;
            if (hash_id == 0)
                return NULL;
        }
        else
            hash_id = scrVarGlob_high[var->hash.u.prevSibling].hash.id;

        var = &scrVarGlob_high[hash_id];
        int type = var->w.type & 0x1f;

        array[i]->type = type;
        array[i]->u = var->u.u;

        ++i;
    } while (var->hash.u.prevSibling && scrVarGlob_high[var->hash.u.prevSibling].hash.id && i < arrLength);

    return array;
}

int Scr_GetFunc(unsigned int paramnum)
{
    VariableValue *var;

    if (paramnum >= scrVmPub.outparamcount)
    {
        Plugin_PrintError("parameter %d does not exist", paramnum + 1);
        return -1;
    }

    var = &scrVmPub.top[-paramnum];
    if (var->type == GSC_FUNCTION)
    {
        int vmRomAddress = var->u.codePosValue - scrVarPub.programBuffer;
        return vmRomAddress;
    }
    scrVarPub.error_index = paramnum + 1;
    Plugin_PrintError("type %s is not an function", var_typename[var->type]);
    return -1;
}

VariableValue *Scr_SelectParam(unsigned int paramnum)
{
    VariableValue *var;

    if (paramnum >= scrVmPub.outparamcount)
    {
        Plugin_PrintError("parameter %d does not exist", paramnum + 1);
        return NULL;
    }

    var = &scrVmPub.top[-paramnum];
    return var;
}

qboolean Scr_SetParamFloat(unsigned int paramnum, float value)
{
    VariableValue *funcParam = Scr_SelectParam(paramnum);
    if (funcParam == NULL)
        return qfalse;
    else
    {
        funcParam->type = GSC_FLOAT;
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
        funcParam->type = GSC_INT;
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
        funcParam->type = GSC_OBJECT;
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
        funcParam->type = GSC_ENTITY;
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
        funcParam->type = GSC_STRING;
        funcParam->u.stringValue = Plugin_Scr_AllocString(string);
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
        funcParam->type = GSC_FUNCTION;
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
        funcParam->type = GSC_STACK;
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
        funcParam->type = GSC_VECTOR;
        funcParam->u.vectorValue = value;
        __callArgNumber++;
        return qtrue;
    }
}

void Scr_FreeArray(VariableValue **array, uint32_t size)
{
    for (int i = 0; i < size; i++)
        Plugin_Free(array[i]);
    Plugin_Free(array);
}

void Scr_AddVariable(VariableValue *var)
{
    Plugin_Printf("entity type: %s\n", var_typename[var->type]);

    switch (var->type)
    {
        case GSC_FLOAT:
            Plugin_Scr_AddFloat(var->u.floatValue);
            break;
        case GSC_INT:
            Plugin_Scr_AddInt(var->u.intValue);
            break;
        case GSC_STRING:
            Plugin_Scr_AddString(Plugin_SL_ConvertToString(var->u.stringValue));
            break;
    }
}

void Scr_CallFunction(void (*function)(void), ...)
{
    function();
    __callArgNumber = 0;
}
