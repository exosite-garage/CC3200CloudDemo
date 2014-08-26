/*****************************************************************************
*
*  cloud_demo.c - Exosite Cloud Application.
*  Copyright (C) 2014 Exosite LLC
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
#include "uart_if.h"
#include "gpio_if.h"
#include "netcfg.h"
#include "utils.h"
#include "tmp006drv.h"
#include "bma222drv.h"
#include <stdio.h>

// local variables
#define VENDOR_NAME            "texasinstruments"
#define MODEL_NAME             "cc3200lp_v1"
#define PING_ALIAS             "ontime"
#define TEMP_ALIAS             "sensortemp"
#define LEDCTRL_ALIAS          "ledd7"
#define SW2_ALIAS              "usrsw"
#define IP_ALIAS               "ipaddr"
#define ALARM_ALIAS            "move"
#define TRIGGER_HOLDOFF_AMOUNT	32;
static char once_on_boot = 0;
volatile char led_onoff = 1;
volatile float max_accel = 0;
volatile unsigned long tickcnt = 0;
volatile unsigned long uptime = 0;
volatile int sw2_button_on = 0;
volatile _NetCfgIpV4Args_t g_ipV4 = {0};
int threshold = 300;
int holdoff = 0;

// local functions

// externs
//extern unsigned short g_usStatusInfo;
//extern unsigned int wlan_connected;
extern unsigned char g_ucConnectedToConfAP;
// global variables


/*****************************************************************************
*
*  get_acc_movment
*
*  \param  None
*
*  \return movment value
*
*  \brief  Computes a "movment" value based on acceleromter.
*
*****************************************************************************/
float get_acc_movement(void)
{
  float movement;

  signed char accX = 0;
  signed char accY = 0;
  signed char accZ = 0;
  BMA222Read(&accX, &accY, &accZ);

  movement = (((float)abs(accX)) + abs(accY) + abs(accZ)) - 64;

  movement = abs(movement) - 10;

  if(movement < 0){
	  return 0;
  }else{
	  return movement;
  }

}

/*****************************************************************************
*
* blinky_led7
*
*  \param  times - count times
*
*  \return None
*
*  \brief  control the led7 blinky times
*
*****************************************************************************/
void blinky_led7(char times)
{
  int intern_cnt = times;

  while (intern_cnt > 0)
  {
    GPIO_IF_LedToggle(MCU_RED_LED_GPIO);
    UtilsDelay(10000000 / 3);  // 250 ms
    GPIO_IF_LedToggle(MCU_RED_LED_GPIO);
    UtilsDelay(10000000 / 3);  // 250 ms
    intern_cnt--;
  }
}

/*****************************************************************************
*
* SW2_Pressed
*
*  \param  None
*
*  \return None
*
*  \brief  Handle the Switch 2 press event
*
*****************************************************************************/
void SW2_Pressed(void)
{
  sw2_button_on++;
  Report("SW2 pressed.\r\n");
}

/*****************************************************************************
*
* SW3_Pressed
*
*  \param  None
*
*  \return None
*
*  \brief  Handle the Switch 3 press event
*
*****************************************************************************/
void SW3_Pressed(void)
{
  Report("SW3 pressed.\r\n");
}

/*****************************************************************************
*
* Demo_Tick
*
*  \param  None
*
*  \return None
*
*  \brief  Counts timer tick and do something every 100 msec
*
*****************************************************************************/
void Demo_Tick(void)
{
  float new_accel;
  // NOTE: This should be called every 100 ms,
  // but it seems like it's more about every 500 ms.
  if (tickcnt % 2 == 0) // 1 sec
  {
    // do something every 1 sec
	uptime++;
  }
  if (tickcnt % 1200 == 0) // 2 mins
  {
	// do something every 2 mins
  }

  tickcnt++;
  if (tickcnt >= 0xFFFFFFFF)
    tickcnt = 0;

  // Check for higher accelermoter movment
  new_accel =  get_acc_movement();
  if(new_accel > max_accel)
	max_accel = new_accel;
}

/*****************************************************************************
*
* Status_Idicate
*
*  \param  None
*
*  \return None
*
*  \brief  Blinky the LED D7 to indicate the current status
*
*****************************************************************************/
void Status_Idicate(void)
{
  int state = Exosite_StatusCode();
  if ((EXO_STATUS_NOAUTH == state || EXO_STATUS_BAD_TCP == state))
  {
    blinky_led7(2);
  }
  else if ((EXO_STATUS_CONFLICT == state || EXO_STATUS_BAD_CIK == state))
  {
    blinky_led7(3);
  }
}

/*****************************************************************************
*
* Cloud_Read
*
*  \param  None
*
*  \return None
*
*  \brief  Reads data from Exosite Cloud, and turn ON/OFF led .. etc. from Cloud
*
*****************************************************************************/
void Cloud_Read(void)
{
  char ledx[5];
  char Read_status = 0, cloud_data = 0;
  Read_status = Exosite_Read(LEDCTRL_ALIAS, ledx, 1);
  if (Read_status)
  {
    ledx[Read_status] = 0;
    cloud_data = atoi(&ledx[0]);
    Report("Exosite Read:  %s=%d\r\n", LEDCTRL_ALIAS, cloud_data);
    if (cloud_data == 1)
      GPIO_IF_LedOn(MCU_RED_LED_GPIO);
    else
      GPIO_IF_LedOff(MCU_RED_LED_GPIO);

    led_onoff = cloud_data;
  }
}

