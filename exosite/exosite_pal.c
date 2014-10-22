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

#include "exosite_pal.h"

#include "simplelink.h"
#include <stdio.h>
#include <stdlib.h>

#define CIK_LENGTH 40
#define CIK_FILENAME "exosite_cik.txt"

#define EXOSITE_URL "m2.exosite.com"
#define MAC_LENGTH 6
// holds ID of current socket.  val is negative if no socket is open
static int curSocketID = -1;

char exoPal_rxBuffer[RX_BUFFER_SIZE];

//SlSockAddrIn_t Addr = {0};

//Addr.sin_family = SL_AF_INET;
//Addr.sin_port = sl_Htons((UINT16)80);

//#define ADDR_SIZE sizeof(SlSockAddrIn_t)

static unsigned long ip = 0xADFFD11c; //1p
//static uint32_t ip = 0xC0A80339; // netbook
//static uint32_t ip = 0xADE692d2; // http 1.0 proxy


//*****************************************************************************
//
//! GetHostIP
//!
//! @brief  This function obtains the server IP address
//!
//! @param  void
//!
//! @return Success or Failure.
//!
//
//*****************************************************************************
unsigned long exoPal_GetHostIP()
{
	int iStatus = 0;

    /* Check for Internet connection */
    /* Querying for ti.com IP address */
    iStatus = sl_NetAppDnsGetHostByName((signed char *)EXOSITE_URL,
                                           strlen(EXOSITE_URL), &ip, SL_AF_INET);
    if (iStatus < 0)
    {
        // LAN connection is successful
        // Problem with Internet connection
        return 0;
    }
    else
    {
    	return 1;
    }
}

/*!
 * \brief Closes a tcp socket
 *
 * Closes the currently open socket.  This can be done either through
 * an OS, or direct calls to the modem.
 *
 * \return 0 if successful, else error code
 * \sa exoPal_tcpSocketOpen
 */
uint8_t exoPal_tcpSocketClose()
{
    close(curSocketID);
    return 0;
}

/*!
 * \brief
 *
 * Any HW or SW initialization should be performed in here
 *
 * This function is meant to perform any one time initialization and/or setup.
 * This will be called everytime Exosite_init is called.  An example use for
 * this function would be to perform a DNS lookup of m2.exosite.com
 *
 */
void exoPal_init()
{
    return;

}

/*!
 * \brief Opens a tcp socket
 *
 * Opens the currently open socket.  This can be done either through
 * an OS, or direct calls to the modem.
 *
 * \return 0 if successful, else error code
 *
 * \sa exoPal_tcpSocketClose
 */
uint8_t exoPal_tcpSocketOpen()
{
    int SockIDorError = 0;
    int LenorError = 0;

    SlSockAddrIn_t Addr;
    int AddrSize;

    Addr.sin_family = SL_AF_INET;
    Addr.sin_port = sl_Htons(80);

    //Change the DestinationIP endianity , to big endian
    Addr.sin_addr.s_addr = sl_Htonl(ip);

    AddrSize = sizeof(SlSockAddrIn_t);

    SockIDorError = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);
    if( SockIDorError < 0 )
    {
        //CLI_Write((unsigned char *)"Error creating socket\n\r\n\r");
        return 1;
    }


    LenorError = sl_Connect(SockIDorError, ( SlSockAddr_t *)&Addr, AddrSize);
    if( LenorError < 0 )
    {
        // error
        //CLI_Write((unsigned char *)"Error connecting to socket\n\r\n\r");
        return 2;
    }
    curSocketID = SockIDorError;

    //
    // Set Timeout on Socket
    //
    struct SlTimeval_t timeVal;
    timeVal.tv_sec =  2;             // Seconds
    timeVal.tv_usec = 0;             // Microseconds. 10000 microseconds resolution
    sl_SetSockOpt(SockIDorError,     // Enable receive timeout
    		      SL_SOL_SOCKET,
    		      SL_SO_RCVTIMEO,
    		      (_u8 *)&timeVal,
    		      sizeof(timeVal));

    return 0; //success, connection created


}




