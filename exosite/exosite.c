/*****************************************************************************
*
*  exosite.c - Exosite cloud communications.
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
#include "exosite.h"

static const char STR_TIMESTAMP_URL[] = "GET /timestamp ";
static const char STR_CIK_HEADER[] = "X-Exosite-CIK: ";
static const char STR_CONTENT_LENGTH[] = "Content-Length: ";
static const char STR_READ_URL[] = "GET /onep:v1/stack/alias?";
static const char STR_WRITE_URL[] = "POST /onep:v1/stack/alias ";
static const char STR_ACTIVATE_URL[] = "POST /provision/activate ";
static const char STR_RPC_URL[] = "POST /onep:v1/rpc/process ";
static const char STR_HTTP[] = "HTTP/1.1";
static const char STR_HOST[] = "Host: m2.exosite.com";
static const char STR_ACCEPT[] = "Accept: application/x-www-form-urlencoded; charset=utf-8";
static const char STR_ACCEPT_JSON[] = "Accept: application/json; charset=utf-8";
static const char STR_CONTENT[] = "Content-Type: application/x-www-form-urlencoded; charset=utf-8";
static const char STR_CONTENT_JSON[] = "Content-Type: application/json; charset=utf-8";
static const char STR_CRLF[] = "\r\n";

// local functions
static uint8_t exosite_connect();
static uint8_t exosite_disconnect();
static uint8_t exosite_checkResponse(char * response, const char * code);


#define STR_VENDOR  "vendor="
#define STR_MODEL   "&model="
#define STR_SN      "&sn="

static char cikBuffer[CIK_LENGTH];
static char vendorBuffer[MAX_VENDOR_LENGTH];
static char modelBuffer[MAX_MODEL_LENGTH];
static char uuidBuffer[MAX_UUID_LENGTH];


/*!
 * Used to determine if there is currently an open socket.  This value is 1 if
 * we have an open socket, else 0.
 */
static uint8_t isSocketOpen = 0;


static EXO_STATE initState = EXO_STATE_NOT_COMPLETE;

int32_t exosite_getBody(char *response, char **bodyStart, uint16_t *bodyLength);


/*!
 * \brief Reset the cik to ""
 *
 * The following code would reset the contents of the cik to be an empty string.
   \code{.c}
   exosite_resetCik();
   \endcode
 *
 * \return Returns 0 if successful, else error code
 * \sa
 * \note
 * \warning
 */
uint8_t exosite_resetCik()
{

    exoPal_setCik("");
    cikBuffer[0] = '\0';
    return 0;
}

/*!
 * \brief  Initializes the Exosite libraries and attempts to activate the
 *          with Exosite
 *
 * This **MUST** be called before any other exosite library calls are called.
 *
 * Assumes that the modem is setup and ready to make a socket connection.
 * This will fail if activation fails.  After initialization, this function
 * calls exosite_activate
 *
 * \param[in] vendor Pointer to string containing vendor name
 * \param[in] model Pointer to string containing model name
 *
 * \return Device Activation status
 */
EXO_STATE exosite_init(const char * vendor, const char *model)
{
    uint8_t retStatus = 0;
    // reset state
    initState = EXO_STATE_NOT_COMPLETE;
    

    exoPal_init();

    // get cik and uuid and any other nvm stored data, into ram.
    retStatus = exoPal_getCik(cikBuffer);
    retStatus = exoPal_getUuid(uuidBuffer);
    exoPal_memcpy(vendorBuffer, vendor, MAX_VENDOR_LENGTH);
    exoPal_memcpy(modelBuffer, model, MAX_MODEL_LENGTH);
    // create activation request
    if (retStatus)
    {
        initState = EXO_STATE_INIT_ERROR;
        return initState;
    }
    initState = exosite_activate();
    if (initState == EXO_STATE_VALID_CIK)
    {
        initState = EXO_STATE_INIT_COMPLETE;
    }
    return initState;
}




/*!
 * \brief  Makes a provisioning request to Exosite.
 *
 *
 *
 * \return The devices activation status
 */
