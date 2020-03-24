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

void Scr_FreeArray(VariableValueArray *array)
{
	for (int i = 0; i < array->length; i++)
		free(array->items[i]);
	free(array->items);
	free(array);
}

VariableValueArray *Scr_GetArray(unsigned int paramnum)
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
	
	VariableValueArray *array = (VariableValueArray *)malloc(sizeof(VariableValueArray));
	array->length = length;
	array->items = (VariableValue **)malloc(length * sizeof(VariableValue *));
	id = gScrVarGlob.variableList[parentId + VARIABLELIST_PARENT_BEGIN].nextSibling;
	if (id)
	{
		while (true)
		{
			entryValue = &gScrVarGlob.variableList[id + VARIABLELIST_CHILD_BEGIN];
			assert((entryValue->w.status & VAR_STAT_MASK) != VAR_STAT_FREE);
			assert(!IsObject( entryValue ));

			array->items[index] = (VariableValue *)malloc(sizeof(VariableValue));
			array->items[index]->type = entryValue->w.type & VAR_MASK;
			array->items[index]->u = entryValue->u.u;

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

VariableValue *Scr_AllocReturnResult()
{
	VariableValue *returnRef = Scr_GetTop(-1);
	VariableValue *var = Scr_AllocVariable(returnRef);
	// clean unused returnRef
	returnRef->type = 0;
	return var;
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

VariableValue *Scr_GetTop(unsigned int paramnum)
{
	VariableValue *var;
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

qboolean HasFlag(int var, int flag)
{
	return (var & flag) == flag;
}

qboolean IsFlag(int var, int flag)
{
	return var == flag;
}

uint32_t GetFlagsFromGSCArray(VariableValueArray *array)
{
	uint32_t flags = 0;
	for (int i = 0; i < array->length; i++)
	{
		switch (array->items[i]->type)
		{
			case VAR_UNDEFINED: 		flags |= FLAG_UNDEFINED; 		 break;
			case VAR_POINTER: 			flags |= FLAG_POINTER; 			 break;
			case VAR_STRING: 			flags |= FLAG_STRING; 			 break;
			case VAR_ISTRING: 			flags |= FLAG_ISTRING; 			 break;
			case VAR_VECTOR: 			flags |= FLAG_VECTOR; 			 break;
			case VAR_FLOAT: 			flags |= FLAG_FLOAT; 			 break;
			case VAR_INTEGER: 			flags |= FLAG_INTEGER; 			 break;
			case VAR_CODEPOS: 			flags |= FLAG_CODEPOS; 			 break;
			case VAR_PRECODEPOS: 		flags |= FLAG_PRECODEPOS; 		 break;
			case VAR_FUNCTION: 			flags |= FLAG_FUNCTION; 		 break;
			case VAR_STACK: 			flags |= FLAG_STACK; 			 break;
			case VAR_ANIMATION: 		flags |= FLAG_ANIMATION; 		 break;
			case VAR_DEVELOPER_CODEPOS: flags |= FLAG_DEVELOPER_CODEPOS; break;
			case VAR_INCLUDE_CODEPOS: 	flags |= FLAG_INCLUDE_CODEPOS; 	 break;
			case VAR_THREAD: 			flags |= FLAG_THREAD; 			 break;
			case VAR_NOTIFY_THREAD: 	flags |= FLAG_NOTIFY_THREAD; 	 break;
			case VAR_TIME_THREAD: 		flags |= FLAG_TIME_THREAD; 		 break;
			case VAR_CHILD_THREAD: 		flags |= FLAG_CHILD_THREAD; 	 break;
			case VAR_OBJECT: 			flags |= FLAG_OBJECT; 			 break;
			case VAR_DEAD_ENTITY: 		flags |= FLAG_DEAD_ENTITY; 		 break;
			case VAR_ENTITY: 			flags |= FLAG_ENTITY; 			 break;
			case VAR_ARRAY: 			flags |= FLAG_ARRAY; 			 break;
			case VAR_DEAD_THREAD: 		flags |= FLAG_DEAD_THREAD; 		 break;
			case VAR_COUNT: 			flags |= FLAG_COUNT; 			 break;
			case VAR_THREAD_LIST: 		flags |= FLAG_THREAD_LIST; 		 break;
			case VAR_ENDON_LIST: 		flags |= FLAG_ENDON_LIST; 		 break;
		}
	}
	return flags;
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
