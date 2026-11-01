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

SDK Version		: AT32F415_Firmware_Library_V2.0.6

AT32F415RBT7   LQFP64 10x10mm 128KFlash
AT32F415RCT7   LQFP64 10x10mm 256KFlash

Version	V1.00	: 
(1)20240326		: First Release Version(BaseOn DDR V1.06)
Version	V1.01	: 
(1)20240827		: KeyCode Mapping
Version	V1.02	: 
(1)20240828		: Support HID Mode
Version	V1.03	: 
(1)20240831		: Enalbe Sensitivity Save To Flash
Version	V1.04	: 
(1)20240910		: Switch Left/Right Board GPIO
Version	V1.05	: 
(1)20240915		: Switch RGB Issue
Version	V1.06	: 
(1)20240922		: Production ID and FW Version
Version	V1.07	: 
(1)20241015		: Switch HID Mode RGB
Version	V1.08	: 
(1)20241016		: Brightness For RGB  
Version	V1.09	: 
(1)20241020		: IR RGB Mode
Version	V1.10	: 
(1)20241026		: IO4 Mode
(2)20241109 	: SW1&SW2 LED
(3)20241207		: IO4 Mode Touch
Version	V1.11	: 
(1)20250101 	: HID Mode PID: 2001 -> 2022,VID: 0x1973 -> 0518
Version	V1.12	: 
(1)20250102 	: IR Btn Delay Issue
(2)20250102 	: Touch Chan 3 Sensitivity ADJ
Version	V1.13	: 
(1)20250102 	: IO4 Touch Delay Issue
Version	V1.14	: 
(1)20250102 	: Auto Switch RGB Brightness
Version	V1.15	: 
(1)20250222 	: Touch Crush Issue : Main.c App_CHSC6X_TP.c
Version	V1.16	: 
(1)20250224 	: RGB Mode
Version	V1.17	: 
(1)20250227 	: HID MODE RGB Display
Version	V1.18	: 
(1)20250228 	: HID MODE Test and Sevice Button
Version	V1.19	: 
(1)20250309 	: IO4 MODE RGB Game Ctrl Support
Version	V1.20	: 
(1)20250310 	: IO4 MODE Touch Latency
Version	V1.21	: 
(1)20250316 	: HID MODE KB Button 1&2  : main.c,App_User.c,USB_Desc.c,usb_class.h
(2)20250316 	: IO4 MODE Touch Latency
Version	V1.22	: 
(1)20250318 	: IO4 MODE Touch Latency
Version	V1.23	: 
(1)20250319 	: IO4 MODE Touch Latency
Version	V1.24	: 
(1)20250327 	: IO4 MODE Touch Issue
Version	V1.25	: 
(1)20250328 	: IO4 MODE Touch Issue
Version	V1.26	: 
(1)20250401 	: Touch Chan 3 Sensitivity ADJ 200

Version	V2.00	: 
(1)20250401 	: For New IR Sensor Base On V1.26
Version	V2.01	: 
(1)20250416 	: Default Keyboard Mode
Version	V2.02	: 
(1)20250701 	: IO Conflict For IR Sensor



******************************************************************************************/


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

#ifndef __APP_BSP_H__
#define __APP_BSP_H__


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

#define 	FW_VERSION_MAJOR       0x02
#define 	FW_VERSION_MINOR       02

#define 	HW_VERSION			       0x01

//============================================================================= 
//Version	V1.06	: (1)20240922		: Production ID and FW Version
#define 	PRODUCTION_ID_ADX	       0xA1   //ADX
#define 	PRODUCTION_ID_DDR	       0xA2   //DDR (Yuanpad)
#define 	PRODUCTION_ID_ZER	       0xA3   //ÖÐ¶þ(Laverita)
#define 	PRODUCTION_ID_YJI	       0xA4   //Òô»÷

#define 	PRODUCTION_ID			       PRODUCTION_ID_ZER
//============================================================================= 



//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "at32f415.h"

/**
  * this header include define support list:
  * 1. at-start-f415 v1.x board
  * if define AT_START_F415_V1, the header file support at-start-f415 v1.x board
  */

#if !defined (AT_START_F415_V1)
#error "please select first the board at-start device used in your application (in at32f415_board.h file)"
#endif

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