/*****************************************************************************
*
* Report_Sensors
*
*  \param  None
*
*  \return None
*
*  \brief  Posts the data to Exosite Cloud
*
*****************************************************************************/
void Report_Sensors(void)
{
  int state = 0;
  char post_str[200];
  int post_len = 0;
  float fCurrentTemp = 0;
  float cCurrentTemp = 0;
  signed char accX = 0;
  signed char accY = 0;
  signed char accZ = 0;

  BMA222Read(&accX, &accY, &accZ);

  TMP006DrvGetTemp(&fCurrentTemp);
  cCurrentTemp = ((fCurrentTemp - 32) * 5) / 9;
  //Report("Temputure : %.2f F, %.2f C\r\n", fCurrentTemp, cCurrentTemp);

  memcpy(&post_str[post_len], PING_ALIAS, strlen(PING_ALIAS));
  post_len += strlen(PING_ALIAS);
  post_str[post_len] = '=';
  post_len++;
  sprintf(&post_str[post_len], "%d", uptime);

  post_len = strlen(post_str);
  post_str[post_len] = '&';
  post_len++;

  memcpy(&post_str[post_len], TEMP_ALIAS, strlen(TEMP_ALIAS));
  post_len += strlen(TEMP_ALIAS);
  post_str[post_len] = '=';
  post_len++;
  post_len += snprintf(&post_str[post_len], 10, "%.2f", cCurrentTemp);

  post_str[post_len] = '&';
  post_len++;
  memcpy(&post_str[post_len], SW2_ALIAS, strlen(SW2_ALIAS));
  post_len += strlen(SW2_ALIAS);
  post_str[post_len] = '=';
  post_len++;
  sprintf(&post_str[post_len], "%d", (char)sw2_button_on);
  post_len = strlen(post_str);

  post_len += snprintf(&post_str[post_len], 33, "&acc=%.0f&accX=%d&accY=%d&accZ=%d",
		  max_accel,
		  accX,
		  accY,
		  accZ);

  // Reset
  max_accel = 0;

  if (once_on_boot == 0)
  {
    post_str[post_len] = '&';
    post_len++;
    memcpy(&post_str[post_len], IP_ALIAS, strlen(IP_ALIAS));
    post_len += strlen(IP_ALIAS);
    post_str[post_len] = '=';
    post_len++;
    sprintf(&post_str[post_len], "%d", (char)SL_IPV4_BYTE(g_ipV4.ipV4,3));
    post_len = strlen(post_str);
    post_str[post_len] = '.';
    post_len++;
    sprintf(&post_str[post_len], "%d", (char)SL_IPV4_BYTE(g_ipV4.ipV4,2));
    post_len = strlen(post_str);
    post_str[post_len] = '.';
    post_len++;
    sprintf(&post_str[post_len], "%d", (char)SL_IPV4_BYTE(g_ipV4.ipV4,1));
    post_len = strlen(post_str);
    post_str[post_len] = '.';
    post_len++;
    sprintf(&post_str[post_len], "%d", (char)SL_IPV4_BYTE(g_ipV4.ipV4,0));
    post_len = strlen(post_str);
  }

  Report("Exosite Write: %s\r\n", post_str);
  state = Exosite_Write(post_str, post_len);
  if (state)
  {
    once_on_boot = 1;
  }
}

/*****************************************************************************
*
* cloud_demo
*
*  \param  None
*
*  \return None
*
*  \brief  The Exosite Cloud main application
*
*****************************************************************************/
void cloud_demo( void )
{
  int exo_state = -1;
  unsigned char ucDHCP = 0;
  unsigned int demo_delay = 1000;//800000;
  unsigned char len = sizeof(_NetCfgIpV4Args_t);

  Exosite_Init(VENDOR_NAME, MODEL_NAME, IF_WIFI, 0);

  sl_NetCfgGet(SL_IPV4_STA_P2P_CL_GET_INFO,&ucDHCP,&len,(unsigned char *)&g_ipV4);
  Report("IP Addess : %d.%d.%d.%d\r\n", SL_IPV4_BYTE(g_ipV4.ipV4,3), SL_IPV4_BYTE(g_ipV4.ipV4,2), SL_IPV4_BYTE(g_ipV4.ipV4,1), SL_IPV4_BYTE(g_ipV4.ipV4,0));
  Report("Exosite Cloud App Start.\r\n");
  while (1)
  {
    if (g_ucConnectedToConfAP)
    {
      exo_state = Exosite_StatusCode();
      if (EXO_STATUS_OK == exo_state)
      {
        Report_Sensors();
        Cloud_Read();
      }
      else if (EXO_STATUS_BAD_TCP != exo_state)
      {
        if (!Exosite_Activate())
          Report("Exosite: Failed to Activate (Re-Enable or Add to Account)\r\n");
        else
          Report("Exosite: Activated\r\n");
      }
      demo_delay = 2;
    }
    else
    {
      Report("WiFi Disconnected\r\n");
      demo_delay = 30;
    }
    Status_Idicate();
    osi_Sleep(demo_delay * 1000);  // delay * 1 sec
  }
}




