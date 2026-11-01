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

#ifndef __APP_USER_H__
#define __APP_USER_H__	

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

#include 		"App_BSP.h"

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//******************************************************************************
//
//------------- Constant  define    -------------------
//
//******************************************************************************

//-------------------------------------------------------------------
#define 	KEY_CODE_NULL								    0x0000					//
#define 	KEY_CODE_01									    0x0001					//
#define 	KEY_CODE_02									    0x0002					//
#define 	KEY_CODE_03									    0x0004					//
#define 	KEY_CODE_04									    0x0008					//
#define 	KEY_CODE_05									    0x0010					//
#define 	KEY_CODE_06									    0x0020					//
#define 	KEY_CODE_07									    0x0040					//
#define 	KEY_CODE_08									    0x0080					//
#define 	KEY_CODE_09									    0x0100					//
#define 	KEY_CODE_10									    0x0200					//
#define 	KEY_CODE_11									    0x0400					//
#define 	KEY_CODE_12									    0x0800					//
#define 	KEY_CODE_13									    0x1000					//
#define 	KEY_CODE_14									    0x2000					//
#define 	KEY_CODE_15									    0x4000					//
#define 	KEY_CODE_16									    0x8000					//

//------------------------------------------------------------------------------






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
typedef struct 
{
	//------------------------------------------------------------
	unsigned char 	T1MS_FG	:1;	
  unsigned char 	TREPT_FG	:1;   //Version	V1.10	: (1)20241026		: IO4 Mode
	unsigned char 	T10MS_FG	:1;
	unsigned char 	T100MS_FG	:1;
	unsigned char 	T1Sec_FG	:1;
  unsigned char 	FlashFlag	:1;
  unsigned char 	TempFlag	:1;
	unsigned char 	TSCAN_FG	:1;
  unsigned char 	N10MS_FG	:1;   //Version	V1.05	: (1)20240719		: Mult Key Issue  
  unsigned char 	MDRAW_FG	:1;   //Version	V1.06	: (1)20240720		: RAW Date Fort App Seni Config 
  unsigned char 	TPTick_FG	:1;   //Version	V1.23	: (1)20250319 	: IO4 MODE Touch Latency
	unsigned char		Tick1MS;
  unsigned char		TickREPT;		    //Version	V1.10	: (1)20241026		: IO4 Mode
  unsigned char 	TPTickCnt;      //Version	V1.23	: (1)20250319 	: IO4 MODE Touch Latency
	unsigned char		Tick10MS;
	unsigned char		Tick100MS;
	unsigned char		Tick1Sec;
	unsigned char		TickScan;
  unsigned int		FlashSpeed;
	unsigned int		SysTimeTick;
	//------------------------------------------------------------
	unsigned char		BtnDownFG	:1;
	unsigned char		BtnHoldFG	:1;
	unsigned char		BtnHRelFG	:1;
	unsigned char		BtnNewsFG	:1;
	unsigned char		BtnADJFG	:1;
  unsigned char		ADCNewsFG	:1;
  unsigned char		KeyNewsFG	:1;
  unsigned char		TPKeyRDFG	:1; //Version	V1.10	: (3)20241207		: IO4 Mode Touch 
	unsigned short	IO_BUF;
	unsigned short	IO_REG;
  unsigned short	IR_VAL;
  unsigned short	IR_BAK;
	unsigned short	BTN_DN;
	unsigned short	BTN_UP;
//  unsigned short	DebStep;      //Version	V1.10	: (3)20241207		: IO4 Mode Touch 
//  unsigned int  	DebKey;
//  unsigned char  	DebState;
  
  
	unsigned int  	Value;
  unsigned int  	ValBK;
  //------------------------------------------------------------
  unsigned int  	TPValue;
  unsigned char 	TPRAW[32];
  //------------------------------------------------------------
  unsigned char		Chan;
	unsigned short	Buf[ADC_MAX_CHAN];
	unsigned short	Reg[ADC_MAX_CHAN];
  unsigned short	Max[ADC_MAX_CHAN];        //Version	V1.06	: (1)20240720		: RAW Date Fort App Seni Config 
	unsigned char		Cnt[ADC_MAX_CHAN];
  unsigned short	Tck[ADC_MAX_CHAN];        //同一通道的4个传感器消抖计时(单位:mS) //Version	V1.01	: (1)20240617		: App Param Config
  //------------------------------------------------------------
//  unsigned short	Sensi[ADC_MAX_CHAN];      //灵敏度
//  unsigned short	Color[ADC_MAX_CHAN];      //按键背光灯颜色                       //Version	V1.01	: (1)20240617		: App Param Config
//  unsigned short	DTick[ADC_MAX_CHAN];      //同一通道的4个传感器消抖时间(单位:mS) //Version	V1.01	: (1)20240617		: App Param Config
  unsigned char 	Mapping[32+6];
	//------------------------------------------------------------
  unsigned short	KeyCode;
  unsigned short	KeyCodeBK;
  //------------------------------------------------------------
  
}AppUser_t;







