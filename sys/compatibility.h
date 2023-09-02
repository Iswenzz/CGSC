#pragma once
#include "../cgsc.h"

/// @brief Get server version.
EXPORT(CPP_EXTERN float, Sys_GetCommonVersion());

/// @brief Get CGSC version.
EXPORT(float, CGSC_Version());

/// @brief Check if CGSC version is unsupported, throws a GSC error if true.
/// @param versionCondition - The version condition returned by macros such as CGSC(ver) or CGSC_EQ(ver) etc...
/// @return
EXPORT(qboolean, CGSC_Unsupported(qboolean versionCondition));

/// @brief Check if CGSC version is unsupported, throws a GSC error with a message if true.
/// @param versionCondition - The version condition returned by macros such as CGSC(ver) or CGSC_EQ(ver) etc...
/// @param format - The format string.
/// @param ... - The format arguments.
/// @return
EXPORT(qboolean, CGSC_UnsupportedMessage(qboolean versionCondition, char *format, ...));

/// @brief Check if CGSC supports indexed string.
/// @return
EXPORT(qboolean, CGSC_SupportIndexedString());