/*!
 * \brief Sends data to the open tcp socket
 *
 * Write data out to the currently open socket
 *
 * \param[in] buffer Data to write to socket
 * \param[in] len Length of data to write to socket
 *
 * \sa exoPal_socketRead
 *
 * \return 0 if successful, else error code
 */
uint8_t exoPal_socketWrite( const char * buffer, uint16_t len)
{
    int32_t writeStatus;
    // check if socket is open
    if (curSocketID < 0)
    {
        return 1;
    }
    writeStatus = sl_Send(curSocketID,buffer,len,0);
    if( writeStatus <= 0 )
    {
        // error
        return 1;
    }

    return 0;
}


/*!
 * \brief
 *
 * Reads data from a socket, into \a buffer
 *
 * \param[in] bufferSize Size of buffer
 * \param[out] buffer Buffer received data will be written to
 * \param[out] responseLength amount of data received from modem
 *
 *
 * \sa exoPal_socketWrite
 *
 * \note len must be greater than sizeof(buffer)
 *
 * \return 0 if successful, else error code
 */
uint8_t exoPal_socketRead( char * buffer, uint16_t bufferSize, uint16_t * responseLength)
{
    if (curSocketID < 0)
    {
        return 1;
    }

    *responseLength = recv(curSocketID, buffer, bufferSize,0);
    // read from socket
    return 0;

}


/*!
* @brief Used to do any operations before
*
* @note unused on this platform
*
*
* @return 0 if successful
*/
int32_t exoPal_sendingComplete()
{
	return 0;
}


/*!
 * \brief Sets the cik
 *
 * Writes the 40 chars starting at cik* to nvm.
 *
 * \param[in] cik cik to write to nvm
 *
 * \return 0 if successful, else error code
 */
uint8_t exoPal_setCik(const char * cik)
{
	int iRetVal;
	long lFileHandle;
	unsigned long ulToken;

	//
	// open the cik file for writing
	//
	iRetVal = sl_FsOpen((unsigned char *) CIK_FILENAME,
			            FS_MODE_OPEN_CREATE(CIK_LENGTH, _FS_FILE_OPEN_FLAG_COMMIT|_FS_FILE_PUBLIC_WRITE|_FS_FILE_PUBLIC_READ),
	                    &ulToken,
	                    &lFileHandle);
	if(iRetVal < 0)
	{
		iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
		return 1;
	}

	//
	// write the cik to file
	//
	iRetVal = sl_FsWrite(lFileHandle,
	                     (unsigned int)0,
	                     (unsigned char *)cik,
	                     CIK_LENGTH);
	if (iRetVal < 0)
	{
		iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
		return 2;
	}

	//
	// close the cik file
	//
	iRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
    return 0;
}


/*!
 * \brief Retrieves the cik from nvm
 *
 * The CIK must persist through power cycles and therefore must be in some
 * sort of NVM
 *
 * \param[out] read_buffer Buffer to write data to, must be at least 40 chars wide
 *
 * \return 0 if successful, else error code
 */
uint8_t exoPal_getCik(char * read_buffer)
{
    unsigned long ulToken;
    long lFileHandle;
    long lRetVal = -1;

    //
    // open a the cik file for reading
    //
    lRetVal = sl_FsOpen((unsigned char *) CIK_FILENAME,
                        FS_MODE_OPEN_READ,
                        &ulToken,
                        &lFileHandle);
    if(lRetVal < 0)
    {
        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
        return 1;
    }

    //
    // read the cik from file
    //
	lRetVal = sl_FsRead(lFileHandle,
				(unsigned int)0,
				(unsigned char *) read_buffer,
				 CIK_LENGTH);
	if ((lRetVal < 0) || (lRetVal != CIK_LENGTH))
	{
		lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
		return 2;
	}

    //
    // close the cik file
    //
    lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
    if (SL_RET_CODE_OK != lRetVal)
    {
        return 3;
    }

    return 0;
}


