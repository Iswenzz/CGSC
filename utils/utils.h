#pragma once
#include "CGSC/cgsc.h"

/// @brief Print a formated string to the console.
#if CGSC(4)
	#ifdef PLUGIN
		#define CGSC_Printf(fmt, ...) Com_Printf(fmt, __VA_ARGS__);
	#else
		#define CGSC_Printf(fmt, ...) Com_Printf(0, fmt, __VA_ARGS__);
	#endif
#elif CGSC(3)
	#define CGSC_Printf(fmt, ...) Com_Printf(fmt, __VA_ARGS__);
#endif

/// @brief Get a register value.
#define GetRegisterValue(var, reg)   \
	register int _register asm(reg); \
	*var = _register;

/// @brief Format a string.
/// @param format - The string to format.
/// @param ... - The format arguments.
/// @return The formatted string.
char *fmt(char *format, ...);

/// @brief Check if a flag is set in the GSC array flag variable.
/// @param var - GSC array flag variable.
/// @param flag - The flag to test.
/// @return The result of the test.
qboolean HasFlag(int var, int flag);

/// @brief Check if the specified flag is the only flag set in the GSC array flag variable.
/// @param var - GSC array flag variable.
/// @param flag - The flag to test.
/// @return The result of the test.
qboolean IsFlag(int var, int flag);
