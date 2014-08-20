/*****************************************************************************
*
*  exosite_hal.c - Exosite hardware & environmenat adapation layer.
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
#include "exosite.h"
#include "exosite_hal.h"
#include "exosite_meta.h"
#include "socket.h"
#include <string.h>
#include <stdio.h>

// local variables
#define EXOMETA_ADDR
#define EXO_META_NAME   "exo_meta"
static sockaddr exo_SocketAddr;

// local functions

// externs

// global variables

/*****************************************************************************
*
*  exoHAL_ReadUUID
*
*  \param  if_nbr - Interface Number (1 - WiFi)
*          UUID_buf - buffer to return hexadecimal MAC
*
*  \return 0 if failure; len of UUID if success;
*
*  \brief  Reads the MAC address from the hardware
*
*****************************************************************************/
int
exoHAL_ReadUUID(unsigned char if_nbr, char * UUID_buf)
{
  int valuelen = 0;
  unsigned char maclen = 6, i = 0;
  unsigned char raw_mac[6];
  char *mac = "001122334455";
  switch (if_nbr) {
    case IF_GPRS:
      break;
    case IF_ENET:
      break;
    case IF_WIFI:
      memcpy(UUID_buf, mac, 12);
	  sl_NetCfgGet(SL_MAC_ADDRESS_GET,NULL,&maclen,(unsigned char *)raw_mac);
	  Report("MAC - %02x:%02x:%02x:%02x:%02x:%02x\r\n", raw_mac[0], raw_mac[1], raw_mac[2], raw_mac[3], raw_mac[4], raw_mac[5]);
	  for (i = 0; i < maclen; i++)
	  {
	    sprintf(UUID_buf + valuelen, "%x", ((raw_mac[i] & 0xf0) >> 4)); //make a string for length
        valuelen++;
	    sprintf(UUID_buf + valuelen, "%x", (raw_mac[i] & 0xf)); //make a string for length
        valuelen++;
	  }
      valuelen = 12;
      break;
    default:
      break;
  }

  return valuelen;
}


/*****************************************************************************
*
* exoHAL_EnableNVMeta
*
*  \param  None
*
*  \return None
*
*  \brief  No Function, exoHAL_EraseNVMeta handles creating file.
*
*****************************************************************************/
void
exoHAL_EnableMeta(void)
{
  return;
}


/*****************************************************************************
*
*  exoHAL_EraseNVMeta
*
*  \param  None
*
*  \return None
*
*  \brief  Wipes out meta information - replaces with 0's, creates file if needed.
*
*****************************************************************************/
void
exoHAL_EraseMeta(void)
{
  int iRetVal;
  long lFileHandle;
  unsigned long ulToken;
  char nullbuf[META_SIZE];

  // Create Null Buffer to Copy From
  memset(nullbuf, 0, META_SIZE);

  // Create the file if needed.
  // Note: This SHOULD return an error if the file exists, but it just clobbers the
  //       existing file if there is one. If TI fixes this, return the next two statements
  //       to the `exoHAL_EnableMeta` function.
  iRetVal = sl_FsOpen((unsigned char *)EXO_META_NAME,
                      FS_MODE_OPEN_CREATE(META_SIZE,_FS_FILE_OPEN_FLAG_COMMIT|_FS_FILE_PUBLIC_WRITE|_FS_FILE_PUBLIC_READ),
                      &ulToken,
                      &lFileHandle);

  iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);

  //  open a user file for writing
  iRetVal = sl_FsOpen((unsigned char *)EXO_META_NAME,
                       FS_MODE_OPEN_WRITE,
                       &ulToken,
                       &lFileHandle);
  if(iRetVal < 0)
  {
    iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
    return;
  }

  iRetVal = sl_FsWrite(lFileHandle,
                        (unsigned int)0,
                        (unsigned char *)nullbuf, META_SIZE);
  if (iRetVal < 0)
  {
    iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
    return;
  }

  // close the user file
  iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);

  return;
}


/*****************************************************************************
*
*  sl_write
*
*  \param  char *all - the buffer for writing the all of file data
*
*  \return None
*
*  \brief  Writes the data into sFlash
*
*****************************************************************************/
void sl_write(char *all)
{
  int iRetVal;
  long lFileHandle;
  unsigned long ulToken;

  //  open a user file for writing
  iRetVal = sl_FsOpen((unsigned char *)EXO_META_NAME,
					   FS_MODE_OPEN_WRITE,
					   &ulToken,
					   &lFileHandle);
  if(iRetVal < 0)
  {
	iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
	return;
  }

  iRetVal = sl_FsWrite(lFileHandle,
						(unsigned int)0,
						(unsigned char *)all, META_SIZE);
  if (iRetVal < 0)
  {
	iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
	return;
  }

  // close the user file
  iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
  return;
}


/*****************************************************************************
*
*  sl_read
*
*  \param  char *all - the buffer for reading the all of file data
*
*  \return None
*
*  \brief  Reads the data from sFlash
*
*****************************************************************************/
void sl_read(char *all)
{
  int iRetVal;
  long lFileHandle;
  unsigned long ulToken;

  iRetVal = sl_FsOpen((unsigned char *)EXO_META_NAME,
									   FS_MODE_OPEN_READ,
									   &ulToken,
									   &lFileHandle);
  if(iRetVal < 0)
  {
	iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
	return;
  }

  iRetVal = sl_FsRead(lFileHandle,
							(unsigned int)0,
							(unsigned char *)all, META_SIZE);
  if ((iRetVal < 0) || (iRetVal != META_SIZE))
  {
	iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
	return;
  }

  // close
  iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
}


