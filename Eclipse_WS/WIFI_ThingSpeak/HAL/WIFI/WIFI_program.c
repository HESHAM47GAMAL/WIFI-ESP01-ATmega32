 /******************************************************************************
 *
 * Module: Finger Print
 *
 * File Name: FingerPrint_program.c
 *
 * Description: contain Implementation of Finger Print Driver 
 *
 * Author: Hesham Shehata
 *
 *******************************************************************************/


/**************************                   INCLUDES                   **************************/
#include "WIFI_interface.h"
#include "WIFI_private.h"
#include "WIFI_config.h"
#include "../../SERVICE/STD_TYPES.h"
#include "../../MCAL/USART/USART_interface.h"
#include "../../SERVICE/IVT.h"
#include "../../MCAL/GPIO/GPIO_interface.h"
#include "../../MCAL/TIMER0/TIMER0_interface.h"
#include "../LCD/LCD_interface.h"
#define F_CPU   8000000
#include "util/delay.h"


uint8 operation_Done = 0;

#define DEFAULT_BUFFER_SIZE 100
uint8 Rec_Data[DEFAULT_BUFFER_SIZE] = {};
uint8 Counter=0;

uint16 Max_TimeOut = 400; //by Try and error this acceptable number
/*  And also can be reduced for 500 and this is too much time */

void WIFI_Buffer(void)
{
    uint8 oldsrg = SREG;
	cli();
	Rec_Data[Counter] = USART_Catch_UDR_Value();
	Counter++;
	if(Counter == DEFAULT_BUFFER_SIZE){
		Counter = 0; //pointer = 0;
	}
	SREG = oldsrg;
}

uint8 Str_GetLength(uint8 * Str)
{
    uint8 Counter = 0 ;
    while(Str[Counter] != '\0')
    {
        Counter++;
    }
    return Counter ;
}

sint8 Str_Comp(uint8 * s1 ,uint8 * s2 , uint8 length )
{
    while(length-- && *s1 && (*s1 == *s2) )
    {
        s1++;
        s2++;
    }
    if(length == 0)
    {return 0;}
    else
    {
        return *s1 -*s2 ;
    }
}

uint8 is_subset(const char* haystack, const char* needle) {
    if (*needle == '\0') {
        return TRUE;  // Empty needle is always a subset
    }

    while (*haystack != '\0') {
        const char* h = haystack;
        const char* n = needle;

        while (*n != '\0' && *h == *n) {
            h++;
            n++;
        }

        if (*n == '\0') {
            return TRUE;  // Needle found as a subset
        }

        haystack++;
    }

    return FALSE;  // Needle not found as a subset
}

void WIFI_CheckRespond(uint8 * expected_Respond,WIFI_Result * WIFI_state)
{
    // uint8 Buffer_Length = 0 ;
    // Buffer_Length = Str_GetLength(expected_Respond);
    // if(Str_Comp(Rec_Data,expected_Respond,Buffer_Length) == 0)
    LCD_MoveCursor(0,0);
    LCD_DisplayString(Rec_Data);
    if(is_subset(Rec_Data,expected_Respond) == TRUE)
    {
         _delay_ms(500);

        
        // WIFI_BufferClear();
        *WIFI_state =  WIFI_OK ;
        return ;
    }
    *WIFI_state = WIFI_ERROR ;
}


void WIFI_BufferClear(void)
{
    Counter = 0 ;
    WIFI_BufferMemoryReset(Rec_Data , 0 , DEFAULT_BUFFER_SIZE);
}

void WIFI_BufferMemoryReset(uint8 * string , uint8 value , uint8 length)
{
    for(uint8 it = 0 ; it < length ;it++)
    {
        string[it] = value ;
    }
}


void WIFI_SendATCommand(uint8 * command ,uint8 size , uint8 * expected_Respond,WIFI_Result * WIFI_state)
{
    LCD_ClearScreen();
    _delay_ms(500);
    operation_Done++;
    if(size == 0)
    {
        /*  Clear buffer if filled for any reason  */
        WIFI_BufferClear();
        USART_SendStringPolling(command);
    }
    uint16 Timeout = 0 ;
    while((Timeout != Max_TimeOut) && (* WIFI_state != WIFI_OK) )
    {
        WIFI_CheckRespond(expected_Respond,WIFI_state);
        Timer0_UpdateValue(0);
        Timer0_DelayOVFPolling();
        Timeout++;
        LCD_MoveCursor(1,7);
        LCD_intToString(Timeout);
    }
    LCD_MoveCursor(1,9);
    LCD_intToString(operation_Done);
}

