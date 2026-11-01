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

#include 		<stdio.h>
#include 		"flash.h"
#include    "usb_class.h"
#include 		"App_SW_I2C.h"
#include 		"App_UART.h"
#include 		"App_User.h"
#include 		"App_RGB.h"
#include 		"App_BSP.h"
#include    "Chip2813.h"
#include 		"App_CHSC6X_TP.h"

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

#define APP_USER_LOG(x)           		printf x
#define APP_USER_DBG(x)           		//printf x

//******************************************************************************
//
//------------- Constant  define    -------------------
//
//******************************************************************************

//******************************************************************************
//
//-------------  I/O  define   --------------------
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

AppUser_t					AppUser;
AppADC_t					AppADC;

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_User_TimeTick_CB(void)
{
  unsigned char i;
  //----------------------------------------------------------
	AppUser.SysTimeTick++;
	AppUser.T1MS_FG = 1;
  //----------------------------------------------------------
  for(i=0;i<ADC_MAX_CHAN;i++) //Version	V1.01	: (1)20240617		: App Param Config 
  {
    if(AppUser.Tck[i]>0)AppUser.Tck[i]--;
  }
  //----------------------------------------------------------
	AppUser.TickREPT++;				//Version	V1.10	: (1)20241026		: IO4 Mode
	if(AppUser.TickREPT>=2)
	{
		AppUser.TickREPT 	= 0;
		AppUser.TREPT_FG	=	1;
	}
	//----------------------------------------------------------
	AppUser.Tick10MS++;
	if(AppUser.Tick10MS>=10)
	{
		AppUser.Tick10MS 	= 0;
		AppUser.T10MS_FG	=	1;
    AppUser.N10MS_FG	=	1;  //Version	V1.05	: (1)20240719		: Mult Key Issue
	}
  //----------------------------------------------------------
  //Version	V1.23	: (1)20250319 	: IO4 MODE Touch Latency
  AppUser.TPTickCnt++;
	if(AppUser.TPTickCnt>=15)
	{
		AppUser.TPTickCnt = 0;
		AppUser.TPTick_FG	=	1;
	}
  //----------------------------------------------------------
	AppUser.Tick100MS++;
	if(AppUser.Tick100MS>=100)
	{
		AppUser.Tick100MS	= 0;
		AppUser.T100MS_FG	=	1;
		
		AppUser.Tick1Sec++;
		if(AppUser.Tick1Sec>=10)
		{
			AppUser.Tick1Sec 	= 0;
			AppUser.T1Sec_FG	=	1;
		}
	}
  //----------------------------------------------------------
  AppUser.FlashSpeed++;
  if(AppUser.FlashSpeed>=400)
  {
    AppUser.FlashSpeed = 0;
    AppUser.FlashFlag  = 1;
  }
  //----------------------------------------------------------
}


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


unsigned short 	const Default_Sensi_Tab[ADC_MAX_CHAN]=		
{
  DEFAULT_SENSI, // 0 : KeyPad-1A
  DEFAULT_SENSI, // 1 : KeyPad-1B
  DEFAULT_SENSI, // 2 : KeyPad-1C
  DEFAULT_SENSI, // 3 : KeyPad-1D
  
  DEFAULT_SENSI, // 4 : KeyPad-2A
  DEFAULT_SENSI, // 5 : KeyPad-2B
  DEFAULT_SENSI, // 6 : KeyPad-2C
  DEFAULT_SENSI, // 7 : KeyPad-2D
  
  DEFAULT_SENSI, // 8 : KeyPad-3A
  DEFAULT_SENSI, // 9 : KeyPad-3B
  DEFAULT_SENSI, //10 : KeyPad-3C
  DEFAULT_SENSI, //11 : KeyPad-3D
  
  DEFAULT_SENSI, //12 : KeyPad-4A
  DEFAULT_SENSI, //13 : KeyPad-4B
  DEFAULT_SENSI, //14 : KeyPad-4C
  DEFAULT_SENSI, //15 : KeyPad-4D
};

