/******************************************************************************************
;FileName 		:	 
;ProjectName  :
;MCU 	       	:   
;Security			:	 
;Fcpu					:	 
;OSC					:
;Watch_Dog		:	  
;LVD					:	 
;Writen by 		:	
;Date      		:													  
;*******************************************************************************************/

#include <stdio.h>
#include "stdbool.h"
#include "App_UART.h"
#include "App_User.h"
#include "App_BSP.h"

//******************************************************************************
//
//------------- Constant  define    -------------------
//
//******************************************************************************


//******************************************************************************
//
//-------------  Bit  Variable  define   --------------
//
//******************************************************************************

//******************************************************************************
//
//-------------  Variable  define   --------------------
//
//******************************************************************************


//******************************************************************************
//
//------------- Const  define    -------------------
//
//******************************************************************************	
AppUart_t					AppUart1;
//AppUart_t				AppUart2;
//AppUart_t			  AppUart3;
//AppUart_t				AppUart4;
//AppUart_t			  AppUart5;


#define					USART3_DMA_BUF_SIZE					41 	
unsigned char 	USART3_DMA_Ready;
unsigned char 	USART3_DMA_RxdBuf[USART3_DMA_BUF_SIZE];
unsigned char 	USART3_DMA_RxdReg[USART3_DMA_BUF_SIZE];

#define					USART5_DMA_BUF_SIZE					41 	
unsigned char 	USART5_DMA_Ready;
unsigned char 	USART5_DMA_RxdBuf[USART5_DMA_BUF_SIZE];
unsigned char 	USART5_DMA_RxdReg[USART5_DMA_BUF_SIZE];

//unsigned char 	USART_DMA_RxdReg[USART3_DMA_BUF_SIZE];

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