void WIFI_Init(WIFI_Result * WIFI_state)
{
     /*  ⚠️⚠️⚠️⚠️ go to Config.h and Config.c modifiy baude rate as It start communication by default by
    * 115200 bps   
    * 8 bit data
    * no Parity
    * 1 stop bit
    */    

    /*  Initialize UART */ 
    USART_Init();

    USART_EnableReceiveInerrupt();

    USART_receiveByteAsynchCallBack(WIFI_Buffer);
    
    /*  initilaize Timer0 with prescaler 8 normal mode as one time delay = 256 us  */
    Timer0_Init();

    * WIFI_state = WIFI_ERROR ;

    /*  1st thing   */
    WIFI_SendATCommand("AT\r\n",0,"\r\nOK\r\n",WIFI_state);
    
    /*  2nd disable Echo  */   
    if(*WIFI_state == WIFI_OK) 
    {
        * WIFI_state = WIFI_ERROR ;
        WIFI_SendATCommand("ATE0\r\n",0,"\r\nOK\r\n",WIFI_state);
        _delay_ms(500);
    }

    /*  Set Wifi mode */
    if(*WIFI_state == WIFI_OK)
    {
        * WIFI_state = WIFI_ERROR ;
        WIFI_SendATCommand(WIFI_Current_Mode,0,"\r\nOK\r\n",WIFI_state);
    }

    /*  Set Wifi channel number */
    if(*WIFI_state == WIFI_OK)
    {
        * WIFI_state = WIFI_ERROR ;
        WIFI_SendATCommand(WIFI_Current_Channel_mode,0,"\r\nOK\r\n",WIFI_state);
    }


    /*  Set Wifi Transfer mode */
    if(*WIFI_state == WIFI_OK)
    {
        * WIFI_state = WIFI_ERROR ;
        WIFI_SendATCommand(WIFI_Current_Transfer_Mode,0,"\r\nOK\r\n",WIFI_state);
    }

}



void WiFi_joinAccessPoint(uint8* SSID, uint8* pass, WIFI_SSD_Connection_Type * pRresult)
{
    uint8 ATCommand[60];
    WIFI_BufferMemoryReset(ATCommand,0,60);
    sprintf(ATCommand, "AT+CWJAP_DEF=\"%s\",\"%s\"\r\n", SSID, pass);
    ATCommand[59] = 0;
    *pRresult =WIFI_Connection_FAil;
    WIFI_SendATCommand(ATCommand,0,"\r\nWIFI CONNECTED\r\n",pRresult);
    if(*pRresult == WIFI_OK)
    {
        *pRresult = WIFI_Connection_OK;
         /* After connection Done ✔️*/
         /*ESP8266 do some things to respond also and tell you 
         1.WIFI Connected  then
         2.OK
         
         ⚠️⚠️⚠️Sooo any command will send during this process ESP will tell you "Busy..."  and ignore this command  */


         /* So I will wait \r\nOK\r\n  respond and Go ahead in my code  */
         /* Initialize status will false*/
         WIFI_Result WIFI_connection_catch_ok = WIFI_InProgress;
         do
         {
            WIFI_CheckRespond("\r\nOK\r\n",&WIFI_connection_catch_ok);
         } while (WIFI_connection_catch_ok != WIFI_OK);
         
        return ;
    }
    else
    {
        if(is_subset(Rec_Data, "+CWJAP:1"))
		 *pRresult = WIFI_Connection_TimeOut;
		else if(is_subset(Rec_Data,  "+CWJAP:2"))
		 *pRresult = WIFI_Connection_WrongPassword;
		else if(is_subset(Rec_Data,  "+CWJAP:3"))
		 *pRresult = WIFI_COnnection_CanNotFindAP;
		else if(is_subset(Rec_Data,  "+CWJAP:4"))
		 *pRresult = WIFI_Connection_FAil;
		else /* For any unexpected result   */
         *pRresult = WIFI_Connection_FAil;
        /*  When Connection fail for any reasons You need to catch "Fail" respond as any command before this respond will ignores and ESP resond Busy... */
        WIFI_Result WIFI_connection_catch_ok = WIFI_InProgress;
        do
        {
        WIFI_CheckRespond("\r\nFAIL\r\n",&WIFI_connection_catch_ok);
        } while (WIFI_connection_catch_ok != WIFI_OK);
        return ;
    }
}


