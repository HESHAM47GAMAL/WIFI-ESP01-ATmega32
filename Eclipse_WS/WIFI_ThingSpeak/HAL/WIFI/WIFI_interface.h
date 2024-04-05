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

typedef enum
{
    WIFI_Connection_OK                  = 1 ,
    WIFI_Connection_TimeOut             = 2 ,
    WIFI_Connection_WrongPassword       = 3 ,
    WIFI_COnnection_CanNotFindAP        = 4 ,
    WIFI_Connection_FAil                = 5 ,
}WIFI_SSD_Connection_Type;


typedef enum 
{
    WIFI_AP_GotIP           = 2,
    WIFI_AP_Connected       = 3,
    WIFI_AP_Disconnected    = 4,
    WIFI_AP_Connection_Fail =5
}WIFI_AP_Status_Type;

typedef enum 
{
    WIFI_Socket_OK                  = 1,
    WIFI_Socket_ERROR               = 2,
    WIFI_Socket_AlreadyConnected    = 3
}WIFI_Socket_Result_Type ;


typedef enum
{
    WIFI_SEND_ERROR         = 0,
    WIFI_SEND_OK            =1 
}WIFI_SEND_Result_Type;

void WIFI_Init(WIFI_Result * WIFI_state);


void WiFi_joinAccessPoint(uint8* SSID, uint8* pass, WIFI_SSD_Connection_Type * pRresult);

void WiFi_checkConnection(WIFI_AP_Status_Type * pStatus);

void WiFi_start(uint8* Domain, uint8* Port, WIFI_Socket_Result_Type * pRresult );

void WiFi_send(uint8* data,WIFI_SEND_Result_Type *pRresult);

#endif