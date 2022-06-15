#pragma once
#include "cgsc.h"

/**
 * @brief Create a new VariableValueArray with a fixed length.
 * The array can be freed with Scr_FreeArray.
 *
 * @param length - The array length.
 */
Plugin(VariableValueArray, Scr_CreateArray(int length));

/**
 * @brief Free the variables of VariableValueArray.
 *
 * @param array - The VariableValueArray to free.
 */
Plugin(void, Scr_FreeArray(VariableValueArray* array));

/**
 * @brief Get a GSC array from specified param number.
 *
 * @param paramnum - GSC param number.
 * @return VariableValueArray - The GSC array.
 */
Plugin(VariableValueArray, Scr_GetArray(unsigned int paramnum));

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
 * @brief Get a copy of the last GSC return variable.
 *
 * @return VariableValue - The copied variable.
 */
Plugin(VariableValue, Scr_ReturnResult());

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
Plugin(void, Scr_AddVariable(VariableValue var));

/**
 * @brief Get the flags from GSC array object.
 *
 * @param array - The GSC array.
 * @return uint32_t - Flags value.
 */
Plugin(uint32_t, Scr_GetArrayFlags(VariableValueArray array));

/**
 * @brief Get the pointer object type.
 *
 * @param int - The type id.
 */
Plugin(unsigned int, Scr_GetObjectType(unsigned int id));