void WiFi_checkConnection(WIFI_AP_Status_Type * pStatus)
{
    WIFI_SendATCommand("AT+CIPSTATUS\r\n",0,"STATUS:2",pStatus);
    if(*pStatus == WIFI_OK)
    {
        *pStatus = WIFI_AP_GotIP ;
        return ;
    }
    else
    {
        if(is_subset(Rec_Data, "STATUS:3"))
		 *pStatus = WIFI_AP_Connected;
		else if(is_subset(Rec_Data,  "STATUS:4"))
		 *pStatus = WIFI_AP_Disconnected;
		else if(is_subset(Rec_Data,  "STATUS:5"))
		 *pStatus = WIFI_AP_Connection_Fail;
		else /* For any unexpected result   */
         *pStatus = WIFI_AP_Connection_Fail;
        return ;
    }
}

void WiFi_start(uint8* Domain, uint8* Port, WIFI_Socket_Result_Type * pRresult )
{
    uint8 ATCommand[60];
    WIFI_BufferMemoryReset(ATCommand,0,60);
    sprintf(ATCommand, "AT+CIPSTART=\"%s\",\"%s\",%s\r\n", "TCP", Domain,Port);
    ATCommand[59] = 0;
    *pRresult = WIFI_Socket_ERROR;
    WIFI_SendATCommand(ATCommand,0,"CONNECT\r\n",pRresult);
    if(*pRresult == WIFI_OK)
    {
        *pRresult = WIFI_Socket_OK ;
        /*  Here mean that Socket openned successfully  */
        /*  Wifi respond with specific respond  consist of
        1."CONNECT\r\n"   then
        2."\r\nOK\r\n"
        */

       /*   So I need to catch "OK" then go ahead*/
       WIFI_Result WIFI_connection_catch_ok = WIFI_InProgress;
        do
        {
        WIFI_CheckRespond("\r\nOK\r\n",&WIFI_connection_catch_ok);
        } while (WIFI_connection_catch_ok != WIFI_OK);
        
        return ;
    }
    else 
    {   /*  Here case that Socket openned and I still connected*/
        /*  Wifi respond with specific respond  consist of
            1."ALREADY CONNECTED\r\n"   then
            2."\r\nERROR\r\n"
            */
        if(is_subset(Rec_Data, "ALREADY CONNECTED\r\n"))
        {
            *pRresult = WIFI_Socket_AlreadyConnected ;

            /*   So I need to catch "ERROR" then go ahead*/
            WIFI_Result WIFI_connection_catch_ok = WIFI_InProgress;
            do
            {
            WIFI_CheckRespond("\r\nERROR\r\n",&WIFI_connection_catch_ok);
            } while (WIFI_connection_catch_ok != WIFI_OK);
        }
        /*  Here case that Socket closed*/
        /*  Wifi respond with specific respond  consist of
            1."\r\nERROR\r\n"   then
            2."CLOSED\r\n"
            */
        else if(is_subset(Rec_Data, "\r\nERROR\r\n"))
        {
            *pRresult = WIFI_Socket_ERROR ;

            /*   So I need to catch "ERROR" then go ahead*/
            WIFI_Result WIFI_connection_catch_ok = WIFI_InProgress;
            do
            {
            WIFI_CheckRespond("CLOSED\r\n",&WIFI_connection_catch_ok);
            } while (WIFI_connection_catch_ok != WIFI_OK);
        }

        return ;
    }

}


void WiFi_send(uint8* data,WIFI_SEND_Result_Type *pRresult)
{
    uint8 Buffer_Length = 0 ;
    /*  Detect data length to send data to make WIFI know size of data will send    */
    Buffer_Length =Str_GetLength(data);
    Buffer_Length +=2 ; /*  Add 2 for "\r\n"*/
    uint8 ATCommand[90] = { };
    sprintf(ATCommand, "AT+CIPSEND=%d\r\n",Buffer_Length);
    ATCommand[89] = 0;
    *pRresult = WIFI_ERROR ;
    WIFI_SendATCommand(ATCommand,0,"\r\nOK\r\n",pRresult);
    if(*pRresult == WIFI_OK)
    {
        WIFI_BufferMemoryReset(ATCommand,0,90);
        sprintf(ATCommand, "%s\r\n",data);
        ATCommand[89] = 0;
        *pRresult = WIFI_ERROR ;
        WIFI_SendATCommand(ATCommand,0,"\r\nSEND OK\r\n",pRresult);
        if(*pRresult == WIFI_OK)
        {
            *pRresult = WIFI_SEND_OK;
        }
        else 
        {
            *pRresult = WIFI_SEND_ERROR;
        }
    }
    else 
    {
        *pRresult = WIFI_SEND_ERROR ;
    }
    
}

