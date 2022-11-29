#pragma once
#include "cgsc.h"

/// @brief Returns server version.
EXPORT(float, Sys_GetCommonVersion());

/// @brief Check if CGSC version is unsupported, throws a GSC error if true.
/// @param versionCondition - The version condition returned by macros such as CGSC(ver) or CGSC_EQ(ver) etc...
/// @return
qboolean CGSC_Unsupported(qboolean versionCondition);

/// @brief Check if CGSC version is unsupported, throws a GSC error with a message if true.
/// @param versionCondition - The version condition returned by macros such as CGSC(ver) or CGSC_EQ(ver) etc...
/// @param format - The format string.
/// @param ... - The format arguments.
/// @return
qboolean CGSC_UnsupportedMessage(qboolean versionCondition, char *format, ...);

/// @brief Check if CGSC supports indexed string.
/// @return
qboolean CGSC_SupportIndexedString();
