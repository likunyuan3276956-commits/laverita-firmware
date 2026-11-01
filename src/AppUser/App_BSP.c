/******************************************************************************************
//FileName 			:
//ProjectName  	:
//MCU 	       	:
//Security			:
//Fosc					:
//PLL INPUT  		:
//PLL OUTPUT 		:
//Fcpu					:
//Watch_Dog			:
//LVD						:
//Writen by 		:
//Date      		:
******************************************************************************************/


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "App_BSP.h"

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

#define APP_BSP_LOG(x)           		printf x
#define APP_BSP_DBG(x)           		//printf x

//******************************************************************************
//
//------------- Constant  define    -------------------
//
//******************************************************************************

 
//******************************************************************************
//
//------------- I/O Defs    -------------------
//
//******************************************************************************


//******************************************************************************
//
//------------- Type Defs    -------------------
//
//******************************************************************************


//******************************************************************************
//
//-------------  Variable  define   --------------------
//
//******************************************************************************


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

/* support printf function, usemicrolib is unnecessary */
#if (__ARMCC_VERSION > 6000000)
  __asm (".global __use_no_semihosting\n\t");
  void _sys_exit(int x)
  {
    x = x;
  }
  /* __use_no_semihosting was requested, but _ttywrch was */
  void _ttywrch(int ch)
  {
    ch = ch;
  }
  FILE __stdout;
#else
 #ifdef __CC_ARM
  #pragma import(__use_no_semihosting)
  struct __FILE
  {
    int handle;
  };
  FILE __stdout;
  void _sys_exit(int x)
  {
    x = x;
  }
  /* __use_no_semihosting was requested, but _ttywrch was */
  void _ttywrch(int ch)
  {
    ch = ch;
  }
 #endif
#endif

#if defined (__GNUC__) && !defined (__clang__)
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

/**
  * @brief  retargets the c library printf function to the usart.
  * @param  none
  * @retval none
  */