EXO_STATE exosite_activate()
{
    // assume content length will never be over 9999 bytes
    char contentLengthStr[5];
    uint8_t len_of_contentLengthStr;
    EXO_STATE retVal;
    uint16_t responseLen;
    
    
    // Try and activate device with Exosite, four possible cases:
    // * We don't have a stored CIK and receive a 200 response with a CIK
    //    * Means device was enabled and this was our first connection
    // * We don't have a stored CIK and receive a 409 response
    //    * The device is not enabled.
    // * We have a stored CIK and receive a 409 response.
    //     *  Device has already been activated and has a valid CIK
    // * We have a stored CIK and receive a 401 response
    //    * R/W error
    
    uint8_t vendorLength = exoPal_strlen(vendorBuffer);
    uint8_t modelLength = exoPal_strlen(modelBuffer);
    uint8_t uuidLength = exoPal_strlen(uuidBuffer);

    // get body length
    uint16_t bodyLength = sizeof(STR_VENDOR) - 1 +
                          sizeof(STR_MODEL) - 1 +
                          sizeof(STR_SN) - 1;
    bodyLength += vendorLength + modelLength + uuidLength;

    
    len_of_contentLengthStr = exoPal_itoa((int)bodyLength, contentLengthStr, 5);


    exosite_connect();


    // send request
    exoPal_socketWrite(STR_ACTIVATE_URL, sizeof(STR_ACTIVATE_URL) - 1);
    exoPal_socketWrite(STR_HTTP, sizeof(STR_HTTP) - 1);
    exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF) - 1);

    // send Host header
    exoPal_socketWrite(STR_HOST, sizeof(STR_HOST) - 1);
    exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF) - 1);

    // send content type header
    exoPal_socketWrite(STR_CONTENT, sizeof(STR_CONTENT) - 1);
    exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF) - 1);

    // send content length header
    exoPal_socketWrite(STR_CONTENT_LENGTH, sizeof(STR_CONTENT_LENGTH) - 1);
    exoPal_socketWrite(contentLengthStr, len_of_contentLengthStr);
    exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF) - 1);
    exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF) - 1);

    // send body
    exoPal_socketWrite(STR_VENDOR, sizeof(STR_VENDOR) - 1);
    exoPal_socketWrite(vendorBuffer, vendorLength);
    exoPal_socketWrite(STR_MODEL, sizeof(STR_MODEL) - 1);
    exoPal_socketWrite(modelBuffer, modelLength);
    exoPal_socketWrite(STR_SN, sizeof(STR_SN) - 1);
    exoPal_socketWrite(uuidBuffer, uuidLength);
   
    exoPal_sendingComplete();

    retVal = EXO_STATE_CONNECTION_ERROR;


    

    exoPal_socketRead( exoPal_rxBuffer, RX_BUFFER_SIZE, &responseLen);

    exosite_disconnect();

    
    if (responseLen == 0)
    {
        // if we didn't receive any data from the modem
        retVal = EXO_STATE_NO_RESPONSE;
    }
    else if (exosite_checkResponse(exoPal_rxBuffer, "200"))
    {
        // we received a CIK.
        char * bodyStart;
        uint16_t bodyLength = 0;
        int32_t response;
        response = exosite_getBody(exoPal_rxBuffer, &bodyStart, &bodyLength);
        if (response < 0)
        {
            // some error getting the body
            return EXO_STATE_CONNECTION_ERROR;
        }
        if ((exosite_isCIKValid(bodyStart)) && (bodyLength == CIK_LENGTH))
        {
            // got a valid cik in the response
            exoPal_setCik(bodyStart);
            exoPal_memcpy(cikBuffer, bodyStart, CIK_LENGTH);
            retVal = EXO_STATE_VALID_CIK;
        }
    }
    else if (exosite_checkResponse(exoPal_rxBuffer, "409"))
    {
        exoPal_getCik(cikBuffer);

        if (exosite_isCIKValid(cikBuffer))
        {
            // If we receive a 409 and we do have a valid CIK, we will
            // assume we are good to go.
            retVal = EXO_STATE_VALID_CIK;

        }
        else
        {
            // if we don't have a CIK in nvm and we receive a 409
            // The device isn't enabled in the dashboard
            retVal = EXO_STATE_DEVICE_NOT_ENABLED;

        }
    }
    else if (exosite_checkResponse(exoPal_rxBuffer, "404"))
    {
		// platform doesn't know about this device
		retVal = EXO_STATE_DEVICE_NOT_ENABLED;
    }
    else if (exosite_checkResponse(exoPal_rxBuffer, "401"))
    {
        // RW error
        retVal = EXO_STATE_R_W_ERROR;
    }

    return retVal;

}


/*!
 * @brief  Sets bodyStart to point to the start of the http body
 *
 * @param response [in] Full http response with headers
 * @param bodyStart [out] Will be updated to point at the start of the http body
 * @param bodyLength [out] Length of the body
 * 
 * @return int32_t 0 if successful, else negative
 */