/*****************************************************************************
*
*  exoHAL_WriteMetaItem
*
*  \param  buffer - string buffer containing info to write to meta; len -
*          size of string in bytes; offset - offset from base of meta
*          location to store the item
*
*  \return None
*
*  \brief  Stores information to the NV meta structure
*
*****************************************************************************/
void
exoHAL_WriteMetaItem(unsigned char * buffer, unsigned char len, int offset)
{
  //memcpy(exo_meta + offset, buffer, len);
  char meta[META_SIZE];
  sl_read(meta);
  memcpy(meta + offset, buffer, len);
  sl_write(meta);
  return;
}


/*****************************************************************************
*
*  exoHAL_ReadMetaItem
*
*  \param  buffer - buffer we can read meta info into; len - size of the
*          buffer (max 256 bytes); offset - offset from base of meta to begin
*          reading from;
*
*  \return None
*
*  \brief  Reads information from the NV meta structure
*
*****************************************************************************/
void
exoHAL_ReadMetaItem(unsigned char * buffer, unsigned char len, int offset)
{
  //memcpy(buffer, exo_meta + offset, len);
  char meta[META_SIZE];
  sl_read(meta);
  memcpy(buffer, meta + offset, len);
  return;
}


/*****************************************************************************
*
* exoHAL_dns
*
*  \param  char *host - host name
*
*  \return None
*
*  \brief  Search the host's IP from internet
*
*****************************************************************************/
int
exoHAL_dns(char *host, char *ipbuf)
{
  int status = -1;
  unsigned long dIP = 0;
  unsigned short namelen = strlen(host);
  status = gethostbyname(host, namelen, &dIP, SL_AF_INET );
  if (status == 0)
  {
    ipbuf[0] = (dIP >> 24) & 0xff;
    ipbuf[1] = (dIP >> 16) & 0xff;
    ipbuf[2] = (dIP >> 8) & 0xff;
    ipbuf[3] = dIP & 0xff;
    ipbuf[4] = 0;
    ipbuf[5] = 80;
    status = 1;
  }
  else
    status = 0;

  return status;
}


/*****************************************************************************
*
*  exoHAL_SocketClose
*
*  \param  socket - socket handle
*
*  \return None
*
*  \brief  Closes a socket
*
*****************************************************************************/
void
exoHAL_SocketClose(long socket)
{
  close(socket);
  return;
}


/*****************************************************************************
*
*  exoHAL_SocketOpenTCP
*
*  \param  server - server's ip
*
*  \return -1: failure; Other: socket handle
*
*  \brief  Opens a TCP socket
*
*****************************************************************************/
long
exoHAL_SocketOpenTCP(unsigned char *server)
{
  long sock = -1;
  sock = (long)socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

  exo_SocketAddr.sa_family = 2;
  exo_SocketAddr.sa_data[0] = server[4];
  exo_SocketAddr.sa_data[1] = server[5];
  exo_SocketAddr.sa_data[2] = server[0];  // First octet of destination IP
  exo_SocketAddr.sa_data[3] = server[1];  // Second Octet of destination IP
  exo_SocketAddr.sa_data[4] = server[2];  // Third Octet of destination IP
  exo_SocketAddr.sa_data[5] = server[3];  // Fourth Octet of destination IP
  return (long)sock;
}


/*****************************************************************************
*
*  exoHAL_ServerConnect
*
*  \param  None
*
*  \return socket - socket handle
*
*  \brief  The function opens a TCP socket
*
*****************************************************************************/
long
exoHAL_ServerConnect(long socket)
{
  if (connect((int)socket, &exo_SocketAddr, sizeof(exo_SocketAddr)) < 0)
    socket = -1;
  //else
  //  Report("M2 Server IP : %d.%d.%d.%d Connected\r\n", exo_SocketAddr.sa_data[2], exo_SocketAddr.sa_data[3], exo_SocketAddr.sa_data[4], exo_SocketAddr.sa_data[5]);
  return (long)socket;
}

/*****************************************************************************
*
*  exoHAL_SocketSend
*
*  \param  socket - socket handle; buffer - string buffer containing info to
*          send; len - size of string in bytes;
*
*  \return Number of bytes sent
*
*  \brief  Sends data out to the internet
*
*****************************************************************************/
unsigned char
exoHAL_SocketSend(long socket, char * buffer, unsigned char len)
{
  int send_len = 0;
  //Report("send :%s\r\n", buffer);
  send_len = send((int)socket, buffer, (int)len, 0);
  return send_len;
}


/*****************************************************************************
*
*  exoHAL_SocketRecv
*
*  \param  socket - socket handle; buffer - string buffer to put info we
*          receive; len - size of buffer in bytes;
*
*  \return Number of bytes received
*
*  \brief  Receives data from the internet
*
*****************************************************************************/
unsigned char
exoHAL_SocketRecv(long socket, char * buffer, unsigned char len)
{
  int recv_len = 0;
  recv_len = (unsigned char)recv((int)socket, buffer, (int)len, 0);
  //Report("Recv: %s\r\n",buffer);
  return recv_len;
}

