#pragma once
#include "cgsc.h"

#if CGSC_EQ(3)

#define IGScrVmPub scrVmPub
#define IGScrVarGlob scrVarGlob

extern char* SL_ConvertToString(unsigned int index);
extern char* var_typename[];

#endif
