#pragma once
#include "cgsc.h"

/**
 * @todo
 *
 * @brief This variable is used to keep track of param count for a single call,
 * and can only be used in non-thread safe environment.
 */
__attribute__((used)) static int __callArgNumber = 0;

/**
 * @brief Alloc a new GSC float param before calling the function with Scr_CallFunction/Scr_CallMethod.
 */
#define FLOAT(val) Scr_SetParamGeneric(__callArgNumber, val, VAR_FLOAT)

/**
 * @brief Alloc a new GSC int param before calling the function with Scr_CallFunction/Scr_CallMethod.
 */
#define INT(val) Scr_SetParamGeneric(__callArgNumber, val, VAR_INTEGER)

/**
 * @brief Alloc a new GSC vector param before calling the function with Scr_CallFunction/Scr_CallMethod.
 */
#define VECTOR(val) Scr_SetParamGeneric(__callArgNumber, val, VAR_VECTOR)

/**
 * @brief Alloc a new GSC pointer param before calling the function with Scr_CallFunction/Scr_CallMethod.
 */
#define POINTER(val) Scr_SetParamGeneric(__callArgNumber, val, VAR_POINTER)

/**
 * @brief Alloc a new GSC string param before calling the function with Scr_CallFunction/Scr_CallMethod.
 */
#define STRING(val) Scr_SetParamGeneric(__callArgNumber, val, VAR_STRING)

/**
 * @brief Alloc a new GSC localized string param before calling the function with Scr_CallFunction/Scr_CallMethod.
 */
#define ISTRING(val) Scr_SetParamGeneric(__callArgNumber, val, VAR_ISTRING)

/**
 * @brief Alloc a new GSC function param before calling the function with Scr_CallFunction/Scr_CallMethod.
 */
#define FUNC(val) Scr_SetParamGeneric(__callArgNumber, val, VAR_FUNCTION)

/**
 * @brief Alloc a new GSC undefined param before calling the function with Scr_CallFunction/Scr_CallMethod.
 */
#define UNDEFINED() Scr_SetParamGeneric(__callArgNumber, NULL, VAR_UNDEFINED)

/**
 * @brief Alloc a new GSC generic param before calling the function with Scr_CallFunction/Scr_CallMethod.
 *
 * @param paramnum - The param index to alloc.
 * @param var - The value of the generic variable.
 * @param type - The type of the generic variable.
 * @return qboolean - Boolean result.
 */
Plugin(qboolean, Scr_SetParamGeneric(unsigned int paramnum, void *var, int type));

/**
 * @todo
 *
 * @brief Call the specified GSC function pointer
 * This function is experimental.
 *
 * @param function - The GSC function pointer.
 * @param ... - GSC alloc macros can be written here only for convenience.
 */
Plugin(void, Scr_CallFunction(void (*function)(void), ...));

/**
 * @todo
 *
 * @brief Call the specified GSC method pointer
 * This function is experimental.
 *
 * @param function - The GSC function pointer.
 * @param ent - The GSC entref.
 * @param ... - GSC alloc macros can be written here only for convenience.
 */
Plugin(void, Scr_CallMethod(void (*function)(scr_entref_t), scr_entref_t ent, ...));

/**
 * @brief GScr test function.
 *
 * @param entref - GSC entref.
 */
void GScr_Test(scr_entref_t entref);
