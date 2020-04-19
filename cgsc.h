#pragma once
#include "../scr_vm.h"

#ifndef PLUGIN_HANDLER_VERSION_MAJOR
#include "../../plugins/plugin_declarations.h"
#endif

#ifdef PLUGIN_HANDLER_VERSION_MAJOR
    #if PLUGIN_HANDLER_VERSION_MAJOR >= 4
		// Tested with 17.7.1
		#define _CGSC_4
        #include "../cscr_variable.h"
        #include "../cscr_stringlist.h"

        extern struct scrVarGlob_t gScrVarGlob;
        extern unsigned int Scr_AllocString(const char *s);
        extern void Scr_AddIString(const char *value);
        extern void IncInParam();
		
        #define IGScrVmPub gScrVmPub
		#define IGScrVmGlob gScrVmGlob
        #define IGScrVarGlob gScrVarGlob.variableList

    #elif PLUGIN_HANDLER_VERSION_MAJOR >= 2
		// Tested with 17.6.0
		#define _CGSC_2
        #define IGScrVmPub scrVmPub
        #define IGScrVarGlob scrVarGlob

		extern char* SL_ConvertToString(unsigned int index);
        extern char* var_typename[];

		enum $0E0E04F36A22A28F2C0A7A22DC12DAE9
		{
			VAR_UNDEFINED = 0x0,
			VAR_BEGIN_REF = 0x1,
			VAR_POINTER = 0x1,
			VAR_STRING = 0x2,
			VAR_ISTRING = 0x3,
			VAR_VECTOR = 0x4,
			VAR_END_REF = 0x5,
			VAR_FLOAT = 0x5,
			VAR_INTEGER = 0x6,
			VAR_CODEPOS = 0x7,
			VAR_PRECODEPOS = 0x8,
			VAR_FUNCTION = 0x9,
			VAR_STACK = 0xA,
			VAR_ANIMATION = 0xB,
			VAR_DEVELOPER_CODEPOS = 0xC,
			VAR_INCLUDE_CODEPOS = 0xD,
			VAR_THREAD = 0xE,
			VAR_NOTIFY_THREAD = 0xF,
			VAR_TIME_THREAD = 0x10,
			VAR_CHILD_THREAD = 0x11,
			VAR_OBJECT = 0x12,
			VAR_DEAD_ENTITY = 0x13,
			VAR_ENTITY = 0x14,
			VAR_ARRAY = 0x15,
			VAR_DEAD_THREAD = 0x16,
			VAR_COUNT = 0x17,
			VAR_THREAD_LIST = 0x18,
			VAR_ENDON_LIST = 0x19
		};
    #endif
#endif

#define VAR_STAT_MASK 0x60
#define VAR_MASK 0x1F
#define VAR_STAT_FREE 0
#define VAR_STAT_MOVABLE 32
#define VAR_STAT_HEAD 64
#define IsObject(var) ((var->w.type & VAR_MASK) >= VAR_THREAD)
#define IsObjectVal(var) ((var->type & VAR_MASK) >= VAR_THREAD)
#define VAR_TYPE(var) (var->w.type & VAR_MASK)
#define VARIABLELIST_CHILD_BEGIN 0x8002
#define VARIABLELIST_PARENT_BEGIN 0x1
#define VAR_STAT_EXTERNAL 96
#define VAR_NAME_HIGH_MASK 0xFFFFFF00
#define VARIABLELIST_CHILD_SIZE 0xFFFE
#define VARIABLELIST_PARENT_SIZE 0x8000
#define CLASS_NUM_COUNT sizeof(gScrClassMap) / sizeof(gScrClassMap[0])
#define UNK_ENTNUM_MASK 0x3FFF
#define SL_MAX_STRING_INDEX 0x10000
#define MAX_ARRAYINDEX 0x800000
#define VAR_ARRAYINDEXSTART 0x800000
#define VAR_NAME_BITS 8
#define SCR_GET_ENTITY_FROM_ENTCLIENT(entcl) (entcl & UNK_ENTNUM_MASK)
#define MAX_LOCAL_CENTITIES 1536
#define OBJECT_STACK 0x18001u

enum GSCTypeFlag
{
	FLAG_UNDEFINED = 1,
	FLAG_BEGIN_REF = 2,
	FLAG_POINTER = 4,
	FLAG_STRING = 8,
	FLAG_ISTRING = 16,
	FLAG_VECTOR = 32,
	FLAG_END_REF = 64,
	FLAG_FLOAT = 128,
	FLAG_INTEGER = 256,
	FLAG_CODEPOS = 512,
	FLAG_PRECODEPOS = 1024,
	FLAG_FUNCTION = 2048,
	FLAG_STACK = 4096,
	FLAG_ANIMATION = 8192,
	FLAG_DEVELOPER_CODEPOS = 16384,
	FLAG_INCLUDE_CODEPOS = 32768,
	FLAG_THREAD = 65536,
	FLAG_NOTIFY_THREAD = 131072,
	FLAG_TIME_THREAD = 262144,
	FLAG_CHILD_THREAD = 524288,
	FLAG_OBJECT = 1048576,
	FLAG_DEAD_ENTITY = 2097152,
	FLAG_ENTITY = 4194304,
	FLAG_ARRAY = 8388608,
	FLAG_DEAD_THREAD = 16777216,
	FLAG_COUNT = 33554432,
	FLAG_THREAD_LIST = 67108864,
	FLAG_ENDON_LIST = 134217728
};

typedef struct
{
    uint32_t length;
    VariableValue **items;
} VariableValueArray;

struct __attribute__((aligned(64))) scrVarGlob_t
{
    VariableValueInternal *variableList;
};

#define DEBUG_REFCOUNT_SIZE 65536
struct scrStringDebugGlob_t
{
    volatile int refCount[DEBUG_REFCOUNT_SIZE];
    volatile int totalRefCount;
    int ignoreLeaks;
};