//******************************************************************************
//
//------------- Constant  define    -------------------
//
//******************************************************************************
//============================================================================= 
#define PMCU_SYNC_H					              0xA5                        //
#define PMCU_SYNC_L					              0x5A	                      //
#define SYNC_LEN                          3					                  //
#define HEAD_LEN                          5					                  //
#define DATA_LEN                          ADC_MAX_CHAN*2              //
#define CRC8_LEN                          1                           //
#define PAKE_LEN                          HEAD_LEN+DATA_LEN+CRC8_LEN  //
#define MAXS_LEN                          SYNC_LEN+PAKE_LEN           //

//============================================================================= 
////Version	V1.01	: (1)20240617		: App Param Config  
#define FLASH_PAGE_SIZE                   2048        // this parameter depends on the specific model of the chip 
#define FLASH_DATA_ADDR_START             0x0800F800  //
#define FLASH_DATA_ADDR_WMODE             FLASH_DATA_ADDR_START+FLASH_PAGE_SIZE*0
#define FLASH_DATA_ADDR_SENSI             FLASH_DATA_ADDR_START+FLASH_PAGE_SIZE*1
#define FLASH_DATA_ADDR_COLOR             FLASH_DATA_ADDR_START+FLASH_PAGE_SIZE*2
#define FLASH_DATA_ADDR_MAPP              FLASH_DATA_ADDR_START+FLASH_PAGE_SIZE*3

#define FLASH_BUFEER_SIZE                 128

//Version	V1.02	: (1)20240715		: Enter OTA By App
#define FLASH_APP_ADDR							      0x08004000  	//User App Start address
#define IAP_CMD_FINISH_FLAG_ADDR          (FLASH_APP_ADDR - 0x800) 
#define IAP_DOWNLOAD_FLAG         	      0x41544B38


//#define USER_FLASH_ADDRESS_START         (0x0800F800)

#define DEFAULT_SENSI  							      1000					//
#define DEFAULT_DTICK  							      3   					//



//============================================================================= 
//
#define IO_PWM_FREQ       				        16000
#define IO_PWM_MAX_DUTY   					      100
#define IO_PWM_OFF_DUTY 						      0
//============================================================================= 
////Version	V1.04	: (1)20240718		: Switch To RGBx25
#define USER_COLOR_R     						      0x01
#define USER_COLOR_G     						      0x02
#define USER_COLOR_B     						      0x04
#define USER_COLOR_Y     						      USER_COLOR_R|USER_COLOR_G
#define USER_COLOR_P     						      USER_COLOR_R|USER_COLOR_B
#define USER_COLOR_C     						      USER_COLOR_G|USER_COLOR_B
#define USER_COLOR_W     						      USER_COLOR_R|USER_COLOR_G|USER_COLOR_B
      

//******************************************************************************
//
//------------- I/O Defs    -------------------
//
//******************************************************************************

//============================================================================= 
#define	GPIO_CONFIG(PORT,PIN,PREIH,MODE,PULL)	{ gpio_init_type gpio_init_struct;                                      \
                                                crm_periph_clock_enable(PREIH, TRUE);                                 \
                                                gpio_default_para_init(&gpio_init_struct);                            \
                                                gpio_init_struct.gpio_drive_strength  = GPIO_DRIVE_STRENGTH_STRONGER; \
                                                gpio_init_struct.gpio_out_type        = GPIO_OUTPUT_PUSH_PULL;        \
                                                gpio_init_struct.gpio_mode            = MODE;                         \
                                                gpio_init_struct.gpio_pins            = PIN;                          \
                                                gpio_init_struct.gpio_pull            = PULL;                         \
                                                gpio_init(PORT, &gpio_init_struct);}  

#define	GPIO_IO_SET(PORT,PIN)							      PORT->scr  = PIN
#define	GPIO_IO_CLR(PORT,PIN)							      PORT->clr  = PIN
#define GPIO_IO_TOGGLE(PORT,PIN)					      PORT->odt ^= PIN
#define	GPIO_IO_RDV(PORT,PIN)							      gpio_input_data_bit_read(PORT, PIN)

//============================================================================= IO Define For SWD
#define	IO_JLINK_SWC_OUTPUT()							GPIO_CONFIG(GPIOA,	PIN_14,	CRM_GPIOA_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE)		// PA.14
#define	IO_JLINK_SWC_INPUT()							GPIO_CONFIG(GPIOA,	PIN_14,	CRM_GPIOA_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE)
#define IO_JLINK_SWC_HIGH() 							GPIO_IO_SET(GPIOA,	PIN_14)
#define IO_JLINK_SWC_LOW() 								GPIO_IO_CLR(GPIOA,	PIN_14)
#define IO_JLINK_SWC_READ() 							GPIO_IO_RDV(GPIOA, 	PIN_14)
																					