unsigned short 	const Default_Color_Tab[ADC_MAX_CHAN]=		
{
  USER_COLOR_W, // 0 : KeyPad-1A
  USER_COLOR_W, // 1 : KeyPad-1B
  USER_COLOR_W, // 2 : KeyPad-1C
  USER_COLOR_W, // 3 : KeyPad-1D
  
  USER_COLOR_W, // 4 : KeyPad-2A
  USER_COLOR_W, // 5 : KeyPad-2B
  USER_COLOR_W, // 6 : KeyPad-2C
  USER_COLOR_W, // 7 : KeyPad-2D
  
  USER_COLOR_W, // 8 : KeyPad-3A
  USER_COLOR_W, // 9 : KeyPad-3B
  USER_COLOR_W, //10 : KeyPad-3C
  USER_COLOR_W, //11 : KeyPad-3D
  
  USER_COLOR_W, //12 : KeyPad-4A
  USER_COLOR_W, //13 : KeyPad-4B
  USER_COLOR_W, //14 : KeyPad-4C
  USER_COLOR_W, //15 : KeyPad-4D
};


unsigned short 	const Default_DTick_Tab[ADC_MAX_CHAN]=		
{
  DEFAULT_DTICK, // 0 : KeyPad-1A
  DEFAULT_DTICK, // 1 : KeyPad-1B
  DEFAULT_DTICK, // 2 : KeyPad-1C
  DEFAULT_DTICK, // 3 : KeyPad-1D
  
  DEFAULT_DTICK, // 4 : KeyPad-2A
  DEFAULT_DTICK, // 5 : KeyPad-2B
  DEFAULT_DTICK, // 6 : KeyPad-2C
  DEFAULT_DTICK, // 7 : KeyPad-2D
  
  DEFAULT_DTICK, // 8 : KeyPad-3A
  DEFAULT_DTICK, // 9 : KeyPad-3B
  DEFAULT_DTICK, //10 : KeyPad-3C
  DEFAULT_DTICK, //11 : KeyPad-3D
  
  DEFAULT_DTICK, //12 : KeyPad-4A
  DEFAULT_DTICK, //13 : KeyPad-4B
  DEFAULT_DTICK, //14 : KeyPad-4C
  DEFAULT_DTICK, //15 : KeyPad-4D
};

