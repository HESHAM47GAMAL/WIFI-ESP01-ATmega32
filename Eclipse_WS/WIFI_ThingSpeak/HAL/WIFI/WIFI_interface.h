/******************************************************************************
 *
 * Module: WIFI
 *
 * File Name: WIFI_interface.h
 *
 * Description: Header File contain function Prototype  
 *
 * Author: Hesham Shehata
 *
 *******************************************************************************/

#ifndef WIFI_INTERFACE_H_
#define WIFI_INTERFACE_H_

/**************************                   INCLUDES                   **************************/
#include "../../SERVICE/STD_TYPES.h"


typedef enum 
{
    WIFI_ERROR  = 0,
    WIFI_OK     = 1 ,
    WIFI_InProgress = 20
}WIFI_Result ;


/*  user defined data type for connection with Network result   */
typedef enum
{
    WIFI_Connection_OK                  = 1 ,
    WIFI_Connection_TimeOut             = 2 ,
    WIFI_Connection_WrongPassword       = 3 ,
    WIFI_COnnection_CanNotFindAP        = 4 ,
    WIFI_Connection_FAil                = 5 ,
}WIFI_SSD_Connection_Type;


/*  user defined data type for status of connection with network*/
typedef enum 
{
    WIFI_AP_GotIP           = 2,
    WIFI_AP_Connected       = 3,
    WIFI_AP_Disconnected    = 4,
    WIFI_AP_Connection_Fail =5
}WIFI_AP_Status_Type;


/*  user defined data type for openning socket (also can called onnection with server)*/
typedef enum 
{
    WIFI_Socket_OK                  = 1,
    WIFI_Socket_ERROR               = 2,
    WIFI_Socket_AlreadyConnected    = 3
}WIFI_Socket_Result_Type ;


/*  user defined data type for result of sending data for server */
typedef enum
{
    WIFI_SEND_ERROR         = 0,
    WIFI_SEND_OK            =1 
}WIFI_SEND_Result_Type;


/*
*   @brief : this function used to initailize Wifi Module (with modes that will configured in WIFI_config.h  and also need to go to USART_config.h , .c to configure mode for it)
*   and also initilaize timer0 as used to make timeout 
*   arg1 WIFI_state: will carry result for this function that can be any state from this defined data type (WIFI_Result)
*   @return: no return
*   @synchronous / Asynchronous : Synchronous
*   @ Reentrant / Non Reentrant : Reentrant
*/
void WIFI_Init(WIFI_Result * WIFI_state);

/*
*   @brief : this function used to initailize LCD  pins like RS , RW , E and Data pins 
*   @arg1  SSID: pointer for array of characters for your network name
*   @arg2  pass: pointer for array of characters for password of your network
*   @arg3  pRresult:  will carry result for this function that can be any state from this defined data type (WIFI_SSD_Connection_Type)
*   @return: no return
*   @synchronous / Asynchronous : Synchronous
*   @ Reentrant / Non Reentrant : Reentrant
*/
void WiFi_joinAccessPoint(uint8* SSID, uint8* pass, WIFI_SSD_Connection_Type * pRresult);

/*
*   @brief : this function used to tell you current state for network connection 
*   @arg1  pStatus : will carry current state that can be any state from this defined data type (WIFI_AP_Status_Type)
*   @return: no return
*   @synchronous / Asynchronous : Synchronous
*   @ Reentrant / Non Reentrant : Reentrant
*/
void WiFi_checkConnection(WIFI_AP_Status_Type * pStatus);

/*
*   @brief : this function used to openSocket (connect to server )
*   @arg1  Domain: pointer for array of characters for server domain like "api.thingspeak.com"
*   @arg2  Port: pointer for array of characters for port of server like "80"
*   @arg3  pRresult:  will carry result for this function that can be any state from this defined data type (WIFI_Socket_Result_Type)
*   @return: no return
*   @synchronous / Asynchronous : Synchronous
*   @ Reentrant / Non Reentrant : Reentrant
*/
void WiFi_start(uint8* Domain, uint8* Port, WIFI_Socket_Result_Type * pRresult );

/*
*   @brief : this function used to handle sending data for server (include inside it handling counting number of chacters will be sent)
*   @arg1  data: pointer for array of characters for "way of how update in  server " + "value "
*   Like                        "GET /update?api_key=TKVS1WL2NHLMCXDH&field2="  +  "150"
*   @arg2 pRresult:  will carry result for this function that can be any state from this defined data type (WIFI_SEND_Result_Type)
*   @return: no return
*   @synchronous / Asynchronous : Synchronous
*   @ Reentrant / Non Reentrant : Reentrant
*/
void WiFi_send(uint8* data,WIFI_SEND_Result_Type *pRresult);

#endif