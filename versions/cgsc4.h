#pragma once
#include "cgsc.h"

#if CGSC_EQ(4)

extern struct scrVarGlob_t gScrVarGlob;
extern unsigned int Scr_AllocString(const char *s);
extern void Scr_AddIString(const char *value);
extern void IncInParam();

#define IGScrVmPub gScrVmPub
#define IGScrVmGlob gScrVmGlob
#define IGScrVarGlob gScrVarGlob.variableList

#endif
