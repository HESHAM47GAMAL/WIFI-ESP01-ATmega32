
 /******************************************************************************
 *
 * Module: WIFI
 *
 * File Name: WIFI_config.h
 *
 * Description: Header to configure WIFI
 *
 * Author: Hesham Shehata
 *
 *******************************************************************************/


#ifndef WIFI_CONFIG_H_
#define WIFI_CONFIG_H_



#define    WIFI_STATION_mode         "AT+CWMODE_DEF=1\r\n"
#define    WIFI_API_mode             "AT+CWMODE_DEF=2\r\n"
#define    WIFI_STATION_API_mode     "AT+CWMODE_DEF=3\r\n"


#define WIFI_Current_Mode       WIFI_STATION_API_mode

#define WIFI_Single_Channel_mode        "AT+CIPMUX=0\r\n"
#define WIFI_Multiple_Channel_mode      "AT+CIPMUX=1\r\n"

#define WIFI_Current_Channel_mode       WIFI_Single_Channel_mode


#define WIFI_Transfer_normal_mode               "AT+CIPMODE=0\r\n"
#define WIFI_Transfer_Passthrough_mode          "AT+CIPMODE=1\r\n"

#define WIFI_Current_Transfer_Mode         WIFI_Transfer_normal_mode

#endif