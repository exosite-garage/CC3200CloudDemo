//*****************************************************************************
// main.c
//
// Reference code to demonstrate AP Provisioning
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
// 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//    Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the   
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

//****************************************************************************
//
//! \addtogroup ap_provisioning
//! @{
//
//****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes.h"
#include "socket.h"
#include "simplelink.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "gpio.h"
#include "pin.h"
#include "utils.h"
#include "prcm.h"
#include "pinmux.h"
#include "timer.h"
#include "uart_if.h"
#include "i2c_if.h"
#include "gpio_if.h"
#include "timer_if.h"
#include "button_if.h"
#include "tmp006drv.h"
#include "bma222drv.h"

#define DBG_PRINT                (x)
#define APP_TASK_PRIORITY        (1)
#define SPAWN_TASK_PRIORITY      (4)
#define OSI_STACK_SIZE		     (2048)
#define CONNECTION_TIMEOUT_COUNT (5000) /* 5sec */
#define TOKEN_ARRAY_SIZE         16
#define STRING_TOKEN_SIZE        10
#define AP_SSID_LEN_MAX          33
#define SL_STOP_TIMEOUT          30
#define TASK_PRIORITY            (1)

//****************************************************************************
//                     GLOBAL VARIABLES                                    
//****************************************************************************
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

unsigned char g_ucIpObtained = 0, g_ucProfileAdded = 1, g_ucConnectionStatus = 0;
unsigned char g_ucConnectedToConfAP = 0, g_ucProvisioningDone = 0;
unsigned int g_uiConnectTimeoutCnt =0, g_uiIpAddress = 0;
unsigned char g_ucPriority;
char g_cWlanSSID[AP_SSID_LEN_MAX];
char g_cWlanSecurityKey[50];
SlSecParams_t g_SecParams;
Sl_WlanNetworkEntry_t g_NetEntries[20];
char g_token_get [TOKEN_ARRAY_SIZE][STRING_TOKEN_SIZE] = {"__SL_G_US0","__SL_G_US1",
                    "__SL_G_US2", "__SL_G_US3", "__SL_G_US4", "__SL_G_US5", "__SL_G_US6",
                    "__SL_G_US7", "__SL_G_US8", "__SL_G_US9", "__SL_G_USa", "__SL_G_USb",
                    "__SL_G_USc", "__SL_G_USd", "__SL_G_USe", "__SL_G_USf"};

OsiTaskHandle g_ProvisioningAP;

// externs
extern void cloud_demo( void );
extern void Demo_Tick(void);
extern void SW2_Pressed(void);
extern void SW3_Pressed(void);
//extern volatile int sw2_button_on;

//***************************************************************************** 
// Callback function for FreeRTOS 
//*****************************************************************************
#ifdef USE_FREERTOS
void vApplicationStackOverflowHook( OsiTaskHandle pxTask, signed char *pcTaskName )
{
    while(1)
    {
      // Infinite loop;
    }
}

void vApplicationTickHook( void )
{

}

void vApplicationIdleHook( void )
{
      
}

void vAssertCalled( const char *pcFile, unsigned long ulLine )
{
    while(1)
    {
       
    }
}

#endif

//****************************************************************************
//
//! \brief This function handles WLAN events
//!
//! \param[in]      pSlWlanEvent is the event passed to the handler
//!
//! \return	    None
//
//****************************************************************************
void sl_WlanEvtHdlr(SlWlanEvent_t *pSlWlanEvent)
{
    switch(((SlWlanEvent_t*)pSlWlanEvent)->Event)
    {
    case SL_WLAN_CONNECT_EVENT:
           g_ucConnectionStatus = 1;
           break;
        case SL_WLAN_DISCONNECT_EVENT:
           g_ucConnectionStatus = 0;
           break;
        default:
          break;
    }
}

//****************************************************************************
//
//! \brief This function handles events for IP address acquisition via DHCP
//!		   indication
//!
//! \param[in]  	pNetAppEvent is the event passed to the handler
//!
//! \return	                	None
//
//****************************************************************************
void sl_NetAppEvtHdlr(SlNetAppEvent_t *pNetAppEvent)
{
    switch( pNetAppEvent->Event )
    {
        case SL_NETAPP_IPV4_ACQUIRED:
            g_ucIpObtained = 1;
            g_uiIpAddress = pNetAppEvent->EventData.ipAcquiredV4.ip;
            break;
        default:
            break;
    }
}

