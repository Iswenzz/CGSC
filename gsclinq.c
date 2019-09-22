#include "../../src/q_shared.h"
#include "../../src/cvar.h"
#include "../../src/entity.h"
#include "../../src/player.h"
#include "../../src/g_sv_shared.h"
#include "../../src/g_shared.h"
#include "../../src/cmd.h"
#include "../../src/qcommon_io.h"
#include "../../src/server.h"
#include "../../src/scr_vm.h"
#include "../../src/scr_vm_functions.h"
#include "pinc.h"

#include "_cgsc.h"

int __callArgNumber = 0;

const char *var_typename[23] =
{
	"undefined", "object", "string", "localized string",
	"vector", "float", "int", "codepos", "precodepos",
	"function", "stack", "animation", "developer codepos",
	"include codepos", "thread", "thread", "thread", "thread",
	"struct", "removed entity", "entity", "array", "removed thread"
};

typedef enum
{
	GSC_UNDEFINED, GSC_OBJECT, GSC_STRING, GSC_LOCALIZEDSTRING,
	GSC_VECTOR, GSC_FLOAT, GSC_INT, GSC_CODEPOS, GSC_PRECODEPOS,
	GSC_FUNCTION, GSC_STACK, GSC_ANIMATION, GSC_DEVCODEPOS, GSC_INCLUDECODEPOS,
	GSC_THREAD, GSC_THREAD2, GSC_THREAD3, GSC_THREAD4, GSC_STRUCT,
	GSC_REMOVEDENTITY, GSC_ENTITY, GSC_ARRAY, GSC_REMOVEDTHREAD	
} gsctype_t;

int Q_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
	int retval;
	retval = _vsnprintf(str, size, format, ap);

	if(retval < 0 || retval == size)
	{
		str[size - 1] = '\0';
		return size;
	}
	return retval;
}

char *va_replacement(char *dest, int size, const char *fmt, ...)
{
	int	len;
	va_list	argptr;

	va_start (argptr,fmt);
	len = Q_vsnprintf(dest, size, fmt, argptr);
	va_end (argptr);

	if(len >= size)
		Plugin_Printf("Com_sprintf: Output length %d too short, require %d bytes.\n", size, len + 1);

	return dest;
}

unsigned int Scr_GetObject(unsigned int paramnum)
{
	mvabuf;
	VariableValue *var;

	if (paramnum >= scrVmPub.outparamcount)
	{
		Plugin_Scr_Error(va("parameter %d does not exist", paramnum + 1));
		return 0;
	}

	var = &scrVmPub.top[-paramnum];
	if (var->type == GSC_OBJECT)
		return var->u.pointerValue;

	scrVarPub.error_index = paramnum + 1;
	Plugin_Scr_Error(va("type %s is not an object", var_typename[var->type]));
	return 0;
}

