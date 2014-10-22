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
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
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
#include <stdint.h>


/*!< This defines the size of the rx buffer in the PAL.  This buffer is used
   to place incoming data from the modem in.*/
#define RX_BUFFER_SIZE                         512

extern char exoPal_rxBuffer[RX_BUFFER_SIZE];

// defines


// functions for export
void exoPal_init();
uint8_t exoPal_setCik(const char * read_buffer);
uint8_t exoPal_getCik(char * read_buffer);
uint8_t exoPal_getModel(char * read_buffer);
uint8_t exoPal_getVendor(char * read_buffer);
uint8_t exoPal_getUuid(char * read_buffer);

uint8_t exoPal_tcpSocketClose();
uint8_t exoPal_tcpSocketOpen();
uint8_t exoPal_socketRead( char * buffer, uint16_t bufSize, uint16_t * responseLength);
uint8_t exoPal_socketWrite( const char * buffer, uint16_t len);
int32_t exoPal_sendingComplete( );

uint8_t exoPal_itoa(int value, char* str, uint8_t radix);
int32_t exoPal_atoi(char* val);
uint16_t exoPal_strlen(const char *s);
uint8_t exoPal_memcpy(char * dst, const char * src, uint16_t length);
char* exoPal_strstr(const char *in, const char *str);


#endif


