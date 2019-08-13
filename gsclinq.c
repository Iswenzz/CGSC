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

const char *var_typename[23] =
{
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

typedef enum
{
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

__cdecl char* va_replacement(char *dest, int size, const char *fmt, ...)
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

void Scr_SetParamFloat(unsigned int paramnum, float value)
{
	VariableValue* funcParam = Scr_SelectParam(paramnum);
	funcParam->type = GSC_FLOAT;
	funcParam->u.floatValue = value;
}

void testPtr()
{
	mvabuf;
	if (Plugin_Scr_GetNumParam() != 1)
    {
        Plugin_Scr_Error("Usage: testPtr(<::function>)");
        return;
    }
	uint32_t funcAddress = Scr_GetFunc(0);

	// call stock GSC function address, change value of GSC params before
	Scr_SetParamFloat(0, 0.05f);
	void (*ambientStop)(void) = (void(*)(void))0x80c146c;
	ambientStop();

	Plugin_Printf(va("$%x\n", (int)funcAddress));
	
	// funcAddress doesnt seem to point an address in memory 
	// but it can be started with ExecThread
	// int tid = Plugin_Scr_ExecThread(funcAddress, 0);
	// Plugin_Scr_FreeThread(tid);
}

__cdecl void* Scr_GetFunction(const char** v_functionName, qboolean* v_developer)
{
	// TODO
	/*scr_function_t *cmd;

	for(cmd = scr_functions; cmd != NULL; cmd = cmd->next)
	{
		if(!Q_stricmp(*v_functionName, cmd->name))
		{
			*v_developer = cmd->developer;
			*v_functionName = cmd->name;
			return cmd->function;
		}
	}*/
	return NULL;
}

// Q_isLower
// Q_isUpper
// Q_isInteral
// Q_isNumber
// Q_isAlpha

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
	Plugin_ScrAddFunction("testPtr", &testPtr);

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