//*****************************************************************************
//
//! The interrupt handler for the 100ms interrupt.
//!
//! \param  None
//!
//! \return none
//
//*****************************************************************************
void
TimerBaseIntHandler(void)
{
  //
  // Clear the timer interrupt.
  //
  Timer_IF_InterruptClear(TIMERA0_BASE);
  Demo_Tick();
}

//*****************************************************************************
//
//! The interrupt handler for the 100ms interrupt.
//!
//! \param  None
//!
//! \return none
//
//*****************************************************************************
void
TimerA1IntHandler(void)
{
  //
  // Clear the timer interrupt.
  //
  Timer_IF_InterruptClear(TIMERA1_BASE);
  if (GPIOPinRead(GPIOA1_BASE,GPIO_PIN_5)) //sw3
  {
    SW3_Pressed();
  }
  if (GPIOPinRead(GPIOA2_BASE,GPIO_PIN_6)) //sw2
  {
    SW2_Pressed();
  }
}

//*****************************************************************************
//
//! DeInitDriver
//! The function de-initializes a CC3200 device
//!  
//! \param  None
//!  
//! \return none
//
//*****************************************************************************
void
DeInitDriver(void)
{
    //
    // Disconnect from the AP
    //
    sl_WlanDisconnect();
    //
    // Stop the simplelink host
    //
    sl_Stop(SL_STOP_TIMEOUT);
}

void
DeInitDriverAP(void)
{
    //
    // Stop the simplelink host
    //
    sl_Stop(SL_STOP_TIMEOUT);

}
//*****************************************************************************
//
//! initDriver
//! The function initializes a CC3200 device and triggers it to start operation
//!  
//! \param  None
//!  
//! \return none
//
//*****************************************************************************
int
InitDriver(void)
{
    //
    // Start the simplelink host
    //
    return sl_Start(NULL,NULL,NULL);

}

void WlanConnect()
{
    //Add Profile
    sl_WlanProfileAdd(g_cWlanSSID, strlen((char*)g_cWlanSSID), 0, &g_SecParams, 0,g_ucPriority,0);

    //Connecting to the Access point
    sl_WlanConnect(g_cWlanSSID, strlen((char*)g_cWlanSSID), 0, &g_SecParams, 0);

    //waiting for the device to connect to the AP and obtain ip address
    while (g_uiConnectTimeoutCnt<CONNECTION_TIMEOUT_COUNT && ((g_ucConnectionStatus == 0) || (g_ucIpObtained == 0)))
    {
        osi_Sleep(1); //Sleep 1 millisecond
        g_uiConnectTimeoutCnt++;
    }

    g_uiConnectTimeoutCnt = 0;

}