#define	IO_JLINK_SWD_OUTPUT()							GPIO_CONFIG(GPIOA,	PIN_13,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE)		// PA.13
#define	IO_JLINK_SWD_INPUT()							GPIO_CONFIG(GPIOA,	PIN_13,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_UP)		  //
#define IO_JLINK_SWD_HIGH() 							GPIO_IO_SET(GPIOA,	PIN_13)
#define IO_JLINK_SWD_LOW() 								GPIO_IO_CLR(GPIOA,	PIN_13)
#define IO_JLINK_SWD_READ() 							GPIO_IO_RDV(GPIOA, 	PIN_13)

#define IO_JLINK_FOR_GPIO()               crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE); gpio_pin_remap_config(SWJTAG_MUX_010, TRUE);
  
//============================================================================= IO Define For USB
#define USB_DP_PIN          							GPIO_PINS_12						                                          //PA.12		Request USB OTG/Device
#define USB_DM_PIN          							GPIO_PINS_11						                                          //PA.11		Request USB OTG/Device
#define OTG_FS_ID_PIN          						GPIO_PINS_10						                                          //PA.10		Request USB OTG
#define OTG_FS_ID_VBUS         						GPIO_PINS_9							                                          //PA.9		Request USB Device
#define USB_GPIO            							GPIOA
#define USB_GPIO_RCC_CLK    							CRM_GPIOA_PERIPH_CLOCK

//============================================================================= IO Define For UART1
//Uart1: BaudRate = 115200	:	Log  
#define IO_USART1_TXD_OUTPUT()						{ GPIO_CONFIG(GPIOB,	GPIO_PINS_6,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_MUX, GPIO_PULL_NONE)\
                                            crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);  \
                                            gpio_pin_remap_config(USART1_MUX, TRUE);}                                                 // PB.6 
#define	IO_USART1_RXD_INPUT()							GPIO_CONFIG(GPIOB,	GPIO_PINS_7,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_INPUT,GPIO_PULL_UP)		  // PB.7
#define IO_USART1_ENABLE()             		crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE)

#define AT32_PRINT_UART                		USART1
//============================================================================= IO Define For UART2
//Uart2: BaudRate = 115200	:	
#define IO_USART2_TXD_OUTPUT()						GPIO_CONFIG(GPIOA,	GPIO_PINS_2,	CRM_GPIOA_PERIPH_CLOCK,	GPIO_MODE_MUX, GPIO_PULL_NONE)	  // PA.2 
#define	IO_USART2_RXD_INPUT()							GPIO_CONFIG(GPIOA,	GPIO_PINS_3,	CRM_GPIOA_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_UP)		// PA.3
#define IO_USART2_ENABLE()             		crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE)

//============================================================================= IO Define For UART3
//Uart3: BaudRate = 115200	:	
#define IO_USART3_TXD_OUTPUT()						GPIO_CONFIG(GPIOB,	GPIO_PINS_10,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_MUX, GPIO_PULL_NONE)    // PB.10 
#define	IO_USART3_RXD_INPUT()							GPIO_CONFIG(GPIOB,	GPIO_PINS_11,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_UP)	  // PB.11
#define IO_USART3_ENABLE()             		crm_periph_clock_enable(CRM_USART3_PERIPH_CLOCK, TRUE)

//============================================================================= IO Define For UART4
//Uart4: BaudRate = 115200	:	
#define IO_UART4_TXD_OUTPUT()							GPIO_CONFIG(GPIOC,	GPIO_PINS_10,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_MUX, GPIO_PULL_NONE)		// PC.10 
#define	IO_UART4_RXD_INPUT()							GPIO_CONFIG(GPIOC,	GPIO_PINS_11,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_UP)	  // PC.11
#define IO_UART4_ENABLE()             		crm_periph_clock_enable(CRM_UART4_PERIPH_CLOCK, TRUE)