void App_USART_DataUpdate(void);

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: TimeTick 1mS
******************************************************************************************/
void App_UART_TimeTick_CB(void)
{
  //-------------------------------------------------
	App_UART_Time_Tick(USART1, &AppUart1);
//App_UART_Time_Tick(USART2, &AppUart2);
//App_UART_Time_Tick(USART3, &AppUart3);
//App_UART_Time_Tick(UART4,  &AppUart4);
//App_UART_Time_Tick(UART5,  &AppUart5);
  //-------------------------------------------------
//App_USART_DataUpdate();
  //-------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
void App_UART_Config(void)
{
  //-------------------------------------------------
	memset((uint8_t *)&AppUart1,0,sizeof(AppUart1));
//memset((uint8_t *)&AppUart2,0,sizeof(AppUart2));
//memset((uint8_t *)&AppUart3,0,sizeof(AppUart3));
//memset((uint8_t *)&AppUart4,0,sizeof(AppUart4));
//memset((uint8_t *)&AppUart5,0,sizeof(AppUart5));  
  //-------------------------------------------------
	App_UART_Configura(USART1, &AppUart1,460800);	//Uart1: BaudRate = 460800	:	Log
//App_UART_Configura(USART2, &AppUart2,115200);	//Uart2: BaudRate = 115200	:
//App_UART_Configura(USART3, &AppUart3,115200);	//Uart3: BaudRate = 115200	:	PMCU2
//App_UART_Configura(UART4,  &AppUart4,115200); //Uart4: BaudRate = 115200  :	
//App_UART_Configura(UART5,  &AppUart5,115200); //Uart5: BaudRate = 115200  : PMCU1
  //-------------------------------------------------
} 

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
void App_UART_Update(void)
{
  //-------------------------------------------------
	App_UART_Rxd_Updat(USART1, &AppUart1);
//App_UART_Rxd_Updat(USART2, &AppUart2);
//App_UART_Rxd_Updat(USART3, &AppUart3);
//App_UART_Rxd_Updat(UART4,  &AppUart4);
//App_UART_Rxd_Updat(UART5,  &AppUart5);
  //-------------------------------------------------
} 

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void USART1_IRQHandler(void)
{
	//----------------------------------------------------------
  if(usart_flag_get(USART1, USART_RDBF_FLAG) != RESET)
  {
		//usart_flag_clear(USART1, USART_RDBF_FLAG);
		App_UART_Rece_Byte(USART1, &AppUart1, usart_data_receive(USART1));	
  }
  //----------------------------------------------------------
  if(usart_flag_get(USART1, USART_TDC_FLAG) != RESET)
  {  
		usart_flag_clear(USART1, USART_TDC_FLAG);
  }		
	//----------------------------------------------------------
}

void USART2_IRQHandler(void)
{
	//----------------------------------------------------------
  if(usart_flag_get(USART2, USART_RDBF_FLAG) != RESET)
  {
		//usart_flag_clear(USART2, USART_RDBF_FLAG);
		//App_UART_Rece_Byte(USART2, &AppUart2, usart_data_receive(USART2));	
  }
  //----------------------------------------------------------
  if(usart_flag_get(USART2, USART_TDC_FLAG) != RESET)
  {  
		usart_flag_clear(USART2, USART_TDC_FLAG);	
  }		
	//----------------------------------------------------------
}

void USART3_IRQHandler(void)
{
	//----------------------------------------------------------
  if(usart_flag_get(USART3, USART_RDBF_FLAG) != RESET)
  {
		//usart_flag_clear(USART3, USART_RDBF_FLAG);
		//App_UART_Rece_Byte(USART3, &AppUart3, usart_data_receive(USART3));	
  }
  //----------------------------------------------------------
  if(usart_flag_get(USART3, USART_TDC_FLAG) != RESET)
  {  
		usart_flag_clear(USART3, USART_TDC_FLAG);	
  }		
	//----------------------------------------------------------
}
void UART4_IRQHandler(void)
{
	//----------------------------------------------------------
  if(usart_flag_get(UART4, USART_RDBF_FLAG) != RESET)
  {
		//usart_flag_clear(UART4, USART_RDBF_FLAG);
		//App_UART_Rece_Byte(UART4, &AppUart4, usart_data_receive(UART4));	
  }
  //----------------------------------------------------------
  if(usart_flag_get(UART4, USART_TDC_FLAG) != RESET)
  {  
		usart_flag_clear(UART4, USART_TDC_FLAG);	
  }		
	//----------------------------------------------------------
}
void UART5_IRQHandler(void)
{
	//----------------------------------------------------------
  if(usart_flag_get(UART5, USART_RDBF_FLAG) != RESET)
  {
		//usart_flag_clear(UART5, USART_RDBF_FLAG);
		//printf("%02X ",usart_data_receive(UART5));
		//App_UART_Rece_Byte(UART5, &AppUart5, usart_data_receive(UART5));	
  }
  //----------------------------------------------------------
  if(usart_flag_get(UART5, USART_TDC_FLAG) != RESET)
  {  
		usart_flag_clear(UART5, USART_TDC_FLAG);	
  }		
	//----------------------------------------------------------
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_UART_Send_Byte(usart_type* USARTx, AppUart_t *AppUart, unsigned char TxData)
{
	//----------------------------------------------------------
	usart_data_transmit(USARTx, TxData);
	while(usart_flag_get(USARTx, USART_TDBE_FLAG) == RESET)
	{
	}
	//----------------------------------------------------------
}
/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: SYNCH + SYNCL + LEN + CMD + IDH + IDL + Data0..DataN + CS
******************************************************************************************/
void App_UART_Rece_Byte(usart_type* USARTx, AppUart_t *AppUart, unsigned char RxData)
{
	//----------------------------------------------------------
	uint8_t i;
	//----------------------------------------------------------
	AppUart->RxCount++;
	AppUart->RXD_BUF[AppUart->RxNose++] = RxData;
	if(AppUart->RxNose>=APP_FIFO_BUF_SIZE){AppUart->RxNose=0;}
	//----------------------------------------------------------		
	if(AppUart->RXD_BUF[AppUart->RxTail+0]==AppUart->SyncH && 
		 AppUart->RXD_BUF[AppUart->RxTail+1]==AppUart->SyncL)
	{
		uint8_t Len = 3+AppUart->RXD_BUF[AppUart->RxTail+2]+1;	
		if(AppUart->RxCount == Len)
		{
			AppUart->RxLen	 = AppUart->RxCount;
			for(i=0;i<AppUart->RxLen;i++)
			{
				AppUart->RXD_REG[i] = AppUart->RXD_BUF[AppUart->RxTail];
				AppUart->RXD_BUF[AppUart->RxTail] = 0x00;
				AppUart->RxTail++;
				if(AppUart->RxTail>=APP_FIFO_BUF_SIZE)AppUart->RxTail=0;
			}
			AppUart->Timeout	= 0;
			AppUart->RxCount	= 0;
			AppUart->RxNose		= 0;
			AppUart->RxTail		= 0;
			AppUart->ReadyFG	= 1;
		}
	}
	//----------------------------------------------------------	
	AppUart->Timeout = 5;	
	//----------------------------------------------------------		
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_UART_Send_Buff(usart_type* USARTx, AppUart_t *AppUart, unsigned char *Buffer, unsigned short len)
{
	//----------------------------------------------------------
	uint32_t i = 0;
	//----------------------------------------------------------
//	if((USARTx) == USART1)			{}	//1=TXD,0=RXD
//	else	if((USARTx) == USART2){}
//	else	if((USARTx) == USART3){}//IO_USART3_TX_EN_H();App_BSP_Delay_US(100);}	//1=TXD,0=RXD
//	else	if((USARTx) == UART4)	{}
//	else	if((USARTx) == UART5)	{}
	//----------------------------------------------------------
	for(i=0; i<len; i++)
	{
		App_UART_Send_Byte(USARTx,AppUart,Buffer[i]);	//App_BSP_Delay_US(100);
	}	
	//----------------------------------------------------------
//	if((USARTx) == USART1)			{}	//1=TXD,0=RXD
//	else	if((USARTx) == USART2){}
//	else	if((USARTx) == USART3){}//App_BSP_Delay_US(100);IO_USART3_TX_EN_L();}	//1=TXD,0=RXD
//	else	if((USARTx) == UART4)	{}
//	else	if((USARTx) == UART5)	{}
	//----------------------------------------------------------
	if((USARTx) == USART1)			{}//App_BSP_LogBuf((unsigned char*)"TXD1",Buffer,len);}
	else	if((USARTx) == USART2){}//App_BSP_LogBuf((unsigned char*)"TXD2",Buffer,len);}
	else	if((USARTx) == USART3){App_BSP_LogBuf((unsigned char*)"TXD3",Buffer,len);}
	else	if((USARTx) == UART4)	{}//App_BSP_LogBuf((unsigned char*)"TXD4",Buffer,len);}
	else	if((USARTx) == UART5)	{App_BSP_LogBuf((unsigned char*)"TXD5",Buffer,len);}
	//----------------------------------------------------------
}


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
void DMA_Config_FOR_USART3(void)
{
  //----------------------------------------------------------------
  dma_init_type dma_init_struct;
	//----------------------------------------------------------------
  // enable dma1 clock 
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);
	//---------------------------------------------------------------- USART2 DMA Channe2-2
  // dma1 channel1 for usart1 tx configuration 
  dma_reset(DMA2_CHANNEL2); //Version	V1.03	: (1)20240716		: App Param Config Bug
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size 					= USART3_DMA_BUF_SIZE;            //Size
  dma_init_struct.direction 						= DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_base_addr 			= (uint32_t)USART3_DMA_RxdBuf;    //Buffer
  dma_init_struct.memory_data_width 		= DMA_MEMORY_DATA_WIDTH_BYTE;
  dma_init_struct.memory_inc_enable 		= TRUE;
  dma_init_struct.peripheral_base_addr 	= (uint32_t)&USART3->dt;          //USART3->DT
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority 							= DMA_PRIORITY_MEDIUM;
  dma_init_struct.loop_mode_enable 			= TRUE;
  dma_init(DMA2_CHANNEL2, &dma_init_struct);
  //----------------------------------------------------------------//Version	V1.03	: (1)20240716		: App Param Config Bug
  dma_interrupt_enable(DMA2_CHANNEL2, DMA_FDT_INT, TRUE);
  nvic_irq_enable(DMA2_Channel2_IRQn, 0, 0);
  dma_flexible_config(DMA2, FLEX_CHANNEL2, DMA_FLEXIBLE_UART3_RX);	      //USART3 RX
  dma_channel_enable(DMA2_CHANNEL2, TRUE); 
	//----------------------------------------------------------------
  memset((uint8_t *)&USART3_DMA_RxdBuf,	0,USART3_DMA_BUF_SIZE);
  USART3_DMA_Ready = 0;
  //----------------------------------------------------------------
}


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
void DMA_Config_FOR_UART5(void)
{
  //----------------------------------------------------------------
  dma_init_type dma_init_struct;
	//----------------------------------------------------------------
  // enable dma1 clock 
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);
	//---------------------------------------------------------------- UART5 DMA Channe2-3
  // dma1 channel1 for usart1 tx configuration 
  dma_reset(DMA2_CHANNEL3);   //Version	V1.03	: (1)20240716		: App Param Config Bug
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size 					= USART5_DMA_BUF_SIZE;            //Size
  dma_init_struct.direction 						= DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_base_addr 			= (uint32_t)USART5_DMA_RxdBuf;    //Buffer
  dma_init_struct.memory_data_width 		= DMA_MEMORY_DATA_WIDTH_BYTE;
  dma_init_struct.memory_inc_enable 		= TRUE;
  dma_init_struct.peripheral_base_addr 	= (uint32_t)&UART5->dt;           //UART5->DT
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority 							= DMA_PRIORITY_MEDIUM;
  dma_init_struct.loop_mode_enable 			= TRUE;
  dma_init(DMA2_CHANNEL3, &dma_init_struct);
  //----------------------------------------------------------------//Version	V1.03	: (1)20240716		: App Param Config Bug
  dma_interrupt_enable(DMA2_CHANNEL3, DMA_FDT_INT, TRUE);
  nvic_irq_enable(DMA2_Channel3_IRQn, 0, 0);
  dma_flexible_config(DMA2, FLEX_CHANNEL3, DMA_FLEXIBLE_UART5_RX);	      //UART5 RX
  dma_channel_enable(DMA2_CHANNEL3, TRUE); 
	//----------------------------------------------------------------
  memset((uint8_t *)&USART5_DMA_RxdBuf,	0,USART5_DMA_BUF_SIZE);
  USART5_DMA_Ready = 0;
  //----------------------------------------------------------------
}



/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_UART_Configura(usart_type* USARTx, AppUart_t *AppUart, unsigned int  BaudRate)
{
	//----------------------------------------------------------
	if((USARTx) == USART1)
	{
		//--------------------------------------------------------
		//AppUart->SyncH	=	PMCU_SYNC_H;			//
		//AppUart->SyncL	=	PMCU_SYNC_L;			//
		//--------------------------------------------------------
		IO_USART1_ENABLE();             		//
		IO_USART1_TXD_OUTPUT();							//
		IO_USART1_RXD_INPUT();							//
    //--------------------------------------------------------
    // configure usart1 param
    usart_init(USART1, BaudRate, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_transmitter_enable(USART1, TRUE);
    usart_receiver_enable(USART1, TRUE);
		//--------------------------------------------------------
		//usart_dma_transmitter_enable(USART1, TRUE);
		//usart_dma_receiver_enable(USART1, TRUE);
    //--------------------------------------------------------
    // enable usart1 interrupt
    //usart_interrupt_enable(USART1, USART_RDBF_INT, TRUE);		  //Enable RXD Interrupt
    //usart_interrupt_enable(USART1, USART_TDBE_INT, TRUE);   	//Enable TXD Interrupt
    //nvic_irq_enable(USART1_IRQn, 0, 0);
    //nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
    //--------------------------------------------------------
    usart_enable(USART1, TRUE);
    //--------------------------------------------------------
	}
	//----------------------------------------------------------
	else	if((USARTx) == USART2)
	{
		//--------------------------------------------------------
		AppUart->SyncH	=	PMCU_SYNC_H;			//
		AppUart->SyncL	=	PMCU_SYNC_L;			//
		//--------------------------------------------------------
		IO_USART2_ENABLE();             		//
		IO_USART2_TXD_OUTPUT();							//
		IO_USART2_RXD_INPUT();							//
    //--------------------------------------------------------
    // configure usart2 param
    usart_init(USART2, BaudRate, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_transmitter_enable(USART2, TRUE);
    usart_receiver_enable(USART2, TRUE);
		//--------------------------------------------------------
		//usart_dma_transmitter_enable(USART2, TRUE);
		//usart_dma_receiver_enable(USART2, TRUE);
    //DMA_FOR_Touch_Config();    
    //--------------------------------------------------------
    // enable usart2 interrupt
    usart_interrupt_enable(USART2, USART_RDBF_INT, TRUE);     //Enable RXD Interrupt
    //usart_interrupt_enable(USART2, USART_TDBE_INT, TRUE);   //Enable TXD Interrupt
    nvic_irq_enable(USART2_IRQn, 0, 0);
    nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
    //--------------------------------------------------------
		usart_enable(USART2, TRUE);		
    //--------------------------------------------------------			
	}
	//----------------------------------------------------------
	else	if((USARTx) == USART3)
	{
		//--------------------------------------------------------
		AppUart->SyncH	=	PMCU_SYNC_H;		//
		AppUart->SyncL	=	PMCU_SYNC_L;		//
		//--------------------------------------------------------
		IO_USART3_ENABLE();             				//
		IO_USART3_TXD_OUTPUT();									//
		IO_USART3_RXD_INPUT();									//	
    //--------------------------------------------------------
    // configure usart3 param
    usart_init(USART3, BaudRate, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_transmitter_enable(USART3, TRUE);
    usart_receiver_enable(USART3, TRUE);
    //--------------------------------------------------------
    // enable usart2 interrupt
    //usart_interrupt_enable(USART3, USART_RDBF_INT, TRUE);   //Enable RXD Interrupt
    ////usart_interrupt_enable(USART3, USART_TDBE_INT, TRUE);   //Enable TXD Interrupt
    //nvic_irq_enable(USART3_IRQn, 0, 0);
    //nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
    //usart_enable(USART3, TRUE);    
		//--------------------------------------------------------
		usart_dma_transmitter_enable(USART3, TRUE);
		usart_dma_receiver_enable(USART3, TRUE);
		usart_enable(USART3, TRUE);
		DMA_Config_FOR_USART3();	
    //--------------------------------------------------------
	}
	//----------------------------------------------------------
	else	if((USARTx) == UART4)
	{
		//--------------------------------------------------------
		AppUart->SyncH	=	PMCU_SYNC_H;			//
		AppUart->SyncL	=	PMCU_SYNC_L;			//
		//--------------------------------------------------------
		IO_UART4_TXD_OUTPUT();							//
		IO_UART4_RXD_INPUT();								//
		IO_UART4_ENABLE();             			//	
		//--------------------------------------------------------
    // configure UART4 param
    usart_init(UART4, BaudRate, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_transmitter_enable(UART4, TRUE);
    usart_receiver_enable(UART4, TRUE);
    //--------------------------------------------------------
    // enable usart4 interrupt
    usart_interrupt_enable(UART4, USART_RDBF_INT, TRUE);     //Enable RXD Interrupt
    //usart_interrupt_enable(UART4, USART_TDBE_INT, TRUE);   //Enable TXD Interrupt
    nvic_irq_enable(UART4_IRQn, 0, 0);
    nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
    usart_enable(UART4, TRUE); 
		//--------------------------------------------------------
	}
	//----------------------------------------------------------
	else	if((USARTx) == UART5)
	{
		//--------------------------------------------------------
		AppUart->SyncH	=	PMCU_SYNC_H;			//
		AppUart->SyncL	=	PMCU_SYNC_L;			//
		//--------------------------------------------------------
		IO_UART5_TXD_OUTPUT();							//
		IO_UART5_RXD_INPUT();								//
		IO_UART5_ENABLE();             			//	
		//--------------------------------------------------------
		// configure UART5 param
    usart_init(UART5, BaudRate, USART_DATA_8BITS, USART_STOP_1_BIT);
    usart_transmitter_enable(UART5, TRUE);
    usart_receiver_enable(UART5, TRUE);
    //--------------------------------------------------------
    // enable usart5 interrupt
    //usart_interrupt_enable(UART5, USART_RDBF_INT, TRUE);     //Enable RXD Interrupt
    ////usart_interrupt_enable(UART5, USART_TDBE_INT, TRUE);   //Enable TXD Interrupt
    //nvic_irq_enable(UART5_IRQn, 0, 0);
    //nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
    //usart_enable(UART5, TRUE);
    //--------------------------------------------------------
		usart_dma_transmitter_enable(UART5, TRUE);
		usart_dma_receiver_enable(UART5, TRUE);
		usart_enable(UART5, TRUE);
    DMA_Config_FOR_UART5();	
    //--------------------------------------------------------
	}
	//----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_UART_Rxd_Updat(usart_type* USARTx, AppUart_t *AppUart)
{
	//----------------------------------------------------------
	if(AppUart->ReadyFG!=0)
	{
		//--------------------------------------------------------
		if((USARTx) == USART1)
		{
			App_UART1_RX_Decode(USARTx,AppUart,AppUart->RXD_REG,AppUart->RxLen);	
		}
		//--------------------------------------------------------
		else	if((USARTx) == USART2)
		{
			App_UART2_RX_Decode(USARTx,AppUart,AppUart->RXD_REG,AppUart->RxLen);	
		}
		//--------------------------------------------------------
		else	if((USARTx) == USART3)
		{
			App_UART3_RX_Decode(USARTx,AppUart,AppUart->RXD_REG,AppUart->RxLen);	
		}
		//--------------------------------------------------------
		else	if((USARTx) == UART4)
		{
			App_UART4_RX_Decode(USARTx,AppUart,AppUart->RXD_REG,AppUart->RxLen);	
		}
		//--------------------------------------------------------
		else	if((USARTx) == UART5)
		{
			App_UART5_RX_Decode(USARTx,AppUart,AppUart->RXD_REG,AppUart->RxLen);	
		}
		//--------------------------------------------------------
		AppUart->ReadyFG 	= 0;
		AppUart->RxLen 		= 0;
		//--------------------------------------------------------
	}	
	//----------------------------------------------------------	
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_UART_Time_Tick(usart_type* USARTx, AppUart_t *AppUart)
{
	//----------------------------------------------------------
	uint8_t i;
	//----------------------------------------------------------
	if(AppUart->Timeout>0)
	{
		AppUart->Timeout--;
		if(AppUart->Timeout==0)
		{
			if(AppUart->RxCount>0)
			{
				AppUart->RxLen	 = AppUart->RxCount;
				for(i=0;i<AppUart->RxLen;i++)
				{
					AppUart->RXD_REG[i] = AppUart->RXD_BUF[AppUart->RxTail];
					AppUart->RXD_BUF[AppUart->RxTail] = 0x00;
					AppUart->RxTail++;
					if(AppUart->RxTail>=APP_FIFO_BUF_SIZE)AppUart->RxTail=0;
				}		
				AppUart->Timeout	= 0;
				AppUart->RxCount	= 0;
				AppUart->RxNose		= 0;
				AppUart->RxTail		= 0;
				AppUart->ReadyFG	= 1;
			}
		}
	}
	//----------------------------------------------------------		
}



//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
void App_USART_DataUpdate(void)
{
  //------------------------------------------------------------
  static unsigned char Tick,Count;
  //------------------------------------------------------------
	if(USART3_DMA_Ready!=0)
  {
    USART3_DMA_Ready = 0;
    Tick = Count = 0; 
    //App_BSP_LogBuf((unsigned char*)"RXD3",USART3_DMA_RxdBuf,USART3_DMA_BUF_SIZE);
    App_User_Res_Update(USART3,USART3_DMA_RxdBuf,USART3_DMA_BUF_SIZE);  
    memset((uint8_t *)&USART3_DMA_RxdBuf,	0,USART3_DMA_BUF_SIZE);
  }
  //------------------------------------------------------------
	if(USART5_DMA_Ready!=0)
  {
    USART5_DMA_Ready = 0;
    Tick = Count = 0; 
    //App_BSP_LogBuf((unsigned char*)"RXD5",USART5_DMA_RxdBuf,USART5_DMA_BUF_SIZE);
    App_User_Res_Update(UART5,USART5_DMA_RxdBuf,USART5_DMA_BUF_SIZE);  
    memset((uint8_t *)&USART5_DMA_RxdBuf,	0,USART5_DMA_BUF_SIZE);
  }
  //------------------------------------------------------------
  if(Tick++>=100)
  {
    Tick = 0;    
    if(USART3_DMA_RxdBuf[0]!=0 || USART5_DMA_RxdBuf[0]!=0)
    {
      if(Count++>=10)
      {
        Count = 0;
        printf("Uart DMA RESET\r\n");
        DMA_Config_FOR_USART3();
        DMA_Config_FOR_UART5();
      }
    }
  }
  //------------------------------------------------------------
}



/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_UART1_RX_Decode(usart_type* USARTx, AppUart_t *AppUart, unsigned char *Buffer, unsigned short len)
{
	//App_BSP_LogBuf((unsigned char*)"RXD1",Buffer,len);
//	//----------------------------------------------------------
//  unsigned char SyncByte,i,n;
//	unsigned char PktLen,CRCPKT,CRCNew;
//  unsigned char PacketRaw[64];
//	//----------------------------------------------------------
//	//App_BSP_LogBuf((unsigned char*)"RXD1",Buffer,len);
//	//App_UART_Send_Buff(USARTx, AppUart, Buffer,len);
//	//App_LCD_CMD_Report(Buffer,len);
//	//----------------------------------------------------------
//	if(len<4)return;
//	if(len>64)len = 64;
//	//----------------------------------------------------------
//	for(n=0;n<len;)
//	{
//		//--------------------------------------------------------
//		SyncByte = Buffer[n++];
//		//--------------------------------------------------------
//		//SYNCH+SYNCL+LEN+CMD+Data0..DataN+CS
//		if(SyncByte == APP_RTX_SYNC_H)
//		{	
//			SyncByte = Buffer[n++];
//			if(SyncByte == APP_RTX_SYNC_L)
//			{
//				//----------------------------------------------------
//				PktLen 	= Buffer[n++];
//				if(PktLen>32)PktLen = 32;
//				for(i=0;i<PktLen;i++)
//				{
//					PacketRaw[i] = Buffer[n++];
//				}
//				CRCPKT = Buffer[n++];
//				CRCNew = UPDATE_CRC(PacketRaw,PktLen);
//				//----------------------------------------------------	
//				if(CRCPKT == CRCNew||CRC==0x00)
//				{
//					App_PCBA_CMD_Report(PacketRaw,PktLen);
//				}
//        else
//        {
//          printf("CRC:%02X-%02X\r\n",CRCPKT,CRCNew);
//        }
//				//----------------------------------------------------
//			}			
//		}
//		//--------------------------------------------------------
//	}
//	//----------------------------------------------------------	
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_UART2_RX_Decode(usart_type* USARTx, AppUart_t *AppUart, unsigned char *Buffer, unsigned short len)
{
	//----------------------------------------------------------
	//App_BSP_LogBuf((unsigned char*)"RXD2",Buffer,len);
	//App_UART_Send_Buff(USARTx, AppUart, Buffer,len);
  //----------------------------------------------------------
  //LCD_Command_Report(Buffer,len);
	//----------------------------------------------------------	
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_UART3_RX_Decode(usart_type* USARTx, AppUart_t *AppUart, unsigned char *Buffer, unsigned short len)
{
	//----------------------------------------------------------
	//App_BSP_LogBuf((unsigned char*)"RXD3",Buffer,len);
	//App_UART_Send_Buff(USARTx, AppUart, Buffer,len);
  //----------------------------------------------------------
  //App_RGB_GameReport(Buffer,len);
	//App_User_Res_Update(USARTx,Buffer,len);
	//----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_UART4_RX_Decode(usart_type* USARTx, AppUart_t *AppUart, unsigned char *Buffer, unsigned short len)
{
	//----------------------------------------------------------
	//App_BSP_LogBuf((unsigned char*)"RXD4",Buffer,len);
	//App_UART_Send_Buff(USARTx, AppUart, Buffer,len);
  //----------------------------------------------------------
  //App_Power_CMD_Report(Buffer,len);  
  //App_Scaner_CMD_Report(Buffer,len);  
  //----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_UART5_RX_Decode(usart_type* USARTx, AppUart_t *AppUart, unsigned char *Buffer, unsigned short len)
{
	//----------------------------------------------------------
	App_BSP_LogBuf((unsigned char*)"RXD5",Buffer,len);
	//App_UART_Send_Buff(USARTx, AppUart, Buffer,len);
  //----------------------------------------------------------
	//App_Game_Command_Report(Buffer,len);
  //App_User_Res_Update(USARTx,Buffer,len);
	//----------------------------------------------------------
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&






