#pragma once
#include "cgsc.h"

#ifdef _CGSC_4
    /**
     * @brief Print a string to the console.
     */
    #define CGSC_Print(fmt) \
    Com_Printf(0, fmt);
    /**
     * @brief Print a formated string to the console.
     */
    #define CGSC_Printf(fmt,...) \
    Com_Printf(0, fmt, __VA_ARGS__);
#endif

#ifdef _CGSC_3
    /**
     * @brief Print a string to the console.
     */
    #define CGSC_Print(fmt) \
    Com_Printf(fmt);
    /**
     * @brief Print a formated string to the console.
     */
    #define CGSC_Printf(fmt,...) \
    Com_Printf(fmt, __VA_ARGS__);
#endif

/**
 * @brief Create a formated string,
 * The buffer variable is named "cgsc_va".
 */
#define CGSC_va(fmt,...) 							    \
char cgsc_va[1024];									    \
snprintf(cgsc_va, sizeof(cgsc_va), fmt, __VA_ARGS__);   \

/**
 * @brief Check if a flag is set in the GSC array flag variable.
 * 
 * @param var - GSC array flag variable.
 * @param flag - The flag to test.
 * @return qboolean - The result of the test.
 */
qboolean HasFlag(int var, int flag);

/**
 * @brief Check if the specified flag is the only flag set in the GSC array flag variable.
 * 
 * @param var - GSC array flag variable.
 * @param flag - The flag to test.
 * @return qboolean - The result of the test.
 */
qboolean IsFlag(int var, int flag);

/**
 * @brief Get the flags from GSC array object.
 * 
 * @param array - The GSC array.
 * @return uint32_t - Flags value.
 */
uint32_t GetFlagsFromGSCArray(VariableValueArray *array);