//============================================================================= IO Define For UART5
//Uart5: BaudRate = 115200 :  
#define IO_UART5_TXD_OUTPUT()							GPIO_CONFIG(GPIOC,	GPIO_PINS_12,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_MUX, GPIO_PULL_NONE)		// PC.12 
#define	IO_UART5_RXD_INPUT()							GPIO_CONFIG(GPIOD,	GPIO_PINS_2,	CRM_GPIOD_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_UP)		// PD.2
#define IO_UART5_ENABLE()             		crm_periph_clock_enable(CRM_UART5_PERIPH_CLOCK, TRUE)

//============================================================================= IO Define SDCARD
//sdio1_ck(pc4), sdio1_cmd(pc5), sdio1_d0(pc0), sdio1_d1(pc1), sdio1_d2(pc2), sdio1_d3(pc3)
//#define IO_SDCARD_PIN_REMAP()							{ crm_periph_clock_enable(CRM_SDIO1_PERIPH_CLOCK, TRUE)   \
//                                            crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);  \
//                                            gpio_pin_remap_config(SDIO1_GMUX_0100, TRUE);}
//#define IO_SDCARD_D0_PIN()							  GPIO_CONFIG(GPIOC,	GPIO_PINS_0,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_MUX, GPIO_PULL_NONE)		// PC.0 
//#define IO_SDCARD_D1_PIN()							  GPIO_CONFIG(GPIOC,	GPIO_PINS_1,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_MUX, GPIO_PULL_NONE)		// PC.1 
//#define IO_SDCARD_D2_PIN()							  GPIO_CONFIG(GPIOC,	GPIO_PINS_2,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_MUX, GPIO_PULL_NONE)		// PC.2 
//#define IO_SDCARD_D3_PIN()							  GPIO_CONFIG(GPIOC,	GPIO_PINS_3,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_MUX, GPIO_PULL_NONE)		// PC.3                                            
//#define IO_SDCARD_SCK_PIN()						    GPIO_CONFIG(GPIOC,	GPIO_PINS_4,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_MUX, GPIO_PULL_NONE)		// PC.4                                            
//#define IO_SDCARD_CMD_PIN()						    GPIO_CONFIG(GPIOC,	GPIO_PINS_5,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_MUX, GPIO_PULL_NONE)		// PC.5                                            
//#define IO_SDCARD_DET_PIN()						    GPIO_CONFIG(GPIOB,	GPIO_PINS_0,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_UP)		// PB.0                                            
//#define IO_SDCARD_DET_READ() 							GPIO_IO_RDV(GPIOB, 	GPIO_PINS_0)

//============================================================================= ADC Define
//#define ADC_DMA_ENABLE          	      1
#define ADC_MAX_CHAN							        16
#define ADC_NOISE_RANGE						        50

#define IO_ADC_CH00_INPUT()							  GPIO_CONFIG(GPIOA,	GPIO_PINS_0,	CRM_GPIOA_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PA.00
#define IO_ADC_CH01_INPUT()							  GPIO_CONFIG(GPIOA,	GPIO_PINS_1,	CRM_GPIOA_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PA.01
#define IO_ADC_CH02_INPUT()							  GPIO_CONFIG(GPIOA,	GPIO_PINS_2,	CRM_GPIOA_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PA.02
#define IO_ADC_CH03_INPUT()							  GPIO_CONFIG(GPIOA,	GPIO_PINS_3,	CRM_GPIOA_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PA.03
#define IO_ADC_CH04_INPUT()							  GPIO_CONFIG(GPIOA,	GPIO_PINS_4,	CRM_GPIOA_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PA.04
#define IO_ADC_CH05_INPUT()							  GPIO_CONFIG(GPIOA,	GPIO_PINS_5,	CRM_GPIOA_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PA.05
#define IO_ADC_CH06_INPUT()							  GPIO_CONFIG(GPIOA,	GPIO_PINS_6,	CRM_GPIOA_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PA.06
#define IO_ADC_CH07_INPUT()							  GPIO_CONFIG(GPIOA,	GPIO_PINS_7,	CRM_GPIOA_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PA.07