VariableValue** Scr_GetArray(unsigned int paramnum, unsigned int arrLength)
{
	unsigned int ptr = Scr_GetObject(paramnum);
	VariableValueInternal *var;
	VariableValue **array = malloc(arrLength * sizeof(VariableValue *));

	unsigned int hash_id = 0;
	int i = 0;

	do
	{
		array[i] = (VariableValue *)malloc(sizeof(VariableValue));

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
    mvabuf;
    VariableValue *var;

    if (paramnum >= scrVmPub.outparamcount)
    {
        Plugin_Scr_Error(va("parameter %d does not exist", paramnum + 1));
        return -1;
    }

    var = &scrVmPub.top[-paramnum];
    if (var->type == GSC_FUNCTION)
    {
        int vmRomAddress = var->u.codePosValue - scrVarPub.programBuffer;
        return vmRomAddress;
    }
    scrVarPub.error_index = paramnum + 1;
    Plugin_Scr_Error(va("type %s is not an function", var_typename[var->type]));
    return -1;
}

VariableValue* Scr_SelectParam(unsigned int paramnum)
{
    mvabuf;
    VariableValue *var;

    if (paramnum >= scrVmPub.outparamcount)
    {
        Plugin_Scr_Error(va("parameter %d does not exist", paramnum + 1));
        return NULL;
    }

    var = &scrVmPub.top[-paramnum];
	return var;
}

void Scr_CallFunction(void (*function)(void), ...)
{
	function();
	__callArgNumber = 0;
}

qboolean Scr_SetParamFloat(unsigned int paramnum, float value)
{
	VariableValue* funcParam = Scr_SelectParam(paramnum);
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

qboolean Scr_SetParamString(unsigned int paramnum, int stringPtr)
{
	VariableValue *funcParam = Scr_SelectParam(paramnum);
	if (funcParam == NULL)
		return qfalse;
	else
	{
		funcParam->type = GSC_STRING;
		funcParam->u.stringValue = stringPtr;
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
		free(array[i]);
	free(array);
}

void Scr_AddVariable(VariableValue *var)
{
	switch (var->type)
	{
		case GSC_FLOAT: Plugin_Scr_AddFloat(var->u.floatValue);			break;
		case GSC_INT: Plugin_Scr_AddInt(var->u.intValue); 				break;
		case GSC_STRING: Plugin_Scr_AddString(var->u.codePosValue);		break;
	}
}

#define FLOAT(val) Scr_SetParamFloat(__callArgNumber, val)
#define INT(val) Scr_SetParamInt(__callArgNumber, val)
#define VECTOR(val) Scr_SetParamVector(__callArgNumber, val)
#define OBJECT(val) Scr_SetParamObject(__callArgNumber, val)
#define ENTITY(val) Scr_SetParamEntity(__callArgNumber, val)
#define STRING(val) Scr_SetParamString(__callArgNumber, val)
#define FUNC(val) Scr_SetParamFunc(__callArgNumber, val)
#define STACK(val) Scr_SetParamStack(__callArgNumber, val)

void LINQ_All()
{
	if (Plugin_Scr_GetNumParam() != 3)
    {
		Plugin_Scr_Error("Usage: all(<array>, <array size>, <::function>)");
		return;
    }
	const uint32_t length = Plugin_Scr_GetInt(1);
	VariableValue **array = Scr_GetArray(0, length);
	const uint32_t threadId = Scr_GetFunc(2);

	qboolean result = qtrue;

	for (int i = 0; i < length; i++)
	{
		// Call predicate(item)
		Scr_AddVariable(array[i]);
		const short tid = Plugin_Scr_ExecThread(threadId, 1);
		const register int gscPredicate asm("edx");

		if (!gscPredicate)
		{
			result = qfalse;
			Plugin_Scr_FreeThread(tid);
			break;
		}

		Plugin_Scr_FreeThread(tid);
	}
	Scr_FreeArray(array, length);
	Plugin_Scr_AddBool(result);
}

void LINQ_Where()
{
	if (Plugin_Scr_GetNumParam() != 3)
	{
		Plugin_Scr_Error("Usage: where(<array>, <array size>, <::function>)");
		return;
	}
	const uint32_t length = Plugin_Scr_GetInt(1);
	VariableValue **array = Scr_GetArray(0, length);
	const uint32_t threadId = Scr_GetFunc(2);

	Plugin_Scr_MakeArray();
	for (int i = 0; i < length; i++)
	{
		// Call predicate(item)
		Scr_AddVariable(array[i]);
		const short tid = Plugin_Scr_ExecThread(threadId, 1);
		const register int gscPredicate asm("edx");

		if (gscPredicate)
		{
			Scr_AddVariable(array[i]);
			Plugin_Scr_AddArray();
		}

		Plugin_Scr_FreeThread(tid);
	}
	Scr_FreeArray(array, length);
}

void comPrintf()
{
	if (Plugin_Scr_GetNumParam() != 1)
    {
        Plugin_Scr_Error("Usage: comPrintf(<string>)");
        return;
    }
	Plugin_Printf(Plugin_Scr_GetString(0));
}

PCL int OnInit()
{
	Plugin_ScrAddFunction("comPrintf", &comPrintf);
	Plugin_ScrAddFunction("all", &LINQ_All);
	Plugin_ScrAddFunction("where", &LINQ_Where);

	return 0;
}

/**
 * Function used to obtain information about the plugin
 * Memory pointed by info is allocated by the server binary, just fill in the fields
 */
PCL void OnInfoRequest(pluginInfo_t *info)
{
    // ===== MANDATORY FIELDS =====
    info->handlerVersion.major = PLUGIN_HANDLER_VERSION_MAJOR;
    info->handlerVersion.minor = PLUGIN_HANDLER_VERSION_MINOR;	// Requested handler version

    // ===== OPTIONAL FIELDS =====
    info->pluginVersion.major = 1;
    info->pluginVersion.minor = 0;	// Plugin version
    strncpy(info->fullName, "GSC Linq", sizeof(info->fullName));
    strncpy(info->shortDescription, "Adds useful function for GSC Linq", sizeof(info->shortDescription));
    strncpy(info->longDescription, "Adds useful function for GSC Linq", sizeof(info->longDescription));
}