unsigned char Default_KeyMapping[APP_MAX_TP_CHAN+APP_MAX_IR_CHAN] = 
{
//  '1','q','2','w',  '3','e','4','r', '5','t','6','y',  '7','u','8','i',
//  'a','z','s','x',  'd','c','f','v', 'g','b','h','n',  'j','m','k',',',

//  '/','.',',',';',  '[',']',
  
  
//  0x08,// BS Backspace
//  0x09,// 0x09: TAB	Tab
//  0x0d,// 0x0D: CR	Enter 
//  0x13,// 0x13: DC3 Pause   
//  0x14,// 0x14: DC4 CapLock     
//  0x1b,// 0x1B: ESC Esc 

//  0x20, // 0x20: ' '
//  0x21, // 0x21: PageUp
//  0x22, // 0x22: PageDown
//  0x23, // 0x23: End
//  0x24, // 0x24: Home
//  0x25, // 0x25: Left  Arrow
//  0x26, // 0x26: Up    Arrow
//  0x27, // 0x27: Right Arrow
//  0x28, // 0x28: Down  Arrow
//  0x2c, // 0x2C: PrtSc
//  0x2d, // 0x2D: Insert
//  0x2e, // 0x2E: Delete

//  0x5b,// 0x5B: Win-L ??
//  0x5c,// 0x5C: Win-R ??
//  0x5d,// 0x5D: Paste
  
//  0x60,// 0x60: NumberKeyPad 0
//  0x61,// 0x61: NumberKeyPad 1
//  0x62,// 0x62: NumberKeyPad 2
//  0x63,// 0x63: NumberKeyPad 3
//  0x64,// 0x64: NumberKeyPad 4
//  0x65,// 0x65: NumberKeyPad 5
//  0x66,// 0x66: NumberKeyPad 6
//  0x67,// 0x67: NumberKeyPad 7
//  0x68,// 0x68: NumberKeyPad 8
//  0x69,// 0x69: NumberKeyPad 9
//  0x6a,// 0x6A: NumberKeyPad *
//  0x6b,// 0x6B: NumberKeyPad +
//  0x6d,// 0x6D: NumberKeyPad -
//  0x6e,// 0x6E: NumberKeyPad .
//  0x6f,// 0x6F: NumberKeyPad /
//  
//  0x70,// 0x70: F1
//  0x71,// 0x71: F2
//  0x72,// 0x72: F3
//  0x73,// 0x73: F4
//  0x74,// 0x74: F5
//  0x75,// 0x75: F6
//  0x76,// 0x76: F7
//  0x77,// 0x77: F8
//  0x78,// 0x78: F9
//  0x79,// 0x79: F10
//  0x7A,// 0x7A: F11
//  0x7B,// 0x7B: F12

//  0x90,// 0x90:NumLock
//  0x91,// 0x91:ScrLock  
//  
//  0xA0,// 0xA0:Shift-L  ??
//  0xA1,// 0xA1:Shift-R  ??
//  0xA2,// 0xA2:Ctrl-L   ??
//  0xA3,// 0xA3:Ctrl-R   ??
//  0xA4,// 0xA4:Alt-L    ??
//  0xA5,// 0xA5:Alt-R    ??
//  
//  0xBA,// 0xBA: ;
//  0xBB,// 0xBB: =
//  0xBC,// 0xBC: ,
//  0xBD,// 0xBD: -
//  0xBE,// 0xBE: .
//  0xBF,// 0xBF: /  
//  
//  0xDB,// 0xDB: [
//  0xDC,// 0xDC: 
//  0xDD,// 0xDD: ]
//  0xDE,// 0xDE: '
  
  
  0x31,0x51,0x32,0x57,0x33,0x45,0x34,0x52,0x35,0x54,0x36,0x59,0x37,0x55,0x38,0x49,
  0x41,0x5a,0x53,0x58,0x44,0x43,0x46,0x56,0x47,0x42,0x48,0x4e,0x4a,0x4d,0x4b,0xbc,

  0xbf,0xbe,0xbc,0xba,0xdb,0xdd
};
/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
void App_Get_Flash_Cofnig(unsigned int Addr, unsigned char *SensiBuffer, unsigned char *Default, unsigned char Size)
{
	//----------------------------------------------------------
	uint32_t i;
	uint16_t CS=0;
  uint16_t Flash_Buffer[FLASH_BUFEER_SIZE];
	//----------------------------------------------------------
  // read data from flash 
  flash_read(Addr, Flash_Buffer, FLASH_BUFEER_SIZE);
	printf("FlashR:%08X,%d \r\n",Addr,FLASH_BUFEER_SIZE);
	for(i=0;i<(1+Size+1);i++)
	{
		printf("%04X",Flash_Buffer[i]);
		if((i!=0)&&(((i+1)%8)==0))printf("\r\n");
	}
	printf("\r\n");
	//----------------------------------------------------------
	CS = UPDATE_CRC_16((unsigned char *)Flash_Buffer,(Size+1)*2);
	if(Flash_Buffer[0]==0xA55A && Flash_Buffer[Size+1]==CS)
	{
		printf("Load Flash Config:CS=%04X\r\n",Flash_Buffer[Size+1]);
		for(i=0;i<Size;i++)
		{
			SensiBuffer[i] = Flash_Buffer[i+1];
		}		
	}
	else
	{
		printf("Load Default Config:%04X,%04X\r\n",Flash_Buffer[0],Flash_Buffer[Size+1]);
		for(i=0;i<Size;i++)
		{
			SensiBuffer[i] = Default[i];
		}		
	}
	//----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
void App_Set_Flash_Cofnig(unsigned int Addr, unsigned char *SensiBuffer, unsigned char Size)
{
	//----------------------------------------------------------
	uint32_t i;
  error_status err_status;
	flash_status_type status = FLASH_OPERATE_DONE;
  uint16_t Flash_Buffer[FLASH_BUFEER_SIZE];
	//----------------------------------------------------------
	flash_unlock();
	status = flash_sector_erase(Addr);
	flash_lock();
	printf("Erase Flash: %08X,%08X\r\n",Addr,status);
	//----------------------------------------------------------
  // fill buffer_write data to test 
	Flash_Buffer[0]=0xA55A;
  for(i = 0; i < Size; i++)
  {
    Flash_Buffer[i+1] = SensiBuffer[i];
  }	
	Flash_Buffer[Size+1] = UPDATE_CRC_16((unsigned char *)Flash_Buffer,(Size+1)*2);
	//----------------------------------------------------------
	printf("FlashW:%08X,%02X,%d \r\n",Addr,err_status,FLASH_BUFEER_SIZE);
	for(i=0;i<(Size+2);i++)
	{
		printf("%04X",Flash_Buffer[i]);
		if((i!=0)&&(((i+1)%8)==0))printf("\r\n");
	}
	printf("\r\n");	
	//----------------------------------------------------------
	// write data to flash 
  err_status = flash_write(Addr, Flash_Buffer, FLASH_BUFEER_SIZE);
	//----------------------------------------------------------
}


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
unsigned char	CommandBuffer[128];		
void App_MAPPING_Command_Report(unsigned char *CMDBuffer,unsigned char CMDLen,unsigned char *RSPBuffer,unsigned char RSPLen)	//Version	V4.01	: (2)20240616		: USB CMD for Touch Senitivity Config
{
	//----------------------------------------------------------------
	unsigned char 	i;
	unsigned char 	CRC_U8;
  //unsigned short  *ValueBuffer; //Version	V1.08	: (1)20241016		: Brightness For RGB  
    //----------------------------------------------------------------
//  printf("TPS Cmd: %02X: Raw: ",CMDLen);
//  for(i=0;i<CMDLen;i++)
//  {
//    printf("%02X ",CMDBuffer[i]);
//  }
//  printf("\r\n"); 
//  //----------------------------------------------------------------
//  if((CMDBuffer[4]&0xF0)==0x00)   
//  {
//    for(i=0; i<RSPLen; i++)
//    {
//      RSPBuffer[i] = 0x00;
//    }
//  }
	//----------------------------------------------------------------
  //CMD : 4E 45 41 44 00
  //RSP : 4E 52 53 50 00
  //Data: 03 E8 03 E8 03 E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 
  //CS	: 66		
	//----------------------------------------------------------------
	if(	CMDBuffer[0]==0x4E &&		//N
			CMDBuffer[1]==0x45 && 	//E
			CMDBuffer[2]==0x41 && 	//A
			CMDBuffer[3]==0x44)			//D
	{
    printf("Read Mapping \r\n");
    printf("Read Mapping RAW : ");
    for(i=0;i<(APP_MAX_TP_CHAN+APP_MAX_IR_CHAN);i++)
    {
      printf("%02X",AppUser.Mapping[i]);
    }
    printf("\r\n");
      
    RSPBuffer[0]=0x4E;				  //N
    RSPBuffer[1]=0x52;				  //R
    RSPBuffer[2]=0x53;				  //S
    RSPBuffer[3]=0x50;				  //P
    RSPBuffer[4]=0x00;          //Null
    for(i=0;i<(APP_MAX_TP_CHAN+APP_MAX_IR_CHAN);i++)
    {
      RSPBuffer[HEAD_LEN+(i)]= AppUser.Mapping[i];
    }
    RSPBuffer[HEAD_LEN+(APP_MAX_TP_CHAN+APP_MAX_IR_CHAN)]=UPDATE_CRC(RSPBuffer,HEAD_LEN+(APP_MAX_TP_CHAN+APP_MAX_IR_CHAN));
	}  
  //----------------------------------------------------------------
	//CMD : 4F 52 49 54 00
	//Data: 03 E8 03 E8 03 E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 03E8 
	//CS	: FA
	//RSP : 57 4F 4B FA 00 
	//----------------------------------------------------------------
	else if(CMDBuffer[0]==0x4F && //P
					CMDBuffer[1]==0x52 && 
					CMDBuffer[2]==0x49 && 
					CMDBuffer[3]==0x54 &&
					CMDLen>=38)						//5Bytes Head+(32+8)Bytes Data + 1Byte CS
	{
    //--------------------------------------------------------------
		for(i=0;i<(HEAD_LEN+(APP_MAX_TP_CHAN+APP_MAX_IR_CHAN)+1);i++)
		{
			CommandBuffer[i]= CMDBuffer[i];
		}
		CRC_U8=UPDATE_CRC(CommandBuffer,(HEAD_LEN+(APP_MAX_TP_CHAN+APP_MAX_IR_CHAN)));		
		//--------------------------------------------------------------
		if(CRC_U8==CommandBuffer[(HEAD_LEN+(APP_MAX_TP_CHAN+APP_MAX_IR_CHAN))])
		{
      printf("Write Tp Sens 1-16 \r\n");
      for(i=0;i<(APP_MAX_TP_CHAN+APP_MAX_IR_CHAN);i++)
      {
        AppUser.Mapping[i] = CommandBuffer[HEAD_LEN+(i)];
      }
      //App_Set_TP_Sens_Cofnig();
      App_Set_Flash_Cofnig(FLASH_DATA_ADDR_MAPP, AppUser.Mapping, (APP_MAX_TP_CHAN+APP_MAX_IR_CHAN));
      
      RSPBuffer[0]=0x4F;        //P
      RSPBuffer[1]=0x4F;	      //O
      RSPBuffer[2]=0x4B;	      //K
      RSPBuffer[3]=CRC_U8;
      RSPBuffer[4]=CMDBuffer[4];
		}
		//--------------------------------------------------------------
		else
		{
      printf("Write Tp Sens CS:%02X-%02X\r\n",CRC_U8,CommandBuffer[HEAD_LEN+(APP_MAX_TP_CHAN/2)*2]);
			RSPBuffer[0]=0x4F;        //P
			RSPBuffer[1]=0x46;        //F
			RSPBuffer[2]=0x49;        //I
			RSPBuffer[3]=CRC_U8;
			RSPBuffer[4]=CMDBuffer[4];
		}
		//--------------------------------------------------------------
	}
	//----------------------------------------------------------------
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
void App_User_SetKeyStatus(unsigned char Index,unsigned char State)
{
  //--------------------------------------------------
  if(State==TRUE)
  {
    AppUser.KeyCode  |= (1<<Index);
    AppUser.KeyNewsFG = 1;
  }
  //--------------------------------------------------
  else
  {
    AppUser.KeyCode  &= (1<<Index)^0xffff;
    AppUser.KeyNewsFG = 1;
  }
  //--------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
unsigned char App_User_GetKeyStatus(unsigned char Index)
{
  //--------------------------------------------------
  unsigned char State = 0;
  //--------------------------------------------------
  if((AppUser.KeyCode & (1<<Index)) !=0)
  {
    State = 1;
  }
  //--------------------------------------------------
  return State;
  //--------------------------------------------------
}


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_User_Btn_Scan(void)
{	
	//-------------------------------------------------- READ BTN
	AppUser.IO_BUF	=	0;  
  //-------------------------------------------------- 
  if (IO_IR_BTN_L4_READ()==0) AppUser.IO_BUF |= KEY_CODE_01;  //Version	V2.00	: (1)20250401 	: For New IR Sensor Base On V1.26
  if (IO_IR_BTN_L5_READ()==0) AppUser.IO_BUF |= KEY_CODE_02;  //Version	V2.00	: (1)20250401 	: For New IR Sensor Base On V1.26
  if (IO_IR_BTN_L6_READ()==0) AppUser.IO_BUF |= KEY_CODE_03;  //Version	V2.00	: (1)20250401 	: For New IR Sensor Base On V1.26
  if (IO_IR_BTN_R4_READ()==0) AppUser.IO_BUF |= KEY_CODE_04;  //Version	V2.00	: (1)20250401 	: For New IR Sensor Base On V1.26
  if (IO_IR_BTN_R5_READ()==0) AppUser.IO_BUF |= KEY_CODE_05;  //Version	V2.00	: (1)20250401 	: For New IR Sensor Base On V1.26
  if (IO_IR_BTN_R6_READ()==0) AppUser.IO_BUF |= KEY_CODE_06;  //Version	V2.00	: (1)20250401 	: For New IR Sensor Base On V1.26
  
//  if (IO_IR_BTN_L4_READ()!=0) AppUser.IO_BUF |= KEY_CODE_01;
//  if (IO_IR_BTN_R6_READ()!=0) AppUser.IO_BUF |= KEY_CODE_02;
//  
//  if (IO_IR_BTN_L5_READ()!=0) AppUser.IO_BUF |= KEY_CODE_03;
//  if (IO_IR_BTN_R5_READ()!=0) AppUser.IO_BUF |= KEY_CODE_04;
//  
//  if (IO_IR_BTN_L6_READ()!=0) AppUser.IO_BUF |= KEY_CODE_05;
//  if (IO_IR_BTN_R4_READ()!=0) AppUser.IO_BUF |= KEY_CODE_06;
  //-------------------------------------------------- 
  //Version	V1.10	: (2)20241109 	: SW1&SW2 LED
	if (IO_SW_1_READ()!=0)     {IO_SW_LED1_ON();  AppUser.IO_BUF |= KEY_CODE_10;}
  else                       {IO_SW_LED1_OFF();}
	if (IO_SW_2_READ()!=0)     {IO_SW_LED2_ON();  AppUser.IO_BUF |= KEY_CODE_11;}
  else                       {IO_SW_LED2_OFF();}
//	if (IO_SW_3_READ()==0)     {IO_SW_LED3_ON();  AppUser.IO_BUF |= KEY_CODE_09;}
//  else                       {IO_SW_LED3_OFF();}
//  if (IO_SW_4_READ()==0)     {IO_SW_LED4_ON();  AppUser.IO_BUF |= KEY_CODE_10;}
//  else                       {IO_SW_LED4_OFF();}
//  //-------------------------------------------------- 
//  if (IO_USB_PWR_READ()==0)   AppUser.IO_BUF |= KEY_CODE_11;  
//	if (IO_LED_PWR_READ()==0)   AppUser.IO_BUF |= KEY_CODE_12;
  //--------------------------------------------------
  if(AppUser.IO_REG != AppUser.IO_BUF)
  {
    AppUser.BTN_DN++;
    if(AppUser.BTN_DN>5)  //Version	V1.12	: (1)20250102 	: IR Btn Delay Issue
    {
      AppUser.IO_REG			=	AppUser.IO_BUF;		// Key Down
      AppUser.IR_VAL			=	AppUser.IO_BUF;		// New Key Value
      AppUser.BtnDownFG	=	1;		
      AppUser.BtnNewsFG	=	1;
      
      
//      if(AppUser.IO_REG&KEY_CODE_09)
//      {
//        AppUser.DebStep++;
//        if(AppUser.DebStep>=32)AppUser.DebStep=0;
//      }   
//      if(AppUser.IO_REG&KEY_CODE_10) AppUser.DebKey = 1<< AppUser.DebStep;
//      else                           AppUser.DebKey = 0;
      
//      if(AppUser.IO_REG&KEY_CODE_09)
//      {
//        AppUser.DebKey = 1<< 9;
//        AppUser.DebStep++;
//        if(AppUser.DebStep>=32)AppUser.DebStep=0;
//      }
      
 
       //Version	V1.10	: (3)20241207		: IO4 Mode Touch 
//      if(AppUser.IO_REG==(KEY_CODE_09|KEY_CODE_10)) 
//      { 
//        if(AppUser.DebState==0)
//              AppUser.DebState  = 1;
//        else  AppUser.DebState  = 0;
//      }
//      else if(AppUser.IO_REG&KEY_CODE_09)           
//      { 
//        if(AppUser.DebState==0)
//        {
//          AppUser.DebKey = 1<< 9;  
//        }
//        else   
//        {          
//          AppUser.DebStep++;  
//          if(AppUser.DebStep>=32)
//          {
//            AppUser.DebStep=0; 
//          } 
//        }          
//      }
//      else if(AppUser.IO_REG&KEY_CODE_10)           
//      { 
//        if(AppUser.DebState==0)
//        {
//          AppUser.DebKey = 1<< 10;   
//        }
//        else   
//        { 
//          AppUser.DebKey = 1<< (AppUser.DebStep);  
//        }
//      }  
//      else                                          
//      { 
//        AppUser.DebKey = 0;       
//      }
//      
      
//      APP_USER_LOG(("BTN  DN : %04X,%04X,%02d-%02d,%08X\r\n",AppUser.IO_REG,AppUser.IR_VAL,AppUser.DebState,AppUser.DebStep,AppUser.DebKey));
      APP_USER_LOG(("BTN  DN : %04X,%04X\r\n",AppUser.IO_REG,AppUser.IR_VAL));  //Version	V2.00	: (1)20250401 	: For New IR Sensor Base On V1.26
    }						
  }
  else
  {
    AppUser.BTN_DN = 0;
  }
  //--------------------------------------------------	
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_User_Sta_Update(void)
{
  //-----------------------------------------------------------------
  if((AppUser.IR_BAK != AppUser.IR_VAL) || (AppUser.Value != AppUser.TPValue))
	{
    //---------------------------------------------------------------
    if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_KBS)     
    {
      #ifdef  SOFT_CONIFG_KBS
      App_USB_KBS_Report_Normal(AppUser.IR_VAL,AppUser.TPValue);
      #endif
    }
    else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_HID)
    {
      //Version	V1.21	: (1)20250316 	: HID MODE KB Button 1&2
      #ifdef  SOFT_CONIFG_KBS
      App_USB_KBS_Report_Normal_For_HID(AppUser.IR_VAL,AppUser.TPValue);
      #endif
      
      #ifdef  SOFT_CONIFG_HID
      App_USB_HID_Report(AppUser.IR_VAL,AppUser.TPValue);
      #endif
    }
    else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_IO4)
    {
      //printf("USB Work Mode : IO4");
    }
    //---------------------------------------------------------------
    if(AppUser.Value != AppUser.TPValue)
    {
      if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_KBS)     
      {
        #ifdef  SOFT_CONIFG_KBS
        App_Set_TP_RGB_Value(AppUser.TPValue);
        #endif
      }
      else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_HID)
      {
        #ifdef  SOFT_CONIFG_HID
        //App_Set_TP_RGB_Value(AppUser.TPValue);
        #endif
      }
      else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_IO4)
      {
        #ifdef   SOFT_CONIFG_IO4
        //App_Set_TP_RGB_Value(AppUser.TPValue);
        #endif
      }
      AppUser.Value = AppUser.TPValue;
    }
    //---------------------------------------------------------------
    if(AppUser.IR_BAK != AppUser.IR_VAL)
    {
      if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_KBS)     
      {
        #ifdef  SOFT_CONIFG_KBS
        //App_Set_IR_RGB_Value(AppUser.IR_VAL); //Version	V1.09	: (1)20241020		: IR RGB Mode
        #endif
      }
      else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_HID)
      {
        #ifdef  SOFT_CONIFG_HID
        //App_Set_IR_RGB_Value(AppUser.IR_VAL);
        #endif
      }
      else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_IO4)
      {
        #ifdef   SOFT_CONIFG_IO4
        //App_Set_IR_RGB_Value(AppUser.IR_VAL);
        #endif
      }
      
      AppUser.IR_BAK = AppUser.IR_VAL;
    }    
    //---------------------------------------------------------------
  }
	//-----------------------------------------------------------------
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
void App_User_Init(void)
{
	//----------------------------------------------------------
	memset((uint8_t *)&AppUser,	0,sizeof(AppUser));
  memset((uint8_t *)&AppADC,	0,sizeof(AppADC));
	//----------------------------------------------------------
	App_RGB_Init();
	//----------------------------------------------------------
  //Version	V1.01	: (1)20240617		: App Param Config  
//  App_Get_Flash_Cofnig(FLASH_DATA_ADDR_SENSI, AppUser.Sensi, (unsigned short*)Default_Sensi_Tab, ADC_MAX_CHAN);	
//  App_Get_Flash_Cofnig(FLASH_DATA_ADDR_COLOR, AppUser.Color, (unsigned short*)Default_Color_Tab, ADC_MAX_CHAN);	
//  App_Get_Flash_Cofnig(FLASH_DATA_ADDR_DTICK, AppUser.DTick, (unsigned short*)Default_DTick_Tab, ADC_MAX_CHAN);	
//  App_BSP16_LogBuf((unsigned char*)"Sensi RAW",AppUser.Sensi,ADC_MAX_CHAN);
//  App_BSP16_LogBuf((unsigned char*)"Color RAW",AppUser.Color,ADC_MAX_CHAN);
//  App_BSP16_LogBuf((unsigned char*)"DTick RAW",AppUser.DTick,ADC_MAX_CHAN);
  
  
  App_Get_Flash_Cofnig(FLASH_DATA_ADDR_MAPP, AppUser.Mapping, Default_KeyMapping, APP_MAX_TP_CHAN+APP_MAX_IR_CHAN);	
  App_BSP_LogBuf((unsigned char*)"Mapping RAW",AppUser.Mapping,APP_MAX_TP_CHAN+APP_MAX_IR_CHAN);
  //----------------------------------------------------------
}