PUTCHAR_PROTOTYPE
{
  while(usart_flag_get(AT32_PRINT_UART, USART_TDBE_FLAG) == RESET);
  usart_data_transmit(AT32_PRINT_UART, ch);
  return ch;
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/	
void App_BSP_Log_Uart_Init(uint32_t baudrate)
{
  //----------------------------------------------------------
  IO_USART1_TXD_OUTPUT();
  IO_USART1_RXD_INPUT();
  IO_USART1_ENABLE();
  //----------------------------------------------------------
  // configure uart param 
  usart_init(AT32_PRINT_UART, baudrate, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(AT32_PRINT_UART, TRUE);
  usart_enable(AT32_PRINT_UART, TRUE);
  //----------------------------------------------------------
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


#define STEP_DELAY_MS   50
static __IO uint32_t    fac_us;
static __IO uint32_t    fac_ms;

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/	
void App_BSP_Delay_Init()
{
  //----------------------------------------------------------
  // configure systick
  systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_NODIV);
  fac_us = system_core_clock / (1000000U);
  fac_ms = fac_us * (1000U);
  //----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/	
void App_BSP_Delay_US(uint32_t nus)
{
  //----------------------------------------------------------
  uint32_t temp = 0;
  //----------------------------------------------------------
  SysTick->LOAD = (uint32_t)(nus * fac_us);
  SysTick->VAL = 0x00;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
  do
  {
    temp = SysTick->CTRL;
  }while((temp & 0x01) && !(temp & (1 << 16)));
  //----------------------------------------------------------
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0x00;
  //----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/	
void App_BSP_Delay_MS(uint16_t nms)
{
  //----------------------------------------------------------
  uint32_t temp = 0;
  //----------------------------------------------------------
  while(nms)
  {
    if(nms > STEP_DELAY_MS)
    {
      SysTick->LOAD = (uint32_t)(STEP_DELAY_MS * fac_ms);
      nms -= STEP_DELAY_MS;
    }
    else
    {
      SysTick->LOAD = (uint32_t)(nms * fac_ms);
      nms = 0;
    }
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
      temp = SysTick->CTRL;
    }while((temp & 0x01) && !(temp & (1 << 16)));

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
  }
  //----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/	
void App_BSP_Delay_SEC(uint16_t sec)
{
  //----------------------------------------------------------
  uint16_t index;
  //----------------------------------------------------------
  for(index = 0; index < sec; index++)
  {
    App_BSP_Delay_MS(500);
    App_BSP_Delay_MS(500);
  }
  //----------------------------------------------------------
}

/******************************************************************************************
Name		:
Input   :
Output  :
describe:
******************************************************************************************/
void usb_delay_ms(uint32_t ms)
{
  App_BSP_Delay_MS(ms);
}

/******************************************************************************************
Name		:
Input   :
Output  :
describe:
******************************************************************************************/
void usb_delay_us(uint32_t us)
{
  App_BSP_Delay_US(us);
}

/******************************************************************************************
Name		:
Input   :
Output  :
describe:
******************************************************************************************/
unsigned char	UPDATE_CRC (unsigned char *Buffer,unsigned char Len)
{ 
	//--------------------------------------------------
	unsigned char i=0,j=0,crc=0;
	//--------------------------------------------------	
	for (j = 0; j < Len; j++)
	{
		crc ^= Buffer[j];
		for (i = 0; i < 8; i++)
		{
			if ((crc & 0x01)!=0)
			{
				crc = (unsigned char)((crc >> 1) ^ 0x8C);
			}
			else
			{
				crc >>= 1;
			}
		}
	}	
  //-------------------------------------------------- 
	return crc;
  //-------------------------------------------------- 	
}

/******************************************************************************************
Name		:
Input   :
Output  :
describe:
******************************************************************************************/
unsigned short UPDATE_CRC_16(unsigned char *Buffer,unsigned char Len)
{
  //-------------------------------------------------- 	
	unsigned short crc=0xFFFF;
	unsigned char i,j;
  //-------------------------------------------------- 	
	for(j=0;j<Len;j++)
	{
		crc=crc ^ *Buffer++;
		for(i=0;i<8;i++)
		{
			if((crc & 0x0001)>0)
			{
				crc=crc>>1;
				crc=crc^0xa001;
			}
			else
				crc=crc>>1;
		}
	}
  //-------------------------------------------------- 	
	return(crc);
  //-------------------------------------------------- 	
}


/******************************************************************************************
Name		:
Input   :
Output  :
describe:
******************************************************************************************/
void App_BSP_LogBuf(const unsigned char *str,unsigned char *Buffer,unsigned short Len)
{ 
	//----------------------------------------------------------
	unsigned short  i;
	//----------------------------------------------------------
	printf("%s:%d,",str,Len);
	for(i=0;i<Len;i++)
	{
		printf("%02X ",Buffer[i]);
	}
	printf("\r\n");
	//----------------------------------------------------------
}

void App_BSP16_LogBuf(const unsigned char *str,unsigned short *Buffer,unsigned short Len)
{ 
	//----------------------------------------------------------
	unsigned short  i;
	//----------------------------------------------------------
	printf("%s:%d,",str,Len);
	for(i=0;i<Len;i++)
	{
		printf("%04X ",Buffer[i]);
	}
	printf("\r\n");
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
void TMR1_OVF_TMR10_IRQHandler(void)
{
  if(tmr_flag_get(TMR1, TMR_OVF_FLAG) != RESET)
  {
    TMR_1mS_Interrupt_CB();
    tmr_flag_clear(TMR1, TMR_OVF_FLAG);
  }
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
Prescaler 	= (TMR1CLK / TMR1 counter clock) - 1	
						= (SystemCoreClock / 24000000) - 1;
						= (144MHz/24MHz)-1
						= 5
TMR_Period 	= (TMR1 counter clock/Frequency)-1
						=	(24MHz / 1KHz) - 1
						= 23999
******************************************************************************************/
void Timer1_Config(void)
{
  //----------------------------------------------------------
  crm_clocks_freq_type crm_clocks_freq_struct = {0};
  uint32_t Prescaler  = 0;
  uint32_t TMR_Period = 0;
  //----------------------------------------------------------
  // get system clock
  crm_clocks_freq_get(&crm_clocks_freq_struct);
  //----------------------------------------------------------
  // enable tmr1 clock 
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
  //----------------------------------------------------------
  Prescaler = (crm_clocks_freq_struct.ahb_freq / 24000000) - 1;
  TMR_Period= (24000000/1000)-1;
  //----------------------------------------------------------
  // tmr1 configuration 
  // time base configuration 
  tmr_base_init(TMR1, TMR_Period, Prescaler);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);
  //----------------------------------------------------------
  // overflow interrupt enable 
  tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);
  //----------------------------------------------------------
  // tmr1 overflow interrupt nvic init
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(TMR1_OVF_TMR10_IRQn, 0, 0);
  //----------------------------------------------------------
  // enable tmr1
  tmr_counter_enable(TMR1, TRUE);
  //----------------------------------------------------------
}


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
//void TMR3_GLOBAL_IRQHandler(void)
//{
//  if(tmr_flag_get(TMR3, TMR_OVF_FLAG) != RESET)
//  {
//    TMR_1mS_Interrupt_CB();
//    tmr_flag_clear(TMR3, TMR_OVF_FLAG);
//  }
//}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
Prescaler 	= (TMR3CLK / TMR3 counter clock) - 1	
						= (SystemCoreClock / 24000000) - 1;
						= (144MHz/24MHz)-1
						= 5
TMR_Period 	= (TMR3 counter clock/Frequency)-1
						=	(24MHz / 1KHz) - 1
						= 23999
******************************************************************************************/
//void Timer3_Config(void)
//{
//  //----------------------------------------------------------
//  crm_clocks_freq_type crm_clocks_freq_struct = {0};
//  uint32_t Prescaler  = 0;
//  uint32_t TMR_Period = 0;
//  //----------------------------------------------------------
//  // get system clock
//  crm_clocks_freq_get(&crm_clocks_freq_struct);
//  //----------------------------------------------------------
//  // enable tmr3 clock 
//  crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);
//  //----------------------------------------------------------
//  Prescaler = (crm_clocks_freq_struct.ahb_freq / 24000000) - 1;
//  TMR_Period= (24000000/1000)-1;
//  //----------------------------------------------------------
//  // tmr3 configuration 
//  // time base configuration 
//  tmr_base_init(TMR3, TMR_Period, Prescaler);
//  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);
//  //----------------------------------------------------------
//  // overflow interrupt enable 
//  tmr_interrupt_enable(TMR3, TMR_OVF_INT, TRUE);
//  //----------------------------------------------------------
//  // tmr3 overflow interrupt nvic init
//  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
//  nvic_irq_enable(TMR3_GLOBAL_IRQn, 0, 0);
//  //----------------------------------------------------------
//  // enable tmr3
//  tmr_counter_enable(TMR3, TRUE);
//  //----------------------------------------------------------
//}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
TMRx Configuration: generate 4 PWM signals with 4 different duty cycles:
The TMRxCLK frequency is set to SystemCoreClock (Hz), to get TMRx counter
clock at 24 MHz the Prescaler is computed as following:
TMR_Clock   = 24000000
Prescaler 	= (TMRxCLK / TMRx counter clock) - 1	
            = (SystemCoreClock / 24000000) - 1;
            = (144MHz/24MHz)-1
            = 5
TMR_Period 	= (TMRx counter clock/Frequency)-1
            =	(24MHz / 16KHz) - 1
            = 1499
PA8: TMR1 Channel1 duty cycle = (TMR1_CCR1/ TMR1_ARR)* 100
PA9: TMR1 Channel2 duty cycle = (TMR1_CCR2/ TMR1_ARR)* 100
PA10:TMR1 Channel3 duty cycle = (TMR1_CCR3/ TMR1_ARR)* 100
PA11:TMR1 Channel4 duty cycle = (TMR1_CCR4/ TMR1_ARR)* 100
******************************************************************************************/
void Timer1_PWM_Config(void)
{
  //----------------------------------------------------------
  gpio_init_type gpio_init_struct;
  uint32_t TMR_Clock  = 24000000;
  uint16_t div_value  = 0;
  uint32_t TMR_Period = 0;
  //----------------------------------------------------------
  // tmr clock enable 
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
  //----------------------------------------------------------
  // gpioa gpiob clock enable
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  //----------------------------------------------------------
  //gpio configuration
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_pins            = GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_11;
  gpio_init_struct.gpio_out_type        = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull            = GPIO_PULL_NONE;
  gpio_init_struct.gpio_mode            = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength  = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);
  //----------------------------------------------------------
  //compute the div value 
  div_value = (uint16_t)(system_core_clock / 24000000) - 1;
  TMR_Period= (TMR_Clock / IO_PWM_FREQ ) - 1;
  //----------------------------------------------------------
  // tmr time base configuration 
  tmr_base_init(TMR1, TMR_Period, div_value);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);
  //----------------------------------------------------------
  //tmr_repetition_counter_set(TMR1, 4);
  
  tmr_primary_mode_select(TMR1, TMR_PRIMARY_SEL_OVERFLOW);
  tmr_sub_sync_mode_set(TMR1, TRUE);
  tmr_output_enable(TMR1, TRUE);
  //----------------------------------------------------------
  PWM_Duty_Update(TMR_SELECT_CHANNEL_1,20);
  PWM_Duty_Update(TMR_SELECT_CHANNEL_2,40);
  PWM_Duty_Update(TMR_SELECT_CHANNEL_3,60);
  PWM_Duty_Update(TMR_SELECT_CHANNEL_4,80);
  //----------------------------------------------------------
  tmr_period_buffer_enable(TMR1, TRUE);
  //----------------------------------------------------------
  // tmr enable counter 
  tmr_counter_enable(TMR1, TRUE);
  //----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
TMRx Configuration: generate 4 PWM signals with 4 different duty cycles:
The TMRxCLK frequency is set to SystemCoreClock (Hz), to get TMRx counter
clock at 24 MHz the Prescaler is computed as following:
TMR_Clock   = 24000000
Prescaler 	= (TMRxCLK / TMRx counter clock) - 1	
            = (SystemCoreClock / 24000000) - 1;
            = (144MHz/24MHz)-1
            = 5
TMR_Period 	= (TMRx counter clock/Frequency)-1
            =	(24MHz / 16KHz) - 1
            = 1499
PA6: TMR3 Channel1 duty cycle = (TMR3_CCR1/ TMR3_ARR)* 100
PA7: TMR3 Channel2 duty cycle = (TMR3_CCR2/ TMR3_ARR)* 100
PB0: TMR3 Channel3 duty cycle = (TMR3_CCR3/ TMR3_ARR)* 100
PB1: TMR3 Channel4 duty cycle = (TMR3_CCR4/ TMR3_ARR)* 100
******************************************************************************************/
void Timer3_PWM_Config(void)
{
  //----------------------------------------------------------
  gpio_init_type gpio_init_struct;
  uint32_t TMR_Clock  = 24000000;
  uint16_t div_value  = 0;
  uint32_t TMR_Period = 0;
  //----------------------------------------------------------
  // tmr3 clock enable 
  crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);
  //----------------------------------------------------------
  // gpioa gpiob clock enable
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE); 
  //----------------------------------------------------------
  //gpio configuration
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_pins            = GPIO_PINS_6 | GPIO_PINS_7;
  gpio_init_struct.gpio_out_type        = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull            = GPIO_PULL_NONE;
  gpio_init_struct.gpio_mode            = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength  = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);
  gpio_init_struct.gpio_pins            = GPIO_PINS_0 | GPIO_PINS_1;
  gpio_init(GPIOB, &gpio_init_struct);  
  //----------------------------------------------------------
  //compute the div value 
  div_value = (uint16_t)(system_core_clock / 24000000) - 1;
  TMR_Period= (TMR_Clock / IO_PWM_FREQ ) - 1;
  //----------------------------------------------------------
  // tmr3 time base configuration 
  tmr_base_init(TMR3, TMR_Period, div_value);
  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR3, TMR_CLOCK_DIV1);
  //----------------------------------------------------------
  PWM_Duty_Update(TMR_SELECT_CHANNEL_1,20);
  PWM_Duty_Update(TMR_SELECT_CHANNEL_2,40);
  PWM_Duty_Update(TMR_SELECT_CHANNEL_3,60);
  PWM_Duty_Update(TMR_SELECT_CHANNEL_4,80);
  //----------------------------------------------------------
  tmr_period_buffer_enable(TMR3, TRUE);
  //----------------------------------------------------------
  // tmr enable counter 
  tmr_counter_enable(TMR3, TRUE);
  //----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
