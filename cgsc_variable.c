#include "cgsc_variable.h"
#include <assert.h>

void Scr_FreeArray(VariableValueArray *array)
{
	for (int i = 0; i < array->length; i++)
		free(array->items[i]);
	free(array->items);
	free(array);
}

#ifdef CGSC_4
void Scr_AddFunc(const char *codePosValue)
{
	IncInParam();
	IGScrVmPub.top->type = VAR_FUNCTION;
	IGScrVmPub.top->u.codePosValue = codePosValue;
}
#endif

#ifdef CGSC_3
unsigned int Scr_GetObjectType(unsigned int id)
{
	assert((IGScrVarGlob[VARIABLELIST_PARENT_BEGIN + id].w.status & VAR_STAT_MASK) != VAR_STAT_FREE);
	return VAR_TYPE((&IGScrVarGlob[id + VARIABLELIST_PARENT_BEGIN]));
}

int GetArraySize(int id)
{
	VariableValueInternal *entryValue;
	assert(id != 0);

	entryValue = &IGScrVarGlob[id + VARIABLELIST_PARENT_BEGIN];
	assert(VAR_TYPE(entryValue) == VAR_ARRAY);
	return entryValue->u.o.u.size;
}
#endif

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
	if (paramnum >= IGScrVmPub.outparamcount)
	{
		CGSC_va("parameter %d does not exist\n", paramnum + 1)
		Scr_Error(cgsc_va);
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
	VariableValue *var;
	// alloc param if undefined
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
		#ifdef CGSC_4
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

		#ifdef CGSC_3
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
			#ifdef CGSC_3
			Scr_AddVector((vec_t *)var->u.vectorValue);
			#endif
			#ifdef CGSC_4
			Scr_AddVector(var->u.vectorValue);
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
