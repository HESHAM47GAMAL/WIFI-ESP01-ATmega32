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


#ifndef  WIFI_PRIAVTE_H_
#define WIFI_PRIVATE_H_


/*
*   @brief : this function used to update buffer of received character  
*   args : void
*   @return: no return
*   @synchronous / Asynchronous : ASynchronous
*   @ Reentrant / Non Reentrant : Reentrant
*/
void WIFI_Buffer(void);

/*
*   @brief : this function used to count number of characters for string
*   arg1 Str : pointer to array of characters
*   @return: Number of characters
*   @synchronous / Asynchronous : Synchronous
*   @ Reentrant / Non Reentrant : Reentrant
*/
uint8 Str_GetLength(uint8 * Str);

/*
*   @brief : this function used compare two strings
*   args : void
*   @return: no return
*   @synchronous / Asynchronous : ASynchronous
*   @ Reentrant / Non Reentrant : Reentrant
*/
sint8 Str_Comp(uint8 * s1 ,uint8 * s2 , uint8 length );

/*
*   @brief : this function used to check ifstring is subset from Another string  
*   arg1 haystack: full string 
*   arg2 needle: string that I want to check if is sebset from  "haystack"
*   @return: no return
*   @synchronous / Asynchronous : ASynchronous
*   @ Reentrant / Non Reentrant : Reentrant
*/
uint8 is_subset(const char* haystack, const char* needle);

/*
*   @brief : this function used to check of expected respond from ESP with curently received in buffer and make update state for this operation
*   @arg1  expected_Respond: pointer for array of characters of expected respond
*   @arg2 WIFI_state:  will carry result for this function that can be any state from this defined data type (WIFI_Result)
*   @return: no return
*   @synchronous / Asynchronous : Synchronous
*   @ Reentrant / Non Reentrant : Reentrant
*/
void WIFI_CheckRespond(uint8 * expected_Respond,WIFI_Result * WIFI_state);


/*
*   @brief : this function used to clear buffer by make value for each index = 0 
*   @args  : void 
*   @return: no return
*   @synchronous / Asynchronous : Synchronous
*   @ Reentrant / Non Reentrant : Reentrant
*/
void WIFI_BufferClear(void);

/*
*   @brief : this function used to make value reset for string with specefic value 
*   @arg1  string: string that will make update  
*   @arg2  value: value that will be uploaded for string
*   @arg3  length: string length
*   @return: no return
*   @synchronous / Asynchronous : Synchronous
*   @ Reentrant / Non Reentrant : Reentrant
*/
void WIFI_BufferMemoryReset(uint8 * string , uint8 value , uint8 length);
#endif