TMRx Configuration: generate 4 PWM signals with 4 different duty cycles:
The TMRxCLK frequency is set to SystemCoreClock (Hz), to get TMRx counter
clock at 24 MHz the Prescaler is computed as following:
TMR_Clock   = 24000000
Prescaler 	= (TMRxCLK / TMRx counter clock) - 1	
            = (SystemCoreClock / 24000000) - 1;
            = (144MHz/24MHz)-1
            = 5
TMR_Period 	= (TMRx counter clock/Frequency)-1
            =	(24MHz / 16KHz) - 1
            = 1499
PA0: TMR5 Channel1 duty cycle = (TMR5_CCR1/ TMR5_ARR)* 100
PA1: TMR5 Channel2 duty cycle = (TMR5_CCR2/ TMR5_ARR)* 100
PA2: TMR5 Channel3 duty cycle = (TMR5_CCR3/ TMR5_ARR)* 100
PA3: TMR5 Channel4 duty cycle = (TMR5_CCR4/ TMR5_ARR)* 100
******************************************************************************************/
void Timer5_PWM_Config(void)
{
  //----------------------------------------------------------
  gpio_init_type gpio_init_struct;
  uint32_t TMR_Clock  = 24000000;
  uint16_t div_value  = 0;
  uint32_t TMR_Period = 0;
  //----------------------------------------------------------
  // tmr5 clock enable 
  crm_periph_clock_enable(CRM_TMR5_PERIPH_CLOCK, TRUE);
  //----------------------------------------------------------
  // gpioa gpiob clock enable
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE); 
  //----------------------------------------------------------
  //gpio configuration
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_pins            = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2 | GPIO_PINS_3;
  gpio_init_struct.gpio_out_type        = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull            = GPIO_PULL_NONE;
  gpio_init_struct.gpio_mode            = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength  = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);
  //----------------------------------------------------------
  //compute the div value 
  div_value = (uint16_t)(system_core_clock / 24000000) - 1;
  TMR_Period= (TMR_Clock / IO_PWM_FREQ ) - 1;
  //----------------------------------------------------------
  // tmr5 time base configuration 
  tmr_base_init(TMR5, TMR_Period, div_value);
  tmr_cnt_dir_set(TMR5, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR5, TMR_CLOCK_DIV1);
  //----------------------------------------------------------
  PWM_Duty_Update(TMR_SELECT_CHANNEL_1,20);
  PWM_Duty_Update(TMR_SELECT_CHANNEL_2,40);
  PWM_Duty_Update(TMR_SELECT_CHANNEL_3,60);
  PWM_Duty_Update(TMR_SELECT_CHANNEL_4,80);
  //----------------------------------------------------------
  tmr_period_buffer_enable(TMR5, TRUE);
  //----------------------------------------------------------
  // tmr enable counter 
  tmr_counter_enable(TMR5, TRUE);
  //----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void PWM_Duty_Update(unsigned char Chan,unsigned short Duty)
{
	//----------------------------------------------------------
	tmr_output_config_type tmr_oc_init_structure;
  uint32_t TMR_Clock    = 24000000;
  uint16_t TimerPeriod  = 0;
  uint16_t PWM_Duty_Val = 0;
	//----------------------------------------------------------
  // Compute the value to be set in ARR regiter to generate
  TimerPeriod = (uint16_t)((TMR_Clock /IO_PWM_FREQ )-1);
  //----------------------------------------------------------
  // Compute CCR1 value to generate a duty cycle
  PWM_Duty_Val = (uint16_t)(((uint32_t)Duty*(TimerPeriod-1))/99);

//  //----------------------------------------------------------TMR1
//  tmr_output_default_para_init(&tmr_oc_init_structure);
//  tmr_oc_init_structure.oc_mode           = TMR_OUTPUT_CONTROL_PWM_MODE_B;
//  tmr_oc_init_structure.oc_idle_state     = TRUE;
//  tmr_oc_init_structure.occ_idle_state    = FALSE;
//  tmr_oc_init_structure.oc_polarity       = TMR_OUTPUT_ACTIVE_LOW;
//  tmr_oc_init_structure.occ_polarity      = TMR_OUTPUT_ACTIVE_LOW;
//  tmr_oc_init_structure.oc_output_state   = TRUE;
//  tmr_oc_init_structure.occ_output_state  = TRUE;
//  tmr_output_channel_config(TMR1, (tmr_channel_select_type)Chan, &tmr_oc_init_structure);
//  //tmr_output_channel_buffer_enable(TMR1, (tmr_channel_select_type)Chan, TRUE);
//  tmr_channel_value_set(TMR1, (tmr_channel_select_type)Chan, PWM_Duty_Val);  
  
  //---------------------------------------------------------- TMR3
//  tmr_output_default_para_init(&tmr_oc_init_structure);
//  tmr_oc_init_structure.oc_mode         = TMR_OUTPUT_CONTROL_PWM_MODE_A;
//  tmr_oc_init_structure.oc_idle_state   = FALSE;
//  tmr_oc_init_structure.oc_polarity     = TMR_OUTPUT_ACTIVE_HIGH;
//  tmr_oc_init_structure.oc_output_state = TRUE;

//  tmr_output_channel_config(TMR3, (tmr_channel_select_type)Chan, &tmr_oc_init_structure);
//  tmr_output_channel_buffer_enable(TMR3, (tmr_channel_select_type)Chan, TRUE);
//  tmr_channel_value_set(TMR3, (tmr_channel_select_type)Chan, PWM_Duty_Val);

  //----------------------------------------------------------TRM5
  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode         = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_oc_init_structure.oc_idle_state   = FALSE;
  tmr_oc_init_structure.oc_polarity     = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_oc_init_structure.oc_output_state = TRUE;

  tmr_output_channel_config(TMR5, (tmr_channel_select_type)Chan, &tmr_oc_init_structure);
  tmr_output_channel_buffer_enable(TMR5, (tmr_channel_select_type)Chan, TRUE);
  tmr_channel_value_set(TMR5, (tmr_channel_select_type)Chan, PWM_Duty_Val);
	//----------------------------------------------------------
}


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: Configure PA0 as EXTI Line0 interrupt mode
******************************************************************************************/
void EXINT0_IRQHandler(void)
{
  if(exint_flag_get(EXINT_LINE_0) != RESET)
  {
    printf("ExINT0 \r\n");
    //EXTI_LINE0_CB();
    exint_flag_clear(EXINT_LINE_0);
  }
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: Configure PA0 as EXTI Line0 interrupt mode
******************************************************************************************/
void EXTI_LINE0_Config(void)
{
  //----------------------------------------------------------
  exint_init_type exint_init_struct;
  //----------------------------------------------------------
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  //----------------------------------------------------------
  gpio_exint_line_config(GPIO_PORT_SOURCE_GPIOA, GPIO_PINS_SOURCE0);
  //----------------------------------------------------------
  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable   = TRUE;
  exint_init_struct.line_mode     = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select   = EXINT_LINE_0;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);
  //----------------------------------------------------------
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(EXINT0_IRQn, 1, 0);
  //----------------------------------------------------------
  
//	//----------------------------------------------------------
//	EXTI_InitType   EXTI_InitStructure;
//	GPIO_InitType   GPIO_InitStructure;
//	NVIC_InitType   NVIC_InitStructure;
//	//----------------------------------------------------------
//	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE);        
//	GPIO_InitStructure.GPIO_Pins 			= GPIO_Pins_4;
//  GPIO_InitStructure.GPIO_MaxSpeed 	= GPIO_MaxSpeed_2MHz;
//	GPIO_InitStructure.GPIO_Mode 			= GPIO_Mode_IN_PU;					//Configure PB.4 as Input
//	GPIO_Init(GPIOB, &GPIO_InitStructure);                       
//	//----------------------------------------------------------
//	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);         //<Enable AFIO clock
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinsSource4); //<Connect EXTI0 Line to PB4 pin

