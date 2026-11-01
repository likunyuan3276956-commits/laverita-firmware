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

#include <stdbool.h>
#include <string.h>
#include "at32f415_clock.h"
#include "App_BSP.h"
#include "App_USER.h"
#include "App_UART.h"
#include "usb_conf.h"
#include "usb_core.h"
#include "usbd_int.h"
#include "usb_class.h"
#include "flash.h"
#include	"App_CHSC6X_TP.h"
#include  "App_RGB.h"

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

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

otg_core_type otg_core_struct;
uint8_t keyboard_send_flag;
uint8_t usb_buffer[256];
uint8_t VersionLock=0;
uint16_t data_len;

uint8_t HIDReportState=0;   //Version	V3.04	: (1)20240522		: Support Maimai IO4 Protocol
uint8_t IO4TouchState=0; 
uint8_t usb_buffer_bk[256];
uint8_t rgb_buffer[256];    //Version	V1.20	: (1)20250310 	: IO4 MODE Touch Latency


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: TMR3: MCU Base TimeTick 1mS
******************************************************************************************/
void TMR_1mS_Interrupt_CB(void)
{
  //-------------------------------------------------
  App_User_TimeTick_CB();
	//-------------------------------------------------
  App_UART_TimeTick_CB();
	//-------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: Configure PB4 as EXTI0 interrupt mode : Ecode A Input
******************************************************************************************/
void EXTI4_Interrupt_CB(void)
{
	//printf("EXTI4_Interrupt:");
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifdef  SOFT_CONIFG_CDC
void App_USB_CDC_SendPacket(unsigned char *PayLoadRaw,unsigned short Length)
{
  //Version	V1.10	: (1)20241026		: IO4 Mode
  //------------------------------------------------------------------
  usbd_core_type *pudev = (usbd_core_type *)(&otg_core_struct.dev);
  //usbd_type_t *vcpkybrd = (usbd_type_t *)pudev->class_handler->pdata; 
  //------------------------------------------------------------------  
  if(usbd_connect_state_get(pudev) == USB_CONN_STATE_CONFIGURED)	
	{  
	//----------------------------------------------------------
	//usb_vcp_send_data(&otg_core_struct.dev, &Reporte_Buffer[0], 8);
	usbd_cdc_send_data(&otg_core_struct.dev, PayLoadRaw, Length);
	//----------------------------------------------------------
  }
}
#endif



/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifdef  SOFT_CONIFG_KBS         //Version	V4.00	: (1)20240528		: Composite USB KeyBoard, HID, IO4 Protocol
void App_USB_KBS_SendPacket(unsigned char *PayLoadRaw,unsigned short Length)
{
	//------------------------------------------------------------------
  usbd_core_type *pudev = (usbd_core_type *)(&otg_core_struct.dev);
  usbd_type_t *vcpkybrd = (usbd_type_t *)pudev->class_handler->pdata;
	//------------------------------------------------------------------
	if(usbd_connect_state_get(pudev) == USB_CONN_STATE_CONFIGURED)	//
	{
		while(1)
		{
			if(vcpkybrd->g_kbs_tx_completed == 1)
			{
				vcpkybrd->g_kbs_tx_completed = 0;
        usbd_kbs_class_send_report(&otg_core_struct.dev, PayLoadRaw,Length);
				break;
			}
		}
	}
	//------------------------------------------------------------------
}
#endif


extern unsigned char Default_KeyMapping[APP_MAX_TP_CHAN+APP_MAX_IR_CHAN];

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifdef  SOFT_CONIFG_KBS
void App_USB_KBS_Report_Normal(unsigned char IRCode,unsigned int TPKeyCode)
{
	//------------------------------------------------------------------
  uint8_t Count			= 0;
	uint8_t	Index			= 0;
	uint8_t AsciiChar = 0;
	uint8_t	AsciiCode = 0;
	uint8_t keyboard_buf[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	//------------------------------------------------------------------
	for(Index=0;Index<APP_MAX_TP_CHAN;Index++)
	{
    if((TPKeyCode&(1<<Index))!=0)	
    {
      //-----------------------------------------------
      AsciiChar = AppUser.Mapping[Index]; //Default_KeyMapping[Index];
      //-----------------------------------------------//Version	V1.01	: (1)20240827		: KeyCode Mapping   
      if((AsciiChar >= 0x5B && AsciiChar<= 0x5c)||
         (AsciiChar >= 0xA0 && AsciiChar<= 0xA5))
      {
        if(AsciiChar == 0x5B)     { keyboard_buf[0] = 0x08; }  //Win-L
        else if(AsciiChar == 0x5C){ keyboard_buf[0] = 0x80; }  //Win-R
        else if(AsciiChar == 0xA0){ keyboard_buf[0] = 0x02; }  //Shift-L
        else if(AsciiChar == 0xA1){ keyboard_buf[0] = 0x20; }  //Shift-R
        else if(AsciiChar == 0xA2){ keyboard_buf[0] = 0x01; }  //Ctrl-L 
        else if(AsciiChar == 0xA3){ keyboard_buf[0] = 0x10; }  //Ctrl-R
        else if(AsciiChar == 0xA4){ keyboard_buf[0] = 0x04; }  //Alt-L
        else if(AsciiChar == 0xA5){ keyboard_buf[0] = 0x40; }  //Alt-R
      }
      else
      {
        AsciiCode = get_ascii_code(AsciiChar);
        if(AsciiCode & SHIFT)
        {
          keyboard_buf[0] = 0x2;
          AsciiCode &= 0x7F;
        }   
        keyboard_buf[2+Count] = AsciiCode;
      }
      //-----------------------------------------------
      Count++;
      if(Count>=8)
      {
        printf("\r\nTP Key Over ---------------------- \r\n");
        break;
      }
      //-----------------------------------------------
    }
	}
  //------------------------------------------------------------------
  if(Count < 8)
  {
    for(Index=0;Index<APP_MAX_IR_CHAN;Index++)
    {
      if((IRCode&(1<<Index))!=0)	
      {
        //-----------------------------------------------
        AsciiChar = AppUser.Mapping[APP_MAX_TP_CHAN+Index]; //Default_KeyMapping[APP_MAX_TP_CHAN+Index];
        //-----------------------------------------------//Version	V1.01	: (1)20240827		: KeyCode Mapping   
        if((AsciiChar >= 0x5B && AsciiChar<= 0x5c)||
           (AsciiChar >= 0xA0 && AsciiChar<= 0xA5))
        {
          if(AsciiChar == 0x5B)     { keyboard_buf[0] = 0x08; }  //Win-L
          else if(AsciiChar == 0x5C){ keyboard_buf[0] = 0x80; }  //Win-R
          else if(AsciiChar == 0xA0){ keyboard_buf[0] = 0x02; }  //Shift-L
          else if(AsciiChar == 0xA1){ keyboard_buf[0] = 0x20; }  //Shift-R
          else if(AsciiChar == 0xA2){ keyboard_buf[0] = 0x01; }  //Ctrl-L 
          else if(AsciiChar == 0xA3){ keyboard_buf[0] = 0x10; }  //Ctrl-R
          else if(AsciiChar == 0xA4){ keyboard_buf[0] = 0x04; }  //Alt-L
          else if(AsciiChar == 0xA5){ keyboard_buf[0] = 0x40; }  //Alt-R
        }
        else
        {
          AsciiCode = get_ascii_code(AsciiChar);
          if(AsciiCode & SHIFT)
          {
            keyboard_buf[0] = 0x2;
            AsciiCode &= 0x7F;
          }   
          keyboard_buf[2+Count] = AsciiCode;
        }
        //-----------------------------------------------
        Count++;
        if(Count>=8)
        {
          printf("\r\nIR Key Over ---------------------- \r\n");
          break;
        }
        //-----------------------------------------------
      }
    }
  }
	//------------------------------------------------------------------
  //Version	V2.00	: (1)20250401 	: For New IR Sensor Base On V1.26
//	printf("BTN  New: %08X,%02d: %02X%02X-%02X%02X%02X%02X%02X%02X%02X%02X(%02X)\r\n",TPKeyCode,Count,
//	keyboard_buf[0],keyboard_buf[1],keyboard_buf[2],keyboard_buf[3],
//  keyboard_buf[4],keyboard_buf[5],keyboard_buf[6],keyboard_buf[7],
//  keyboard_buf[8],keyboard_buf[9],AsciiChar);
	//------------------------------------------------------------------
  App_USB_KBS_SendPacket(keyboard_buf,10);
	//------------------------------------------------------------------
}


//Version	V1.21	: (1)20250316 	: HID MODE KB Button 1&2
void App_USB_KBS_Report_Normal_For_HID(unsigned char IRCode,unsigned int TPKeyCode)
{
	//------------------------------------------------------------------
  uint8_t Count			= 0;
	uint8_t	Index			= 0;
	uint8_t AsciiChar = 0;
	uint8_t	AsciiCode = 0;
	uint8_t keyboard_buf[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  //------------------------------------------------------------------
  uint8_t NewIR			= 0;
  if(AppUser.IR_VAL&KEY_CODE_10) NewIR |= 0x40;   //Version	V1.21	: (1)20250316 	: HID MODE KB Button 1&2 
  if(AppUser.IR_VAL&KEY_CODE_11) NewIR |= 0x80;   //Version	V1.21	: (1)20250316 	: HID MODE KB Button 1&2
  //------------------------------------------------------------------
  if(Count < 8)
  {
    for(Index=0;Index<2;Index++)  //APP_MAX_IR_CHAN
    {
      if((NewIR&(1<<(Index+6)))!=0)	
      {
        //-----------------------------------------------
        //AsciiChar = AppUser.Mapping[APP_MAX_TP_CHAN+Index]; //Default_KeyMapping[APP_MAX_TP_CHAN+Index];
        
        if(Index==0)      AsciiChar = 0x31;
        else if(Index==1) AsciiChar = 0x32;
        {
          AsciiCode = get_ascii_code(AsciiChar);
          if(AsciiCode & SHIFT)
          {
            keyboard_buf[0] = 0x2;
            AsciiCode &= 0x7F;
          }   
          keyboard_buf[2+Count] = AsciiCode;
        }
        //-----------------------------------------------
        Count++;
        if(Count>=8)
        {
          printf("\r\nIR Key Over ---------------------- \r\n");
          break;
        }
        //-----------------------------------------------
      }
    }
  }
	//------------------------------------------------------------------
  //Version	V2.00	: (1)20250401 	: For New IR Sensor Base On V1.26
//	printf("BTN  New: %08X,%02d: %02X%02X-%02X%02X%02X%02X%02X%02X%02X%02X(%02X)\r\n",TPKeyCode,Count,
//	keyboard_buf[0],keyboard_buf[1],keyboard_buf[2],keyboard_buf[3],
//  keyboard_buf[4],keyboard_buf[5],keyboard_buf[6],keyboard_buf[7],
//  keyboard_buf[8],keyboard_buf[9],AsciiChar);
	//------------------------------------------------------------------
  App_USB_KBS_SendPacket(keyboard_buf,10);
	//------------------------------------------------------------------
}
#endif


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifdef  SOFT_CONIFG_KBS       //Version	V4.00	: (1)20240528		: Composite USB KeyBoard, HID, IO4 Protocol

#define BUTTON_NKRO_MAP_P1 "\x1a\x08\x07\x06\x1b\x1d\x04\x14\x20\x3a\x3b\x3c"
#define BUTTON_NKRO_MAP_P2 "\x60\x61\x5e\x5b\x5a\x59\x5c\x5f\x55\x3a\x3b\x3c"

const char keymap_p1[] = BUTTON_NKRO_MAP_P1;
const char keymap_p2[] = BUTTON_NKRO_MAP_P2;

void App_USB_KBS_Report_IO4(unsigned char IRCode,unsigned int TPKeyCode)
{
  //------------------------------------------------------------------
  //if (!mai_cfg->hid.nkro) {
  //    return;
  //}
  //------------------------------------------------------------------
  uint8_t   i;
  uint8_t   keyboard_buf[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint16_t  buttons = AppUser.Value;//button_read();
  //const char *keymap = (mai_cfg->hid.nkro == 2) ? keymap_p2 : keymap_p1;
  const char *keymap = keymap_p1;
  //------------------------------------------------------------------
  for (i = 0; i < 12; i++)     //button_num()
  {
    uint8_t code = keymap[i];
    uint8_t byte = code / 8;
    uint8_t bit = code % 8;
    if (buttons & (1 << i)) 
    {
      //hid_nkro.keymap[byte] |= (1 << bit);
      keyboard_buf[1+byte] |= (1 << bit);
    } 
    else 
    {
      //hid_nkro.keymap[byte] &= ~(1 << bit);
      keyboard_buf[1+byte]  &= ~(1 << bit);
    }
  }
  //------------------------------------------------------------------
//  printf("BNT  New: %04X, %02X%02X%02X%02X-%02X%02X%02X%02X-%02X%02X%02X%02X-%02X%02X%02X%02X\r\n",
//  AppUser.Value,
//	keyboard_buf[ 0],keyboard_buf[ 1],keyboard_buf[ 2],keyboard_buf[ 3],
//  keyboard_buf[ 4],keyboard_buf[ 5],keyboard_buf[ 6],keyboard_buf[ 7],
//  keyboard_buf[ 8],keyboard_buf[ 9],keyboard_buf[10],keyboard_buf[11],
//  keyboard_buf[12],keyboard_buf[13],keyboard_buf[14],keyboard_buf[15]);
	//------------------------------------------------------------------
  App_USB_KBS_SendPacket(keyboard_buf, 16); 	
	//------------------------------------------------------------------
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
#ifdef  SOFT_CONIFG_HID                   //Version	V3.04	: (1)20240522		: Support Maimai IO4 Protocol
void App_USB_HID_SendPacket(unsigned char *PayLoadRaw,unsigned short Length)
{
  //----------------------------------------------------------
  usbd_core_type *pudev = (usbd_core_type *)(&otg_core_struct.dev);
  usbd_type_t *vcpkybrd = (usbd_type_t *)pudev->class_handler->pdata; 
	//----------------------------------------------------------
  if(usbd_connect_state_get(pudev) == USB_CONN_STATE_CONFIGURED)	
	{
		while(1)
		{
			if(vcpkybrd->g_hid_tx_completed == 1)
			{
				vcpkybrd->g_hid_tx_completed = 0;
        usbd_hid_class_send_report(&otg_core_struct.dev, PayLoadRaw,Length); 
				break;
			}
		}
	}  
	//----------------------------------------------------------
}
#endif



/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifdef  SOFT_CONIFG_HID
void App_USB_HID_Report(unsigned char IRCode,unsigned int TPKeyCode)
{
  //------------------------------------------------------------------GamePad
  #ifdef  SOFT_CONIFG_HID_GAMPAD  
  //------------------------------------------------------------------
  unsigned char		  HidReportBuf[32];  
  unsigned short    KeyCodeNew = 0x000;  
  //------------------------------------------------------------------
  if(KeyCode &0x0001) KeyCodeNew |= (1<<1);
  if(KeyCode &0x0002) KeyCodeNew |= (1<<7);
  if(KeyCode &0x0004) KeyCodeNew |= (1<<3);
  if(KeyCode &0x0008) KeyCodeNew |= (1<<5);
  if(KeyCode &0x0010) KeyCodeNew |= (1<<4);
  if(KeyCode &0x0020) KeyCodeNew |= (1<<0);
  if(KeyCode &0x0040) KeyCodeNew |= (1<<2);
  if(KeyCode &0x0080) KeyCodeNew |= (1<<6);
  if(KeyCode &0x0100) KeyCodeNew |= (1<<8); 
	//------------------------------------------------------------------
  HidReportBuf[0] = USBD_HID_RP_ID; 
  HidReportBuf[1] = 0x80;                   //Left  Stick X 
  HidReportBuf[2] = 0x80;                   //Left  Stick Y
  HidReportBuf[3] = 0x80;                   //Right Stick X 
  HidReportBuf[4] = 0x80;                   //Right Stick Y  
  HidReportBuf[5] = 0x80;                   //L2
  HidReportBuf[6] = 0x80;                   //R2 
  HidReportBuf[7] = (KeyCodeNew>>0)&0xff;   //Button 1-16
  HidReportBuf[8] = (KeyCodeNew>>8)&0xff;   // 
  HidReportBuf[9] = 0x00;                   //Button17-32
  HidReportBuf[10]= 0x00;                   //   
  HidReportBuf[11]= 0x00;                   //HatSwitch:Left/Right/Up/Down  (4Bits)
  App_USB_HID_SendPacket(HidReportBuf,USBD_HID_REPORT_LEN);  
  //------------------------------------------------------------------ÖÐ¶þ HID
  #elif   SOFT_CONIFG_HID_ZER
  //uint8_t Count			= 0;    //Version	V1.08	: (1)20241016		: Brightness For RGB  
	uint8_t	Index			= 0;
  uint8_t	NewIR			= 0;
  //uint8_t	NewFlag		= 0;    //Version	V1.08	: (1)20241016		: Brightness For RGB  
	static uint8_t ReportBuffer[64];
  
  unsigned char 	HIDChanMapping[34]=	//34*10			
  {
    30,31,28,29,26,27,24,25,22,23,20,21,18,19,16,17,
    14,15,12,13,10,11,8,9,6,7,4,5,2,3,0,1,       
    32,33,
  };
	//------------------------------------------------------------------
  for(Index=0;Index<APP_MAX_TP_CHAN;Index++)
	{
		if(AppUser.TPRAW[Index]!=0)	
		{
      if(ReportBuffer[2+HIDChanMapping[Index]]!=AppUser.TPRAW[Index])
      {
        ReportBuffer[2+HIDChanMapping[Index]] = AppUser.TPRAW[Index];
        //NewFlag = 1;    //Version	V1.08	: (1)20241016		: Brightness For RGB  
      }
		}
    else
    {
      if(ReportBuffer[2+HIDChanMapping[Index]]!=0x00)
      {
        ReportBuffer[2+HIDChanMapping[Index]] = 0x00;
        //NewFlag = 1;    //Version	V1.08	: (1)20241016		: Brightness For RGB  
      }
    }
	}
  //------------------------------------------------------------------
  //Version	V1.02	: (1)20240828 : Support HID Mode
  //Version	V1.18	: (1)20250228 	: HID MODE Test and Sevice Button  
//  NewIR = 0;
//  if(IRCode&0x01) NewIR |= 0x02;
//  if(IRCode&0x02) NewIR |= 0x01;
//  if(IRCode&0x04) NewIR |= 0x08;
//  if(IRCode&0x08) NewIR |= 0x04;
//  if(IRCode&0x10) NewIR |= 0x20;
//  if(IRCode&0x20) NewIR |= 0x10;
//  ReportBuffer[0] = NewIR;
  
  NewIR = 0;
  if(IRCode&KEY_CODE_01) NewIR |= 0x02;
  if(IRCode&KEY_CODE_02) NewIR |= 0x01;
  if(IRCode&KEY_CODE_03) NewIR |= 0x08;
  if(IRCode&KEY_CODE_04) NewIR |= 0x04;
  if(IRCode&KEY_CODE_05) NewIR |= 0x20;
  if(IRCode&KEY_CODE_06) NewIR |= 0x10;
  //if(IRCode&KEY_CODE_10) NewIR |= 0x40;   //Version	V1.18	: (1)20250228 	: HID MODE Test and Sevice Button  
  //if(IRCode&KEY_CODE_11) NewIR |= 0x80;   //Version	V1.18	: (1)20250228 	: HID MODE Test and Sevice Button 
  if(AppUser.IR_VAL&KEY_CODE_10) NewIR |= 0x40;   //Version	V1.21	: (1)20250316 	: HID MODE KB Button 1&2 
  if(AppUser.IR_VAL&KEY_CODE_11) NewIR |= 0x80;   //Version	V1.21	: (1)20250316 	: HID MODE KB Button 1&2
  
  ReportBuffer[0] = NewIR;

  
//  for(Index=0;Index<APP_MAX_IR_CHAN;Index++)
//  {
//    if((IRCode&(1<<Index))!=0)	
//    {
//      ReportBuffer[2+APP_MAX_TP_CHAN+Index] = 0xff;
//      NewFlag = 1;
//    }
//    else if(ReportBuffer[2+APP_MAX_TP_CHAN+Index]!=0x00)
//    {
//      ReportBuffer[2+APP_MAX_TP_CHAN+Index] = 0x00;
//      NewFlag = 1;
//    }
//  }
  //------------------------------------------------------------------
//  if(NewFlag)
//  {
//    printf("HID  New: %04X,%02X: %02X%02X%02X%02X%02X-%02X%02X%02X%02X%02X-%02X%02X-------HID END",AppUser.Value,Count,
//    ReportBuffer[0],ReportBuffer[1],ReportBuffer[2],ReportBuffer[3],ReportBuffer[4],
//    ReportBuffer[5],ReportBuffer[6],ReportBuffer[7],ReportBuffer[8],ReportBuffer[9],
//    ReportBuffer[10],ReportBuffer[11]);
//  }
	//------------------------------------------------------------------ 
  App_USB_HID_SendPacket(ReportBuffer,USBD_HID_REPORT_LEN);  
  //------------------------------------------------------------------MiMi
  #else //#elif  SOFT_CONIFG_HID_MIMI
	//------------------------------------------------------------------
  //uint8_t Count			= 0;
	uint8_t	Index			= 0;
	static uint8_t ReportBuffer[32];
  //------------------------------------------------------------------
  Index = 1;
  if((KeyCode&0x0001)!=0)	ReportBuffer[Index] = 0x01;
  else                    ReportBuffer[Index] = 0x00; Index++;
  if((KeyCode&0x0002)!=0)	ReportBuffer[Index] = 0x01;
  else                    ReportBuffer[Index] = 0x00; Index++;
  if((KeyCode&0x0004)!=0)	ReportBuffer[Index] = 0x01;
  else                    ReportBuffer[Index] = 0x00; Index++;
  if((KeyCode&0x0008)!=0)	ReportBuffer[Index] = 0x01;
  else                    ReportBuffer[Index] = 0x00; Index++;  
  if((KeyCode&0x0010)!=0)	ReportBuffer[Index] = 0x01;
  else                    ReportBuffer[Index] = 0x00; Index++;
  if((KeyCode&0x0020)!=0)	ReportBuffer[Index] = 0x01;
  else                    ReportBuffer[Index] = 0x00; Index++;
  if((KeyCode&0x0040)!=0)	ReportBuffer[Index] = 0x01;
  else                    ReportBuffer[Index] = 0x00; Index++;
  if((KeyCode&0x0080)!=0)	ReportBuffer[Index] = 0x01;
  else                    ReportBuffer[Index] = 0x00; Index++; 

  if((KeyCode&0x0100)!=0)	ReportBuffer[Index] = 0x01;
  else                    ReportBuffer[Index] = 0x00; Index++; 
  if((KeyCode&0x0200)!=0)	ReportBuffer[Index] = 0x01;
  else                    ReportBuffer[Index] = 0x00; Index++;   
  if((KeyCode&0x0400)!=0)	ReportBuffer[Index] = 0x01;
  else                    ReportBuffer[Index] = 0x00; Index++; 
  if((KeyCode&0x0800)!=0)	ReportBuffer[Index] = 0x01;
  else                    ReportBuffer[Index] = 0x00; Index++; 
  //------------------------------------------------------------------
  ReportBuffer[0] = USBD_HID_RP_ID; //ReportID
  ReportBuffer[31]++;               //
  //------------------------------------------------------------------
  App_USB_HID_SendPacket(ReportBuffer,USBD_HID_REPORT_LEN);
  //------------------------------------------------------------------
  #endif
  //------------------------------------------------------------------
}
#endif






/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 

27=IR6
11=IR5
28=IR4
12=IR3
29=IR2
13=IR1
******************************************************************************************/
#ifdef  SOFT_CONIFG_IO4                       //Version	V3.04	: (1)20240522		: Support Maimai IO4 Protocol
static uint32_t native_to_io4(uint8_t index,uint16_t button)
{
  //------------------------------------------------------------------
  uint8_t   i;
  uint32_t io4btn = 0;
//  static const int target1_pos[] = { 2, 3, 0, 15, 14, 13, 12, 11, 1, 9, 6 };
//  static const int target2_pos[] = {18,19,16, 31, 30, 29, 28, 27, 1, 9, 6 };
  
  static const int target1_pos[] = {13,29,12, 28, 11, 27,  3,  2, 1, 9, 6 };
  static const int target2_pos[] = {14,15,16, 17, 18, 19,  5,  4, 1, 9, 6 };
  //------------------------------------------------------------------
  for (i = 0; i < 8; i++) 
  {
    bool pressed = button & (1 << i);
    if(index==0)
    {
      io4btn |= pressed ? 0 : (1 << target1_pos[i]);
    }
    else
    {
      io4btn |= pressed ? 0 : (1 << target2_pos[i]);
    }
  }
  //------------------------------------------------------------------
  for (i = 8; i < 11; i++) 
  {
    bool pressed = button & (1 << i);
    if(index==0)
    {
      io4btn |= pressed ? (1 << target1_pos[i]) : 0;
    }
    else
    {
      io4btn |= pressed ? (1 << target2_pos[i]) : 0;
    }
  }
  //------------------------------------------------------------------
  //printf("IO4 In:%04X,Out:%04X\r\n",button,io4btn);
  //------------------------------------------------------------------
  return io4btn;
  //------------------------------------------------------------------
}
#endif


unsigned char TouchBuf_OFF[36] = 
{ 
  0xFF,0x01,0x20,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0xE0
}; 
    
unsigned char TouchBuf_1[36] = 
{ 
  0xFF,0x01,0x20,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x8A,0x2F,0x67,0x8E,0x5F,
  0x93,  
}; 

unsigned char TouchBuf_2[36] = 
{ 
//  0xFF,0x01,0x20,
//  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x24,0x18,0x46,0x5C,0xAF,0xA4,0x5C,0x6F,
//  0xE4
  
//  0xFF,0x01,0x20,
//  0x00,0x00,0x2F,0x00,0x8C,0x37,0xFC,0x45,0x6E,0x6B,0x41,0xF8,0x00,0x00,0x00,0x00,
//  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//  0x9B  


  0xFF,0x01,0x20,
  0x00,0x00,0x00,0x00,0x8C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x54

  
}; 

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
TXD:36,FF 01 20 00 00 00 00 8C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 54

TXD:36,FF 01 20 00 00 00 00 8C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 54
******************************************************************************************/
#ifdef  SOFT_CONIFG_IO4                        //Version	V1.10	: (1)20241026		: IO4 Mode

unsigned char CS=0;
unsigned char TouchBuf[36] = 
{ 0xFF,0x01,0x20,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xE0
}; 

void App_USB_IO4_TouchTxd(unsigned int TPKeyCode,unsigned char *TouchRaw,unsigned short Length)       //Version	V1.10	: (3)20241207		: IO4 Mode Touch 
{
  //------------------------------------------------------------------
  unsigned char i = 0;
  //Version	V1.25	: (1)20250328 	: IO4 MODE Touch Issue
  static unsigned char marked[1+0x23*2];  //Max possible size 
  unsigned char j  = 1;                   // Total marked packetLength
  unsigned char flag  = 0;
  //------------------------------------------------------------------
//  for(i=0;i<Length;i++) //if(IRCode<32 && TPKeyCode!=0)
//  {
//    TouchBuf[3+i] = TouchRaw[0];
//  }
  
  TouchBuf[ 3] = AppUser.TPRAW[ 0];
  TouchBuf[ 4] = AppUser.TPRAW[ 1];
  TouchBuf[ 5] = AppUser.TPRAW[ 2];
  TouchBuf[ 6] = AppUser.TPRAW[ 3];
  TouchBuf[ 7] = AppUser.TPRAW[ 4];
  TouchBuf[ 8] = AppUser.TPRAW[ 5];
  TouchBuf[ 9] = AppUser.TPRAW[ 6];
  TouchBuf[10] = AppUser.TPRAW[ 7];
  
  TouchBuf[11] = AppUser.TPRAW[ 8];
  TouchBuf[12] = AppUser.TPRAW[ 9];
  TouchBuf[13] = AppUser.TPRAW[10];
  TouchBuf[14] = AppUser.TPRAW[11];
  TouchBuf[15] = AppUser.TPRAW[12];
  TouchBuf[16] = AppUser.TPRAW[13];
  TouchBuf[17] = AppUser.TPRAW[14];
  TouchBuf[18] = AppUser.TPRAW[15];
  
  TouchBuf[19] = AppUser.TPRAW[16];
  TouchBuf[20] = AppUser.TPRAW[17];
  TouchBuf[21] = AppUser.TPRAW[18];
  TouchBuf[22] = AppUser.TPRAW[19];
  TouchBuf[23] = AppUser.TPRAW[20];
  TouchBuf[24] = AppUser.TPRAW[21];
  TouchBuf[25] = AppUser.TPRAW[22];
  TouchBuf[26] = AppUser.TPRAW[23];

  TouchBuf[27] = AppUser.TPRAW[24];
  TouchBuf[28] = AppUser.TPRAW[25];
  TouchBuf[29] = AppUser.TPRAW[26];
  TouchBuf[30] = AppUser.TPRAW[27];
  TouchBuf[31] = AppUser.TPRAW[28];
  TouchBuf[32] = AppUser.TPRAW[29];
  TouchBuf[33] = AppUser.TPRAW[30];
  TouchBuf[34] = AppUser.TPRAW[31];
  //------------------------------------------------------------------
  CS = 0;
  for(i=0;i<35;i++)
  {
    CS += TouchBuf[i];
  }
  CS ^= 0xff;
  CS += 0x01;
  TouchBuf[35] = CS;
  //------------------------------------------------------------------
  //Version	V1.25	: (1)20250328 	: IO4 MODE Touch Issue
//  App_USB_CDC_SendPacket(TouchBuf,36);
//  //if(Length<=32 && TPKeyCode!=0)  //Version	V1.23	: (1)20250319 	: IO4 MODE Touch Latency  //Version	V1.22	: (1)20250318 	: IO4 MODE Touch Latency //Version	V1.15	: (1)20250222 	: Touch Crush Issue
//  {
//    //App_BSP_LogBuf((unsigned char*)"TXD",TouchBuf,36);
//    printf("TXD : %04X,%08X\r\n",Length,TPKeyCode);
//  }
  
  marked[0] = 0xFF;                       // Sync byte isn'tmarked 
  for(i=1,j=1;i< 36; i++,j++)
  {
    if(TouchBuf[i] == 0xFF || TouchBuf[i] == 0xFD)
    {
      marked[j++]= 0xFD; // Mark byte
      marked[j]= TouchBuf[i]-1;
      flag = 1;
    }
    else 
    {
      marked[j] = TouchBuf[i];
    }
  }

  App_USB_CDC_SendPacket(marked,j);
  
  if(flag)
  {
    //App_BSP_LogBuf((unsigned char*)"TXD1",TouchBuf,36);
    //App_BSP_LogBuf((unsigned char*)"TXD2",marked,j);
    printf("TXD : %04X,%08X,%d\r\n",Length,TPKeyCode,j);
  }
  //------------------------------------------------------------------
  
  
//  if(IRCode<32 && TPKeyCode!=0)
//  {
//    TouchBuf[3+IRCode] = 0x8C;
//  }
//  //------------------------------------------------------------------
//  for(i=0;i<35;i++)
//  {
//    CS += TouchBuf[i];
//  }
//  CS ^= 0xff;
//  CS += 0x01;
//  TouchBuf[35] = CS;
//  //------------------------------------------------------------------
//  App_USB_CDC_SendPacket(TouchBuf,36);
//  if(IRCode<32 && TPKeyCode!=0)
//  {
//    App_BSP_LogBuf((unsigned char*)"TXD",TouchBuf,36);
//  }
//  //------------------------------------------------------------------
  
//  if(TPKeyCode==KEY_CODE_09)
//  {
//    App_BSP_LogBuf((unsigned char*)"TXD",TouchBuf_1,36);
//    App_USB_CDC_SendPacket(TouchBuf_1,36);
//  }
//  else if(TPKeyCode==KEY_CODE_10)
//  {
//    App_BSP_LogBuf((unsigned char*)"TXD",TouchBuf_2,36);
//    App_USB_CDC_SendPacket(TouchBuf_2,36);
//  }
//  else
//  {
//    App_BSP_LogBuf((unsigned char*)"TXD",TouchBuf_OFF,36);
//    App_USB_CDC_SendPacket(TouchBuf_OFF,36);
//  }
//  //------------------------------------------------------------------  
  
  //------------------------------------------------------------------
}


void App_USB_IO4_TouchRsp(unsigned char Step)
{
  //------------------------------------------------------------------
  unsigned char		  TouchRsp_0[4] = { 0xFF,0x10,0x00,0xf1}; 
  unsigned char		  TouchRsp_1[23]= { 0xFF,0xf0,0x12,0x31,0x35,0x33,0x33,0x30,0x20,0x20,0x20,0xA0,0x30,0x36,0x37,0x31,0x32,0xFD,0xFE,0x90,0x00,0x64,0x10}; 
  unsigned char		  TouchRsp_2[36]= { 0xFF,0x01,0x20,
                                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                      0xE0};
  unsigned char		  TouchRsp_3[5] = { 0xFF,0x04,0x00,0xfd,0xfc};                           
  //------------------------------------------------------------------
  if(Step==0)
  {
    App_BSP_LogBuf((unsigned char*)"TXD",TouchRsp_0,4);
    App_USB_CDC_SendPacket(TouchRsp_0,4);
  }
  else if(Step==1)
  {
    App_BSP_LogBuf((unsigned char*)"TXD",TouchRsp_1,23);
    App_USB_CDC_SendPacket(TouchRsp_1,23);
  }
  else if(Step==2)
  {
    App_BSP_LogBuf((unsigned char*)"TXD",TouchRsp_2,36);
    App_USB_CDC_SendPacket(TouchRsp_2,36);
  }
  else if(Step==3)
  {
    App_BSP_LogBuf((unsigned char*)"TXD",TouchRsp_3,5);
    App_USB_CDC_SendPacket(TouchRsp_3,5);
  }
  
  
  //------------------------------------------------------------------  
  
  //------------------------------------------------------------------
}


#endif

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifdef  SOFT_CONIFG_IO4                         //Version	V4.00	: (1)20240528		: Composite USB KeyBoard, HID, IO4 Protocol
void App_USB_IO4_Report(unsigned char IRCode,unsigned int TPKeyCode)
{
  //------------------------------------------------------------------
  unsigned char		  HidReportBuf[64];  
  //------------------------------------------------------------------MAI_IO4
  uint16_t  hid_joy_chutes[2] = {0x0002,0x0002};
  uint32_t  hid_joy_btn1      = 0x00000000;
  uint32_t  hid_joy_btn2      = 0x00000000;
  uint32_t  hid_joy_raw       = 0x00000000;
  //------------------------------------------------------------------
  memset((uint8_t *)&HidReportBuf,	0,sizeof(HidReportBuf));
  //------------------------------------------------------------------
  //if (tud_hid_ready()) 
  {
    //if (mai_cfg->hid.joy || mai_runtime.key_stuck) 
    {
      //Version	V1.10	: (3)20241207		: IO4 Mode Touch 
      
//      static uint16_t last_buttons = 0;
//      uint16_t buttons = IRCode;//AppUser.Value;//button_read();
//      //hid_joy.buttons[0] = native_to_io4(buttons);
//      //hid_joy.buttons[1] = native_to_io4(0);
//      hid_joy_btn1    = native_to_io4(0,buttons);
//      hid_joy_btn2    = native_to_io4(1,0);
//      hid_joy_raw     = hid_joy_btn1|hid_joy_btn2;
//      if ((last_buttons ^ buttons) & (1 << 11)) 
//      {
//        if (buttons & (1 << 11)) 
//        {
//          // just pressed coin button
//          //hid_joy.chutes[0] += 0x100;
//          hid_joy_chutes[0] += 0x100;
//        }
//      }
      
      static uint16_t last_buttons1 = 0;
      static uint16_t last_buttons2 = 0;
//      uint16_t buttons1 = (AppUser.DebKey>> 0)&0xffff;    //AppUser.Value;//button_read();
//      uint16_t buttons2 = (AppUser.DebKey>>16)&0xffff;
      uint16_t buttons1 = ((AppUser.IR_VAL>> 0)&0xffff);// | (AppUser.TPValue&0x3f);    //AppUser.Value;//button_read();
      uint16_t buttons2 = 0;      
      //hid_joy.buttons[0] = native_to_io4(buttons);
      //hid_joy.buttons[1] = native_to_io4(0);
      hid_joy_btn1    = native_to_io4(0,buttons1);
      hid_joy_btn2    = native_to_io4(1,buttons2);
      hid_joy_raw     = hid_joy_btn1|hid_joy_btn2;
      if(last_buttons1!=buttons1 || last_buttons2!=buttons2)
      {
        printf("IO4 : %08X-%08X (%d)\r\n",AppUser.IR_VAL,hid_joy_raw,AppUser.Value);
      }
      if ((last_buttons1 ^ buttons1) & (1 << 11)) 
      {
        if (buttons1 & (1 << 11)) 
        {
          // just pressed coin button
          //hid_joy.chutes[0] += 0x100;
          hid_joy_chutes[0] += 0x100;
        }
      }
      //tud_hid_n_report(0, REPORT_ID_JOYSTICK, &hid_joy, sizeof(hid_joy));
      last_buttons1 = buttons1;
      last_buttons2 = buttons2;
    }
    //if (mai_cfg->hid.nkro && !mai_runtime.key_stuck) 
    //{
    //  tud_hid_n_report(1, 0, &hid_nkro, sizeof(hid_nkro));
    //}
  }
  //------------------------------------------------------------------
//      static uint8_t i=0;
//      static uint32_t last_buttons = 0;
//      for(i=0;i<32;i++)
//      {
//        if(AppUser.DebKey&(1<<i))
//        {
//          hid_joy_raw |= (1<<i);
//        }
//        else
//        {
//          hid_joy_raw &=~(1<<i);
//        }
//      }
//      if(last_buttons!=AppUser.DebKey)
//      {
//        printf("IO4 : %08X-%08X\r\n",AppUser.DebKey,hid_joy_raw);
//      }
//      last_buttons = AppUser.DebKey;
	//------------------------------------------------------------------
  HidReportBuf[0] = USBD_IO4_RP_ID; 
  
  HidReportBuf[1] = 0x00;                   //Left  Stick X 
  HidReportBuf[2] = 0x80;
  HidReportBuf[3] = 0x00;                   //Left  Stick Y
  HidReportBuf[4] = 0x80;
  
  HidReportBuf[5] = 0x00;                   //Left  Stick X 
  HidReportBuf[6] = 0x80;
  HidReportBuf[7] = 0x00;                   //Left  Stick Y
  HidReportBuf[8] = 0x80;

  HidReportBuf[9] = 0x00;                   //Left  Stick X 
  HidReportBuf[10]= 0x80;
  HidReportBuf[11]= 0x00;                   //Left  Stick Y
  HidReportBuf[12]= 0x80;
  
  HidReportBuf[13]= 0x00;                   //Left  Stick X 
  HidReportBuf[14]= 0x80;
  HidReportBuf[15]= 0x00;                   //Left  Stick Y
  HidReportBuf[16]= 0x80;

  HidReportBuf[17]= 0x00;                   //Left  RX 
  HidReportBuf[18]= 0x00;
  HidReportBuf[19]= 0x00;                   //Left  RY
  HidReportBuf[20]= 0x00;

  HidReportBuf[21]= 0x00;                   //Left  RX 
  HidReportBuf[22]= 0x00;
  HidReportBuf[23]= 0x00;                   //Left  RY
  HidReportBuf[24]= 0x00;

  HidReportBuf[25]= (hid_joy_chutes[0]>>0)&0xff;//0x02; //Slider
  HidReportBuf[26]= (hid_joy_chutes[0]>>8)&0xff;//0x00;
  HidReportBuf[27]= (hid_joy_chutes[1]>>0)&0xff;//0x02; //Slider
  HidReportBuf[28]= (hid_joy_chutes[1]>>8)&0xff;//0x00;
  
  if(HIDReportState==0x03)
  {
    HidReportBuf[29]= 0x00;                   //Button 1-16
    HidReportBuf[30]= 0x00;                   // 
    HidReportBuf[31]= 0x00;                   //Button17-32
    HidReportBuf[32]= 0x00;                   // 
    HidReportBuf[33]= 0x00;                   //system_status
    HidReportBuf[34]= 0x00;                   //usb_status  
  }
  else
  {
    HidReportBuf[29]= (hid_joy_raw>> 0)&0xff; //Button 1-16
    HidReportBuf[30]= (hid_joy_raw>> 8)&0xff; // 
    HidReportBuf[31]= (hid_joy_raw>>16)&0xff; //Button17-32
    HidReportBuf[32]= (hid_joy_raw>>24)&0xff; //        
    HidReportBuf[33]= 0x30;                   //system_status
    HidReportBuf[34]= 0x00;                   //usb_status 
  }
  //------------------------------------------------------------------
  App_USB_HID_SendPacket(HidReportBuf,USBD_IO4_REPORT_LEN);  
  //------------------------------------------------------------------
}
#endif

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifdef  SOFT_CONIFG_IO4                         //Version	V1.21	: (2)20250316 	: IO4 MODE Touch Latency
void App_USB_IO4_RGB_Update(void)
{
  if(memcmp((uint8_t *)&rgb_buffer,(uint8_t *)&usb_buffer_bk,101))
  {
    memcpy((uint8_t *)&rgb_buffer,(uint8_t *)&usb_buffer_bk,101);
    //App_BSP_LogBuf((unsigned char*)"RXD",rgb_buffer,101);
    App_Set_IO4_RGB_Value(rgb_buffer,101);
  }
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
#if  SOFT_CONIFG_KBS|SOFT_CONIFG_HID|SOFT_CONIFG_IO4    //Version	V4.00	: (1)20240528		: Composite USB KeyBoard, HID, IO4 Protocol
void App_User_Get_WorkMode(void)
{
	//----------------------------------------------------------
	uint32_t i;
	uint16_t CS=0;
  uint16_t Flash_RW_Buffer[FLASH_BUFEER_SIZE];
	//----------------------------------------------------------
  // read data from flash 
  flash_read(FLASH_DATA_ADDR_WMODE, Flash_RW_Buffer, FLASH_BUFEER_SIZE);
  printf("FlashR:%08X,%d \r\n",FLASH_DATA_ADDR_WMODE,FLASH_BUFEER_SIZE);  
	for(i=0;i<(FLASH_BUFEER_SIZE);)
	{
		printf("%04X", Flash_RW_Buffer[i]);i++;
    printf("%04X-",Flash_RW_Buffer[i]);i++;
    printf("%04X", Flash_RW_Buffer[i]);i++;
    printf("%04X-",Flash_RW_Buffer[i]);i++;
    
    printf("%04X", Flash_RW_Buffer[i]);i++;
    printf("%04X-",Flash_RW_Buffer[i]);i++;
    printf("%04X", Flash_RW_Buffer[i]);i++;
    printf("%04X", Flash_RW_Buffer[i]);i++;
    printf("\r\n");
		//if((i!=0)&&(((i+1)%8)==0))printf("\r\n");
	}
	//printf("\r\n");
	//----------------------------------------------------------
	CS = UPDATE_CRC_16((unsigned char *)Flash_RW_Buffer,4);
	if(Flash_RW_Buffer[0]==0xA55A && Flash_RW_Buffer[3]==CS)
	{
    ConfigMode = Flash_RW_Buffer[1]&0xff;
		printf("Load Flash WorkMode:CS=%04X,%02X\r\n",Flash_RW_Buffer[3],ConfigMode);
	}
	else
	{
    ConfigMode =  USBD_MODE_1PS | USBD_MKEY_32 | USBD_MODE_KBS; //USBD_MODE_HID;// //Version	V2.01	: (1)20250416 	: Default Keyboard Mode
		printf("Load Default WorkMode:%04X,%04X,%02X\r\n",Flash_RW_Buffer[0],Flash_RW_Buffer[3],ConfigMode);
	}
	//----------------------------------------------------------	
}


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
void App_User_Set_WorkMode(void)    //Version	V4.00	: (1)20240528		: Composite USB KeyBoard, HID, IO4 Protocol
{
	//----------------------------------------------------------
	uint32_t i;
  error_status err_status;
	flash_status_type status = FLASH_OPERATE_DONE;
  uint16_t Flash_RW_Buffer[FLASH_BUFEER_SIZE];
	//----------------------------------------------------------
	flash_unlock();
	status = flash_sector_erase(FLASH_DATA_ADDR_WMODE);
	flash_lock();
	printf("Erase Flash: %08X,%08X\r\n",FLASH_DATA_ADDR_WMODE,status);
	//----------------------------------------------------------
  // fill buffer_write data to test 
	Flash_RW_Buffer[0] = 0xA55A;
  Flash_RW_Buffer[1] = ConfigMode;
  Flash_RW_Buffer[2] = ConfigMode;
	Flash_RW_Buffer[3] = UPDATE_CRC_16((unsigned char *)Flash_RW_Buffer,4);
	//----------------------------------------------------------
	printf("FlashW:%08X,%02X,%d \r\n",FLASH_DATA_ADDR_WMODE,err_status,FLASH_BUFEER_SIZE);
	for(i=0;i<(4);i++)
	{
		printf("%04X",Flash_RW_Buffer[i]);
		if((i!=0)&&(((i+1)%8)==0))printf("\r\n");
	}
	printf("\r\n");	
	//----------------------------------------------------------
	// write data to flash 
  err_status = flash_write(FLASH_DATA_ADDR_WMODE, Flash_RW_Buffer, 4);
	//----------------------------------------------------------
  // read data from flash 
  flash_read(FLASH_DATA_ADDR_WMODE, Flash_RW_Buffer, FLASH_BUFEER_SIZE);
	//----------------------------------------------------------
	printf("FlashR:%08X,%d \r\n",FLASH_DATA_ADDR_WMODE,FLASH_BUFEER_SIZE);
//	for(i=0;i<(FLASH_BUFEER_SIZE);i++)
//	{
//		printf("%04X",Flash_RW_Buffer[i]);
//		if((i!=0)&&(((i+1)%8)==0))printf("\r\n");
//	}
//	printf("\r\n");
  
  for(i=0;i<(FLASH_BUFEER_SIZE);)
	{
		printf("%04X", Flash_RW_Buffer[i]);i++;
    printf("%04X-",Flash_RW_Buffer[i]);i++;
    printf("%04X", Flash_RW_Buffer[i]);i++;
    printf("%04X-",Flash_RW_Buffer[i]);i++;
    
    printf("%04X", Flash_RW_Buffer[i]);i++;
    printf("%04X-",Flash_RW_Buffer[i]);i++;
    printf("%04X", Flash_RW_Buffer[i]);i++;
    printf("%04X", Flash_RW_Buffer[i]);i++;
    printf("\r\n");
		//if((i!=0)&&(((i+1)%8)==0))printf("\r\n");
	}
	//----------------------------------------------------------
}

#endif


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
//Version	V1.06	: (1)20240720		: RAW Date Fort App Seni Config 
void App_User_RAW_MODE_Update(unsigned char Index, unsigned short *CMDBuffer,unsigned short CMDLen)
{
  //----------------------------------------------------------
  unsigned short i;
  //----------------------------------------------------------
  usbd_core_type *pudev = (usbd_core_type *)(&otg_core_struct.dev);
  usbd_type_t *vcpkybrd = (usbd_type_t *)pudev->class_handler->pdata; 
  //----------------------------------------------------------
  vcpkybrd->usb_resp_buf[0]=USBD_USER_CMD_SET_MRAW; 
  vcpkybrd->usb_resp_buf[1]=0x4D;				      //M
  vcpkybrd->usb_resp_buf[2]=0x52;				      //R
  vcpkybrd->usb_resp_buf[3]=0x41;				      //A
  vcpkybrd->usb_resp_buf[4]=0x57;				      //W
  //----------------------------------------------------------
  vcpkybrd->usb_resp_buf[5]=(AppUser.KeyCode>>8)&0xff;   
  vcpkybrd->usb_resp_buf[6]=(AppUser.KeyCode>>0)&0xff;   
  vcpkybrd->usb_resp_buf[7]=0xAA;
  //----------------------------------------------------------
  for(i=0;i<CMDLen;i++)
  {
    vcpkybrd->usb_resp_buf[8+i*2+0] = (CMDBuffer[i]>>8)&0xff;
    vcpkybrd->usb_resp_buf[8+i*2+1] = (CMDBuffer[i]>>0)&0xff;
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

Chip1 Sensi: A5 5A 26  52 52 53 50 10 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 25
Chip1 Color: A5 5A 26  52 52 53 50 11 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 44
Chip1 DTick: A5 5A 26  52 52 53 50 12 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 03

Chip2 Sensi: A5 5A 26  52 52 53 50 20 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 03 E8 86  
Chip2 Color: A5 5A 26  52 52 53 50 21 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 00 07 E7 
Chip2 DTick: A5 5A 26  52 52 53 50 22 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 00 03 A0 

******************************************************************************************/
void App_User_Res_Update(usart_type* USARTx, unsigned char *CMDBuffer,unsigned short CMDLen)
{
  //----------------------------------------------------------
  unsigned short i;
  unsigned char CRC_U8=0;
  //----------------------------------------------------------
  usbd_core_type *pudev = (usbd_core_type *)(&otg_core_struct.dev);
  usbd_type_t *vcpkybrd = (usbd_type_t *)pudev->class_handler->pdata; 
  //----------------------------------------------------------
  if((USARTx) == USART3)
  {
    printf("UART3 Rsp Cmd: %02X: Raw: ",CMDLen);
    for(i=0;i<CMDLen;i++)
    {
      printf("%02X ",CMDBuffer[i]);
    }
    //printf("\r\n");

    if(CMDBuffer[0]==PMCU_SYNC_H &&
       CMDBuffer[1]==PMCU_SYNC_L &&
       CMDBuffer[2]==PAKE_LEN)            //Len = 38
    {
      CRC_U8=UPDATE_CRC(&CMDBuffer[3],(CMDBuffer[2]-1));	
      if(CRC_U8 == CMDBuffer[CMDLen-1])
      {
        printf(": Succeed \r\n");
        for(i=0;i<PAKE_LEN;i++)
        {
          vcpkybrd->usb_resp_buf[i] = CMDBuffer[3+i];
        }
      }
      else
      {
        printf(": CRC Err :%02X,%02X\r\n",CRC_U8,CMDBuffer[CMDLen-1]);
      }
    }
    else
    {
      printf(": Sync Err \r\n");
    } 
  }
  //----------------------------------------------------------
  else	if((USARTx) == UART5)
  {
    printf("UART5 Rsp Cmd: %02X: Raw: ",CMDLen);
    for(i=0;i<CMDLen;i++)
    {
      printf("%02X ",CMDBuffer[i]);
    }
    //printf("\r\n"); 
    
    if(CMDBuffer[0]==PMCU_SYNC_H &&
       CMDBuffer[1]==PMCU_SYNC_L &&
       CMDBuffer[2]==PAKE_LEN)            //Len = 38
    {
      CRC_U8=UPDATE_CRC(&CMDBuffer[3],(CMDBuffer[2]-1));	
      if(CRC_U8 == CMDBuffer[CMDLen-1])
      {
        printf(": Succeed \r\n");
        for(i=0;i<PAKE_LEN;i++)
        {
          vcpkybrd->usb_resp_buf[i] = CMDBuffer[3+i];
        }
      }
      else
      {
        printf(": CRC Err :%02X,%02X\r\n",CRC_U8,CMDBuffer[CMDLen-1]);
      }
    }
    else
    {
      printf(": Sync Err \r\n");
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
void App_User_CMD_Report(unsigned char *CMDBuffer,unsigned short CMDLen)  //Version	V4.00	: (1)20240528		: Composite USB KeyBoard, HID, IO4 Protocol
{
  //----------------------------------------------------------
  unsigned short i;
  unsigned char  Index;
  //----------------------------------------------------------
  usbd_core_type *pudev = (usbd_core_type *)(&otg_core_struct.dev);
  usbd_type_t *vcpkybrd = (usbd_type_t *)pudev->class_handler->pdata; 
  //----------------------------------------------------------
  if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_KBS)
  {
    Index = 0;
  }
  //----------------------------------------------------------
  else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_HID)
  {
    Index = 0;
  }
  //----------------------------------------------------------
  else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_IO4)
  {
    Index = 1;  //CMDBuffer[0]        //Report ID
  }  
  //----------------------------------------------------------
  printf("USB Cmd: %02X, %02X: Raw: ",Index,CMDLen);
  for(i=0;i<8;i++)
  {
    printf("%02X ",CMDBuffer[i]);
  }
  printf("\r\n");  
  //----------------------------------------------------------
  switch(CMDBuffer[Index])  //User Command
  {
//    //--------------------------------------------------------
//    //Version	V1.06	: (1)20240720		: RAW Date Fort App Seni Config 
//    case  USBD_USER_CMD_SET_MRAW:
//    {
//      if( CMDBuffer[Index+1]==0x52 && //R
//          CMDBuffer[Index+2]==0x41 && //A
//          CMDBuffer[Index+3]==0x57 && //W
//          CMDBuffer[Index+4]==0x4D && //M
//          CMDBuffer[Index+5]==0x44 )  //D
//      {
//        printf("Enter RAW Mode \r\n");
//        AppUser.MDRAW_FG  = 1;
//        App_User_RAW_MODE_Update(Index, AppUser.Max,ADC_MAX_CHAN);
//        for(i=0;i<ADC_MAX_CHAN;i++)
//        {
//          vcpkybrd->usb_resp_buf[5+i*2+0] = 0x00;
//          vcpkybrd->usb_resp_buf[5+i*2+1] = 0x00;
//        }
//      }
//      else if(CMDBuffer[Index+1]==0x4E && //N
//              CMDBuffer[Index+2]==0x4F && //O
//              CMDBuffer[Index+3]==0x4C && //L
//              CMDBuffer[Index+4]==0x4D && //M
//              CMDBuffer[Index+5]==0x44 )  //D
//      {
//        printf("Exit RAW Mode \r\n");
//        AppUser.MDRAW_FG  = 0;  
//        for(i=0;i<ADC_MAX_CHAN;i++)
//        {
//          vcpkybrd->usb_resp_buf[5+i*2+0] = 0x00;
//          vcpkybrd->usb_resp_buf[5+i*2+1] = 0x00;
//        }
//      }
//      else
//      {
//        printf("Clear Buffer \r\n");
//        for(i=0;i<ADC_MAX_CHAN;i++)
//        {
//          vcpkybrd->usb_resp_buf[5+i*2+0] = 0x00;
//          vcpkybrd->usb_resp_buf[5+i*2+1] = 0x00;
//        }
//      }
//      break;
//    }
    //--------------------------------------------------------
    case  USBD_USER_CMD_SET_OTA://Version	V1.02	: (1)20240715		: Enter OTA By App
    {
      flash_unlock();	
      flash_sector_erase(IAP_CMD_FINISH_FLAG_ADDR);
      flash_lock();
      printf("Erase Flash: %08X\r\n",IAP_CMD_FINISH_FLAG_ADDR);
      
      printf("MCU ReBoot: \r\n");
      App_BSP_Delay_MS(100);
      nvic_system_reset();
      break;
    }
    //--------------------------------------------------------
    case  USBD_USER_CMD_GET_MODE:
    {
      //Version	V1.06	: (1)20240922		: Production ID and FW Version
      vcpkybrd->usb_resp_buf[0] = USBD_USER_CMD_GET_MODE;
      vcpkybrd->usb_resp_buf[1] = ConfigMode;
      vcpkybrd->usb_resp_buf[2] = PRODUCTION_ID;      //Version	V1.14	: (1)20240921		: Production ID and FW Version
      vcpkybrd->usb_resp_buf[3] = HW_VERSION;
      vcpkybrd->usb_resp_buf[4] = FW_VERSION_MAJOR;
      vcpkybrd->usb_resp_buf[5] = FW_VERSION_MINOR;
      vcpkybrd->usb_resp_buf[7]++;
      
      printf("USB Cmd: Get Work Mode: %02X (%02X-%02X-%02X-%02X)\r\n",vcpkybrd->usb_resp_buf[1],vcpkybrd->usb_resp_buf[2],vcpkybrd->usb_resp_buf[3],vcpkybrd->usb_resp_buf[4],vcpkybrd->usb_resp_buf[5]);
      break;
    }
    //--------------------------------------------------------
    case  USBD_USER_CMD_SET_MODE:
    {
      ConfigMode  = CMDBuffer[Index+1];
      
      printf("USB Cmd: Set Work Mode: %02X\r\n",ConfigMode);
      
      App_User_Set_WorkMode();
      
      printf("MCU ReBoot: \r\n");
      App_BSP_Delay_MS(100);
      nvic_system_reset();
      //NVIC_SystemReset();
      
      break;
    }
    //--------------------------------------------------------
    case  USBD_USER_CMD_GET_SENS:
    {
      App_TP_Sens_Command_Report(&CMDBuffer[Index],CMDLen,vcpkybrd->usb_resp_buf,64);
      break;
    }
    //--------------------------------------------------------
    case  USBD_USER_CMD_SET_SENS:
    {
      App_TP_Sens_Command_Report(&CMDBuffer[Index],CMDLen,vcpkybrd->usb_resp_buf,64);
      break;
    }
    //--------------------------------------------------------
    case  USBD_USER_CMD_GET_MAPP:
    {
      App_MAPPING_Command_Report(&CMDBuffer[Index],CMDLen,vcpkybrd->usb_resp_buf,64);
      break;
    }
    //--------------------------------------------------------
    case  USBD_USER_CMD_SET_MAPP:
    {
      App_MAPPING_Command_Report(&CMDBuffer[Index],CMDLen,vcpkybrd->usb_resp_buf,64);
      break;
    }   
    //--------------------------------------------------------
    default:break;
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
void App_User_USB_CMD_Update(void)  //Version	V4.00	: (1)20240528		: Composite USB KeyBoard, HID, IO4 Protocol
{
  //----------------------------------------------------------
  usbd_core_type *pudev = (usbd_core_type *)(&otg_core_struct.dev);
  usbd_type_t *vcpkybrd = (usbd_type_t *)pudev->class_handler->pdata; 
	//----------------------------------------------------------
  if(usbd_connect_state_get(pudev) == USB_CONN_STATE_CONFIGURED)	
	{
    if(vcpkybrd->g_usb_cmd_flag  == HID_REQ_SET_REPORT)
    {
      App_User_CMD_Report(vcpkybrd->usb_comd_buf,vcpkybrd->m_usb_cmd_len);
      vcpkybrd->g_usb_cmd_flag  = 0;
           
//      vcpkybrd->usb_resp_buf[0] = USBD_IO4_RP_ID;
//      vcpkybrd->usb_resp_buf[1] = vcpkybrd->usb_comd_buf[1];
//      vcpkybrd->usb_resp_buf[2] = vcpkybrd->usb_comd_buf[2];
//      vcpkybrd->usb_resp_buf[3] = vcpkybrd->usb_comd_buf[3];
//      vcpkybrd->usb_resp_buf[10]++;
    }
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
void App_Debug_CMD_Report(unsigned char *CMDBuffer,unsigned char CMDLen)
{
	//------------------------------------------------------------------
//	static unsigned char 	TestIndex=0,TestFlag=0,PortBuf,HallVal;
//	static unsigned short ADCVAL0,ADCVAL1;
//	APP_USER_LOG(("User APP: RXD: %c,%d\n",CMDBuffer[0],TestIndex));
//	//------------------------------------------------------------------
//	switch(CMDBuffer[0])
//	{
//		//----------------------------------------------------------------
//		case	'p':{	IO_PWR_EN_ON();		break;	}		//
//		//----------------------------------------------------------------
//		case	'o':{	IO_PWR_EN_OFF();	break;	}		//
//		//----------------------------------------------------------------
//		case	'f':
//		{	
//			if(TestFlag==0){ TestFlag = 1;} else{TestFlag = 0;	}
//			SET_MOTO_OUT(TestIndex,TestFlag);
//			break;	
//		}
//		//----------------------------------------------------------------
//		case	'b':
//		{	
//			if(TestFlag==0){ TestFlag = 1;} else{TestFlag = 0;	}
//			SET_MOTO_BAK(TestIndex,TestFlag);
//			break;	
//		}
//		//----------------------------------------------------------------
//		case	'1':{	TestIndex = MOTO_INDEX_01; 	break;	}		//
//		//----------------------------------------------------------------
//		case	'2':{	TestIndex = MOTO_INDEX_02; 	break;	}		//
//		//----------------------------------------------------------------
//		case	'3':{	TestIndex = MOTO_INDEX_03; 	break;	}		//
//		//----------------------------------------------------------------
//		case	'4':{	TestIndex = MOTO_INDEX_04; 	break;	}		//
//		//----------------------------------------------------------------
//		case	'5':{	TestIndex = MOTO_INDEX_05; 	break;	}		//
//		//----------------------------------------------------------------
//		case	'6':{	TestIndex = MOTO_INDEX_06; 	break;	}		//
//		//----------------------------------------------------------------
//		case	'7':{	TestIndex = MOTO_INDEX_07; 	break;	}		//
//		//----------------------------------------------------------------
//		case	'8':{	TestIndex = MOTO_INDEX_08; 	break;	}		//
//		//----------------------------------------------------------------
//		case	'r':
//		{	
//			if(IO_MOTO_DT_1_READ()!=0)	PortBuf |= 0x01;
//			else												PortBuf &= 0xfe;
//			if(IO_MOTO_DT_2_READ()!=0)	PortBuf |= 0x02;
//			else												PortBuf &= 0xfd;
//			if(IO_CHRG_PWR_DT_READ()!=0)PortBuf |= 0x04;
//			else												PortBuf &= 0xfb;
//			
//			APP_USER_LOG(("User APP: VAL: %02X\n",PortBuf));
//			break;	
//		}
//		//----------------------------------------------------------------
//		case	'a':
//		{	
//			ADCVAL0 = GetADCValue(0);
//			APP_USER_LOG(("User APP: ADC: %04d,%04d\n",ADCVAL0,ADCVAL1));
//			break;	
//		}
//		//----------------------------------------------------------------
//		case	'c':
//		{	
//			ADCVAL1 = GetADCValue(1);
//			APP_USER_LOG(("User APP: ADC: %04d,%04d\n",ADCVAL0,ADCVAL1));
//			break;	
//		}
//		//----------------------------------------------------------------
//		default:
//		{
//			break;
//		}
//		//----------------------------------------------------------------
//	}
//	//------------------------------------------------------------------	
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
void usb_clock48m_select(usb_clk48_s clk_s)
{
	//----------------------------------------------------------
	switch(system_core_clock)
	{
		// 48MHz 
		case 48000000:
		crm_usb_clock_div_set(CRM_USB_DIV_1);
		break;

		// 72MHz 
		case 72000000:
		crm_usb_clock_div_set(CRM_USB_DIV_1_5);
		break;

		// 96MHz 
		case 96000000:
		crm_usb_clock_div_set(CRM_USB_DIV_2);
		break;

		// 120MHz 
		case 120000000:
		crm_usb_clock_div_set(CRM_USB_DIV_2_5);
		break;

		// 144MHz 
		case 144000000:
		crm_usb_clock_div_set(CRM_USB_DIV_3);
		break;

		default:
		break;
	}
	//----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void usb_gpio_config(void)
{
	//----------------------------------------------------------
  gpio_init_type gpio_init_struct;
	//----------------------------------------------------------
  crm_periph_clock_enable(OTG_PIN_GPIO_CLOCK, TRUE);
  gpio_default_para_init(&gpio_init_struct);
	//----------------------------------------------------------
  gpio_init_struct.gpio_drive_strength 	= GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  			= GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode 						= GPIO_MODE_MUX;
  gpio_init_struct.gpio_pull 						= GPIO_PULL_NONE;
	//----------------------------------------------------------
	#ifdef USB_SOF_OUTPUT_ENABLE
  crm_periph_clock_enable(OTG_PIN_SOF_GPIO_CLOCK, TRUE);
  gpio_init_struct.gpio_pins = OTG_PIN_SOF;
  gpio_init(OTG_PIN_SOF_GPIO, &gpio_init_struct);
	#endif
	//----------------------------------------------------------
  // otgfs use vbus pin 
	#ifndef USB_VBUS_IGNORE
  gpio_init_struct.gpio_pins = OTG_PIN_VBUS;
  gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init(OTG_PIN_GPIO, &gpio_init_struct);
	#endif
	//----------------------------------------------------------
}


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifdef USB_LOW_POWER_WAKUP
void usb_low_power_wakeup_config(void)
{
	//----------------------------------------------------------
  exint_init_type exint_init_struct;
	//----------------------------------------------------------
  exint_default_para_init(&exint_init_struct);
	//----------------------------------------------------------
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = OTG_WKUP_EXINT_LINE;
  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
  exint_init(&exint_init_struct);
	//----------------------------------------------------------
  nvic_irq_enable(OTG_WKUP_IRQ, 0, 0);
	//----------------------------------------------------------
}
#endif

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
#ifdef USB_LOW_POWER_WAKUP
void OTG_WKUP_HANDLER(void)
{
	//----------------------------------------------------------
  exint_flag_clear(OTG_WKUP_EXINT_LINE);
	//----------------------------------------------------------
}
#endif

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void OTG_IRQ_HANDLER(void)
{
	//----------------------------------------------------------
  usbd_irq_handler(&otg_core_struct);
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
void WDT_Config(void)
{
  //----------------------------------------------------------
  // disable register write protection 
  wdt_register_write_enable(TRUE);
	//----------------------------------------------------------
  // set the wdt divider value */
  wdt_divider_set(WDT_CLK_DIV_32);
  //----------------------------------------------------------
  // set reload value
  // timeout      = reload_value * (divider / lick_freq )    (s)
  // lick_freq    = 40000 Hz
  // divider      = 32
  // reload_value = 3000
  // timeout      = 3000 * (32 / 40000 ) = 2.4s = 2400ms
  //----------------------------------------------------------
  wdt_reload_value_set(3000 - 1);
  //----------------------------------------------------------
  // enable wdt 
  wdt_enable(); 
  //----------------------------------------------------------  
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void Get_PCBA_Version(void) //Version	V1.24	: (1)20250327 	: IO4 MODE Touch Issue
{
  //----------------------------------------------------------
  unsigned char PCBA_Version = 0x10;
  //----------------------------------------------------------
  IO_OPT_1_INPUT();
  IO_OPT_2_INPUT();
  //----------------------------------------------------------
  if(IO_OPT_1_READ()==0)
  {
    App_BSP_Delay_MS(2);
    if(IO_OPT_1_READ()==0)
    {
       App_BSP_Delay_MS(2);
      if(IO_OPT_1_READ()==0)
      {
        PCBA_Version = 0x20;
      }
    }
  }
  //----------------------------------------------------------
  if(IO_OPT_2_READ()==0)
  {
    App_BSP_Delay_MS(2);
    if(IO_OPT_2_READ()==0)
    {
       App_BSP_Delay_MS(2);
      if(IO_OPT_2_READ()==0)
      {
        PCBA_Version = 0x21;
      }
    }
  }
  //----------------------------------------------------------
  if(PCBA_Version>=0x20)  //Version	V2.00	: (1)20250401 	: For New IR Sensor Base On V1.26
  {
    printf("PCBA Version:%02X \r\n",PCBA_Version);
  }
  else
  {
    printf("InValid PCBA Version:%02X \r\n",PCBA_Version);
    NVIC_SystemReset();
  }
  //----------------------------------------------------------
}


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void init_devices(void)
{
  //----------------------------------------------------------
  crm_clocks_freq_type clocks_freq;
  //----------------------------------------------------------
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  //----------------------------------------------------------
  system_clock_config();
  //----------------------------------------------------------
  MCU_GPIO_Config();
  //----------------------------------------------------------
	App_UART_Config();
  //----------------------------------------------------------
  crm_clocks_freq_get(&clocks_freq);  
  printf("AppUser Start :%d.%02X\r\n",FW_VERSION_MAJOR,FW_VERSION_MINOR);
  printf("SYSCLK_Freq  = %04dMHz\r\n",system_core_clock/1000000);
  printf("AHBCLK_Freq  = %04dMHz\r\n",clocks_freq.ahb_freq/1000000);
  printf("SCLK_Freq    = %04dMHz\r\n",clocks_freq.sclk_freq/1000000);
  printf("APB1CLK_Freq = %04dMHz\r\n",clocks_freq.apb1_freq/1000000);
  printf("APB2CLK_Freq = %04dMHz\r\n",clocks_freq.apb2_freq/1000000);
  printf("ADCCLK_Freq  = %04dMHz\r\n",clocks_freq.adc_freq/1000000);
  //----------------------------------------------------------
  printf("MCU_FLAG1 :%08X,%08X\r\n",MCU_FLAG1,*(uint32_t*)(MCU_FLAG1));
  printf("MCU_FLAG2 :%08X,%08X\r\n",MCU_FLAG2,*(uint32_t*)(MCU_FLAG2));
  printf("MCU_FLAG3 :%08X,%08X\r\n",MCU_FLAG3,*(uint32_t*)(MCU_FLAG3));
  printf("MCU_FLAG4 :%08X,%08X\r\n",MCU_FLAG4,*(uint32_t*)(MCU_FLAG4));
  //----------------------------------------------------------
  if(*(uint32_t*)(MCU_FLAG4)!=0x31313332)
  {
    printf("Unkown ChipDevice \r\n");
    NVIC_SystemReset();
  }
  //----------------------------------------------------------
  Get_PCBA_Version(); //Version	V1.24	: (1)20250327 	: IO4 MODE Touch Issue
  //----------------------------------------------------------
  //Version	V4.00	: (1)20240528		: Composite USB KeyBoard, HID, IO4 Protocol
  #if  SOFT_CONIFG_KBS|SOFT_CONIFG_HID|SOFT_CONIFG_IO4
  App_User_Get_WorkMode();
  
  if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_KBS)     {printf("USB Work Mode : KBS");}
  else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_HID){printf("USB Work Mode : HID");}
  else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_IO4){printf("USB Work Mode : IO4");}
  
  if((ConfigMode&USBD_MKEY_MAK)==USBD_MKEY_32)     {printf(" 32Key");}
  else if((ConfigMode&USBD_MKEY_MAK)==USBD_MKEY_16){printf(" 16Key");}
  else if((ConfigMode&USBD_MKEY_MAK)==USBD_MKEY_08){printf(" 08Key");}
  else if((ConfigMode&USBD_MKEY_MAK)==USBD_MKEY_04){printf(" 04Key");}
  
  if((ConfigMode&USBD_MSEL_MAK)==USBD_MODE_1PS)     {printf(" 1P \r\n");}
  else                                              {printf(" 2P \r\n");}

  //ConfigMode =  USBD_MODE_1PS | USBD_MODE_IO4;
  //App_User_Set_WorkMode();
  #endif
  //----------------------------------------------------------
	App_User_Init();
  //----------------------------------------------------------
  if(crm_flag_get(CRM_WDT_RESET_FLAG) != RESET)
  {
    // reset from wdt 
    crm_flag_clear(CRM_WDT_RESET_FLAG);
    printf("Reset From WDT ------------------ \r\n");
  }
   //----------------------------------------------------------
  printf("TMR1 Config: 1mS TimeTick \r\n");
  Timer1_Config();
  //----------------------------------------------------------
	#ifdef	APP_HW_TP_MODE
	//App_HW_I2C_Config();
	//printf("I2C HW Config End \r\n");
	App_CHSC6X_TP_Config();
	#endif
  //----------------------------------------------------------  
  //printf("TMR5 Config: PWM Timer \r\n");
  //Timer5_PWM_Config();
  //----------------------------------------------------------
	//printf("Exti Interrupt Config: PA0 as EXTI0 \r\n");
	//EXTI_INT0_Config();
  //----------------------------------------------------------
  printf("App ADC Config \r\n"); 
  #ifdef  ADC_DMA_ENABLE
  MCU_DMA_Config_For_ADC();
  MCU_ADC_Config_With_DMA();
  #else
  MCU_ADC_Config();
  #endif
  //----------------------------------------------------------
  // usb gpio config 
  usb_gpio_config();
  //----------------------------------------------------------
  #ifdef USB_LOW_POWER_WAKUP
  usb_low_power_wakeup_config();
  #endif
  //----------------------------------------------------------
  // enable otgfs clock 
  crm_periph_clock_enable(OTG_CLOCK, TRUE);
  //----------------------------------------------------------
  // select usb 48m clcok source 
  usb_clock48m_select(USB_CLK_HEXT);
  //----------------------------------------------------------
  // enable otgfs irq 
  nvic_irq_enable(OTG_IRQ, 0, 0);
  //----------------------------------------------------------
  // init usb
  usbd_init(&otg_core_struct,
            USB_FULL_SPEED_CORE_ID,
            USB_ID,
            &user_usbd_class_handler,
            &cdc_keyboard_desc_handler);      
  //----------------------------------------------------------
  WDT_Config();  
  //----------------------------------------------------------
	printf("MCU Config End \r\n");	  
  //----------------------------------------------------------
}




//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
unsigned char i = 0,flag=0;
int main(void)
{
	//*********************************************************
	//
	//	System	 Int
	//
	//*********************************************************		
	init_devices(); 
	//*********************************************************
	//
	//	Main 	function	Loop
	//
	//*********************************************************
  while(1)
  {
    //-------------------------------------------------------
    App_User_Loop();
		//-------------------------------------------------------
    #ifdef  SOFT_CONIFG_CDC
		data_len = usbd_cdc_get_rxdata(&otg_core_struct.dev, usb_buffer);
		if(data_len>0)
		{
//      flag = 0;
//      for(i=0;i<data_len;i++)
//      {
//        if(usb_buffer_bk[i]!=usb_buffer[i])
//        {
//          usb_buffer_bk[i] = usb_buffer[i];
//          flag = 1;
//        }
//      }
//      
//      if(flag!=0)
//      {
//        App_BSP_LogBuf((unsigned char*)"RXD",usb_buffer,data_len);
//      }
      
      
 
      if(usb_buffer[0]==0xff) 
      {
        //Version	V1.19	: (1)20250309 	: IO4 MODE RGB Game Ctrl Support
//        flag = 0;
//        for(i=0;i<data_len;i++)
//        {
//          if(usb_buffer_bk[i]!=usb_buffer[i])
//          {
//            usb_buffer_bk[i] = usb_buffer[i];
//            flag = 1;
//          }
//        }

        //Version	V1.20	: (1)20250310 	: IO4 MODE Touch Latency
        memcpy((uint8_t *)&usb_buffer_bk,(uint8_t *)&usb_buffer,data_len);
        
        //printf("RXD:%d, %02X%02X%02X%02X%02X%02X%02X%02X, %02X ",data_len,usb_buffer[0],usb_buffer[1],usb_buffer[2],usb_buffer[3],usb_buffer[4],usb_buffer[5],usb_buffer[6],usb_buffer[7],usb_buffer[data_len-1]);
//Version	V1.22	: (1)20250318 	: IO4 MODE Touch Latency 
//        printf("RXD:%d, %02X%02X%02X%02X %02X ",data_len,usb_buffer[0],usb_buffer[1],usb_buffer[2],usb_buffer[3],usb_buffer[data_len-1]);
//        AppUser.TPKeyRDFG = 1; //Version	V1.10	: (3)20241207		: IO4 Mode Touch 
      }
      else
      {
        //Version	V1.19	: (1)20250309 	: IO4 MODE RGB Game Ctrl Support
//        flag = 0;
//        for(i=0;i<data_len;i++)
//        {
//          if(usb_buffer_bk[64+i]!=usb_buffer[i])
//          {
//            usb_buffer_bk[64+i] = usb_buffer[i];
//            flag = 1;
//          }
//        }
//        
//        if(flag!=0)
//        {
//          App_BSP_LogBuf((unsigned char*)"RXD",usb_buffer_bk,101);
//          
//          App_Set_IO4_RGB_Value(usb_buffer_bk,101);
//        } 

        //Version	V1.20	: (1)20250310 	: IO4 MODE Touch Latency
        memcpy((uint8_t *)&usb_buffer_bk[64],(uint8_t *)&usb_buffer,data_len);
//        if(memcmp((uint8_t *)&rgb_buffer,(uint8_t *)&usb_buffer_bk,101))  //Version	V1.21	: (2)20250316 	: IO4 MODE Touch Latency
//        {
//          memcpy((uint8_t *)&rgb_buffer,(uint8_t *)&usb_buffer_bk,101);
//          //App_BSP_LogBuf((unsigned char*)"RXD",rgb_buffer,101);
//          App_Set_IO4_RGB_Value(rgb_buffer,101);
//        }
        
        //Version	V1.15	: (1)20250222 	: Touch Crush Issue
        //printf("RXD:%d, %02X%02X%02X%02X%02X%02X%02X%02X, %02X \r\n",data_len,usb_buffer[0],usb_buffer[1],usb_buffer[2],usb_buffer[3],usb_buffer[4],usb_buffer[5],usb_buffer[6],usb_buffer[7],usb_buffer[data_len-1]);
        //printf("RXD:%d, %02X%02X%02X%02X %02X \r\n",data_len,usb_buffer[0],usb_buffer[1],usb_buffer[2],usb_buffer[3],usb_buffer[data_len-1]);
        //AppUser.TPKeyRDFG = 1; //Version	V1.10	: (3)20241207		: IO4 Mode Touch 
      }
      
      //App_USB_CDC_SendPacket(usb_buffer,data_len);
			//App_Game_Command_Report(usb_buffer,data_len);
      if(usb_buffer[0]==0xff && data_len >= 4)
      {
        if(usb_buffer[1] == 0x10 && usb_buffer[2] == 0x00 && usb_buffer[3] == 0xF1)
        {
          IO4TouchState = 0;
          App_USB_IO4_TouchRsp(0);
        }
        else if(usb_buffer[1] == 0xf0 && usb_buffer[2] == 0x00 && usb_buffer[3] == 0x11)
        {
          IO4TouchState = 1;
          App_USB_IO4_TouchRsp(1);
        }
        else if(usb_buffer[1] == 0x03 && usb_buffer[2] == 0x00 && usb_buffer[3] == 0xfe)
        {
          IO4TouchState = 2;
          App_USB_IO4_TouchRsp(2);
        }
        else if(usb_buffer[1] == 0x04 && usb_buffer[2] == 0x00 && usb_buffer[3] == 0xfd && usb_buffer[4] == 0xfc)
        {
          IO4TouchState = 3;
          App_USB_IO4_TouchRsp(3);
        }
      }
		}
    #endif
    //-------------------------------------------------------
    //Version	V3.04	: (1)20240522		: Support Maimai IO4 Protocol
    #if   SOFT_CONIFG_KBS|SOFT_CONIFG_HID|SOFT_CONIFG_IO4
    data_len = usbd_hid_get_rxdata(&otg_core_struct.dev, usb_buffer);  
    if(data_len>0)
		{
      //App_BSP_LogBuf((unsigned char*)"HID RXD",usb_buffer,data_len);
      
      //---------------------------------------------------------------
      if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_KBS)     
      {
        #ifdef  SOFT_CONIFG_KBS

        #endif
      }
      else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_HID)
      {
        #ifdef  SOFT_CONIFG_HID
        App_Set_HID_RGB_Value(usb_buffer,data_len);
        #endif
      }
      else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_IO4)
      {
        #ifdef   SOFT_CONIFG_IO4
        if(usb_buffer[0]==0x10)
        {
          HIDReportState = usb_buffer[1];
        }
        #endif
      }
    }
    App_User_USB_CMD_Update();  //Version	V4.00	: (1)20240528		: Composite USB KeyBoard, HID, IO4 Protocol
    #endif
		//-------------------------------------------------------
  }
	//*********************************************************
	//
	//	Main 	function	End
	//
	//*********************************************************  	
}


