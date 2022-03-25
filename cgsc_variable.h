#pragma once
#include "cgsc.h"

/**
 * @brief Free a VariableValueArray and its values.
 *
 * @param array - The VariableValueArray to free.
 */
Plugin(void, Scr_FreeArray(VariableValueArray *array));

/**
 * @brief Get a GSC array from specified param number.
 *
 * @param paramnum - GSC param number.
 * @return VariableValueArray* - The GSC array.
 */
Plugin(VariableValueArray*, Scr_GetArray(unsigned int paramnum));

/**
 * @brief Alloc a VariableValue* and copy the variable info from specified parameter.
 * Call free() when the value is not used anymore.
 *
 * @param varRef - The variable to copy.
 * @return VariableValue* - A copy of the variable.
 */
Plugin(VariableValue*, Scr_AllocVariable(VariableValue *varRef));

/**
 * @brief Get a GSC variable reference from the specified param number.
 * If the param number is not found it will allocate a default one.
 *
 * @param paramnum - GSC param number.
 * @return VariableValue* - GSC variable reference.
 */
Plugin(VariableValue*, Scr_SelectParamOrDefault(unsigned int paramnum));

/**
 * @brief Get a GSC variable reference from the specified param number.
 *
 * @param paramnum - GSC param number.
 * @return VariableValue* - GSC variable reference.
 */
Plugin(VariableValue*, Scr_SelectParam(unsigned int paramnum));

/**
 * @brief Get a GSC variable reference from the GSC stack with the specified index.
 *
 * @param paramnum - Index to get from the GSC stack.
 * @return VariableValue* - GSC variable reference.
 */
Plugin(VariableValue*, Scr_GetTop(unsigned int paramnum));

/**
 * @brief Alloc a copy of the last GSC return variable.
 * Call free() when the value is not used anymore.
 *
 * @return VariableValue* - The allocated variable.
 */
Plugin(VariableValue*, Scr_AllocReturnResult());

/**
 * @brief Call a GSC function and keep the GSC return value uncleared.
 *
 * @param callbackHook - GSC function ID.
 * @param numArgs - GSC function args count.
 * @return short - The GSC thread id to be freed with Scr_FreeThread(<short>).
 */
Plugin(short, Scr_ExecThreadResult(int callbackHook, unsigned int numArgs));

/**
 * @brief Return a GSC Function from specified codeposvalue.
 * This function is untested.
 *
 * @param codePosValue
 */
Plugin(void, Scr_AddFunc(const char *codePosValue));

/**
 * @brief Return a GSC variable.
 *
 * @param var - The GSC variable to return.
 */
Plugin(void, Scr_AddVariable(VariableValue *var));

/**
 * @brief Print GSC variable informations.
 *
 * @param var - The GSC variable to debug.
 */
Plugin(void, Scr_DebugVariable(VariableValue *var));