//****************************************************************************
//
//! Task function implements the ProvisioningAP functionality
//!
//! \param none
//! 
//! This function  
//!    1. Starts Device in STA Mode
//!    2. Scans and Stores all the AP
//!    3. Switch to AP Mode and Wait for AP Configuration from Browser
//!    4. Switch to STA Mode and Connect to Configured AP
//!
//! \return None.
//
//****************************************************************************
void ProvisioningAP(void* pTaskParams)
{

    int iDeviceMode = 0, auto_connect_mode = 0;
    char message[100];
    unsigned long intervalInSeconds = 10;
    char force_ap_mode = 0;

    int Index,i;

    // Check SW3, if pressed force AP mode.
    if (GPIOPinRead(GPIOA1_BASE,GPIO_PIN_5)) //sw3
    {
      Report("Forcing into AP Mode\r\n");
      force_ap_mode = 1;
    }

    //
    // Start the driver
    //
    iDeviceMode = InitDriver();
    
    if (iDeviceMode == ROLE_AP)
    {
        //Device in AP Mode, Wait for Initialization to Complete
        while (g_ucIpObtained == 0)
        {
            UtilsDelay(100); 
        }
    }
    
    sl_WlanSetMode(ROLE_STA);
    

    if (iDeviceMode == ROLE_AP)
    	DeInitDriverAP();
    else
    	DeInitDriver();
    
    g_ucIpObtained = 0;
    g_ucConnectionStatus = 0;
    
    //UtilsDelay(10000000); // wait for scan to append msec
    InitDriver();
    
  
    while(!g_ucProvisioningDone)
    {
        // Hardcode SSID
    	//char *ssid = "exosite16";
    	//char *key = "ex0p@ssw0rd";
    	//memcpy(g_cWlanSSID, ssid, strlen(ssid));
    	//memcpy(g_cWlanSecurityKey, key, strlen(key));
    	//g_SecParams.Type = SL_SEC_TYPE_WPA;
        //g_SecParams.Key = g_cWlanSecurityKey;
        //g_SecParams.KeyLen = strlen(g_cWlanSecurityKey);
    	//g_ucProvisioningDone = 1;
        //Report("Default SSID : %s\r\n", g_cWlanSSID);
        //Report("Default WiFi Key : %s\r\n", g_cWlanSecurityKey);
        if ((g_cWlanSSID[0] == 0 || g_cWlanSSID[0] == NULL) ||
             ((g_SecParams.Type != SL_SEC_TYPE_OPEN) && (g_cWlanSecurityKey[0] == 0 || g_cWlanSecurityKey[0] == NULL)))
        {
			 //Scan AP in STA mode
			Report("Scan AP in STA mode\r\n");
			sl_WlanPolicySet(SL_POLICY_SCAN,SL_SCAN_POLICY_EN(1),
							 (unsigned char *)&intervalInSeconds,sizeof(intervalInSeconds));

			// wait for scan to complete
			UtilsDelay(20000000);

			//Get Scan Result
			Index = sl_WlanGetNetworkList(0,20,&g_NetEntries[0]);

			for(i=0; i< Index; i++)
			{
				 snprintf(message, 60, "%d) SSID %s\n\r",i,g_NetEntries[i].ssid);
				 Report("%d) SSID %s\n\r",i,g_NetEntries[i].ssid);
			}
			Report("Switch back to AP Mode\r\n");
			//Switch to AP Mode
			sl_WlanSetMode(ROLE_AP);
	        DeInitDriver();
	        g_ucIpObtained = 0;
	        g_ucConnectionStatus = 0;


	        //Initialize the SLHost Driver
	        InitDriver();

	        char ssid[50];
	        int ssidlen = 0;
	        unsigned char mac[12];
	        SlSecParams_t pSecParams;
	        SlGetSecParamsExt_t pSecExtParams;
	        unsigned long pPriority = 0;
	        for (i = 0; i < 8; i++)
	        {
	        	if (sl_WlanProfileGet(i, ssid, &ssidlen, mac, &pSecParams, &pSecExtParams, &pPriority) >= 0)
	        		break;
	        }
	        ssid[ssidlen] = 0;
            if (ssid != NULL && ssidlen > 0 && !force_ap_mode)
            {
    	        Report("Profile SSID:%s\r\n", ssid);
    	        g_ucIpObtained = 0;
    	        g_ucConnectionStatus = 0;
            	Report("Auto Connect..\r\n");
            	sl_WlanSetMode(ROLE_STA);
                sl_Stop(SL_STOP_TIMEOUT);
                iDeviceMode =  sl_Start(NULL,NULL,NULL);
                Report("STA mode setup ready.\r\n");
                if (iDeviceMode == ROLE_STA)
                {
                  Report("Work in STA mode.\r\n");
                  auto_connect_mode = 1;
                }
                else
                  Report("STA mode switch failure.\r\n");
            }

	        //Wait for Ip Acquired Event in AP Mode
	        while (g_ucIpObtained == 0)
	        {
	            UtilsDelay(100);
	        }

	        //
	        // Wait for AP Configuraiton, Open Browser and Configure AP
	        //
	        if (auto_connect_mode == 0)
	        {
	          while (g_ucProfileAdded && !g_ucProvisioningDone)
	          {
	            UtilsDelay(100);
	          }
	        }
        }

        g_ucProfileAdded = 1;
        //if (g_ucProvisioningDone && !auto_connect_mode)
        if (!auto_connect_mode)
        {
          Report("Switch to STA Mode\r\n");
          //Switch to STA Mode
          sl_WlanSetMode(ROLE_STA);
          
          //AP Configured, Restart in STA Mode
          DeInitDriverAP();
           
          g_ucIpObtained = 0;
          g_ucConnectionStatus = 0;

          //UtilsDelay(10000000);
          InitDriver();
          Report("Connect to SSID:%s\r\n", g_cWlanSSID);
          // Connect to the Configured Access Point
          WlanConnect();
        }
        else if (auto_connect_mode)
        {
        	Report("Wait to auto connect..\r\n");
        	while(!g_ucConnectionStatus && !g_ucIpObtained)
        	{
        	  UtilsDelay(100);
        	}
        }
        g_ucConnectedToConfAP = g_ucConnectionStatus;

        cloud_demo();

        sl_WlanDisconnect();
    }

    Report("End of Program\r\n");
}

