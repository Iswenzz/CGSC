#include "cgsc_param.h"
#include "cgsc_variable.h"
#include "../scr_vm_functions.h"

void GScr_DTest(scr_entref_t entref)
{
	#ifdef _CGSC_3
	void (*setplayerangles)(void) = (void (*)(void))0x80ab7b0;
	void (*iprintlnbold)(void) = (void (*)(void))0x80c2c14;
	Scr_CallFunction(iprintlnbold, STRING("TEST"));
	#endif

	#ifdef _CGSC_4
	float vec[3];
	vec[0] = 0;
	vec[1] = 180;
	vec[2] = 0;
	Scr_CallMethod(PlayerCmd_setAngles, entref, VECTOR(vec));
	// Scr_CallFunction(iprintlnbold, INT(1));
	#endif
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

qboolean Scr_SetParamVector(unsigned int paramnum, const float *value)
{
	#ifdef _CGSC_3
	return qfalse;
	#endif

	#ifdef _CGSC_4
	VariableValue *funcParam = Scr_SelectParamOrDefault(paramnum);
	if (funcParam == NULL)
		return qfalse;
	else
	{
		funcParam->type = VAR_VECTOR;
		funcParam->u.vectorValue = Scr_AllocVector(value);
		__callArgNumber++;
		return qtrue;
	}
	#endif
}
