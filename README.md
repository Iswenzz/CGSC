
# CGSC
<!-- Badges -->

This Call of Duty 4X source extension adds new utilities for use in the creation of new plugins. Such things include new functions to get data types that the original source doesn't offer and adds the ability to call functions defined in GSC from the C files. To use this extension you must add the CGSC files included in the release section to the `/src` folder, and then compile the CoD4X server source. More detailed instructions can be found towards the bottom of this document.

``Note: Depending on the version of Call of Duty 4X that you're running, some features of CGSC may not be available.``

## Functions

#### ``Scr_FreeArray(VariableValueArray *array)``
Free the value obtained by Scr_GetArray.

Usage Example:
```c
Scr_FreeArray(array);
```
<hr>

#### ``Scr_GetArray(uin32_t paramnum)``
Returns a GSC array based on the passed index value, which represents the GSC parameter number in which this array will be passed.

Usage Example:
```c
VariableValueArray *array = Scr_GetArray(0);
Scr_FreeArray(array);
```
<hr>

#### ``Scr_AddVariable(VariableValue *var)``
Returns a generic variable to GSC.

Usage Example:
```c
VariableValueArray *array = Scr_GetArray(0);
Scr_AddVariable(array->items[0]);
Scr_FreeArray(array);
```
<hr>

#### ``Scr_ExecThreadResult(int callbackHook, unsigned int numArgs)``
Start a thread with the specified GSC functions and arguments count, and wait for the thread to finish.

Usage Example:
```c
const uint32_t threadId = Plugin_Scr_GetFunc(0);
const short tid = Plugin_Scr_ExecThreadResult(threadId, 0);
```
<hr>

#### ``Scr_AllocReturnResult()``
Copy the GSC thread return value, free the value when its not used anymore.

Usage Example:
```c
const uint32_t threadId = Plugin_Scr_GetFunc(0);
const short tid = Plugin_Scr_ExecThreadResult(threadId, 0);
VariableValue *var = Plugin_Scr_AllocReturnResult();
free(var);
```
<hr>

#### ``Scr_CallFunction(void (*function)(void), ...)``
Call a function with the specified function pointer. The arguments of the GSC function can be assigned with the type macros provided in the header file.

Usage Example:
```c
Scr_CallFunction(iprintlnbold, STRING("Test string"));
```
<hr>

#### ``Scr_CallMethod(void (*function)(scr_entref_t), scr_entref_t ent, ...)``
Call a method on an entity with the specified function pointer. The arguments of the GSC function can be assigned with the type macros provided in the header file.

Usage Example:
```c
float vec[3];
vec[0] = 0;
vec[1] = 180;
vec[2] = 0;
Scr_CallMethod(setPlayerAngles, VECTOR(vec));
```
<hr>

#### ``GetFlagsFromGSCArray(VariableValueArray *array)``
Get type flags from a GSC array.

Usage Example:
```c
VariableValueArray *array = Scr_GetArray(0);
uint32_t flags = GetFlagsFromGSCArray(array);
qboolean a = HasFlag(flags, FLAG_FLOAT);
qboolean b = IsFlag(flags, FLAG_STRING);
Scr_FreeArray(array);
```

## Instructions
In order to use this extension, just download the archived file down below, and extract it to the cod4x server's ``/src`` directory, then copy the makefile snippet below and paste it before the default rule.
Then simply build the cod4x source with ``make``.

### **[Download](https://github.com/Iswenzz/CGSC/releases)**

### **Snippet to add to the cod4x source's makefile:**
```makefile
##################################
# CGSC
CGSC_DIR=$(SRC_DIR)/CGSC

CGSC_C_SOURCES=$(wildcard $(CGSC_DIR)/*.c)
CGSC_C_OBJ=$(patsubst $(CGSC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CGSC_C_SOURCES))
C_OBJ:=$(C_OBJ) $(CGSC_C_OBJ)

$(OBJ_DIR)/%.o: $(CGSC_DIR)/%.c
	@echo   $(CC)  $@
	@$(CC) -c $(CFLAGS) $(DCFLAGS) $(C_DEFINES) -o $@ $<

CGSC_ASM_SOURCES=$(wildcard $(CGSC_DIR)/*.asm)
CGSC_ASM_OBJ=$(patsubst $(CGSC_DIR)/%.asm,$(OBJ_DIR)/%.o,$(CGSC_ASM_SOURCES))
ASM_OBJ:=$(ASM_OBJ) $(CGSC_ASM_OBJ)

$(OBJ_DIR)/%.o: $(CGSC_DIR)/%.asm
	@echo   $(NASM)  $@
	@$(NASM) $(NASMFLAGS) $< -o $@
```

## Building (Any Platform)
_Pre-Requisites:_
1. Windows [MinGW-W64 GCC-7.3.0](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/7.3.0/threads-win32/dwarf/i686-7.3.0-release-win32-dwarf-rt_v5-rev0.7z)
Linux [MinGW-W64 GCC-7.3.0](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/7.3.0/threads-posix/dwarf/i686-7.3.0-release-posix-dwarf-rt_v5-rev0.7z)
2. [NASM](https://www.nasm.us/)
3. The Pre-Requisites of [CoD4x](https://github.com/callofduty4x/CoD4x_Server#compiling-on-linux)

## Contributors:
***Note:*** If you would like to contribute to this repository, feel free to send a pull request, and I will review your code. Also feel free to post about any problems that may arise in the issues section of the repository.