#define IO_ADC_CH08_INPUT()							  GPIO_CONFIG(GPIOB,	GPIO_PINS_0,	CRM_GPIOB_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PB.00
#define IO_ADC_CH09_INPUT()							  GPIO_CONFIG(GPIOB,	GPIO_PINS_1,	CRM_GPIOB_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PB.01
#define IO_ADC_CH10_INPUT()							  GPIO_CONFIG(GPIOC,	GPIO_PINS_0,	CRM_GPIOC_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PC.00
#define IO_ADC_CH11_INPUT()							  GPIO_CONFIG(GPIOC,	GPIO_PINS_1,	CRM_GPIOC_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PC.01
#define IO_ADC_CH12_INPUT()							  GPIO_CONFIG(GPIOC,	GPIO_PINS_2,	CRM_GPIOC_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PC.02
#define IO_ADC_CH13_INPUT()							  GPIO_CONFIG(GPIOC,	GPIO_PINS_3,	CRM_GPIOC_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PC.03
#define IO_ADC_CH14_INPUT()							  GPIO_CONFIG(GPIOC,	GPIO_PINS_4,	CRM_GPIOC_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PC.04
#define IO_ADC_CH15_INPUT()							  GPIO_CONFIG(GPIOC,	GPIO_PINS_5,	CRM_GPIOC_PERIPH_CLOCK,  GPIO_MODE_ANALOG, GPIO_PULL_NONE)	  // PC.05

#define IO_CC1_AD_INPUT()							    IO_ADC_CH12_INPUT()   // PC.02
#define IO_CC2_AD_INPUT()							    IO_ADC_CH13_INPUT()	  // PC.03
#define IO_CC1_CHAN     							    ADC_CHANNEL_12        
#define IO_CC2_CHAN							          ADC_CHANNEL_13

//============================================================================= IO Define For I2C (Touch Screen Interface)
//
#define	SW_I2C_SCL_OUTPUT()								GPIO_CONFIG(GPIOB,	GPIO_PINS_8,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_OUTPUT,GPIO_PULL_NONE)		  // PB.8
#define SW_I2C_SCL_HIGH() 								GPIO_IO_SET(GPIOB,	GPIO_PINS_8)
#define SW_I2C_SCL_LOW() 									GPIO_IO_CLR(GPIOB,	GPIO_PINS_8)

#define	SW_I2C_SDA_OUTPUT()								GPIO_CONFIG(GPIOB,	GPIO_PINS_9,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_OUTPUT,GPIO_PULL_NONE)		  // PB.9
#define	SW_I2C_SDA_INPUT()								GPIO_CONFIG(GPIOB,	GPIO_PINS_9,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_INPUT,GPIO_PULL_UP)		  		//
#define SW_I2C_SDA_HIGH() 								GPIO_IO_SET(GPIOB,	GPIO_PINS_9)
#define SW_I2C_SDA_LOW() 									GPIO_IO_CLR(GPIOB,	GPIO_PINS_9)
#define SW_I2C_SDA_READ() 								GPIO_IO_RDV(GPIOB, 	GPIO_PINS_9)

#define	IO_TP_INT_INPUT()								  GPIO_CONFIG(GPIOC,	GPIO_PINS_14,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_NONE)		  // PC.14		
#define IO_TP_INT_READ() 								  GPIO_IO_RDV(GPIOC, 	GPIO_PINS_14)

#define	IO_TP_RST_OUTPUT()								GPIO_CONFIG(GPIOC,	GPIO_PINS_15,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE)		  // PC.15
#define IO_TP_RST_HIGH() 									GPIO_IO_SET(GPIOC,	GPIO_PINS_15)
#define IO_TP_RST_LOW() 									GPIO_IO_CLR(GPIOC,	GPIO_PINS_15)

//============================================================================= IO Define RGB
//
#define	IO_BTN_RGB_1_OUTPUT()							GPIO_CONFIG(GPIOC,	GPIO_PINS_0,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_OUTPUT,GPIO_PULL_NONE)		  // PC.0
#define IO_BTN_RGB_1_H() 								  GPIO_IO_SET(GPIOC,	GPIO_PINS_0)
#define IO_BTN_RGB_1_L() 									GPIO_IO_CLR(GPIOC,	GPIO_PINS_0)

#define	IO_BTN_RGB_2_OUTPUT()							GPIO_CONFIG(GPIOA,	GPIO_PINS_2,	CRM_GPIOA_PERIPH_CLOCK,	GPIO_MODE_OUTPUT,GPIO_PULL_NONE)		  // PA.2
#define IO_BTN_RGB_2_H() 								  GPIO_IO_SET(GPIOA,	GPIO_PINS_2)
#define IO_BTN_RGB_2_L() 									GPIO_IO_CLR(GPIOA,	GPIO_PINS_2)