int32_t exosite_getBody(char *response, char **bodyStart, uint16_t *bodyLength)
{
    // find content length
    char* strStart;
    char* charAfterContentLengthValue = 0;
    char cr[] = "\r";
    char httpBodyToken[] = "\r\n\r\n";
    // find start of content length header
    strStart = exoPal_strstr(response, STR_CONTENT_LENGTH);
    if (strStart <= 0)
    {
        return -1;
    }
    strStart = strStart + sizeof(STR_CONTENT_LENGTH) - 1;
    // get \r and set to '\0' for atoi
    charAfterContentLengthValue = exoPal_strstr(strStart, cr);
    if (charAfterContentLengthValue <= 0)
    {
        return -2;
    }
    if (charAfterContentLengthValue != 0)
    {
        // temporarily null terminate the content length
        *charAfterContentLengthValue = '\0';
        *bodyLength = exoPal_atoi(strStart);
        *charAfterContentLengthValue = '\r';
        
        
        strStart = exoPal_strstr(strStart, httpBodyToken);
        if (strStart <= 0)
        {
            return -3;
        }
        *bodyStart = (char *)(strStart + sizeof(httpBodyToken)-1);
    }
    return 0;
}


/*!
 * \brief Checks if the given cik is valid
 *
 * Checks that the first 40 chars of `cik` are valid, lowercase hexadecimal bytes.
 *
 *
 *
 * \param[in] cik array of CIK_LENGTH bytes
 *
 * \return Returns 1 if successful, else 0
 * \sa
 * \note
 * \warning
 */
uint8_t exosite_isCIKValid(char cik[CIK_LENGTH])
{
    uint8_t i;

    for (i = 0; i < CIK_LENGTH; i++)
    {
        if (!(cik[i] >= 'a' && cik[i] <= 'f' || cik[i] >= '0' && cik[i] <= '9'))
        {
            return 0;
        }
    }

    return 1;
}



/*!
 *  \brief  Programs a new CIK to flash / non volatile
 *
 * \param[in] pCIK Pointer to CIK
 *
 */
void exosite_setCIK(char * pCIK)
{
    exoPal_setCik((char *)pCIK);
    exoPal_memcpy(cikBuffer, pCIK, CIK_LENGTH);
    return;
}



/*!
 *  \brief  Retrieves a the CIK from NVM and places it in to the string pointed
 *			at by \a cik
 *
 * \param[out] cik Pointer to CIK
 *
 */
void exosite_getCIK(char * cik)
{
    exoPal_getCik((char *)cik);

    return;
}


/*!
 *  \brief  Writes data to Exosite
 *
 * Writes the data in writeData to Exosite.  The data is written as the body of
 * a POST request to Exosite's `/onep:v1/stack/alias request`.
 *
 * Below is how you would write a value of `5` to the `myAlias` alias.
 * \code{.c}
 * exosite_write("myAlias=5", sizeof("myAlias=5"));
 * \endcode
 *
 * \param[in] writeData Pointer to buffer of data to write to Exosite
 * \param[in] length length of data in buffer
 *
 * \return Error code if fails, else 0
 *
 */
