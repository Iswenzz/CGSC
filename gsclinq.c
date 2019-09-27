#include "scr_function.h"
#include "gsclinq.h"

void LINQ_All()
{
	if (Scr_GetNumParam() != 3)
    {
		Scr_Error("Usage: all(<array>, <array size>, <::function>)");
		return;
    }
	VariableValue **array = Scr_GetArray(0);
	const uint32_t length = Scr_GetInt(1);
	const uint32_t threadId = Scr_GetFunc(2);
	qboolean result = qtrue;

	for (int i = 0; i < length; i++)
	{
		// Call predicate(item)
		Scr_AddVariable(array[i]);
		const short tid = Scr_ExecThread(threadId, 1);
		const register int *gscPredicate asm("edx");

		if (!*gscPredicate)
		{
			result = qfalse;
			Scr_FreeThread(tid);
			break;
		}
		Scr_FreeThread(tid);
	}
	Scr_FreeArray(array, length);
	Scr_AddBool(result);
}

void LINQ_Where()
{
	if (Scr_GetNumParam() != 3)
	{
		Scr_Error("Usage: where(<array>, <array_size>, <::function>)");
		return;
	}
	VariableValue **array = Scr_GetArray(0);
	const uint32_t length = Scr_GetInt(1);
	const uint32_t threadId = Scr_GetFunc(2);

	Scr_MakeArray();
	for (int i = 0; i < length; i++)
	{
		// Call predicate(item)
		Scr_AddVariable(array[i]);
		const short tid = Scr_ExecThread(threadId, 1);
		const register int *gscPredicate asm("edx");

		if (*gscPredicate)
		{
			Scr_AddVariable(array[i]);
			Scr_AddArray();
		}
		Scr_FreeThread(tid);
	}
	Scr_FreeArray(array, length);
}

void comPrintf()
{
	if (Scr_GetNumParam() != 1)
    {
        Scr_Error("Usage: comPrintf(<string>)");
        return;
    }
	Com_Printf(0, Scr_GetString(0));
}

void xtest()
{
	// if (Scr_GetNumParam() != 1)
	// {
	// 	Scr_Error("Usage: xtest(<type>)");
	// 	return;
	// }
	// VariableValue *val = Scr_SelectParam(0);
	// Scr_AddVariable(val);

	Scr_CallFunction(Scr_AmbientStop, FLOAT(5.0f));
}

void Scr_AddGscLinq()
{
	Scr_AddFunction("comPrintf", 	comPrintf, 		qfalse);
	Scr_AddFunction("all", 			LINQ_All, 		qfalse);
	Scr_AddFunction("where", 		LINQ_Where, 	qfalse);
	Scr_AddFunction("xtest", 		xtest, 			qfalse);
}