//*****************************************************************************
//
//! This function gets triggered when HTTP Server receives Application
//! defined GET and POST HTTP Tokens.
//!
//! \param pSlHttpServerEvent Pointer indicating http server event
//! \param pSlHttpServerResponse Pointer indicating http server response
//!
//! \return None
//!
//*****************************************************************************
void sl_HttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent, SlHttpServerResponse_t *pSlHttpServerResponse)
{
    int i = 0;
    switch (pSlHttpServerEvent->Event)
    {
        case SL_NETAPP_HTTPGETTOKENVALUE:
        {
            if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                            g_token_get [0],pSlHttpServerEvent->EventData.httpTokenName.len))
            {
                if(g_ucConnectedToConfAP == 1)
                {
                    memcpy (pSlHttpServerResponse->ResponseData.token_value.data, "TRUE",strlen("TRUE")); // Important - Connection Status
                    pSlHttpServerResponse->ResponseData.token_value.len = strlen("TRUE");
                }
                else
                {
                    memcpy (pSlHttpServerResponse->ResponseData.token_value.data, "FALSE",strlen("FALSE")); // Important - Connection Status
                    pSlHttpServerResponse->ResponseData.token_value.len = strlen("FALSE");
                }
            }
           for (i = 0; i < TOKEN_ARRAY_SIZE - 1; i++)
           {
            if (0== memcmp (pSlHttpServerEvent->EventData.httpTokenName.data,
                            g_token_get [i + 1],pSlHttpServerEvent->EventData.httpTokenName.len))
            {
                memcpy (pSlHttpServerResponse->ResponseData.token_value.data,
                        g_NetEntries[i].ssid,g_NetEntries[i].ssid_len); // Important - Token value len should be < MAX_TOKEN_VALUE_LEN
                pSlHttpServerResponse->ResponseData.token_value.data[g_NetEntries[i].ssid_len] = 0;
                pSlHttpServerResponse->ResponseData.token_value.len = g_NetEntries[i].ssid_len;
            }
           }
      }
              break;

      case SL_NETAPP_HTTPPOSTTOKENVALUE:
      {

        if ((0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                          "__SL_P_USC", pSlHttpServerEvent->EventData.httpPostData.token_name.len)) &&
            (0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_value.data,
                          "Add", pSlHttpServerEvent->EventData.httpPostData.token_value.len)))
        {
            g_ucProfileAdded = 0;

        }
        if (0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                         "__SL_P_USD", pSlHttpServerEvent->EventData.httpPostData.token_name.len))
        {
            memcpy (g_cWlanSSID,  pSlHttpServerEvent->EventData.httpPostData.token_value.data,
                    pSlHttpServerEvent->EventData.httpPostData.token_value.len);
            g_cWlanSSID[pSlHttpServerEvent->EventData.httpPostData.token_value.len] = 0;
        }

        if (0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                         "__SL_P_USE", pSlHttpServerEvent->EventData.httpPostData.token_name.len))
        {

            if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '0')
            {
                g_SecParams.Type =  SL_SEC_TYPE_OPEN;//SL_SEC_TYPE_OPEN

            }
            else if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '1')
            {
                g_SecParams.Type =  SL_SEC_TYPE_WEP;//SL_SEC_TYPE_WEP

            }
            else if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '2')
            {
                g_SecParams.Type =  SL_SEC_TYPE_WPA;//SL_SEC_TYPE_WPA

            }
            else 
            {
                g_SecParams.Type =  SL_SEC_TYPE_OPEN;//SL_SEC_TYPE_OPEN
            }
        }
        if (0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                         "__SL_P_USF", pSlHttpServerEvent->EventData.httpPostData.token_name.len))
        {
            memcpy (g_cWlanSecurityKey,pSlHttpServerEvent->EventData.httpPostData.token_value.data,
                    pSlHttpServerEvent->EventData.httpPostData.token_value.len);
            g_cWlanSecurityKey[pSlHttpServerEvent->EventData.httpPostData.token_value.len] = 0;
            g_SecParams.Key = g_cWlanSecurityKey;
            g_SecParams.KeyLen = pSlHttpServerEvent->EventData.httpPostData.token_value.len;
        }
        if (0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USG",
                         pSlHttpServerEvent->EventData.httpPostData.token_name.len))
        {
           g_ucPriority = pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] - 48;
        }
        if (0 == memcmp (pSlHttpServerEvent->EventData.httpPostData.token_name.data,
                         "__SL_P_US0", pSlHttpServerEvent->EventData.httpPostData.token_name.len))
        {
            g_ucProvisioningDone = 1;
        }
      }
          break;
      default:
          break;
    }
}


