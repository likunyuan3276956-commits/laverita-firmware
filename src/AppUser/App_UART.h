/******************************************************************************************
//FileName 			:  
//ProjectName  	:	
//MCU 	       	: 
//Security			:	 
//Fcpu					:	 
//High_Clk			:	
//Watch_Dog			:	
//LVD						:	 
//Writen by 		:	
//Date      		:	
******************************************************************************************/



#ifndef __APP_UART_H__
#define __APP_UART_H__
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <App_BSP.h>

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//******************************************************************************
//
//------------- Constant  define    -------------------
//
//******************************************************************************

#define APP_FIFO_BUF_MASK   							0x7F
#define APP_FIFO_BUF_SIZE   							127

//------------------------------------------------------------------------------
////SYNCH+SYNCL+Length+Command+Data0~DataN+CS
//#define PMCU_SYNC_H					        0xA5
//#define PMCU_SYNC_L					        0x5A	
//#define PMCU_SYNC_LEN     					2+1					//SYNCH+SYNCL+LEN
//#define PMCU_CMD_IDX   					    0x00        //
//#define PMCU_DAT_IDX   					    0x01        //
//#define PMCU_CMD_SIZE 					    32
//#define PMCU_CMD_STATUS				      0x53
//#define PMCU_CMD_CONFIG   					0x43


//******************************************************************************
//
//------------- I/O  Port  define    -------------------
//
//******************************************************************************

//******************************************************************************
//
//------------- Type Defs    -------------------
//
//******************************************************************************
typedef struct 
{
	unsigned char		SyncH;
	unsigned char		SyncL;
	unsigned char		CmdLen;
	unsigned char 	LogEN;
	unsigned char		ReadyFG;
	unsigned char		Timeout;
	unsigned char		RxCount;
	unsigned char		RxNose;
	unsigned char		RxTail;
	unsigned char		TxNose;
	unsigned char		TxTail;
	unsigned char		RxLen;
	unsigned char		RXD_BUF[APP_FIFO_BUF_SIZE];
	unsigned char		RXD_REG[APP_FIFO_BUF_SIZE];
	unsigned char		TXD_BUF[APP_FIFO_BUF_SIZE];
}AppUart_t;




//******************************************************************************
//
//-------------  Variable  define   --------------------
//
//******************************************************************************
extern AppUart_t					AppUart1;
extern AppUart_t					AppUart2;
extern AppUart_t					AppUart3;
extern AppUart_t					AppUart4;
extern AppUart_t					AppUart5;

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

extern void App_UART_TimeTick_CB(void);
extern void App_UART_Config(void);
extern void App_UART_Update(void);

extern void App_UART_Send_Byte(usart_type* USARTx, AppUart_t *AppUart, unsigned char TxData);
extern void App_UART_Rece_Byte(usart_type* USARTx, AppUart_t *AppUart, unsigned char RxData);		
extern void App_UART_Send_Buff(usart_type* USARTx, AppUart_t *AppUart, unsigned char *Buffer, unsigned short len);
extern void App_UART_Configura(usart_type* USARTx, AppUart_t *AppUart, unsigned int  BaudRate);
extern void App_UART_Rxd_Updat(usart_type* USARTx, AppUart_t *AppUart);	
extern void App_UART_Time_Tick(usart_type* USARTx, AppUart_t *AppUart);

extern void App_UART1_RX_Decode(usart_type* USARTx, AppUart_t *AppUart, unsigned char *Buffer, unsigned short len);
extern void App_UART2_RX_Decode(usart_type* USARTx, AppUart_t *AppUart, unsigned char *Buffer, unsigned short len);
extern void App_UART3_RX_Decode(usart_type* USARTx, AppUart_t *AppUart, unsigned char *Buffer, unsigned short len);
extern void App_UART4_RX_Decode(usart_type* USARTx, AppUart_t *AppUart, unsigned char *Buffer, unsigned short len);
extern void App_UART5_RX_Decode(usart_type* USARTx, AppUart_t *AppUart, unsigned char *Buffer, unsigned short len);

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

#endif
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