/*******************************************************************************
* Function Name  : 	
* Description    : 	
* Input          : 
* Output         : 
* Return         : 
* Attention		 	 : 
*******************************************************************************/
void App_User_Loop(void)
{
	//==================================================================
	if(AppUser.T1MS_FG)
	{
		//------------------------------------------------ Button Scan
		App_User_Btn_Scan();
		//------------------------------------------------ Button Press  
		App_User_Sta_Update();
    //------------------------------------------------ 
		AppUser.T1MS_FG = 0;
    //------------------------------------------------ 
	}
  //================================================================== 2mS
	if(AppUser.TREPT_FG)		//Version	V1.10	: (1)20241026		: IO4 Mode
	{
		//------------------------------------------------
    if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_IO4)  
    {
      #ifdef  SOFT_CONIFG_HID
      App_USB_IO4_Report(AppUser.IR_VAL,AppUser.TPValue);
      #endif
    }
		//------------------------------------------------
		AppUser.TREPT_FG = 0;
		//------------------------------------------------
	}
	//================================================================== 10mS
	if(AppUser.T10MS_FG)
	{
    //------------------------------------------------ 
    wdt_counter_reload();
		
		//------------------------------------------------
    if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_HID)
    {
      #ifdef  SOFT_CONIFG_HID
      App_USB_HID_Report(AppUser.IR_VAL,AppUser.TPValue);
      #endif
    }
    else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_IO4)  
    {
      #ifdef  SOFT_CONIFG_IO4   //Version	V1.22	: (1)20250318 	: IO4 MODE Touch Latency 
      //App_USB_IO4_Report(AppUser.IR_VAL,AppUser.TPValue); //Version	V1.10	: (1)20241026		: IO4 Mode
      //App_USB_IO4_TouchTxd(AppUser.IR_VAL,AppUser.TPValue); //Version	V1.10	: (1)20241026		: IO4 Mode
      //Version	V1.13	: (1)20250102 	: IO4 Touch Delay Issue
      //if(IO4TouchState >=2 && AppUser.TPKeyRDFG)
      //{
      //  //Version	V1.10	: (3)20241207		: IO4 Mode Touch 
      //  //App_USB_IO4_TouchTxd(AppUser.DebStep,AppUser.IO_REG); 
      //  App_USB_IO4_TouchTxd(AppUser.TPValue,AppUser.TPRAW,APP_MAX_TP_CHAN); 
      //  AppUser.TPKeyRDFG = 0; 
      //}

//      //Version	V1.23	: (1)20250319 	: IO4 MODE Touch Latency
//      //Version	V1.22	: (1)20250318 	: IO4 MODE Touch Latency 
//      //if(AppUser.TPKeyRDFG)
//      {
//        if(IO4TouchState >=2)
//        {
//          App_USB_IO4_TouchTxd(AppUser.TPValue,AppUser.TPRAW,APP_MAX_TP_CHAN);
//        }        
//        AppUser.TPKeyRDFG = 0; 
//      }
//      App_USB_IO4_RGB_Update();     //Version	V1.21	: (2)20250316 	: IO4 MODE Touch Latency
      #endif
    }
		//------------------------------------------------
		AppUser.T10MS_FG = 0;
		//------------------------------------------------
	}
	//================================================================== 100mS
	if(AppUser.T100MS_FG)
	{
		//------------------------------------------------ 

		//------------------------------------------------
		AppUser.T100MS_FG = 0;
		//------------------------------------------------
	}
	//================================================================== 1Second
	if(AppUser.T1Sec_FG)
	{
		//------------------------------------------------
		AppUser.T1Sec_FG = 0; //App_Set_TP_RGB_TEST(0);
    //------------------------------------------------ 
		CHK_ADC_SCAN(&AppADC);  //Version	V1.08	: (1)20241016		: Brightness For RGB  
		//------------------------------------------------
	}
  //==================================================================
	#ifdef	APP_HW_TP_MODE		//Version	V1.33	: (1)20230207		: HW Touch with CHSC6X
	App_CHSC6X_TP_Update();
	#endif  
  //==================================================================