//****************************************************************************
//
//! Main function
//!
//! \param none
//! 
//! This function  
//!    1. Invokes the SLHost task
//!    2. Invokes the AP Provisioning Task
//!
//! \return None.
//
//****************************************************************************
void main()
{
    //
    // Setup the interrupt vector table
    //
#if defined(ewarm)
    IntVTableBaseSet((unsigned long)&__vector_table);
#endif

    //MCU Initialization
    PRCMCC3200MCUInit();

    IntMasterEnable();
    IntEnable(FAULT_SYSTICK);

    PinMuxConfig();
    GPIO_IF_LedConfigure(LED1|LED2|LED3);
    GPIO_IF_LedOff(MCU_ALL_LED_IND);

    Timer_IF_Init(PRCM_TIMERA0, TIMERA0_BASE, TIMER_CFG_PERIODIC, TIMER_A, 0);
    Timer_IF_Init(PRCM_TIMERA1, TIMERA1_BASE, TIMER_CFG_PERIODIC, TIMER_A, 0);

    // Setup the interrupts for the timer timeouts.
    Timer_IF_IntSetup(TIMERA0_BASE, TIMER_A, TimerBaseIntHandler);
    Timer_IF_IntSetup(TIMERA1_BASE, TIMER_A, TimerA1IntHandler);

    InitTerm();
    ClearTerm();

    //
    // I2C Init
    //
    I2C_IF_Open(I2C_MASTER_MODE_FST);

    //Init Temprature Sensor
    TMP006DrvOpen();

    //Init Accelerometer Sensor
    BMA222Open();

    // Turn on the timers
    Timer_IF_Start(TIMERA0_BASE, TIMER_A,
                    PERIODIC_TEST_CYCLES * PERIODIC_TEST_LOOPS / 10);
    Timer_IF_Start(TIMERA1_BASE, TIMER_A,
                    PERIODIC_TEST_CYCLES * PERIODIC_TEST_LOOPS / 40);
    //
    // Simplelinkspawntask
    //
    VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);

    osi_TaskCreate(ProvisioningAP, (signed char*)"ProvisioningAP",OSI_STACK_SIZE, NULL, APP_TASK_PRIORITY, &g_ProvisioningAP );

    osi_start();
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
	//
	// This application doesn't work w/ socket - Events are not expected
	//
	switch( pSock->Event )
	{
		case SL_NETAPP_SOCKET_TX_FAILED:
			switch( pSock->EventData.status )
			{
				case SL_ECLOSE:
					/*UART_PRINT("[SOCK ERROR] - close socket (%d) operation "
					"failed to transmit all queued packets\n\n",
					pSock->EventData.sd);*/
					break;
				default:
					/*UART_PRINT("[SOCK ERROR] - TX FAILED : socket %d , reason "
					"(%d) \n\n",
					pSock->EventData.sd, pSock->EventData.status); */
					break;
			}
			break;
		default:
			/*UART_PRINT("[SOCK EVENT] - Unexpected Event [%x0x]\n\n",pSock->Event);*/
			break;
	}
}
