#include "compatibility.h"
#include <stdarg.h>

qboolean CGSC_Unsupported(qboolean versionCondition)
{
	if (!versionCondition) return qfalse;

	Scr_Error("CGSC: This feature is unsupported in this version.");
	return qtrue;
}

qboolean CGSC_UnsupportedMessage(qboolean versionCondition, char *format, ...)
{
	if (!versionCondition) return qfalse;
	va_list	argptr;

	va_start(argptr, format);
	Scr_Error(fmt(format, argptr));
	va_end(argptr);
	return qtrue;
}