/*!
 * \brief Retrieves the device model
 *
 * This can either be stored in nvm, or hardcoded into this function.  In most
 * cases, you will want to hardcode this value
 *
 *
 * \param[out] read_buffer Buffer to place contents of model string in.
 *
 * \warning It is up to the user to make sure that \a read_buffer is large
 *          enough to fit the model in.  The size of this buffer can be
 *          adjusted inside exosite.h (MAX_MODEL_LENGTH)
 * \return 0 if successful, else error code
 */
uint8_t exoPal_getModel(char * read_buffer)
{
    exoPal_memcpy(read_buffer,"test",sizeof("test"));
    return 0;
}


/*!
 * \brief Retrieves the device vendor
 *
 * This can either be stored in nvm, or hardcoded into this function.  In most
 * cases, you will want to hardcode this value
 *
 *
 * \param[out] read_buffer Buffer to place contents of vendor string in.
 *
 * \warning It is up to the user to make sure that \a read_buffer is large
 *          enough to fit the model in.  The size of this buffer can be
 *          adjusted inside exosite.h (MAX_VENDOR_LENGTH)
 * \return 0 if successful, else error code
 */
uint8_t exoPal_getVendor(char * read_buffer)
{
    exoPal_memcpy(read_buffer,"chiefmarley",sizeof("chiefmarley"));
    return 0;
}



/*!
 * \brief Retrieves UUID from device
 *
 *	This function retrieves a unique ID from your device.  This is typically
 *	the MEID of a cell modem, MAC address of a network card, or serial number
 *	of the device.
 *
 * \param[in] read_buffer Buffer to put the devices UUID into.
 *
 * \return 0 if successful, else error code
 */
uint8_t exoPal_getUuid(char * read_buffer)
{
	unsigned char maclen = 6;
	unsigned char raw_mac[6];
	sl_NetCfgGet(SL_MAC_ADDRESS_GET,NULL,&maclen,(unsigned char *)raw_mac);
	//Report("MAC - %02x:%02x:%02x:%02x:%02x:%02x\r\n",
    //       raw_mac[0], raw_mac[1], raw_mac[2], raw_mac[3], raw_mac[4], raw_mac[5]);
	snprintf(read_buffer, 13, "%02x%02x%02x%02x%02x%02x", raw_mac[0], raw_mac[1], raw_mac[2], raw_mac[3], raw_mac[4], raw_mac[5]);

    return 0;
}

/*!
 * \brief memcpy implementation
 */
uint8_t exoPal_memcpy(char * dst, const char * src, uint16_t length)
{
    memcpy(dst,src,length);
    return 0;
}

/*!
 * \brief returns the length of the null terminated string
 *
 * This can link to a strlen elsewhere in your code, or you can write
 * your own.
 *
 * \param[in] s String whose length you want to check
 *
 * \return Length of the string \a s, not including the null terminator.
 *
 * \warning It is up to the user to make sure that strlen doesn't overrun.
 *          (e.g. pass a non null terminated string)
 */
uint16_t exoPal_strlen(const char *s)
{

    return strlen(s);
}

/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/*!
 * \brief Gets the decimal ascii representation of an integer
 *
 *
 *
 * \param[in] value Number that you want a string representation of
 * \param[in] buf Buffer to place results
 * \param[in] bufSize Length of buffer

 * \return Length of string written to buf
 */
uint8_t exoPal_itoa(int n, char* s, uint8_t bufSize)
{
    int i, sign;

    if ((sign = n) < 0)   /* record sign */
        n = -n;           /* make n positive */
    i = 0;
    do          /* generate digits in reverse order */
    {
        s[i++] = n % 10 + '0';    /* get next digit */
    }
    while ((n /= 10) > 0);        /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
    return i;
}


int32_t exoPal_atoi(char* val)
{
    return atoi(val);
}


char* exoPal_strstr(const char *str1, const char *str2)
{

	return strstr(str1, str2);
}
