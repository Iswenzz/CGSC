#include "cgsc.h"
#ifdef CGSC_4

void Scr_AddFunc(const char *codePosValue)
{
	IncInParam();
	IGScrVmPub.top->type = VAR_FUNCTION;
	IGScrVmPub.top->u.codePosValue = codePosValue;
}

#endif
