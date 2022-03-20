#include "cgsc.h"
// This file fix undefined references for missing features.

#ifdef _CGSC_3
struct scrVmGlob_t
{
	VariableValue eval_stack[2];
	const char *dialog_error_message;
	int loading;
	int starttime;
	unsigned int localVarsStack[2048];
};

qboolean Scr_IsInOpcodeMemory(const char *pos) { return qfalse; }
unsigned int __cdecl AllocThread(unsigned int self) { return 0; }
void VM_Execute(unsigned a, char const *b, unsigned c) { }
void Scr_AddFunc(const char *codePosValue) { }
void AddRefToObject(unsigned int id) { }
void RemoveRefToObject(unsigned int id) { }

__attribute__((unused)) scrVmPub_t gScrVmPub = {0};
__attribute__((unused)) struct scrVmGlob_t gScrVmGlob = {0};
__attribute__((unused)) scrVarPub_t gScrVarPub;
#endif
