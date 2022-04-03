#include "utils.h"
#include <stdarg.h>

char *fmt(char *format, ...)
{
	va_list argptr;
	char string[32000];

	va_start(argptr, format);
	vsnprintf(string, sizeof(string), format, argptr);
	va_end(argptr);

	return string;
}

qboolean HasFlag(int var, int flag)
{
	return (var & flag) == flag;
}

qboolean IsFlag(int var, int flag)
{
	return var == flag;
}
