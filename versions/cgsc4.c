#include "cgsc4.h"
#if CGSC_EQ(4)

/// @brief Add a function pointer to the GSC stack.
/// @param codePosValue - The code pos value.
void Scr_AddFunc(const char *codePosValue)
{
	IncInParam();
	IGScrVmPub.top->type = VAR_FUNCTION;
	IGScrVmPub.top->u.codePosValue = codePosValue;
}

#endif