#define	IO_BTN_RGB_3_OUTPUT()					    GPIO_CONFIG(GPIOB,	GPIO_PINS_4,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_OUTPUT,GPIO_PULL_NONE)		  // PB.4
#define IO_BTN_RGB_3_H() 								  GPIO_IO_SET(GPIOB,	GPIO_PINS_4)
#define IO_BTN_RGB_3_L() 									GPIO_IO_CLR(GPIOB,	GPIO_PINS_4)

#define IO_BTN_RGB_4_OUTPUT()						  GPIO_CONFIG(GPIOD,	GPIO_PINS_2,	CRM_GPIOD_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE)		  // PD.2
#define IO_BTN_RGB_4_H() 								  GPIO_IO_SET(GPIOD,	GPIO_PINS_2)
#define IO_BTN_RGB_4_L() 							    GPIO_IO_CLR(GPIOD,	GPIO_PINS_2)

//============================================================================= IO Define SW
//
#define	IO_SW_1_INPUT()								    GPIO_CONFIG(GPIOB,	GPIO_PINS_12,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_DOWN)		// PB.12
#define	IO_SW_2_INPUT()								    GPIO_CONFIG(GPIOB,	GPIO_PINS_13,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_DOWN)		// PB.13
//#define	IO_SW_3_INPUT()								  GPIO_CONFIG(GPIOB,	GPIO_PINS_14,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_DOWN)		// PB.14    //Version	V1.10	: (2)20241109 	: SW1&SW2 LED
//#define	IO_SW_4_INPUT()								  GPIO_CONFIG(GPIOB,	GPIO_PINS_15,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_DOWN)		// PB.15
#define IO_SW_1_READ() 								    GPIO_IO_RDV(GPIOB, 	GPIO_PINS_12)
#define IO_SW_2_READ() 								    GPIO_IO_RDV(GPIOB, 	GPIO_PINS_13)
//#define IO_SW_3_READ() 								  GPIO_IO_RDV(GPIOB, 	GPIO_PINS_14)
//#define IO_SW_4_READ() 								  GPIO_IO_RDV(GPIOB, 	GPIO_PINS_15)

//============================================================================= IO Define SW LED
////Version	V1.10	: (2)20241109 	: SW1&SW2 LED
#define	IO_SW_LED1_OUTPUT()							  GPIO_CONFIG(GPIOB,	GPIO_PINS_15,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE)	// PB.15// PC.12
#define	IO_SW_LED2_OUTPUT()							  GPIO_CONFIG(GPIOB,	GPIO_PINS_14,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE)	// PB.14// PC.11
//#define	IO_SW_LED3_OUTPUT()							GPIO_CONFIG(GPIOC,	GPIO_PINS_7,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE)	// PC.7
//#define	IO_SW_LED4_OUTPUT()							GPIO_CONFIG(GPIOC,	GPIO_PINS_6,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE)	// PC.6
#define IO_SW_LED1_ON() 								  GPIO_IO_SET(GPIOB,	GPIO_PINS_15)
#define IO_SW_LED2_ON() 								  GPIO_IO_SET(GPIOB,	GPIO_PINS_14)
//#define IO_SW_LED3_ON() 								GPIO_IO_CLR(GPIOC,	GPIO_PINS_7)
//#define IO_SW_LED4_ON() 								GPIO_IO_CLR(GPIOC,	GPIO_PINS_6)
#define IO_SW_LED1_OFF() 							    GPIO_IO_CLR(GPIOB,	GPIO_PINS_15)
#define IO_SW_LED2_OFF() 							    GPIO_IO_CLR(GPIOB,	GPIO_PINS_14)
//#define IO_SW_LED3_OFF() 							  GPIO_IO_SET(GPIOC,	GPIO_PINS_7)
//#define IO_SW_LED4_OFF() 							  GPIO_IO_SET(GPIOC,	GPIO_PINS_6)

//============================================================================= IO Define PWR DT
//
#define	IO_USB_PWR_DT_INPUT()						  GPIO_CONFIG(GPIOA,	GPIO_PINS_1,	CRM_GPIOA_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_UP)	  // PA.1
#define IO_USB_PWR_READ() 							  GPIO_IO_RDV(GPIOA,	GPIO_PINS_1)
#define	IO_LED_PWR_DT_INPUT()						  GPIO_CONFIG(GPIOA,	GPIO_PINS_0,	CRM_GPIOA_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_UP)	  // PA.0
#define IO_LED_PWR_READ() 							  GPIO_IO_RDV(GPIOA,	GPIO_PINS_0)


