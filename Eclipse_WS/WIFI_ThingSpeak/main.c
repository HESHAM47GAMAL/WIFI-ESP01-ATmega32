// #include "HAL/LCD/LCD_interface.h"
#include "HAL/WIFI/WIFI_interface.h"
#include "MCAL/USART/USART_interface.h"
#include "HAL/LCD/LCD_interface.h"
#include "SERVICE/IVT.h"
#include "MCAL/ADC/ADC_interface.h"

#define F_CPU 8000000
#include "util/delay.h"

WIFI_Result WIFI_Result_OP ;


uint8 WIFI_SSID[] ="anonymous";
uint8 WIFI_Pass[] = "Kill@nyTryToConne(t";
uint8 Domain[]  = "api.thingspeak.com";
uint8 Port[] = "80";

uint8 feild_Data [] = "GET /update?api_key=TKVS1WL2NHLMCXDH&field2=";

WIFI_SSD_Connection_Type WIFI_AP_Connection_Result ;
WIFI_AP_Status_Type WIFI_Status ;
WIFI_Socket_Result_Type WIFI_OpeningSocket_Result ;


WIFI_SEND_Result_Type  Wifi_UploadResult ;

uint16 POT_Result ;

void Int2Str(uint8 * str , uint16 Num)
{
	uint16 temp = 0 ;
	while(Num)
	{
		temp *= 10 ;
		temp += (Num % 10);
		Num /= 10 ;
	}

	uint8 count = 0 ;
	while(temp)
	{
		str[count] = (temp % 10) + '0';
		temp /= 10 ;
		count++;
	}
	str[count] = '\0';
}



int main(void)
{
    sei();
    LCD_init();
    ADC_Init();
    USART_Init();

    LCD_DisplayString("Begin");
	_delay_ms(1000);

    WIFI_Init(&WIFI_Result_OP);
    
    LCD_MoveCursor(0,0);
    if(WIFI_Result_OP == WIFI_ERROR)
    {
        LCD_DisplayString("Test Failed ) : ");
    }
    else if(WIFI_Result_OP == WIFI_OK)
    {
        LCD_DisplayString("Test Succeed ( :");
    }

    WiFi_joinAccessPoint(WIFI_SSID,WIFI_Pass,&WIFI_AP_Connection_Result);
    _delay_ms(2000);
    LCD_MoveCursor(0,0);
    if(WIFI_AP_Connection_Result == WIFI_Connection_OK)
    {
        LCD_DisplayString("AP Connect Done");
    }
    else if(WIFI_AP_Connection_Result == WIFI_Connection_TimeOut)
    {
        LCD_DisplayString("AP TimeOut ");
    }
    else if(WIFI_AP_Connection_Result == WIFI_Connection_WrongPassword)
    {
        LCD_DisplayString("AP Wrong Pass ");
    }
    else if(WIFI_AP_Connection_Result == WIFI_COnnection_CanNotFindAP)
    {
        LCD_DisplayString("AP Not Exist ");
    }
    else if(WIFI_AP_Connection_Result == WIFI_Connection_FAil)
    {
        LCD_DisplayString("AP Failed Conne");
    }
    
    
    WiFi_checkConnection(&WIFI_Status);
	_delay_ms(2000);
	LCD_MoveCursor(0,0);
    if(WIFI_Status ==WIFI_AP_GotIP)
    {
        LCD_DisplayString("Get IP Add");
    }
    else if(WIFI_Status ==WIFI_AP_Connected)
    {
        LCD_DisplayString("Only Connected ");
    }
    else if(WIFI_Status ==WIFI_AP_Disconnected)
    {
        LCD_DisplayString("Dis Connected");
    }
    else if(WIFI_Status ==WIFI_AP_Connection_Fail)
    {
        LCD_DisplayString("Fail Connection");
    }

    WiFi_start(Domain,Port,&WIFI_OpeningSocket_Result);
    LCD_MoveCursor(0,0);
    if(WIFI_OpeningSocket_Result == WIFI_Socket_OK)
    {
        LCD_DisplayString("Open Socket OK");
        
    }
    else if(WIFI_OpeningSocket_Result == WIFI_Socket_ERROR)
    {
       LCD_DisplayString("Open Socket Err") ;
    }
    else if(WIFI_OpeningSocket_Result == WIFI_Socket_AlreadyConnected)
    {
       LCD_DisplayString("Alr Socket open") ;
    }
    _delay_ms(1500);

    WiFi_send(feild_Data,&Wifi_UploadResult) ;
    LCD_MoveCursor(0,0);
    if(Wifi_UploadResult == WIFI_SEND_OK)
    {
        LCD_DisplayString("Upload Done");
    }
    else if(Wifi_UploadResult == WIFI_SEND_ERROR)
    {
        LCD_DisplayString("Upload Failed");
    }

    while(1)
    {


        WiFi_start(Domain,Port,&WIFI_OpeningSocket_Result);
        LCD_MoveCursor(0,0);
        if(WIFI_OpeningSocket_Result == WIFI_Socket_OK)
        {
            LCD_ClearScreen();
            LCD_DisplayString("Open Socket OK");
            _delay_ms(1000);

            POT_Result = ADC_ReadChannelSingleConvertion(ADC_Channel_0);
            uint8 String [6];
            Int2Str(String,POT_Result);

            uint8 Data[90] = { };
            sprintf(Data, "%s%s",feild_Data,String);
            Data[89] = 0;

            WiFi_send(Data,&Wifi_UploadResult) ;
            LCD_ClearScreen();
            if(Wifi_UploadResult == WIFI_SEND_OK)
            {
                LCD_DisplayString("Upload Done");
            }
            else if(Wifi_UploadResult == WIFI_SEND_ERROR)
            {
                LCD_DisplayString("Upload Failed");
            }
            
        }
        else if(WIFI_OpeningSocket_Result == WIFI_Socket_ERROR)
        {
            LCD_DisplayString("Open Socket Err") ;
        }
        else if(WIFI_OpeningSocket_Result == WIFI_Socket_AlreadyConnected)
        {
            LCD_DisplayString("Alr Socket open") ;
        }
        _delay_ms(1500);

        



    }


}

