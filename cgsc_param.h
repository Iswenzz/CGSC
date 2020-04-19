#pragma once
#include "cgsc.h"

__attribute__((unused)) static int __callArgNumber = 0;
#define FLOAT(val) Scr_SetParamFloat(__callArgNumber, val)
#define INT(val) Scr_SetParamInt(__callArgNumber, val)
#define VECTOR(val) Scr_SetParamVector(__callArgNumber, val)
#define OBJECT(val) Scr_SetParamObject(__callArgNumber, val)
#define ENTITY(val) Scr_SetParamEntity(__callArgNumber, val)
#define STRING(val) Scr_SetParamString(__callArgNumber, val)
#define ISTRING(val) Scr_SetParamIString(__callArgNumber, val)
#define FUNC(val) Scr_SetParamFunc(__callArgNumber, val)
#define UNDEFINED() Scr_SetParamUndefined(__callArgNumber)

qboolean Scr_SetParamFloat(unsigned int paramnum, float value);
qboolean Scr_SetParamInt(unsigned int paramnum, int value);
qboolean Scr_SetParamObject(unsigned int paramnum, int structPointer);
qboolean Scr_SetParamEntity(unsigned int paramnum, int entID);
qboolean Scr_SetParamIString(unsigned int paramnum, const char *string);
qboolean Scr_SetParamString(unsigned int paramnum, const char *string);
qboolean Scr_SetParamFunc(unsigned int paramnum, const char *codePos);
qboolean Scr_SetParamStack(unsigned int paramnum, struct VariableStackBuffer *stack);
qboolean Scr_SetParamVector(unsigned int paramnum, const float *value);
qboolean Scr_SetParamUndefined(unsigned int paramnum);

void Scr_CallFunction(void (*function)(void), ...);
void Scr_CallMethod(void (*function)(scr_entref_t), scr_entref_t ent, ...);

void GScr_DTest(scr_entref_t entref);