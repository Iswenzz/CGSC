#include "scr_function.h"
#include "gsclinq.h"
#include "def_cgsc.h"

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

	// test ambientstop
	Scr_CallFunction(ambientstop, FLOAT(0.5f));

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
