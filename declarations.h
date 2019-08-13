/*
===========================================================================
    Copyright (C) 2010-2013  Ninja and TheKelm

    This file is part of CoD4X Plugin Handler source code.

    CoD4X Plugin Handler source code is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    CoD4X Plugin Handler source code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
===========================================================================
 */


#ifndef PLUGIN_INCLUDES

#error Please include pinc.h instead!

#endif /*PLUGIN_INCLUDES*/

#include <string.h>
#include <stdint.h>
#include <time.h>

typedef void convariable_t;

typedef enum {
    FT_PROTO_HTTP,
    FT_PROTO_FTP
} ftprotocols_t;

typedef struct {
    qboolean lock;
    qboolean active;
    qboolean transferactive;
    int transferStartTime;
    int socket;
    int transfersocket;
    int sentBytes;
    int finallen;
    int totalreceivedbytes;
    int transfertotalreceivedbytes;
    msg_t *extrecvmsg;
    msg_t *extsendmsg;
    msg_t sendmsg;
    msg_t recvmsg;
    msg_t transfermsg;
    qboolean complete;
    int code;
    int version;
    char status[32];
    char url[MAX_STRING_CHARS];
    char address[MAX_STRING_CHARS];
    char username[256];
    char password[256];
    char contentType[64];
    char cookie[MAX_STRING_CHARS];
    int mode;
    int headerLength;
    int contentLength;
    int contentLengthArrived;
    int currentChunkLength;
    int currentChunkReadOffset;
    int chunkedEncoding;
    int startTime;
    int stage;
    ftprotocols_t protocol;
    netadr_t remote;
} ftRequest_t;

#pragma pack(push, 1)

#define MAX_POST_VALS 32

struct httpPostValsInternal_s {
    char name[MAX_STRING_CHARS];
    char value[MAX_STRING_CHARS];
};

typedef struct httpPostValsInternal_s httpPostVals_t[MAX_POST_VALS];

#ifdef _WIN32
typedef uint32_t threadid_t;
#else
#include <pthread.h>
typedef pthread_t threadid_t;
#endif
