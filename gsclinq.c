#include "../pinc.h"

#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

char *var_typename[23] =
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

typedef struct val_t
{
	int intValue;
} val;

typedef struct VariableValue_t
{
	val u;
	int type;
} VariableValue;

__cdecl int Plugin_Scr_GetFunc(unsigned int paramnum)
{
	int result; // eax
	VariableValue *var; // [esp+41Ch] [ebp-Ch]

	int *dword_8C06320 = (int*)0x8C06320;
	int *dword_8BE4E80 = (int*)0x8BE4E80;

	if (dword_8C06320[7] > paramnum)
	{
		var = (VariableValue *)(dword_8C06320[4] - 8 * paramnum);
		if (var->type == 9)
			result = var->u.intValue - dword_8BE4E80[18];
		else
		{
			dword_8BE4E80[4] = paramnum + 1;
			char str[1024];
			sprintf(str, "type %s is not a function", var_typename[var->type]);
			Plugin_Scr_Error(str);
			result = -1;
		}
	}
	else
	{
		char str[1024];
		sprintf(str, "parameter %d does not exist", paramnum + 1);
		Plugin_Scr_Error(str);
		result = -1;
	}
	return result;
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