//******************************************************************************
//
//-------------  Variable  define   --------------------
//
//******************************************************************************
extern AppUser_t					AppUser;
extern uint8_t            IO4TouchState; 

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//#ifdef  SOFT_CONIFG_CDC
extern void App_USB_CDC_SendPacket(unsigned char *PayLoadRaw,unsigned short Length);
extern void App_USB_IO4_TouchTxd(unsigned int TPKeyCode,unsigned char *TouchRaw,unsigned short Length);        //Version	V1.10	: (3)20241207		: IO4 Mode Touch 
//#endif

//#ifdef  SOFT_CONIFG_HID
//extern void App_USB_HID_SendPacket(unsigned char *PayLoadRaw,unsigned short Length);
//extern void App_USB_HID_Report(unsigned short KeyCode);
//#endif
//#ifdef  SOFT_CONIFG_KBS
//extern void App_USB_KBS_SendPacket(unsigned char *PayLoadRaw,unsigned short Length);
//extern void App_USB_KBS_Report(unsigned short KeyCode);
//#endif

//--------------------------------------------------------------- 

extern void App_User_TimeTick_CB(void);

//--------------------------------------------------------------- 
void App_Get_Flash_Cofnig(unsigned int Addr, unsigned char *SensiBuffer, unsigned char *Default, unsigned char Size); //Version	V1.01	: (1)20240617		: App Param Config
void App_Set_Flash_Cofnig(unsigned int Addr, unsigned char *SensiBuffer, unsigned char Size);                          //Version	V1.01	: (1)20240617		: App Param Config  

//--------------------------------------------------------------- 
void App_User_Get_WorkMode(void);
void App_User_Set_WorkMode(void);
void App_MAPPING_Command_Report(unsigned char *CMDBuffer,unsigned char CMDLen,unsigned char *RSPBuffer,unsigned char RSPLen);

void App_User_RAW_MODE_Update(unsigned char Index, unsigned short *CMDBuffer,unsigned short CMDLen);//Version	V1.06	: (1)20240720		: RAW Date Fort App Seni Config 
void App_User_Res_Update(usart_type* USARTx, unsigned char *CMDBuffer,unsigned short CMDLen);
void App_User_CMD_Report(unsigned char *CMDBuffer,unsigned short CMDLen);  
void App_User_USB_CMD_Update(void);

//--------------------------------------------------------------- 
void App_User_SetKeyStatus(unsigned char Index,unsigned char State);
unsigned char App_User_GetKeyStatus(unsigned char Index);

//--------------------------------------------------------------- 
extern void App_User_Btn_Scan(void);
extern void App_User_Sta_Update(void);


//---------------------------------------------------------------
extern void App_User_Init(void);
extern void App_User_Loop(void);

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

#endif

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
