/*****************************************************************************
*
*  exosite_hal.h - Common header for Exosite hardware adapation layer
*  Copyright (C) 2012 Exosite LLC
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Exosite LLC nor the names of its contributors may
*    be used to endorse or promote products derived from this software
*    without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/

#ifndef EXOSITE_HAL_H
#define EXOSITE_HAL_H

#include "strlib.h"
#include "string.h"
#include "uart_if.h"

// defines
#define EXOSITE_HAL_SN_MAXLENGTH             25
#define EXOSITE_HAL_VENDOR_MAXLENGTH         25
#define EXOSITE_HAL_MODEL_MAXLENGTH          25

// functions for export
int exoHAL_ReadUUID(unsigned char if_nbr, char * UUID_buf);
void exoHAL_EnableMeta(void);
void exoHAL_EraseMeta(void);
void exoHAL_WriteMetaItem(unsigned char * buffer, unsigned char len, int offset);
void exoHAL_ReadMetaItem(unsigned char * buffer, unsigned char len, int offset);
int exoHAL_dns(char *host, char *ipbuf);
void exoHAL_SocketClose(long socket);
long exoHAL_SocketOpenTCP(unsigned char *server);
long exoHAL_ServerConnect(long socket);
unsigned char exoHAL_SocketSend(long socket, char * buffer, unsigned char len);
unsigned char exoHAL_SocketRecv(long socket, char * buffer, unsigned char len);
void exoHAL_MSDelay(unsigned short delay);

#endif

