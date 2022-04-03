#include "variables.h"
#include <assert.h>

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
	id = IGScrVarGlob[parentId + VARIABLELIST_PARENT_BEGIN].nextSibling;
	if (id)
	{
		while (qtrue)
		{
			entryValue = &IGScrVarGlob[id + VARIABLELIST_CHILD_BEGIN];
			assert((entryValue->w.status & VAR_STAT_MASK) != VAR_STAT_FREE);
			assert(!IsObject(entryValue));

			array->items[index] = (VariableValue *)malloc(sizeof(VariableValue));
			array->items[index]->type = entryValue->w.type & VAR_MASK;
			array->items[index]->u = entryValue->u.u;

			nextSibling = IGScrVarGlob[id + VARIABLELIST_CHILD_BEGIN].nextSibling;
			if (!nextSibling)
				break;

			id = IGScrVarGlob[nextSibling + VARIABLELIST_CHILD_BEGIN].hash.id;
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
	returnRef->type = 0; // Clean unused returnRef
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
	if (paramnum >= IGScrVmPub.outparamcount)
	{
		Scr_Error(fmt("parameter %d does not exist\n", paramnum + 1));
		return NULL;
	}
	var = &IGScrVmPub.top[-paramnum];
	return var;
}

VariableValue *Scr_GetTop(unsigned int paramnum)
{
	VariableValue *var;
	var = &IGScrVmPub.top[-paramnum];
	return var;
}

VariableValue *Scr_SelectParamOrDefault(unsigned int paramnum)
{
	// Alloc param if undefined
	VariableValue *var;
	if (paramnum >= IGScrVmPub.outparamcount)
	{
		IGScrVmPub.top++;
		IGScrVmPub.outparamcount++;
	}
	var = &IGScrVmPub.top[-paramnum];
	return var;
}

void Scr_DebugVariable(VariableValue *var)
{
	CGSC_Printf("type: %s\nintValue: %d\nfloatValue:%f\ncodePosValue:%d\npointerValue:%d\nentityOffset:%d\n",
		var_typename[var->type], var->u.intValue, var->u.floatValue,
		(int)var->u.codePosValue, var->u.pointerValue, var->u.entityOffset);
}

void Scr_AddVariable(VariableValue *var)
{
	switch (var->type)
	{
		#if CGSC(4)
		case VAR_POINTER:
			Scr_AddObject(var->u.pointerValue);
			break;
		case VAR_FUNCTION:
			Scr_AddFunc(var->u.codePosValue);
			break;
		case VAR_ISTRING:
			Scr_AddIString(SL_ConvertToString(var->u.stringValue));
			break;
		#endif

		#if CGSC_EQ(3)
		case VAR_POINTER:
		case VAR_FUNCTION:
		case VAR_ISTRING:
			CGSC_Printf("CGSC: Unsupported type %s.\n", var_typename[var->type]);
			Scr_AddUndefined();
			break;
		#endif

		case VAR_FLOAT:
			Scr_AddFloat(var->u.floatValue);
			break;
		case VAR_INTEGER:
			Scr_AddInt(var->u.intValue);
			break;
		case VAR_STRING:
			Scr_AddString(SL_ConvertToString(var->u.stringValue));
			break;
		case VAR_VECTOR:
		#if CGSC(4)
			Scr_AddVector(var->u.vectorValue);
		#elif CGSC(3)
			Scr_AddVector((vec_t *)var->u.vectorValue);
		#endif
			break;
		case VAR_ENTITY:
			Scr_AddEntity(&g_entities[157 * var->u.entityOffset]);
			break;
		case VAR_UNDEFINED:
		default:
			Scr_AddUndefined();
			break;
	}
}

uint32_t Scr_GetFlagsFromGSCArray(VariableValueArray *array)
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