//============================================================================= IO Define IR Board
//
#define APP_MAX_IR_CHAN   				        6


// Left
#define IO_IR_RGB_L_OUTPUT()						  GPIO_CONFIG(GPIOA,	GPIO_PINS_7,	CRM_GPIOA_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE) // PA.7
#define IO_IR_RGB_L_H() 								  GPIO_IO_SET(GPIOA,	GPIO_PINS_7)
#define IO_IR_RGB_L_L() 							    GPIO_IO_CLR(GPIOA,	GPIO_PINS_7)

#define	IO_IR_BTN_L4_INPUT()						  GPIO_CONFIG(GPIOB,	GPIO_PINS_0,	CRM_GPIOB_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_DOWN) // PB.0 //Version	V1.04	: (1)20240910		: Switch Left/Right Board GPIO
#define IO_IR_BTN_L4_READ() 						  GPIO_IO_RDV(GPIOB, 	GPIO_PINS_0)
#define	IO_IR_BTN_L5_INPUT()						  GPIO_CONFIG(GPIOC,	GPIO_PINS_5,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_DOWN) // PC.5
#define IO_IR_BTN_L5_READ() 						  GPIO_IO_RDV(GPIOC, 	GPIO_PINS_5)
#define	IO_IR_BTN_L6_INPUT()						  GPIO_CONFIG(GPIOC,	GPIO_PINS_4,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_DOWN) // PA.6 //Version	V1.04	: (1)20240910		: Switch Left/Right Board GPIO
#define IO_IR_BTN_L6_READ() 						  GPIO_IO_RDV(GPIOC, 	GPIO_PINS_4)

#define IO_IR_PWR_L_OUTPUT()						  GPIO_CONFIG(GPIOA,	GPIO_PINS_6,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE) // PA.6 //Version	V1.04	: (1)20240910		: Switch Left/Right Board GPIO
#define IO_IR_PWR_L_H() 								  GPIO_IO_SET(GPIOA,	GPIO_PINS_6)
#define IO_IR_PWR_L_L() 							    GPIO_IO_CLR(GPIOA,	GPIO_PINS_6)


// Right
#define IO_IR_RGB_R_OUTPUT()						  GPIO_CONFIG(GPIOA,	GPIO_PINS_4,	CRM_GPIOA_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE) // PA.4 //Version	V1.04	: (1)20240910		: Switch Left/Right Board GPIO
#define IO_IR_RGB_R_H() 								  GPIO_IO_SET(GPIOA,	GPIO_PINS_4)
#define IO_IR_RGB_R_L() 							    GPIO_IO_CLR(GPIOA,	GPIO_PINS_4)

#define	IO_IR_BTN_R4_INPUT()						  GPIO_CONFIG(GPIOF,	GPIO_PINS_5,	CRM_GPIOF_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_DOWN) // PF.5 //Version	V1.04	: (1)20240910		: Switch Left/Right Board GPIO
#define IO_IR_BTN_R4_READ() 						  GPIO_IO_RDV(GPIOF, 	GPIO_PINS_5)
#define	IO_IR_BTN_R5_INPUT()						  GPIO_CONFIG(GPIOF,	GPIO_PINS_4,	CRM_GPIOF_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_DOWN) // PF.4 //Version	V1.04	: (1)20240910		: Switch Left/Right Board GPIO
#define IO_IR_BTN_R5_READ() 						  GPIO_IO_RDV(GPIOF, 	GPIO_PINS_4)
#define	IO_IR_BTN_R6_INPUT()						  GPIO_CONFIG(GPIOA,	GPIO_PINS_3,	CRM_GPIOA_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_DOWN) // PA.3 //Version	V1.04	: (1)20240910		: Switch Left/Right Board GPIO
#define IO_IR_BTN_R6_READ() 						  GPIO_IO_RDV(GPIOA, 	GPIO_PINS_3)

#define IO_IR_PWR_R_OUTPUT()						  GPIO_CONFIG(GPIOA,	GPIO_PINS_5,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE) // PA.5 //Version	V1.04	: (1)20240910		: Switch Left/Right Board GPIO
#define IO_IR_PWR_R_H() 								  GPIO_IO_SET(GPIOA,	GPIO_PINS_5)
#define IO_IR_PWR_R_L() 							    GPIO_IO_CLR(GPIOA,	GPIO_PINS_5)


