#pragma once
#include "cgsc.h"

#ifdef _CGSC_4
    #define CGSC_Print(fmt) \
    Com_Printf(0, fmt);
    #define CGSC_Printf(fmt,...) \
    Com_Printf(0, fmt, __VA_ARGS__);
#endif

#ifdef _CGSC_2
    #define CGSC_Print(fmt) \
    Com_Printf(fmt);
    #define CGSC_Printf(fmt,...) \
    Com_Printf(fmt, __VA_ARGS__);
#endif

#define CGSC_va(fmt,...) 							    \
char cgsc_va[1024];									    \
snprintf(cgsc_va, sizeof(cgsc_va), fmt, __VA_ARGS__);   \

qboolean HasFlag(int var, int flag);
qboolean IsFlag(int var, int flag);
uint32_t GetFlagsFromGSCArray(VariableValueArray *array);