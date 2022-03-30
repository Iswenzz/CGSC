%macro pexport 1

	SECTION .text
		%ifdef Win32
			EXPORT Plugin_%1
		%endif
		global Plugin_%1
		Plugin_%1: jmp dword [p%1]

	SECTION .rodata
		extern %1
		p%1 dd %1

%endmacro

%macro ralias 2

	SECTION .text
		%ifdef Win32
			EXPORT %1
		%endif
		global %1
		%1: jmp dword [p%2]

	SECTION .rodata
		extern %2
		p%2 dd %2

%endmacro

; CGSC
pexport Sys_GetCommonVersionString
pexport Scr_GetFunc
pexport Scr_FreeArray
pexport Scr_GetArray
pexport Scr_AllocVariable
pexport Scr_SelectParamOrDefault
pexport Scr_SelectParam
pexport Scr_SetParamGeneric
pexport Scr_AddFunc
pexport Scr_AddVariable
pexport Scr_DebugVariable
pexport Scr_CallFunction
pexport Scr_CallMethod
pexport Scr_GetObjectType
pexport Scr_GetTop
pexport Scr_ExecThreadResult
pexport Scr_AllocReturnResult
pexport GetFlagsFromGSCArray