//  //Version	V1.22	: (1)20250318 	: IO4 MODE Touch Latency    //Version	V1.13	: (1)20250102 	: IO4 Touch Delay Issue
//  if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_IO4)  
//  {
//    #ifdef  SOFT_CONIFG_HID
//    if(AppUser.TPKeyRDFG)
//    {
//      if(IO4TouchState >=2)
//      {
//        App_USB_IO4_TouchTxd(AppUser.TPValue,AppUser.TPRAW,APP_MAX_TP_CHAN);
//      }        
//      AppUser.TPKeyRDFG = 0; 
//    }
//    #endif
//  }
  
  
  //Version	V1.23	: (1)20250319 	: IO4 MODE Touch Latency
	if(AppUser.TPTick_FG)
	{
    //Version	V1.22	: (1)20250318 	: IO4 MODE Touch Latency 
    //if(AppUser.TPKeyRDFG)
    {
      if(IO4TouchState >=2)
      {
        App_USB_IO4_TouchTxd(AppUser.TPValue,AppUser.TPRAW,APP_MAX_TP_CHAN);
      }        
      AppUser.TPKeyRDFG = 0; 
    }
    App_USB_IO4_RGB_Update();     //Version	V1.21	: (2)20250316 	: IO4 MODE Touch Latency

		AppUser.TPTick_FG	=	0;
	}
	//==================================================================
  App_UART_Update();
	//==================================================================
	App_RGB_Update(AppUser.SysTimeTick);
	//==================================================================
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