int32_t exosite_write(const char * writeData, uint16_t length)
{
    // assume content length won't be greater than 9999.
    char contentLengthStr[5];
    uint8_t len_of_contentLengthStr;
    uint16_t responseLength;
    uint8_t connection_status;
    int32_t results = 0;
    if(!exosite_isCIKValid(cikBuffer))
    {
        // tried to write without a valid CIK
        return -99;
    }
    
    // connect to exosite
    connection_status = exosite_connect();

    // return error message if connect failed.
    if (connection_status != 0)
    {
        return connection_status;
    }


    len_of_contentLengthStr = exoPal_itoa((int)length, contentLengthStr, 5);

    // send request
    results |= exoPal_socketWrite(STR_WRITE_URL, sizeof(STR_WRITE_URL)-1);
    results |= exoPal_socketWrite(STR_HTTP, sizeof(STR_HTTP)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send Host header
    results |= exoPal_socketWrite(STR_HOST, sizeof(STR_HOST)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send cik header
    results |= exoPal_socketWrite(STR_CIK_HEADER, sizeof(STR_CIK_HEADER)-1);
    results |= exoPal_socketWrite(cikBuffer, sizeof(cikBuffer));
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send content type header
    results |= exoPal_socketWrite(STR_CONTENT, sizeof(STR_CONTENT)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send content length header
    results |= exoPal_socketWrite(STR_CONTENT_LENGTH, sizeof(STR_CONTENT_LENGTH)-1);
    results |= exoPal_socketWrite(contentLengthStr, len_of_contentLengthStr);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send body
    results |= exoPal_socketWrite(writeData, length);

    results |= exoPal_sendingComplete();
    
    if (results != 0)
    {
        exosite_disconnect();
        return results;
    }

    responseLength = 0;
    // get response
    exoPal_socketRead(exoPal_rxBuffer, RX_BUFFER_SIZE, &responseLength);


    exosite_disconnect();

    // 204 "No content"
    if (exosite_checkResponse(exoPal_rxBuffer, "204"))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}



/*!
 *  \brief  Reads data from Exosite
 *
 * Allows the reading of for reading data from the Exosite platform.  The alias
 * variable can include one, or multiple alias names.  For example, if you
 * included they must be separated by a '&'.
 *
 * For example, if you want to read from a single alias, you would set the alias
 * parameter to `"myAliasName"`.  If you wanted to read from multiple alias', you
 * would set the alias parameter to `"myAliasName&myOtherAliasName"`.
 *
 * If the read is successful, the value returned in readResponse will be the
 * body of the HTTP response, and will be in this format
 * `"myAliasName=someValue&myOtherAliasName=23"`.
 *
   \code{.c}
   exosite_read("myAlias", readBuffer, lenOfReadBuffer, retLen);
   // After this call, the readBuffer would look something like "myAlias=3". The
   // length variable would be updated with the length of the response string, in this
   // case, 9.
   \endcode
 *
 * \param[in] alias Name/s of data source/s alias to read from
 * \param[out] readResponse buffer to place read response in
 * \param[in] buflen length of buffer
 * \param[out] length Length of data placed into readResponse
 *
 * \return Error code if fail, else 0
 *
 */
int32_t exosite_read(const char * alias, char * readResponse, uint16_t buflen, uint16_t * length)
{
    uint16_t responseLength;
    int i,j;
    uint8_t connection_status;
    int32_t results = 0;
    
    if(!exosite_isCIKValid(cikBuffer))
    {
        // tried to write without a valid CIK
        return -99;
    }
    
    // connect to exosite
    connection_status = exosite_connect();

    // return error message if connect failed.
    if (connection_status != 0)
    {
        return connection_status;
    }

    // send request
    results |= exoPal_socketWrite(STR_READ_URL, sizeof(STR_READ_URL)-1);
    results |= exoPal_socketWrite(alias, exoPal_strlen(alias));
    results |= exoPal_socketWrite(" ", exoPal_strlen(" "));
    results |= exoPal_socketWrite(STR_HTTP, sizeof(STR_HTTP)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send Host header
    results |= exoPal_socketWrite(STR_HOST, sizeof(STR_HOST)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send cik header
    results |= exoPal_socketWrite(STR_CIK_HEADER, sizeof(STR_CIK_HEADER)-1);
    results |= exoPal_socketWrite(cikBuffer, sizeof(cikBuffer));
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send content type header
    results |= exoPal_socketWrite(STR_CONTENT, sizeof(STR_CONTENT)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send accept header
    results |= exoPal_socketWrite(STR_ACCEPT, sizeof(STR_ACCEPT)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    results |= exoPal_sendingComplete();

    if (results != 0)
    {
        exosite_disconnect();
        return results;
    }

    responseLength = 0;
    // get response
    exoPal_socketRead(exoPal_rxBuffer, RX_BUFFER_SIZE, &responseLength);


    exosite_disconnect();
    
    // 204 "No content"
    if (exosite_checkResponse(exoPal_rxBuffer, "200"))
    {
        //find first '\n' char from end of response
        for (i = responseLength - 1; i > 0; i--)
        {

            // if we found the '\n' before we hit the beginning of the buffer
            if (exoPal_rxBuffer[i] == '\n')
            {
                // copy http body into readResponse buffer
                for (j = i; j < (responseLength - 1); j++)
                {
                    readResponse[j-i] = exoPal_rxBuffer[j + 1];
                }

                // add null teminator at end of string
                readResponse[j-i] = '\0';
                // exit out
                i = 0;
            }
        }
    }

    return 0;
}



/*!
 *  \brief  Reads data from Exosite
 *
 * Reads a single alias from the Exosite One Platform.  The alias
 * variable can only include one alias name.
 *
 * For example, if you want to read from the `myAlias` alias, you would set the
 * alias parameter to `"myAliasName"`.
 *
 * If the read is successful, the value returned in readResponse will be the
 * value of your alias.  These will always be strings.  Even if your data source
 * contains a numeric value, you must convert it to an integer before using it.
 *
   \code{.c}
   exosite_readSingle("myAlias", readBuffer, lenOfReadBuffer, retLen);
   // After this call, the readBuffer would look something like this: "3". The
   // length variable would be updated with the length of the response string, in this
   // case, 1.
   \endcode
 *
 * \param[in] alias Name/s of data source/s alias to read from
 * \param[out] readResponse buffer to place read response in
 * \param[in] buflen length of buffer
 * \param[out] length data placed into readResponse
 *
 * \return Error code if fail, else 0
 *
 */
int32_t exosite_readSingle(const char * alias, char * readResponse, uint16_t buflen, uint16_t * length)
{
    uint16_t responseLength;
    int16_t i;
    uint16_t j;
    uint16_t k;
    uint8_t connection_status;
    int32_t results = 0;
    if(!exosite_isCIKValid(cikBuffer))
    {
        // tried to write without a valid CIK
        return -99;
    }
    
    // connect to exosite
    connection_status = exosite_connect();

    // return error message if connect failed.
    if (connection_status != 0)
    {
        return connection_status;
    }

    // send request
    results |= exoPal_socketWrite(STR_READ_URL, sizeof(STR_READ_URL)-1);
    results |= exoPal_socketWrite(alias, exoPal_strlen(alias));
    results |= exoPal_socketWrite(" ", exoPal_strlen(" "));
    results |= exoPal_socketWrite(STR_HTTP, sizeof(STR_HTTP)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send Host header
    results |= exoPal_socketWrite(STR_HOST, sizeof(STR_HOST)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send cik header
    results |= exoPal_socketWrite(STR_CIK_HEADER, sizeof(STR_CIK_HEADER)-1);
    results |= exoPal_socketWrite(cikBuffer, sizeof(cikBuffer));
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send content type header
    results |= exoPal_socketWrite(STR_CONTENT, sizeof(STR_CONTENT)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send accept header
    results |= exoPal_socketWrite(STR_ACCEPT, sizeof(STR_ACCEPT)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    results |= exoPal_sendingComplete();
    
    if (results != 0)
    {
        exosite_disconnect();
        return results;
    }
    
    responseLength = 0;
    // get response
    exoPal_socketRead(exoPal_rxBuffer, RX_BUFFER_SIZE, &responseLength);

    exosite_disconnect();
    
    // if we received a 200
    if (exosite_checkResponse(exoPal_rxBuffer, "200"))
    {
        //find first '\n' char from end of response
        for (i = responseLength; i > 0; i--)
        {
            if (exoPal_rxBuffer[i] == '\n')
            {
                // '\n' found
                uint8_t charNotMatch = 0;
                for ( j = 1; (j <= i) && i > 0; j++)
                {
                    // If we're at the end of the inputted string?
                    if (alias[j-1] == '\0')
                    {
                        // if all chars of our requested alias match, we found the key
                        if (!charNotMatch)
                        {
                            // move j passed the '='
                            j++;

                            for (k = 0;
                                 (k <= buflen) && ((i + j + k) <= responseLength);
                                 k++)
                            {
                                // copy remaining data into buffer
                                readResponse[k] = exoPal_rxBuffer[i+j+k];
                                *length = k;
                            }
                            i = 0;
                        }
                        else
                        {
                            // match not found, exit
                            i = 0;
                            *length = 0;
                        }
                    }

                    // confirm letter by letter
                    charNotMatch |= !(exoPal_rxBuffer[i+j] == alias[j-1]);
                }
            }
        }
    }

    return 0;
}

/*!
 * @brief  Retrieves the timestamp from m2.exosite.com/timestamp
 *
 * @param timestamp Timestamp retrieved from Exosite
 * 
 * @return int8_t Returns negative error code if failed, else returns 0
 */
int8_t exosite_getTimestamp(int32_t * timestamp)
{
    char * bodyStart;
    uint16_t responseLength = 0;
    int32_t status;
    uint8_t connection_status;
    char temp;
    connection_status = exosite_connect();
    exoPal_socketWrite(STR_TIMESTAMP_URL, sizeof(STR_TIMESTAMP_URL)-1);
    exoPal_socketWrite(STR_HTTP, sizeof(STR_HTTP)-1);
    exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send Host header
    exoPal_socketWrite(STR_HOST, sizeof(STR_HOST)-1);
    exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);
    exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);
    
    exoPal_sendingComplete();
    
    exoPal_socketRead(exoPal_rxBuffer, RX_BUFFER_SIZE, &responseLength);
    exosite_disconnect();
    
    status = exosite_getBody(exoPal_rxBuffer, &bodyStart, &responseLength);
    
    if (status < 0 )
    {
        return -1;
    }
    
    temp = bodyStart[responseLength];
    bodyStart[responseLength] = '\0';
    
    *timestamp = exoPal_atoi(bodyStart);
    bodyStart[responseLength] = temp;
    
    return 0;
    
}


/*!
 * @brief  Makes a request to the Exosite RPC API
 *
 *
 * @param requestBody Full json rpc request string
 * @param requestLength Length of json request string
 * 
 * @return int32_t 
 */
int32_t exosite_rawRpcRequest(const char * requestBody, uint16_t requestLength, char * responseBuffer, int32_t responseBufferLength)
{
    char contentLengthStr[5];
    uint16_t responseLength = 0;
    uint8_t connection_status;
    uint8_t len_of_contentLengthStr;
    int32_t results = 0;
    
    if(!exosite_isCIKValid(cikBuffer))
    {
        // tried to write without a valid CIK
        return -99;
    }
    
    // connect to exosite
    connection_status = exosite_connect();
    
    // assume content length will never be over 9999 bytes
    
    len_of_contentLengthStr = exoPal_itoa((int)requestLength, contentLengthStr, 5);
    // return error message if connect failed.
    if (connection_status != 0)
    {
        return connection_status;
    }

    // send request
    results |= exoPal_socketWrite(STR_RPC_URL, sizeof(STR_RPC_URL)-1);
    results |= exoPal_socketWrite(STR_HTTP, sizeof(STR_HTTP)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send Host header
    results |= exoPal_socketWrite(STR_HOST, sizeof(STR_HOST)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send content type header
    results |= exoPal_socketWrite(STR_CONTENT_JSON, sizeof(STR_CONTENT_JSON)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send accept header
    results |= exoPal_socketWrite(STR_ACCEPT_JSON, sizeof(STR_ACCEPT_JSON)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send content length header
    results |= exoPal_socketWrite(STR_CONTENT_LENGTH, sizeof(STR_CONTENT_LENGTH)-1);
    results |= exoPal_socketWrite(contentLengthStr, len_of_contentLengthStr);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);
    results |= exoPal_socketWrite(STR_CRLF, sizeof(STR_CRLF)-1);

    // send body
    results |= exoPal_socketWrite(requestBody, requestLength);

    if (results != 0)
    {
        exosite_disconnect();
        return results;
    }


    exoPal_sendingComplete();
    
    // get response
    exoPal_socketRead(responseBuffer, responseBufferLength, &responseLength);

    exosite_disconnect();
    return responseLength;

}



/*!
 * \brief Connects to Exosite
 *
 * This would typically be a call to open a socket
 *
 * \return Returns a 0 if socket was successfully opened, else returns the
 *          error code
 * \sa
 * \note
 * \warning
 */
uint8_t exosite_connect(void)
{
    // open socket to exosite

    return exoPal_tcpSocketOpen();
}

/*!
 * \brief Connects to Exosite
 *
 * This would typically be a call to close a socket
 *
 * \return Returns a 0 if socket was successfully close, else returns the
 *          error code
 * \sa
 * \note
 * \warning
 */
uint8_t exosite_disconnect(void)
{
    // Close socket to exosite
    return exoPal_tcpSocketClose();
}


/*!
 * \brief  determines if response matches code
 *
 * \param[in] code Pointer to expected HTTP response code
 * \param[in] response an HTTP response string
 *
 * \return 1 if match, 0 if no match
 *
 */
uint8_t exosite_checkResponse(char * response, const char * code)
{
    // get start second ' ' separate column
    // assumes will always be before the 15th char
    // assumes first char isn't a ' '
    uint8_t spaceFound = 0;
    int16_t i;
    for (i = 1; (i < 15) && (spaceFound == 0); i++)
    {
        if (response[i] == ' ')
        {
            spaceFound = i + 1;
        }
    }
    if (spaceFound > 0)
    {
        // If we found a ' ', try to match the code
        if (code[0] == response[spaceFound] && code[1] == response[spaceFound + 1] && code[2] == response[spaceFound + 2])
        {
            return 1;
        }
    }

    return 0;
}