//Version	V1.24	: (1)20250327 	: IO4 MODE Touch Issue
#define	IO_OPT_1_INPUT()						  GPIO_CONFIG(GPIOC,	GPIO_PINS_1,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_OUTPUT, GPIO_PULL_NONE); GPIO_IO_CLR(GPIOC,	GPIO_PINS_1); //Version	V2.02	: (1)20250701 	: IO Conflict For IR Sensor //GPIO_CONFIG(GPIOC,	GPIO_PINS_1,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_UP) // PC.1 
#define IO_OPT_1_READ() 						  GPIO_IO_RDV(GPIOC, 	GPIO_PINS_1)
#define	IO_OPT_2_INPUT()						  GPIO_CONFIG(GPIOC,	GPIO_PINS_13,	CRM_GPIOC_PERIPH_CLOCK,	GPIO_MODE_INPUT,  GPIO_PULL_UP) // PC.13 
#define IO_OPT_2_READ() 						  GPIO_IO_RDV(GPIOC, 	GPIO_PINS_13)



//******************************************************************************
//
//------------- Type Defs    -------------------
//
//******************************************************************************

typedef struct 
{
	unsigned char		Chan;
	unsigned short	Buf[ADC_MAX_CHAN];
	unsigned short	Reg[ADC_MAX_CHAN];
	unsigned char		Cnt[ADC_MAX_CHAN];

	unsigned short	VBAT;
	unsigned short	ADC_Val1;
	unsigned short	ADC_Val2;
  unsigned short	ADC_Val3;
	unsigned short	ADC_Val4;
	unsigned short	NTC;
}AppADC_t;


//******************************************************************************
//
//-------------  Variable  define   --------------------
//
//******************************************************************************




//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//--------------------------------------------------------------- 
void App_BSP_Log_Uart_Init(uint32_t baudrate);

//--------------------------------------------------------------- 
void App_BSP_Delay_Init(void);
void App_BSP_Delay_US(uint32_t nus);
void App_BSP_Delay_MS(uint16_t nms);
void App_BSP_Delay_SEC(uint16_t sec);
void usb_delay_ms(uint32_t ms);
void usb_delay_us(uint32_t us);
unsigned char	 UPDATE_CRC (unsigned char *Buffer,unsigned char Len);
unsigned short UPDATE_CRC_16(unsigned char *Buffer,unsigned char Len);
void App_BSP_LogBuf(const unsigned char *str,unsigned char *Buffer,unsigned short Len);
void App_BSP16_LogBuf(const unsigned char *str,unsigned short *Buffer,unsigned short Len);

//---------------------------------------------------------------
//MCU Base TimeTick 1mS
void Timer1_Config(void);
void Timer3_Config(void);
void TMR_1mS_Interrupt_CB(void);

//---------------------------------------------------------------
void Timer1_PWM_Config(void);	  //TMR1 Config For PWM1 Channel 1~4:PA8,PA9,PA10,PA11
void Timer3_PWM_Config(void);   //TMR3 Config For PWM1 Channel 1~4:PA6,PA7,PB0,PB1
void Timer5_PWM_Config(void);		//TMR5 Config For PWM1 Channel 1~4:PA0,PA1,PA2,PA3
void PWM_Duty_Update(unsigned char Chan,unsigned short Duty);

//---------------------------------------------------------------
//Configure PA0 as EXINT0 interrupt mode
void EXTI_INT0_Config(void);				
void EXTI_INT0_CB(void);

//---------------------------------------------------------------
void MCU_DMA_Config_For_ADC(void);
void MCU_ADC_Config_With_DMA(void);
void MCU_ADC_Config(void);
uint16_t GetADCValue(uint16_t Channel);

void CHK_ADC_SCAN(AppADC_t *AppADC);
extern void App_RGB_CC_PowerUpdate(unsigned int Value);	  //Version	V1.08	: (1)20241016		: Brightness For RGB  
//---------------------------------------------------------------
//void HW_SPI_1_Slave_Config(void);
//void HW_SPI_1_RX_CB(void);
//void HW_SPI_1_TX_CB(void);

//---------------------------------------------------------------
void MCU_GPIO_Config(void);




//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


#ifdef __cplusplus
}
#endif

#endif

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