//	//----------------------------------------------------------
//	EXTI_InitStructure.EXTI_Line 			= EXTI_Line4;
//	EXTI_InitStructure.EXTI_Mode 			= EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger 	= EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineEnable= ENABLE;
//	EXTI_Init(&EXTI_InitStructure);                              //<Configure EXTI0 line
//	//----------------------------------------------------------
//	NVIC_InitStructure.NVIC_IRQChannel 									= EXTI4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0x0F;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0x0F;
//	NVIC_InitStructure.NVIC_IRQChannelCmd 							= ENABLE;
//	NVIC_Init(&NVIC_InitStructure);                              //<Enable and set EXTI0 Interrupt to the lowest priority
//	//----------------------------------------------------------
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

uint16_t 	ADC_Value[ADC_MAX_CHAN];
__IO uint16_t ADCConvertedValue[ADC_MAX_CHAN*10];	

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifdef  ADC_DMA_ENABLE
uint8_t Get_ADC_RAW_Data(void)	
{
  //----------------------------------------------------------
	uint8_t		i=0,j=0,Range=0,NewData=0;
	uint16_t 	ADCTempVal[ADC_MAX_CHAN];
	uint32_t	ADCTempDat[ADC_MAX_CHAN];
	//----------------------------------------------------------
  for(j=0;j<ADC_MAX_CHAN;j++)
  {
    ADCTempVal[j] = 0;
    ADCTempDat[j] = 0;
  }
  //----------------------------------------------------------
	for(i=0;i<10;i++)
	{
    for(j=0;j<ADC_MAX_CHAN;j++)
    {
      ADCTempDat[j] += ADCConvertedValue[i*ADC_MAX_CHAN+j];
    }    
	}
  for(j=0;j<ADC_MAX_CHAN;j++)
  {
    ADCTempVal[j] = ADCTempDat[j]/10;
  }  
	//----------------------------------------------------------
	for(i=0;i<ADC_MAX_CHAN;i++)
	{
		//Range = abs(ADC_Value[i]-ADCTempVal[i]);
		//if(Range>=ADC_NOISE_RANGE)
		{
			NewData = 1;
			ADC_Value[i] = ADCTempVal[i];
		}
	}
	//----------------------------------------------------------
	return	NewData;
	//----------------------------------------------------------
}
#endif

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifdef  ADC_DMA_ENABLE
void MCU_DMA_Config_For_ADC(void)
{
  //----------------------------------------------------------
  dma_init_type dma_init_struct;
  //----------------------------------------------------------
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
  //----------------------------------------------------------
  dma_reset(DMA1_CHANNEL1);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size           = ADC_MAX_CHAN*10;
  dma_init_struct.direction             = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_base_addr      = (uint32_t)ADCConvertedValue;
  dma_init_struct.memory_data_width     = DMA_MEMORY_DATA_WIDTH_HALFWORD;
  dma_init_struct.memory_inc_enable     = TRUE;
  dma_init_struct.peripheral_base_addr  = (uint32_t)&(ADC1->odt);
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority              = DMA_PRIORITY_HIGH;
  dma_init_struct.loop_mode_enable      = TRUE;
  dma_init(DMA1_CHANNEL1, &dma_init_struct);
  //----------------------------------------------------------
  dma_channel_enable(DMA1_CHANNEL1, TRUE);
  //----------------------------------------------------------	
//  // enable transfer full data intterrupt 
//  dma_interrupt_enable(DMA1_CHANNEL1, DMA_FDT_INT, TRUE);
//  // dma1 channel1 interrupt nvic init 
//  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
//  nvic_irq_enable(DMA1_Channel1_IRQn, 1, 0);
//  // tmr2 flexible function enable
//  dma_flexible_config(DMA1, FLEX_CHANNEL1, DMA_FLEXIBLE_ADC1);
  //----------------------------------------------------------	
}
#endif

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifdef  ADC_DMA_ENABLE
void MCU_ADC_Config_With_DMA(void)
{
  //----------------------------------------------------------
  adc_base_config_type adc_base_struct;
  //----------------------------------------------------------
  crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
  crm_adc_clock_div_set(CRM_ADC_DIV_6);
  //----------------------------------------------------------
  IO_ADC_CH00_INPUT();
  IO_ADC_CH01_INPUT();
  IO_ADC_CH02_INPUT();
  IO_ADC_CH03_INPUT();
  IO_ADC_CH04_INPUT();
  IO_ADC_CH05_INPUT();
  IO_ADC_CH06_INPUT();
  IO_ADC_CH07_INPUT();
  
  IO_ADC_CH08_INPUT();
  IO_ADC_CH09_INPUT();
  IO_ADC_CH10_INPUT();
  IO_ADC_CH11_INPUT();
  IO_ADC_CH12_INPUT();
  IO_ADC_CH13_INPUT();
  IO_ADC_CH14_INPUT();
  IO_ADC_CH15_INPUT();
  //----------------------------------------------------------
  adc_base_default_para_init(&adc_base_struct);
  adc_base_struct.sequence_mode           = TRUE;
  adc_base_struct.repeat_mode             = TRUE;
  adc_base_struct.data_align              = ADC_RIGHT_ALIGNMENT;
  adc_base_struct.ordinary_channel_length = ADC_MAX_CHAN;
  adc_base_config(ADC1, &adc_base_struct);
  //----------------------------------------------------------
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_0,  1, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_1,  2, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_2,  3, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_3,  4, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_4,  5, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_5,  6, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_6,  7, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_7,  8, ADC_SAMPLETIME_239_5);
  
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_8,  9, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_9, 10, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_10,11, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_11,12, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_12,13, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_13,14, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_14,15, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_15,16, ADC_SAMPLETIME_239_5);
  //----------------------------------------------------------
  adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);
  adc_voltage_monitor_enable(ADC1, ADC_VMONITOR_SINGLE_ORDINARY);
  adc_voltage_monitor_threshold_value_set(ADC1, 0xBBB, 0xAAA);
  adc_voltage_monitor_single_channel_select(ADC1, ADC_CHANNEL_0);
  //----------------------------------------------------------
  //nvic_irq_enable(ADC1_IRQn, 0, 0);
  //adc_interrupt_enable(ADC1, ADC_VMOR_INT, TRUE);
  //----------------------------------------------------------
  adc_dma_mode_enable(ADC1, TRUE);
  adc_enable(ADC1, TRUE);
  //----------------------------------------------------------
  adc_calibration_init(ADC1);
  while(adc_calibration_init_status_get(ADC1));
  adc_calibration_start(ADC1);
  while(adc_calibration_status_get(ADC1));
  //----------------------------------------------------------
  adc_ordinary_software_trigger_enable(ADC1, TRUE);
  //----------------------------------------------------------
}
#endif

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifndef  ADC_DMA_ENABLE
void MCU_ADC_Config(void)
{
	//----------------------------------------------------------
	adc_base_config_type adc_base_struct;
  //----------------------------------------------------------
  crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
  crm_adc_clock_div_set(CRM_ADC_DIV_2);	
	//----------------------------------------------------------
//  IO_ADC_CH00_INPUT();
//  IO_ADC_CH01_INPUT();
//  IO_ADC_CH02_INPUT();
//  IO_ADC_CH03_INPUT();
//  IO_ADC_CH04_INPUT();
//  IO_ADC_CH05_INPUT();
//  IO_ADC_CH06_INPUT();
//  IO_ADC_CH07_INPUT();
//  
//  IO_ADC_CH08_INPUT();
//  IO_ADC_CH09_INPUT();
//  IO_ADC_CH10_INPUT();
//  IO_ADC_CH11_INPUT();
//  IO_ADC_CH12_INPUT();
//  IO_ADC_CH13_INPUT();
//  IO_ADC_CH14_INPUT();
//  IO_ADC_CH15_INPUT();

  IO_CC1_AD_INPUT();
  IO_CC2_AD_INPUT();
  //----------------------------------------------------------
  adc_base_default_para_init(&adc_base_struct);
  adc_base_struct.sequence_mode           = FALSE;
  adc_base_struct.repeat_mode             = FALSE;
  adc_base_struct.data_align              = ADC_RIGHT_ALIGNMENT;
  adc_base_struct.ordinary_channel_length = 1;//ADC_MAX_CHAN;
  adc_base_config(ADC1, &adc_base_struct);
  //----------------------------------------------------------
  //adc_ordinary_channel_set(ADC1, ADC_CHANNEL_0,  1, ADC_SAMPLETIME_7_5);
  //adc_ordinary_channel_set(ADC1, ADC_CHANNEL_1,  2, ADC_SAMPLETIME_7_5);
  //----------------------------------------------------------
  adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);
  adc_voltage_monitor_enable(ADC1, ADC_VMONITOR_SINGLE_ORDINARY);
  adc_voltage_monitor_threshold_value_set(ADC1, 0xBBB, 0xAAA);
  adc_voltage_monitor_single_channel_select(ADC1, ADC_CHANNEL_0);
  //----------------------------------------------------------
  //nvic_irq_enable(ADC1_IRQn, 0, 0);
  //adc_interrupt_enable(ADC1, ADC_VMOR_INT, TRUE);
  //----------------------------------------------------------
  //adc_dma_mode_enable(ADC1, TRUE);
  adc_enable(ADC1, TRUE);
  //----------------------------------------------------------
  adc_calibration_init(ADC1);
  while(adc_calibration_init_status_get(ADC1));
  adc_calibration_start(ADC1);
  while(adc_calibration_status_get(ADC1));
  //----------------------------------------------------------
  //adc_ordinary_software_trigger_enable(ADC1, TRUE);
  //----------------------------------------------------------
}
#endif

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifndef  ADC_DMA_ENABLE
uint16_t GetADCValue(uint16_t Channel)
{
	//----------------------------------------------------------
  adc_enable(ADC1, FALSE);
	//----------------------------------------------------------
	switch(Channel)
	{
		case	ADC_CHANNEL_0:  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_0,  1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_1:  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_1,  1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_2:  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_2,  1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_3:  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_3,  1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_4:  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_4,  1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_5:  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_5,  1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_6:  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_6,  1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_7:  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_7,  1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_8:  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_8,  1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_9:  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_9,  1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_10: adc_ordinary_channel_set(ADC1, ADC_CHANNEL_10, 1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_11: adc_ordinary_channel_set(ADC1, ADC_CHANNEL_11, 1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_12: adc_ordinary_channel_set(ADC1, ADC_CHANNEL_12, 1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_13: adc_ordinary_channel_set(ADC1, ADC_CHANNEL_13, 1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_14: adc_ordinary_channel_set(ADC1, ADC_CHANNEL_14, 1, ADC_SAMPLETIME_239_5); break;
		case	ADC_CHANNEL_15: adc_ordinary_channel_set(ADC1, ADC_CHANNEL_15, 1, ADC_SAMPLETIME_239_5); break;    
	}
	//----------------------------------------------------------
	adc_enable(ADC1, TRUE);		// Enable ADC1
	//----------------------------------------------------------
  adc_ordinary_software_trigger_enable(ADC1, TRUE);
	//----------------------------------------------------------
	//while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EC));
	//ADC_ClearFlag(ADC1,ADC_FLAG_EC);
  //----------------------------------------------------------
  //App_BSP_Delay_US(10);
  while(adc_flag_get(ADC1,ADC_CCE_FLAG) == RESET);
  adc_flag_clear(ADC1, ADC_CCE_FLAG);
	//----------------------------------------------------------
	return	adc_ordinary_conversion_data_get(ADC1);
	//----------------------------------------------------------
}
#endif

/******************************************************************************************
Name		:
Input   :
Output  :
describe:
******************************************************************************************/
void CHK_ADC_SCAN(AppADC_t *AppADC)	
{
  //------------------------------------------------------------------ 
	//unsigned int ADC_TEMP = 0;
	//------------------------------------------------------------------ 
  #ifdef  ADC_DMA_ENABLE
	Get_ADC_RAW_Data();
  #else
//  ADC_Value[0] = GetADCValue(ADC_CHANNEL_0);
//  ADC_Value[1] = GetADCValue(ADC_CHANNEL_1);
//  ADC_Value[2] = GetADCValue(ADC_CHANNEL_2);
//  ADC_Value[3] = GetADCValue(ADC_CHANNEL_3);  
//  ADC_Value[4] = GetADCValue(ADC_CHANNEL_4);
//  ADC_Value[5] = GetADCValue(ADC_CHANNEL_5); 
//  ADC_Value[6] = GetADCValue(ADC_CHANNEL_6);
//  ADC_Value[7] = GetADCValue(ADC_CHANNEL_7);

//  ADC_Value[8] = GetADCValue(ADC_CHANNEL_8);
//  ADC_Value[9] = GetADCValue(ADC_CHANNEL_9); 
//  ADC_Value[10]= GetADCValue(ADC_CHANNEL_10);
//  ADC_Value[11]= GetADCValue(ADC_CHANNEL_11);
//  ADC_Value[12]= GetADCValue(ADC_CHANNEL_12);
//  ADC_Value[13]= GetADCValue(ADC_CHANNEL_13); 
//  ADC_Value[14]= GetADCValue(ADC_CHANNEL_14);
//  ADC_Value[15]= GetADCValue(ADC_CHANNEL_15);
  
  ADC_Value[0] = GetADCValue(IO_CC1_CHAN);
  ADC_Value[1] = GetADCValue(IO_CC2_CHAN);
  #endif
	//------------------------------------------------------------------
  AppADC->ADC_Val1	=	(ADC_Value[0]*3300)/4096;   //mV
  AppADC->ADC_Val2	=	(ADC_Value[1]*3300)/4096;   //mV
  //------------------------------------------------------------------
//  AppADC->Buf[ 0]  = 4096-ADC_Value[ 0];       //KeyPad-1
//  AppADC->Buf[ 1]  = 4096-ADC_Value[ 1];
//  AppADC->Buf[ 2]  = 4096-ADC_Value[ 2];
//  AppADC->Buf[ 3]  = 4096-ADC_Value[ 3];
//  AppADC->Buf[ 4]  = 4096-ADC_Value[ 4];       //KeyPad-2
//  AppADC->Buf[ 5]  = 4096-ADC_Value[ 5];
//  AppADC->Buf[ 6]  = 4096-ADC_Value[ 6];
//  AppADC->Buf[ 7]  = 4096-ADC_Value[ 7];
//  AppADC->Buf[ 8]  = 4096-ADC_Value[ 8];       //KeyPad-3
//  AppADC->Buf[ 9]  = 4096-ADC_Value[ 9];
//  AppADC->Buf[10]  = 4096-ADC_Value[10];
//  AppADC->Buf[11]  = 4096-ADC_Value[11];
//  AppADC->Buf[12]  = 4096-ADC_Value[12];       //KeyPad-4
//  AppADC->Buf[13]  = 4096-ADC_Value[13];
//  AppADC->Buf[14]  = 4096-ADC_Value[14];
//  AppADC->Buf[15]  = 4096-ADC_Value[15];

  AppADC->Buf[ 0]  = ADC_Value[ 0];
  AppADC->Buf[ 1]  = ADC_Value[ 1];
  
  //Version	V1.08	: (1)20241016		: Brightness For RGB  
  //------------------------------------------------------------------ 
  //APP_BSP_LOG(("ADC : %04d,%04d \n",AppADC->ADC_Val1,AppADC->ADC_Val2));
  //------------------------------------------------------------------ 
  if(AppADC->ADC_Val1>AppADC->ADC_Val2)
  {
    App_RGB_CC_PowerUpdate(AppADC->ADC_Val1);
  }
  else
  {
    App_RGB_CC_PowerUpdate(AppADC->ADC_Val2);
  }
	//------------------------------------------------------------------ 	
  
  
  //------------------------------------------------------------------ 
//  APP_BSP_LOG(("ADC : %04d,%04d (%04d,%04d)\n",ADC_Value[0],ADC_Value[1],AppADC->ADC_Val1,AppADC->ADC_Val2));
//  APP_BSP_LOG(("ADC : %04d,%04d,%04d,%04d  %04d,%04d,%04d,%04d  %04d,%04d,%04d,%04d  %04d,%04d,%04d,%04d\n", 
//  ADC_Value[ 0],ADC_Value[ 1],ADC_Value[ 2],ADC_Value[ 3],
//  ADC_Value[ 4],ADC_Value[ 5],ADC_Value[ 6],ADC_Value[ 7],
//  ADC_Value[ 8],ADC_Value[ 9],ADC_Value[10],ADC_Value[11],
//  ADC_Value[12],ADC_Value[13],ADC_Value[14],ADC_Value[15]));

//  APP_BSP_LOG(("ADC : %04d,%04d,%04d,%04d  %04d,%04d,%04d,%04d  %04d,%04d,%04d,%04d  %04d,%04d,%04d,%04d\n", 
//  AppADC->Buf[ 0],AppADC->Buf[ 1],AppADC->Buf[ 2],AppADC->Buf[ 3],
//  AppADC->Buf[ 4],AppADC->Buf[ 5],AppADC->Buf[ 6],AppADC->Buf[ 7],
//  AppADC->Buf[ 8],AppADC->Buf[ 9],AppADC->Buf[10],AppADC->Buf[11],
//  AppADC->Buf[12],AppADC->Buf[13],AppADC->Buf[14],AppADC->Buf[15]));
	//------------------------------------------------------------------
	//APP_USER_LOG(("User APP: C1=%d, C2=%d\n",AppADC.Curr1,AppADC.Curr2));
	//------------------------------------------------------------------ 		
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
//void HW_SPI_1_Slave_Config(void)
//{
//	//----------------------------------------------------------
//	GPIO_InitType	GPIO_InitStructure;
//	SPI_InitType	SPI_InitStructure;
//	NVIC_InitType NVIC_InitStructure;
//	//----------------------------------------------------------
//  // Enable SPI1 clock and GPIO clock for SPI1
//  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_SPI1 | RCC_APB2PERIPH_GPIOA, ENABLE);

//	//----------------------------------------------------------
//  // Configure SCK and MOSI pins as Input Floating 
//  GPIO_InitStructure.GPIO_Pins = HW_SPI_1_SCK_PIN | HW_SPI_1_MOSI_PIN;
//  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(HW_SPI_1_GPIO, &GPIO_InitStructure);

//	//Configure MISO pin as Alternate Function Push Pull
//  GPIO_InitStructure.GPIO_Pins = HW_SPI_1_MISO_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(HW_SPI_1_GPIO, &GPIO_InitStructure);
//	
//	//----------------------------------------------------------
//	//SPI1 Config
//  SPI_DefaultInitParaConfig(&SPI_InitStructure);
//  SPI_InitStructure.SPI_TransMode = SPI_TRANSMODE_FULLDUPLEX;
//  SPI_InitStructure.SPI_Mode 			= SPI_MODE_SLAVE;//SPI_MODE_MASTER;
//  SPI_InitStructure.SPI_FrameSize = SPI_FRAMESIZE_8BIT;
//  SPI_InitStructure.SPI_CPOL 			= SPI_CPOL_LOW;
//  SPI_InitStructure.SPI_CPHA 			= SPI_CPHA_2EDGE;
//  SPI_InitStructure.SPI_NSSSEL 		= SPI_NSSSEL_SOFT;
//  SPI_InitStructure.SPI_MCLKP 		= SPI_MCLKP_128;
//  SPI_InitStructure.SPI_FirstBit 	= SPI_FIRSTBIT_LSB;
//  SPI_InitStructure.SPI_CPOLY = 7;
//  SPI_Init(HW_SPI_1, &SPI_InitStructure);

//	//----------------------------------------------------------
//	//SPI1 Interrupt Config
//  // 1 bit for pre-emption priority, 3 bits for subpriority
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

//  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);	
//	//----------------------------------------------------------
//  // Enable SPI_MASTER TXE interrupt 
//  //SPI_I2S_INTConfig(SPI1, SPI_I2S_INT_TE, ENABLE);
//  // Enable SPI_SLAVE RXNE interrupt 
//  SPI_I2S_INTConfig(SPI1, SPI_I2S_INT_RNE, ENABLE);
//	// Enalbe SPI 1
//	SPI_Enable(HW_SPI_1, ENABLE);
//	//----------------------------------------------------------
//}


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void MCU_GPIO_Config()
{
  //----------------------------------------------------------
  // initialize delay function 
  App_BSP_Delay_Init();
	//----------------------------------------------------------
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE); //<Enable AFIO clock
  //----------------------------------------------------------
  IO_JLINK_FOR_GPIO();
  //----------------------------------------------------------
//  IO_SDCARD_D0_PIN();
//  IO_SDCARD_D1_PIN();
//  IO_SDCARD_D2_PIN();
//  IO_SDCARD_D3_PIN();
//  IO_SDCARD_SCK_PIN();
//  IO_SDCARD_CMD_PIN();
//  IO_SDCARD_DET_PIN();  
	//----------------------------------------------------------
	SW_I2C_SCL_OUTPUT();
	SW_I2C_SDA_OUTPUT();
	SW_I2C_SCL_HIGH();
	SW_I2C_SDA_HIGH();
  IO_TP_INT_INPUT();  
  IO_TP_RST_OUTPUT();
  IO_TP_RST_HIGH(); 
  //----------------------------------------------------------
  IO_BTN_RGB_1_OUTPUT();
  IO_BTN_RGB_2_OUTPUT();
  IO_BTN_RGB_3_OUTPUT();
  IO_BTN_RGB_4_OUTPUT();
  IO_BTN_RGB_1_L();	
  IO_BTN_RGB_2_L();	
  IO_BTN_RGB_3_L();	
  IO_BTN_RGB_4_L();	
  //----------------------------------------------------------
  IO_SW_1_INPUT();      //Version	V1.10	: (2)20241109 	: SW1&SW2 LED
  IO_SW_2_INPUT();
  //IO_SW_3_INPUT();
  //IO_SW_4_INPUT();
  //----------------------------------------------------------
  IO_SW_LED1_OUTPUT();  //Version	V1.10	: (2)20241109 	: SW1&SW2 LED
  IO_SW_LED2_OUTPUT();
  //IO_SW_LED3_OUTPUT();
  //IO_SW_LED4_OUTPUT();
  IO_SW_LED1_OFF();
  IO_SW_LED2_OFF();
  //IO_SW_LED3_OFF(); 
  //IO_SW_LED4_OFF();  
  //----------------------------------------------------------
  IO_USB_PWR_DT_INPUT();
  IO_LED_PWR_DT_INPUT();  
  //----------------------------------------------------------
  // Left
  IO_IR_RGB_L_OUTPUT();
  IO_IR_RGB_L_L();
  IO_IR_BTN_L4_INPUT();
  IO_IR_BTN_L5_INPUT();
  IO_IR_BTN_L6_INPUT();
  IO_IR_PWR_L_OUTPUT();
  IO_IR_PWR_L_L();
  //----------------------------------------------------------
  // Right
  IO_IR_RGB_R_OUTPUT();
  IO_IR_RGB_R_L();
  IO_IR_BTN_R4_INPUT();
  IO_IR_BTN_R5_INPUT();
  IO_IR_BTN_R6_INPUT();
  IO_IR_PWR_R_OUTPUT();
  IO_IR_PWR_R_L();
  //----------------------------------------------------------
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&




