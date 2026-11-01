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
#include 		"App_SW_I2C.h"
#include 		"App_UART.h"
#include 		"App_User.h"
#include 		"App_RGB.h"
#include 		"App_BSP.h"
#include    "Chip2813.h"
#include    "App_CHSC6X_TP.h"

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
#define 	RGB_VAL     128   

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

AppRGB_t					AppRGB;
Packet_t					AppPacket;

unsigned char HID_RGB_EN;//Version	V1.17	: (1)20250227 	: HID MODE RGB Display    


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_HC595_Drive1(unsigned char data)
{
//  //------------------------------------------------------------------
//  unsigned char i;
//  //------------------------------------------------------------------
//  //׼Ѹ˽ߝìҢ؅ս˽ߝIOࠚʏ
//  for(i = 0;i < 8;i++)
//  {
//    SW_595_CLK_LOW();
//    if(data >= 0x80)
//    {
//      SW_595_DAT_HIGH();  //خٟλΪ1    
//    }
//    else
//    {
//      SW_595_DAT_LOW();   //خٟλΪ0   
//    }
//    SW_595_CLK_HIGH();    //ʏʽҘì˽ߝһλһλքӆɫӆλ݄զǷ
//    data <<= 1;
//  }
//  //------------------------------------------------------------------
//  //ʏʽҘìӆλ݄զǷք˽ߝӆսզԢ݄զǷ
//  SW_595_RCK_LOW();
//  SW_595_RCK_HIGH();
//  //------------------------------------------------------------------
//  //-֍ìٟ͡߹ىɅ
//  //SW_595_DAT_LOW(); 
//  //SW_595_CLK_LOW();
//  SW_595_RCK_LOW();
//  //------------------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_HC595_Drive2(unsigned char dataH, unsigned char dataL)
{
//  //------------------------------------------------------------------
//  unsigned char i;
//  unsigned char temp = 0x00;
//  //------------------------------------------------------------------dataH prepare  
//  temp = dataH;
//  for(i = 0;i < 8;i++)
//  {
//    SW_595_CLK_LOW();  
//    if((temp & 0x80) == 0x80)
//    {
//      SW_595_DAT_HIGH();  //خٟλΪ1    
//    }
//    else
//    {
//      SW_595_DAT_LOW();   //خٟλΪ0   
//    }
//    SW_595_CLK_HIGH();    //ʏʽҘì˽ߝһλһλքӆɫӆλ݄զǷ  
//    //SW_595_RCK_LOW();       
//    //SW_595_RCK_HIGH(); 
//    temp <<= 1;
//  }
//  //------------------------------------------------------------------dataL prepare 
//  temp = dataL;
//  for(i = 0;i < 8;i++)
//  {
//    SW_595_CLK_LOW();    
//    if((temp & 0x80) == 0x80)
//    {
//      SW_595_DAT_HIGH();  //خٟλΪ1    
//    }
//    else
//    {
//      SW_595_DAT_LOW();   //خٟλΪ0   
//    }
//    SW_595_CLK_HIGH();
//    //SW_595_RCK_LOW(); //ʏʽҘì˽ߝһλһλքӆɫӆλ݄զǷ
//    //SW_595_RCK_HIGH();
//    temp <<= 1;
//  }
//  //------------------------------------------------------------------data:shift --> ROM 
//  //ʏʽҘìӆλ݄զǷք˽ߝӆսզԢ݄զǷ
//  SW_595_RCK_LOW();    
//  SW_595_RCK_HIGH();
//  //-֍ìٟ͡߹ىɅ
//  //SW_595_DAT_LOW(); 
//  //SW_595_CLK_LOW();
//  SW_595_RCK_LOW(); 
//  //------------------------------------------------------------------  
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
void	Send_RGB_1_Data (unsigned char *r,unsigned char *g,unsigned char *b)	
{
//  unsigned char  i;
//	//-------------------------------------------------- 
//	if(AppRGB.PwrFlag)		
//	{
//    for(i=0;i<MAX_RGB_NUMBER;i++)
//    {
//      AppRGB.RGB_BUF1[i*3+0]= r[i];
//      AppRGB.RGB_BUF1[i*3+1]= g[i];
//      AppRGB.RGB_BUF1[i*3+2]= b[i];
//    }
//		Chip2813_C1_Send_Data(AppRGB.RGB_BUF1,MAX_RGB_NUMBER*3);
//	}
//	//-------------------------------------------------- 	
}  
/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void Send_RGB_2_Data(unsigned char *r,unsigned char *g,unsigned char *b)
{
//  unsigned char  i;
//	//----------------------------------------------------------
//	if(AppRGB.PwrFlag)		
//	{
//    for(i=0;i<MAX_RGB_NUMBER;i++)
//    {
//      AppRGB.RGB_BUF2[i*3+0]= r[i];
//      AppRGB.RGB_BUF2[i*3+1]= g[i];
//      AppRGB.RGB_BUF2[i*3+2]= b[i];
//    }
//    Chip2813_C2_Send_Data(AppRGB.RGB_BUF2,MAX_RGB_NUMBER*3);
//  }
//	//----------------------------------------------------------
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
//void App_RGB_Brightness_Update(void)	//Version	V1.08	: (1)20241016		: Brightness For RGB  
//{
//  //----------------------------------------------------------
//  if(AppRGB.BrightStep==0)      {AppRGB.Brightness = RGB_VAL+35*2;}  //RGB_VAL=64 
//  else if(AppRGB.BrightStep==1) {AppRGB.Brightness = RGB_VAL+35*1;}  //RGB_VAL=56  
//  else if(AppRGB.BrightStep==2) {AppRGB.Brightness = RGB_VAL-35*0;}  //RGB_VAL=48
//  else if(AppRGB.BrightStep==3) {AppRGB.Brightness = RGB_VAL-35*1;}  //RGB_VAL=40
//  else if(AppRGB.BrightStep==4) {AppRGB.Brightness = RGB_VAL-35*2;}  //RGB_VAL=32
//  //----------------------------------------------------------
//  AppRGB.Brightness /= AppRGB.BrightCutf;
//  //----------------------------------------------------------
//}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void App_RGB_CC_PowerUpdate(unsigned int Value)	
{
  //----------------------------------------------------------
  if(Value>=1300)                   //5V/3A
  {
    if(AppRGB.BrightCutf!=1)
    {
      AppRGB.BrightCutf = 1;
      AppRGB.Brightness = 0xff;//100;       //Version	V1.08	: (1)20241016		: Brightness For RGB  
      printf("CC Value: %d,%d,%d\r\n",Value,AppRGB.BrightCutf,AppRGB.Brightness);  
      //App_RGB_Brightness_Update();
    }
  }
  else if(Value>=700)               //5V/1.5A
  {
    if(AppRGB.BrightCutf!=2)
    {
      AppRGB.BrightCutf = 2;
      AppRGB.Brightness = 191;//75;         //Version	V1.08	: (1)20241016		: Brightness For RGB  
      printf("CC Value: %d,%d,%d\r\n",Value,AppRGB.BrightCutf,AppRGB.Brightness);  
      //App_RGB_Brightness_Update();
    }
  }
  else                              //5V/0.5&0.9A             
  {
    if(AppRGB.BrightCutf!=4)
    {
      AppRGB.BrightCutf = 4;
      AppRGB.Brightness = 63;//25;          //Version	V1.08	: (1)20241016		: Brightness For RGB  
      printf("CC Value: %d,%d,%d\r\n",Value,AppRGB.BrightCutf,AppRGB.Brightness);  
      //App_RGB_Brightness_Update();
    }
  }
  //----------------------------------------------------------
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

/*******************************************************************************
* Function Name  : 	
* Description    : 	
* Input          : 
* Output         : 
* Return         : 
* Attention		 	 : 
*******************************************************************************/
void App_Set_IR_RGB_Value(unsigned short KeyCode)
{
  //----------------------------------------------------------
  //unsigned char  i,c;//,LED_L=0,LED_H=0;        //Version	V1.08	: (1)20241016		: Brightness For RGB  
  //unsigned short LED_Val=0;                     //Version	V1.08	: (1)20241016		: Brightness For RGB  
  unsigned char  Index=0;
  //unsigned char  Color[12];                     //Version	V1.08	: (1)20241016		: Brightness For RGB  
  
  //Version	V1.09	: (1)20241020		: IR RGB Mode
  unsigned char  ONBright1 = AppRGB.Brightness;    
  unsigned char  ONBright2 = AppRGB.Brightness/2; 
  unsigned char  OFFBright1= AppRGB.Brightness/8;
  unsigned char  OFFBright2= AppRGB.Brightness/16;
    
//  unsigned char  ONBright = AppRGB.Brightness;    //Version	V1.08	: (1)20241016		: Brightness For RGB  
//  unsigned char  OFFBright= AppRGB.Brightness/8;  //Version	V1.08	: (1)20241016		: Brightness For RGB  
  //----------------------------------------------------------
  Index = 0;
  if((KeyCode&KEY_CODE_01)!=0)
  {
    AppRGB.RGB_BUF_L[Index*3+0] = 0x00;       //ONBright; //Color[Index*3+0]; //G //Version	V1.09	: (1)20241020		: IR RGB Mode
    AppRGB.RGB_BUF_L[Index*3+1] = ONBright1;  //ONBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+2] = ONBright2;  //ONBright; //Color[Index*3+2]; //B
    AppRGB.RGB_BUF_L[Index*3+3] = 0x00;       //ONBright; //Color[Index*3+0]; //G
    AppRGB.RGB_BUF_L[Index*3+4] = ONBright1;  //ONBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+5] = ONBright2;  //ONBright; //Color[Index*3+2]; //B
  }
  else
  {
    AppRGB.RGB_BUF_L[Index*3+0] = 0x00;       //OFFBright; //Color[Index*3+0]; //G  //Version	V1.09	: (1)20241020		: IR RGB Mode
    AppRGB.RGB_BUF_L[Index*3+1] = OFFBright1; //OFFBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+2] = OFFBright2; //OFFBright; //Color[Index*3+2]; //B
    AppRGB.RGB_BUF_L[Index*3+3] = 0x00;       //OFFBright; //Color[Index*3+0]; //G
    AppRGB.RGB_BUF_L[Index*3+4] = OFFBright1; //OFFBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+5] = OFFBright2; //OFFBright; //Color[Index*3+2]; //B
  }
  //----------------------------------------------------------
  Index = 2;
  if((KeyCode&KEY_CODE_02)!=0)
  {
    AppRGB.RGB_BUF_L[Index*3+0] = 0x00;       //ONBright; //Color[Index*3+0]; //G //Version	V1.09	: (1)20241020		: IR RGB Mode
    AppRGB.RGB_BUF_L[Index*3+1] = ONBright1;  //ONBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+2] = ONBright2;  //ONBright; //Color[Index*3+2]; //B
  }
  else
  {
    AppRGB.RGB_BUF_L[Index*3+0] = 0x00;       //OFFBright; //Color[Index*3+0]; //G  //Version	V1.09	: (1)20241020		: IR RGB Mode
    AppRGB.RGB_BUF_L[Index*3+1] = OFFBright1; //OFFBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+2] = OFFBright2; //OFFBright; //Color[Index*3+2]; //B
  }
  //----------------------------------------------------------
  Index = 3;
  if((KeyCode&KEY_CODE_03)!=0)
  {
    AppRGB.RGB_BUF_L[Index*3+0] = 0x00;       //ONBright; //Color[Index*3+0]; //G //Version	V1.09	: (1)20241020		: IR RGB Mode
    AppRGB.RGB_BUF_L[Index*3+1] = ONBright1;  //ONBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+2] = ONBright2;  //ONBright; //Color[Index*3+2]; //B
  }
  else
  {
    AppRGB.RGB_BUF_L[Index*3+0] = 0x00;       //OFFBright; //Color[Index*3+0]; //G  //Version	V1.09	: (1)20241020		: IR RGB Mode
    AppRGB.RGB_BUF_L[Index*3+1] = OFFBright1; //OFFBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+2] = OFFBright2; //OFFBright; //Color[Index*3+2]; //B
  }
  //----------------------------------------------------------
  Index = 4;
  if((KeyCode&KEY_CODE_04)!=0)
  {
    AppRGB.RGB_BUF_L[Index*3+0] = 0x00;       //ONBright; //Color[Index*3+0]; //G //Version	V1.09	: (1)20241020		: IR RGB Mode
    AppRGB.RGB_BUF_L[Index*3+1] = ONBright1;  //ONBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+2] = ONBright2;  //ONBright; //Color[Index*3+2]; //B
  }
  else
  {
    AppRGB.RGB_BUF_L[Index*3+0] = 0x00;       //OFFBright; //Color[Index*3+0]; //G  //Version	V1.09	: (1)20241020		: IR RGB Mode
    AppRGB.RGB_BUF_L[Index*3+1] = OFFBright1; //OFFBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+2] = OFFBright2; //OFFBright; //Color[Index*3+2]; //B
  }
  //----------------------------------------------------------
  Index = 5;
  if((KeyCode&KEY_CODE_05)!=0)
  {
    AppRGB.RGB_BUF_L[Index*3+0] = 0x00;       //ONBright; //Color[Index*3+0]; //G   //Version	V1.09	: (1)20241020		: IR RGB Mode
    AppRGB.RGB_BUF_L[Index*3+1] = ONBright1;  //ONBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+2] = ONBright2;  //ONBright; //Color[Index*3+2]; //B
  }
  else
  {
    AppRGB.RGB_BUF_L[Index*3+0] = 0x00;       //OFFBright; //Color[Index*3+0]; //G  //Version	V1.09	: (1)20241020		: IR RGB Mode
    AppRGB.RGB_BUF_L[Index*3+1] = OFFBright1; //OFFBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+2] = OFFBright2; //OFFBright; //Color[Index*3+2]; //B
  }
  //----------------------------------------------------------
  Index = 6;
  if((KeyCode&KEY_CODE_06)!=0)
  {
    AppRGB.RGB_BUF_L[Index*3+0] = 0x00;       //ONBright; //Color[Index*3+0]; //G   //Version	V1.09	: (1)20241020		: IR RGB Mode
    AppRGB.RGB_BUF_L[Index*3+1] = ONBright1;  //ONBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+2] = ONBright2;  //ONBright; //Color[Index*3+2]; //B
    AppRGB.RGB_BUF_L[Index*3+3] = 0x00;       //ONBright; //Color[Index*3+0]; //G
    AppRGB.RGB_BUF_L[Index*3+4] = ONBright1;  //ONBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+5] = ONBright2;  //ONBright; //Color[Index*3+2]; //B
  }
  else
  {
    AppRGB.RGB_BUF_L[Index*3+0] = 0x00;       //OFFBright; //Color[Index*3+0]; //R  //Version	V1.09	: (1)20241020		: IR RGB Mode
    AppRGB.RGB_BUF_L[Index*3+1] = OFFBright1; //OFFBright; //Color[Index*3+1]; //G
    AppRGB.RGB_BUF_L[Index*3+2] = OFFBright2; //OFFBright; //Color[Index*3+2]; //B
    AppRGB.RGB_BUF_L[Index*3+3] = 0x00;       //OFFBright; //Color[Index*3+0]; //G
    AppRGB.RGB_BUF_L[Index*3+4] = OFFBright1; //OFFBright; //Color[Index*3+1]; //R
    AppRGB.RGB_BUF_L[Index*3+5] = OFFBright2; //OFFBright; //Color[Index*3+2]; //B
  }
  //----------------------------------------------------------
  Index = 8;
  AppRGB.RGB_BUF_L[Index*3+0] = AppRGB.Col_G[0];//ONBright; //Color[Index*3+0]; //G   //Version	V1.09	: (1)20241020		: IR RGB Mode //Version	V1.08	: (1)20241016		: Brightness For RGB  
  AppRGB.RGB_BUF_L[Index*3+1] = AppRGB.Col_R[0];//ONBright; //Color[Index*3+1]; //R
  AppRGB.RGB_BUF_L[Index*3+2] = AppRGB.Col_B[0];//ONBright; //Color[Index*3+2]; //B
  
  AppRGB.RGB_BUF_L[Index*3+3] = AppRGB.Col_G[1];//ONBright; //Color[Index*3+0]; //G   //Version	V1.09	: (1)20241020		: IR RGB Mode //Version	V1.08	: (1)20241016		: Brightness For RGB  
  AppRGB.RGB_BUF_L[Index*3+4] = AppRGB.Col_R[1];//ONBright; //Color[Index*3+1]; //R
  AppRGB.RGB_BUF_L[Index*3+5] = AppRGB.Col_B[1];//ONBright; //Color[Index*3+2]; //B
  
  AppRGB.RGB_BUF_L[Index*3+6] = AppRGB.Col_G[2];//ONBright; //Color[Index*3+0]; //G   //Version	V1.09	: (1)20241020		: IR RGB Mode //Version	V1.08	: (1)20241016		: Brightness For RGB  
  AppRGB.RGB_BUF_L[Index*3+7] = AppRGB.Col_R[2];//ONBright; //Color[Index*3+1]; //R
  AppRGB.RGB_BUF_L[Index*3+8] = AppRGB.Col_B[2];//ONBright; //Color[Index*3+2]; //B
  
  AppRGB.RGB_BUF_L[Index*3+9] = AppRGB.Col_G[3];//ONBright; //Color[Index*3+0]; //G   //Version	V1.09	: (1)20241020		: IR RGB Mode //Version	V1.08	: (1)20241016		: Brightness For RGB  
  AppRGB.RGB_BUF_L[Index*3+10]= AppRGB.Col_R[3];//ONBright; //Color[Index*3+1]; //R
  AppRGB.RGB_BUF_L[Index*3+11]= AppRGB.Col_B[3];//ONBright; //Color[Index*3+2]; //B

//  //----------------------------------------------------------
//  Index = 0;
//  AppRGB.RGB_BUF_L[Index*3+0] = 0x00; //Color[Index*3+1]; //R
//  AppRGB.RGB_BUF_L[Index*3+1] = 0xff; //Color[Index*3+0]; //G
//  AppRGB.RGB_BUF_L[Index*3+2] = 0x00; //Color[Index*3+2]; //B
//  AppRGB.RGB_BUF_L[Index*3+3] = 0x00; //Color[Index*3+1]; //R
//  AppRGB.RGB_BUF_L[Index*3+4] = 0xff; //Color[Index*3+0]; //G
//  AppRGB.RGB_BUF_L[Index*3+5] = 0x00; //Color[Index*3+2]; //B

//  Index = 2;
//  AppRGB.RGB_BUF_L[Index*3+0] = 0xff; //Color[Index*3+1]; //R
//  AppRGB.RGB_BUF_L[Index*3+1] = 0x00; //Color[Index*3+0]; //G
//  AppRGB.RGB_BUF_L[Index*3+2] = 0x00; //Color[Index*3+2]; //B
//  AppRGB.RGB_BUF_L[Index*3+3] = 0xff; //Color[Index*3+1]; //R
//  AppRGB.RGB_BUF_L[Index*3+4] = 0x00; //Color[Index*3+0]; //G
//  AppRGB.RGB_BUF_L[Index*3+5] = 0x00; //Color[Index*3+2]; //B  
//  
//  Index = 4;
//  AppRGB.RGB_BUF_L[Index*3+0] = 0x00; //Color[Index*3+1]; //R
//  AppRGB.RGB_BUF_L[Index*3+1] = 0x00; //Color[Index*3+0]; //G
//  AppRGB.RGB_BUF_L[Index*3+2] = 0xff; //Color[Index*3+2]; //B
//  AppRGB.RGB_BUF_L[Index*3+3] = 0x00; //Color[Index*3+1]; //R
//  AppRGB.RGB_BUF_L[Index*3+4] = 0x00; //Color[Index*3+0]; //G
//  AppRGB.RGB_BUF_L[Index*3+5] = 0xff; //Color[Index*3+2]; //B    
// 
//  Index = 6;
//  AppRGB.RGB_BUF_L[Index*3+0] = 0xff; //Color[Index*3+1]; //R
//  AppRGB.RGB_BUF_L[Index*3+1] = 0xff; //Color[Index*3+0]; //G
//  AppRGB.RGB_BUF_L[Index*3+2] = 0x00; //Color[Index*3+2]; //B
//  AppRGB.RGB_BUF_L[Index*3+3] = 0xff; //Color[Index*3+1]; //R
//  AppRGB.RGB_BUF_L[Index*3+4] = 0xff; //Color[Index*3+0]; //G
//  AppRGB.RGB_BUF_L[Index*3+5] = 0x00; //Color[Index*3+2]; //B  
  
  //----------------------------------------------------------
//  Index = 8;
//  AppRGB.RGB_BUF_L[Index*3+0] = 0xff; //Color[Index*3+1]; //R
//  AppRGB.RGB_BUF_L[Index*3+1] = 0xff; //Color[Index*3+0]; //G
//  AppRGB.RGB_BUF_L[Index*3+2] = 0xff; //Color[Index*3+2]; //B
//  
//  AppRGB.RGB_BUF_L[Index*3+3] = 0xff; //Color[Index*3+1]; //R
//  AppRGB.RGB_BUF_L[Index*3+4] = 0xff; //Color[Index*3+0]; //G
//  AppRGB.RGB_BUF_L[Index*3+5] = 0xff; //Color[Index*3+2]; //B
//  
//  AppRGB.RGB_BUF_L[Index*3+6] = 0xff; //Color[Index*3+1]; //R
//  AppRGB.RGB_BUF_L[Index*3+7] = 0xff; //Color[Index*3+0]; //G
//  AppRGB.RGB_BUF_L[Index*3+8] = 0xff; //Color[Index*3+2]; //B
//  
//  AppRGB.RGB_BUF_L[Index*3+9] = 0xff; //Color[Index*3+1]; //R
//  AppRGB.RGB_BUF_L[Index*3+10]= 0xff; //Color[Index*3+0]; //G
//  AppRGB.RGB_BUF_L[Index*3+11]= 0xff; //Color[Index*3+2]; //B








  //----------------------------------------------------------
  Chip2813_Left_Send_Data(AppRGB.RGB_BUF_L,12*3);
  Chip2813_Right_Send_Data(AppRGB.RGB_BUF_L,12*3);
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
void App_Set_TP_RGB_Value(unsigned int TPKeyCode)
{
  //Version	V1.16	: (1)20250224 	: RGB Mode  
  /*
//  //----------------------------------------------------------
//  unsigned char  i,c;//,LED_L=0,LED_H=0;
//  unsigned short LED_Val=0;
  unsigned char  Index=0;
  unsigned char  RGB_Idx=0;
//  unsigned char  Color[12];
  unsigned char  ONBright1 = AppRGB.Brightness;    //Version	V1.08	: (1)20241016		: Brightness For RGB  
  unsigned char  ONBright2 = AppRGB.Brightness/2;  //Version	V1.08	: (1)20241016		: Brightness For RGB    
//  //----------------------------------------------------------
//  Index = 0;
//  for(i=0;i<BTN_RGB_NUMBER;i++)
//  {
//    AppRGB.RGB_BTN_1[i*3+0] = Color[Index*3+1]; //R
//    AppRGB.RGB_BTN_1[i*3+1] = Color[Index*3+0]; //G
//    AppRGB.RGB_BTN_1[i*3+2] = Color[Index*3+2]; //B
//  }
//  Chip2813_BTN_1_Send_Data(AppRGB.RGB_BTN_1,BTN_RGB_NUMBER*3);


	for(Index=0;Index<APP_MAX_TP_CHAN;Index+=2)
	{
    RGB_Idx = (Index/2)*3*3;
    if((TPKeyCode&(1<<(Index)))!=0 || (TPKeyCode&(1<<(Index+1)))!=0)	
    {
      //-----------------------------------------------
      AppRGB.RGB_BTN_1[RGB_Idx+0] = 0x00;//Color[Index*3+1]; //G
      AppRGB.RGB_BTN_1[RGB_Idx+1] = ONBright1;//Color[Index*3+0]; //R //Version	V1.08	: (1)20241016		: Brightness For RGB  
      AppRGB.RGB_BTN_1[RGB_Idx+2] = ONBright2;//Color[Index*3+2]; //B //Version	V1.08	: (1)20241016		: Brightness For RGB  
      
      AppRGB.RGB_BTN_1[RGB_Idx+3] = 0x00;//Color[Index*3+1]; //G
      AppRGB.RGB_BTN_1[RGB_Idx+4] = ONBright1;//Color[Index*3+0]; //R //Version	V1.08	: (1)20241016		: Brightness For RGB  
      AppRGB.RGB_BTN_1[RGB_Idx+5] = ONBright2;//Color[Index*3+2]; //B //Version	V1.08	: (1)20241016		: Brightness For RGB  
      
      AppRGB.RGB_BTN_1[RGB_Idx+6] = 0x00;//Color[Index*3+1]; //G
      AppRGB.RGB_BTN_1[RGB_Idx+7] = ONBright1;//Color[Index*3+0]; //R //Version	V1.08	: (1)20241016		: Brightness For RGB  
      AppRGB.RGB_BTN_1[RGB_Idx+8] = ONBright2;//Color[Index*3+2]; //B //Version	V1.08	: (1)20241016		: Brightness For RGB  
      //-----------------------------------------------
    }
    else
    {
      AppRGB.RGB_BTN_1[RGB_Idx+0] = 0x00;//Color[Index*3+1]; //R
      AppRGB.RGB_BTN_1[RGB_Idx+1] = 0x00;//Color[Index*3+0]; //G
      AppRGB.RGB_BTN_1[RGB_Idx+2] = 0x00;//Color[Index*3+2]; //B
      
      AppRGB.RGB_BTN_1[RGB_Idx+3] = 0x00;//Color[Index*3+1]; //R
      AppRGB.RGB_BTN_1[RGB_Idx+4] = 0x00;//Color[Index*3+0]; //G
      AppRGB.RGB_BTN_1[RGB_Idx+5] = 0x00;//Color[Index*3+2]; //B
      
      AppRGB.RGB_BTN_1[RGB_Idx+6] = 0x00;//Color[Index*3+1]; //R
      AppRGB.RGB_BTN_1[RGB_Idx+7] = 0x00;//Color[Index*3+0]; //G
      AppRGB.RGB_BTN_1[RGB_Idx+8] = 0x00;//Color[Index*3+2]; //B
    }
	}
  
//  printf("RGB: %02X-%02X-%02X,%02X-%02X-%02X,%02X-%02X-%02X\r\n"
//  ,AppRGB.RGB_BTN_1[0],AppRGB.RGB_BTN_1[1],AppRGB.RGB_BTN_1[2]
//  ,AppRGB.RGB_BTN_1[3],AppRGB.RGB_BTN_1[4],AppRGB.RGB_BTN_1[5]
//  ,AppRGB.RGB_BTN_1[6],AppRGB.RGB_BTN_1[7],AppRGB.RGB_BTN_1[8]);
  
  
//  Chip2813_BTN_1_Send_Data(AppRGB.RGB_BTN_1,BTN_RGB_NUMBER*3);
//  
//  for(Index=0,RGB_Idx=(BTN_RGB_NUMBER/2)-1;Index<BTN_RGB_NUMBER/2;Index++,RGB_Idx--)
//	{
//    AppRGB.RGB_BTN_2[(RGB_Idx)*3+0] = AppRGB.RGB_BTN_1[Index*3+0];//Color[Index*3+1]; //R
//    AppRGB.RGB_BTN_2[(RGB_Idx)*3+1] = AppRGB.RGB_BTN_1[Index*3+1];//Color[Index*3+0]; //G
//    AppRGB.RGB_BTN_2[(RGB_Idx)*3+2] = AppRGB.RGB_BTN_1[Index*3+2];//Color[Index*3+2]; //B   
//  }
//  Chip2813_BTN_2_Send_Data(AppRGB.RGB_BTN_2,BTN_RGB_NUMBER*3);
  
  //Version	V1.05	: (1)20240915		: Switch RGB Issue
  for(Index=0,RGB_Idx=(BTN_RGB_NUMBER/2)-1;Index<BTN_RGB_NUMBER/2;Index++,RGB_Idx--)
	{
    AppRGB.RGB_BTN_2[(RGB_Idx)*3+0] = AppRGB.RGB_BTN_1[Index*3+0];//Color[Index*3+1]; //R
    AppRGB.RGB_BTN_2[(RGB_Idx)*3+1] = AppRGB.RGB_BTN_1[Index*3+1];//Color[Index*3+0]; //G
    AppRGB.RGB_BTN_2[(RGB_Idx)*3+2] = AppRGB.RGB_BTN_1[Index*3+2];//Color[Index*3+2]; //B   
  }
  
  RGB_Idx = (BTN_RGB_NUMBER/2)*3;
  for(Index=0;Index<BTN_RGB_NUMBER/2;Index++)
	{
    AppRGB.RGB_BTN_2[RGB_Idx+Index*3+0] = AppRGB.RGB_BTN_1[Index*3+0];//Color[Index*3+1]; //R
    AppRGB.RGB_BTN_2[RGB_Idx+Index*3+1] = AppRGB.RGB_BTN_1[Index*3+1];//Color[Index*3+0]; //G
    AppRGB.RGB_BTN_2[RGB_Idx+Index*3+2] = AppRGB.RGB_BTN_1[Index*3+2];//Color[Index*3+2]; //B   
  }
  
  Chip2813_BTN_1_Send_Data(AppRGB.RGB_BTN_2,BTN_RGB_NUMBER*3);
  */
  //Version	V1.16	: (1)20250224 	: RGB Mode  
}

/*******************************************************************************
* Function Name  : 	
* Description    : 	
* Input          : 
* Output         : 
* Return         : 
* Attention		 	 : 
*******************************************************************************/
//Version	V1.17	: (1)20250227 	: HID MODE RGB Display    
void App_Set_TP_RGB_Value_ForHID(void)
{ 
  //----------------------------------------------------------
  unsigned char  Index=0;
  unsigned char  RGB_Idx=0;
  unsigned char  ONBright1  = AppRGB.Brightness;  
  unsigned int   TPKeyCode  = AppUser.TPValue;
  //----------------------------------------------------------
	for(Index=0;Index<APP_MAX_TP_CHAN;Index+=2)
	{
    RGB_Idx = (Index/2)*3*3;
    if((TPKeyCode&(1<<(Index)))!=0 || (TPKeyCode&(1<<(Index+1)))!=0)	
    {
      //-----------------------------------------------
      AppRGB.RGB_BTN_1[RGB_Idx+0] = ONBright1;  //G
      AppRGB.RGB_BTN_1[RGB_Idx+1] = 0x00;       //R
      AppRGB.RGB_BTN_1[RGB_Idx+2] = 0x00;       //B
      
      AppRGB.RGB_BTN_1[RGB_Idx+3] = ONBright1;  //G
      AppRGB.RGB_BTN_1[RGB_Idx+4] = 0x00;       //R
      AppRGB.RGB_BTN_1[RGB_Idx+5] = 0x00;       //B
      
      AppRGB.RGB_BTN_1[RGB_Idx+6] = ONBright1;  //G
      AppRGB.RGB_BTN_1[RGB_Idx+7] = 0x00;       //R
      AppRGB.RGB_BTN_1[RGB_Idx+8] = 0x00;       //B
      //-----------------------------------------------
    }
    else
    {
      AppRGB.RGB_BTN_1[RGB_Idx+0] = 0x00;       //R
      AppRGB.RGB_BTN_1[RGB_Idx+1] = 0x00;       //G
      AppRGB.RGB_BTN_1[RGB_Idx+2] = ONBright1;  //B
      
      AppRGB.RGB_BTN_1[RGB_Idx+3] = 0x00;       //R
      AppRGB.RGB_BTN_1[RGB_Idx+4] = 0x00;       //G
      AppRGB.RGB_BTN_1[RGB_Idx+5] = ONBright1;  //B
      
      AppRGB.RGB_BTN_1[RGB_Idx+6] = 0x00;       //R
      AppRGB.RGB_BTN_1[RGB_Idx+7] = 0x00;       //G
      AppRGB.RGB_BTN_1[RGB_Idx+8] = ONBright1;  //B
    }
	}
  
//  printf("RGB: %02X-%02X-%02X,%02X-%02X-%02X,%02X-%02X-%02X\r\n"
//  ,AppRGB.RGB_BTN_1[0],AppRGB.RGB_BTN_1[1],AppRGB.RGB_BTN_1[2]
//  ,AppRGB.RGB_BTN_1[3],AppRGB.RGB_BTN_1[4],AppRGB.RGB_BTN_1[5]
//  ,AppRGB.RGB_BTN_1[6],AppRGB.RGB_BTN_1[7],AppRGB.RGB_BTN_1[8]);
  
  
//  Chip2813_BTN_1_Send_Data(AppRGB.RGB_BTN_1,BTN_RGB_NUMBER*3);
//  
//  for(Index=0,RGB_Idx=(BTN_RGB_NUMBER/2)-1;Index<BTN_RGB_NUMBER/2;Index++,RGB_Idx--)
//	{
//    AppRGB.RGB_BTN_2[(RGB_Idx)*3+0] = AppRGB.RGB_BTN_1[Index*3+0];//Color[Index*3+1]; //R
//    AppRGB.RGB_BTN_2[(RGB_Idx)*3+1] = AppRGB.RGB_BTN_1[Index*3+1];//Color[Index*3+0]; //G
//    AppRGB.RGB_BTN_2[(RGB_Idx)*3+2] = AppRGB.RGB_BTN_1[Index*3+2];//Color[Index*3+2]; //B   
//  }
//  Chip2813_BTN_2_Send_Data(AppRGB.RGB_BTN_2,BTN_RGB_NUMBER*3);
  
  //Version	V1.05	: (1)20240915		: Switch RGB Issue
  for(Index=0,RGB_Idx=(BTN_RGB_NUMBER/2)-1;Index<BTN_RGB_NUMBER/2;Index++,RGB_Idx--)
	{
    AppRGB.RGB_BTN_2[(RGB_Idx)*3+0] = AppRGB.RGB_BTN_1[Index*3+0];//Color[Index*3+1]; //R
    AppRGB.RGB_BTN_2[(RGB_Idx)*3+1] = AppRGB.RGB_BTN_1[Index*3+1];//Color[Index*3+0]; //G
    AppRGB.RGB_BTN_2[(RGB_Idx)*3+2] = AppRGB.RGB_BTN_1[Index*3+2];//Color[Index*3+2]; //B   
  }
  
  RGB_Idx = (BTN_RGB_NUMBER/2)*3;
  for(Index=0;Index<BTN_RGB_NUMBER/2;Index++)
	{
    AppRGB.RGB_BTN_2[RGB_Idx+Index*3+0] = AppRGB.RGB_BTN_1[Index*3+0];//Color[Index*3+1]; //R
    AppRGB.RGB_BTN_2[RGB_Idx+Index*3+1] = AppRGB.RGB_BTN_1[Index*3+1];//Color[Index*3+0]; //G
    AppRGB.RGB_BTN_2[RGB_Idx+Index*3+2] = AppRGB.RGB_BTN_1[Index*3+2];//Color[Index*3+2]; //B   
  }
  
  Chip2813_BTN_1_Send_Data(AppRGB.RGB_BTN_2,BTN_RGB_NUMBER*3);
 
}


/*******************************************************************************
* Function Name  : 	
* Description    : 	
* Input          : 
* Output         : 
* Return         : 
* Attention		 	 : 
*******************************************************************************/
void App_Set_HID_RGB_Value(unsigned char *LED_BUF,unsigned char Len)
{
  //----------------------------------------------------------
  unsigned char  Index=0,Offset=0;
  unsigned char  RGB_Idx=0;
  //----------------------------------------------------------
  Offset = LED_BUF[0]*10*3*3;
  //----------------------------------------------------------
  for(Index=0;Index<(20);Index++)
  {
    AppRGB.RGB_BTN_1[Offset+Index*3*3+1] = LED_BUF[1+Index*2*3+0]/AppRGB.BrightCutf; //G  //Version	V1.08	: (1)20241016		: Brightness For RGB  
    AppRGB.RGB_BTN_1[Offset+Index*3*3+0] = LED_BUF[1+Index*2*3+1]/AppRGB.BrightCutf; //R
    AppRGB.RGB_BTN_1[Offset+Index*3*3+2] = LED_BUF[1+Index*2*3+2]/AppRGB.BrightCutf; //B
      
    AppRGB.RGB_BTN_1[Offset+Index*3*3+4] = LED_BUF[1+Index*2*3+0]/AppRGB.BrightCutf; //G  //Version	V1.08	: (1)20241016		: Brightness For RGB  
    AppRGB.RGB_BTN_1[Offset+Index*3*3+3] = LED_BUF[1+Index*2*3+1]/AppRGB.BrightCutf; //R
    AppRGB.RGB_BTN_1[Offset+Index*3*3+5] = LED_BUF[1+Index*2*3+2]/AppRGB.BrightCutf; //B  
    
    AppRGB.RGB_BTN_1[Offset+Index*3*3+7] = LED_BUF[1+Index*2*3+3]/AppRGB.BrightCutf; //G  //Version	V1.08	: (1)20241016		: Brightness For RGB  
    AppRGB.RGB_BTN_1[Offset+Index*3*3+6] = LED_BUF[1+Index*2*3+4]/AppRGB.BrightCutf; //R
    AppRGB.RGB_BTN_1[Offset+Index*3*3+8] = LED_BUF[1+Index*2*3+5]/AppRGB.BrightCutf; //B  
  }
  //----------------------------------------------------------
  if(Offset!=0)
  {
    //Version	V1.07	: (1)20241015		: Switch HID Mode RGB
//    Chip2813_BTN_1_Send_Data(AppRGB.RGB_BTN_1,BTN_RGB_NUMBER*3);
//    for(Index=0,RGB_Idx=(BTN_RGB_NUMBER/2)-1;Index<BTN_RGB_NUMBER/2;Index++,RGB_Idx--)
//    {
//      AppRGB.RGB_BTN_2[(RGB_Idx)*3+0] = AppRGB.RGB_BTN_1[Index*3+0];//Color[Index*3+1]; //R
//      AppRGB.RGB_BTN_2[(RGB_Idx)*3+1] = AppRGB.RGB_BTN_1[Index*3+1];//Color[Index*3+0]; //G
//      AppRGB.RGB_BTN_2[(RGB_Idx)*3+2] = AppRGB.RGB_BTN_1[Index*3+2];//Color[Index*3+2]; //B   
//    }
//    Chip2813_BTN_2_Send_Data(AppRGB.RGB_BTN_2,BTN_RGB_NUMBER*3);

    for(Index=0,RGB_Idx=(BTN_RGB_NUMBER/2)-1;Index<BTN_RGB_NUMBER/2;Index++,RGB_Idx--)
    {
      AppRGB.RGB_BTN_2[(RGB_Idx)*3+0] = AppRGB.RGB_BTN_1[Index*3+0];//Color[Index*3+1]; //R
      AppRGB.RGB_BTN_2[(RGB_Idx)*3+1] = AppRGB.RGB_BTN_1[Index*3+1];//Color[Index*3+0]; //G
      AppRGB.RGB_BTN_2[(RGB_Idx)*3+2] = AppRGB.RGB_BTN_1[Index*3+2];//Color[Index*3+2]; //B   
    }
    RGB_Idx = (BTN_RGB_NUMBER/2)*3;
    for(Index=0;Index<BTN_RGB_NUMBER/2;Index++)
    {
      AppRGB.RGB_BTN_2[RGB_Idx+Index*3+0] = AppRGB.RGB_BTN_1[Index*3+0];//Color[Index*3+1]; //R
      AppRGB.RGB_BTN_2[RGB_Idx+Index*3+1] = AppRGB.RGB_BTN_1[Index*3+1];//Color[Index*3+0]; //G
      AppRGB.RGB_BTN_2[RGB_Idx+Index*3+2] = AppRGB.RGB_BTN_1[Index*3+2];//Color[Index*3+2]; //B   
    }
    Chip2813_BTN_1_Send_Data(AppRGB.RGB_BTN_2,BTN_RGB_NUMBER*3);   

    HID_RGB_EN = 1;//Version	V1.17	: (1)20250227 	: HID MODE RGB Display    
  }
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
void App_Set_IO4_RGB_Value(unsigned char *LED_BUF,unsigned char Len)  //Version	V1.19	: (1)20250309 	: IO4 MODE RGB Game Ctrl Support
{
  //----------------------------------------------------------
  unsigned char  Index=0,Offset=0;
  unsigned char  RGB_Idx=0;
  //----------------------------------------------------------
  Offset = 4;
  //----------------------------------------------------------
//  for(Index=0;Index<(31);Index++)
//  {
//    AppRGB.RGB_BTN_1[Index*3+1] = LED_BUF[Offset+Index*3+0]/AppRGB.BrightCutf; //G  //Version	V1.08	: (1)20241016		: Brightness For RGB  
//    AppRGB.RGB_BTN_1[Index*3+0] = LED_BUF[Offset+Index*3+1]/AppRGB.BrightCutf; //R
//    AppRGB.RGB_BTN_1[Index*3+2] = LED_BUF[Offset+Index*3+2]/AppRGB.BrightCutf; //B
//  }
  
  for(Index=0;Index<(16);Index++)
  {
    AppRGB.RGB_BTN_1[Index*3*3+2] = LED_BUF[Offset+Index*2*3+0]/AppRGB.BrightCutf; //B  //Version	V1.08	: (1)20241016		: Brightness For RGB  
    AppRGB.RGB_BTN_1[Index*3*3+1] = LED_BUF[Offset+Index*2*3+1]/AppRGB.BrightCutf; //R
    AppRGB.RGB_BTN_1[Index*3*3+0] = LED_BUF[Offset+Index*2*3+2]/AppRGB.BrightCutf; //G
      
    AppRGB.RGB_BTN_1[Index*3*3+5] = LED_BUF[Offset+Index*2*3+0]/AppRGB.BrightCutf; //B  //Version	V1.08	: (1)20241016		: Brightness For RGB  
    AppRGB.RGB_BTN_1[Index*3*3+4] = LED_BUF[Offset+Index*2*3+1]/AppRGB.BrightCutf; //R
    AppRGB.RGB_BTN_1[Index*3*3+3] = LED_BUF[Offset+Index*2*3+2]/AppRGB.BrightCutf; //G  
    
    AppRGB.RGB_BTN_1[Index*3*3+8] = LED_BUF[Offset+Index*2*3+3]/AppRGB.BrightCutf; //B  //Version	V1.08	: (1)20241016		: Brightness For RGB  
    AppRGB.RGB_BTN_1[Index*3*3+7] = LED_BUF[Offset+Index*2*3+4]/AppRGB.BrightCutf; //R
    AppRGB.RGB_BTN_1[Index*3*3+6] = LED_BUF[Offset+Index*2*3+5]/AppRGB.BrightCutf; //G  
  }
  //----------------------------------------------------------
  if(Offset!=0)
  {
    for(Index=0,RGB_Idx=(BTN_RGB_NUMBER/2)-1;Index<BTN_RGB_NUMBER/2;Index++,RGB_Idx--)
    {
      AppRGB.RGB_BTN_2[(RGB_Idx)*3+0] = AppRGB.RGB_BTN_1[Index*3+0];//Color[Index*3+1]; //R
      AppRGB.RGB_BTN_2[(RGB_Idx)*3+1] = AppRGB.RGB_BTN_1[Index*3+1];//Color[Index*3+0]; //G
      AppRGB.RGB_BTN_2[(RGB_Idx)*3+2] = AppRGB.RGB_BTN_1[Index*3+2];//Color[Index*3+2]; //B   
    }
    RGB_Idx = (BTN_RGB_NUMBER/2)*3;
    for(Index=0;Index<BTN_RGB_NUMBER/2;Index++)
    {
      AppRGB.RGB_BTN_2[RGB_Idx+Index*3+0] = AppRGB.RGB_BTN_1[Index*3+0];//Color[Index*3+1]; //R
      AppRGB.RGB_BTN_2[RGB_Idx+Index*3+1] = AppRGB.RGB_BTN_1[Index*3+1];//Color[Index*3+0]; //G
      AppRGB.RGB_BTN_2[RGB_Idx+Index*3+2] = AppRGB.RGB_BTN_1[Index*3+2];//Color[Index*3+2]; //B   
    }
    Chip2813_BTN_1_Send_Data(AppRGB.RGB_BTN_2,BTN_RGB_NUMBER*3);    
  }
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
void App_Set_TP_RGB_TEST(unsigned int TPKeyCode)
{
  static unsigned char   Step;
  unsigned char  Index=0; 
  unsigned char   R=0,G=0,B=0;

  if(Step==0x00)  {R = AppRGB.Brightness; G=0x00;               B=0x00;             Step++;}  //Version	V1.08	: (1)20241016		: Brightness For RGB  
  else if(Step==1){R = 0x00;              G=AppRGB.Brightness;  B=0x00;             Step++;}  //Version	V1.08	: (1)20241016		: Brightness For RGB  
  else if(Step==2){R = 0x00;              G=0x00;               B=AppRGB.Brightness;Step++;}  //Version	V1.08	: (1)20241016		: Brightness For RGB  
  else if(Step==3){R = AppRGB.Brightness; G=AppRGB.Brightness;  B=AppRGB.Brightness;Step=0;}  //Version	V1.08	: (1)20241016		: Brightness For RGB  
//  else if(Step==4){R = 0xff;G=0x00;B=0xff;Step++;}
//  else if(Step==5){R = 0x00;G=0xff;B=0xff;Step++;}
//  else if(Step==6){R = 0xff;G=0xff;B=0xff;Step++;}
//  else if(Step==7){R = 0x00;G=0x00;B=0x00;Step=0;}
  
	for(Index=0;Index<BTN_RGB_NUMBER;Index++)
	{
      //-----------------------------------------------
      AppRGB.RGB_BTN_1[Index*3+0] = G;
      AppRGB.RGB_BTN_1[Index*3+1] = R;
      AppRGB.RGB_BTN_1[Index*3+2] = B;
      //-----------------------------------------------
	}
  Chip2813_BTN_1_Send_Data(AppRGB.RGB_BTN_1,BTN_RGB_NUMBER*3);
  Chip2813_BTN_2_Send_Data(AppRGB.RGB_BTN_1,BTN_RGB_NUMBER*3);
  Chip2813_Left_Send_Data(AppRGB.RGB_BTN_1,BTN_RGB_NUMBER*3);
  Chip2813_Right_Send_Data(AppRGB.RGB_BTN_1,BTN_RGB_NUMBER*3);
  
  //printf("App RGB :%02X,%02X,%02X,%02X\r\n",Step,R,G,B);
}

/*******************************************************************************
* Function Name  : 	
* Description    : 	
* Input          : 
* Output         : 
* Return         : 
* Attention		 	 : 
*******************************************************************************/
#define MAX_COLOR_STEP			19
unsigned char const RGB_TAB_R[MAX_COLOR_STEP]={255,255,255,255,  170, 85,  0,    0,  0,  0,    0,  0,  0,  42,  85,170,255,   255,255};
unsigned char const RGB_TAB_G[MAX_COLOR_STEP]={  0, 85,170,255,  255,255,255,  255,255,255,  170, 85, 43,   0,   0,  0,  0,     0,  0};
unsigned char const RGB_TAB_B[MAX_COLOR_STEP]={  0,  0,  0,  0,    0,  0,  0,   85,170,255,  255,255,255, 255,  255,255,255,  170, 85};
void App_RGB_AutoSwitch(unsigned char Mode)
{
  //----------------------------------------------------------
  unsigned char i,NewRGB=0; 
  //----------------------------------------------------------
  switch(Mode)
  {
    case  0:  
    {
      
      //-------------------------------------------------- 
      for(i=0;i<MAX_RGB_NUMBER;i++)
      {
        if(AppRGB.NextR[i] > AppRGB.Col_R[i])			{ AppRGB.Col_R[i]++;	NewRGB = 1; }
        else if(AppRGB.NextR[i] < AppRGB.Col_R[i]){ AppRGB.Col_R[i]--;  NewRGB = 1; }
        
        if(AppRGB.NextG[i] > AppRGB.Col_G[i])			{ AppRGB.Col_G[i]++;	NewRGB = 1; }
        else if(AppRGB.NextG[i] < AppRGB.Col_G[i]){ AppRGB.Col_G[i]--;  NewRGB = 1; }
          
        if(AppRGB.NextB[i] > AppRGB.Col_B[i])			{ AppRGB.Col_B[i]++;	NewRGB = 1; }
        else if(AppRGB.NextB[i] < AppRGB.Col_B[i]){ AppRGB.Col_B[i]--;  NewRGB = 1; }
      }	
      //-------------------------------------------------- 
      if(NewRGB!=0)
      {
        //printf("App RGB :%04X,%02X-%02X-%02X\r\n",AppRGB.Step,AppRGB.Col_R[0],AppRGB.Col_G[0],AppRGB.Col_B[0]);
        Send_RGB_1_Data(AppRGB.Col_R,AppRGB.Col_G,AppRGB.Col_B);
      }
      //-------------------------------------------------- 
      else
      {
        AppRGB.Step++;if(AppRGB.Step>=MAX_COLOR_STEP)AppRGB.Step=0;
        for(i=0;i<MAX_RGB_NUMBER;i++)
        {
          AppRGB.NextR[i] = (RGB_TAB_R[AppRGB.Step]/2);   //Version	V1.14	: (1)20250102 	: Auto Switch RGB Brightness
          AppRGB.NextG[i] = (RGB_TAB_G[AppRGB.Step]/2);   //Version	V1.14	: (1)20250102 	: Auto Switch RGB Brightness
          AppRGB.NextB[i] = (RGB_TAB_B[AppRGB.Step]/2);   //Version	V1.14	: (1)20250102 	: Auto Switch RGB Brightness
        }
      }
      //--------------------------------------------------
      break;
    }
    default:  break;
  }
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
//Version	V1.16	: (1)20250224 	: RGB Mode  
#include "usb_class.h"
unsigned char   AutoStep = 0;     
unsigned short  AutoTick = 0;
unsigned char const RGB_RUN_R[MAX_COLOR_STEP]={255,255,255,255,  170, 85,  0,    0,  0,  0,    0,  0,  0,  42,  85,170,255,   255,255};
unsigned char const RGB_RUN_G[MAX_COLOR_STEP]={  0, 85,170,255,  255,255,255,  255,255,255,  170, 85, 43,   0,   0,  0,  0,     0,  0};
unsigned char const RGB_RUN_B[MAX_COLOR_STEP]={  0,  0,  0,  0,    0,  0,  0,   85,170,255,  255,255,255, 255,  255,255,255,  170, 85};
void App_RGB_AutoRunSwitch(unsigned char Mode)
{
  //----------------------------------------------------------
  //unsigned char i,NewRGB=0; 
  //----------------------------------------------------------
  //Version	V1.16	: (1)20250224 	: RGB Mode
  
  #define		RGB_NUMBER	 				48*2
  
  unsigned char  Index=0;
  unsigned char  RGB_Idx=0;
  unsigned char  ONBright1 = AppRGB.Brightness;    //Version	V1.08	: (1)20241016		: Brightness For RGB  
  unsigned char  ONBright2 = AppRGB.Brightness/2;  //Version	V1.08	: (1)20241016		: Brightness For RGB 
  unsigned int   TPKeyCode  = AppUser.TPValue;
  //----------------------------------------------------------
  switch(Mode)
  {
    case  0:  
    {
      //-------------------------------------------------- 
      for(Index=0;Index<((RGB_NUMBER/2)-1);Index++)
      {
        RGB_Idx = Index;

        AppRGB.RGB_BTN_3[RGB_Idx*3+0] = AppRGB.RGB_BTN_3[(RGB_Idx+1)*3+0];  //R
        AppRGB.RGB_BTN_3[RGB_Idx*3+1] = AppRGB.RGB_BTN_3[(RGB_Idx+1)*3+1];  //G
        AppRGB.RGB_BTN_3[RGB_Idx*3+2] = AppRGB.RGB_BTN_3[(RGB_Idx+1)*3+2];  //B 
      }

      Index = ((RGB_NUMBER/2)-1);
      AppRGB.RGB_BTN_3[Index*3+0] = RGB_RUN_G[AutoStep];  //G
      AppRGB.RGB_BTN_3[Index*3+1] = RGB_RUN_R[AutoStep];  //R
      AppRGB.RGB_BTN_3[Index*3+2] = RGB_RUN_B[AutoStep];  //B  

//      for(Index=0;Index<((RGB_NUMBER/2)-3);Index+=3)
//      {
//        RGB_Idx = Index;

//        AppRGB.RGB_BTN_3[RGB_Idx*3+0] = AppRGB.RGB_BTN_3[(RGB_Idx+3)*3+0];  //R
//        AppRGB.RGB_BTN_3[RGB_Idx*3+1] = AppRGB.RGB_BTN_3[(RGB_Idx+3)*3+1];  //G
//        AppRGB.RGB_BTN_3[RGB_Idx*3+2] = AppRGB.RGB_BTN_3[(RGB_Idx+3)*3+2];  //B 

//        AppRGB.RGB_BTN_3[RGB_Idx*3+3] = AppRGB.RGB_BTN_3[(RGB_Idx+3)*3+3];  //R
//        AppRGB.RGB_BTN_3[RGB_Idx*3+4] = AppRGB.RGB_BTN_3[(RGB_Idx+3)*3+4];  //G
//        AppRGB.RGB_BTN_3[RGB_Idx*3+5] = AppRGB.RGB_BTN_3[(RGB_Idx+3)*3+5];  //B 
//        
//        AppRGB.RGB_BTN_3[RGB_Idx*3+6] = AppRGB.RGB_BTN_3[(RGB_Idx+3)*3+6];  //R
//        AppRGB.RGB_BTN_3[RGB_Idx*3+7] = AppRGB.RGB_BTN_3[(RGB_Idx+3)*3+7];  //G
//        AppRGB.RGB_BTN_3[RGB_Idx*3+8] = AppRGB.RGB_BTN_3[(RGB_Idx+3)*3+8];  //B 
//      }
//      
//      
//      Index = ((RGB_NUMBER/2)-3);
//      AppRGB.RGB_BTN_3[Index*3+0] = RGB_RUN_G[AutoStep];  //G
//      AppRGB.RGB_BTN_3[Index*3+1] = RGB_RUN_R[AutoStep];  //R
//      AppRGB.RGB_BTN_3[Index*3+2] = RGB_RUN_B[AutoStep];  //B  
//      
//      AppRGB.RGB_BTN_3[Index*3+3] = RGB_RUN_G[AutoStep];  //G
//      AppRGB.RGB_BTN_3[Index*3+4] = RGB_RUN_R[AutoStep];  //R
//      AppRGB.RGB_BTN_3[Index*3+5] = RGB_RUN_B[AutoStep];  //B  
//      
//      AppRGB.RGB_BTN_3[Index*3+6] = RGB_RUN_G[AutoStep];  //G
//      AppRGB.RGB_BTN_3[Index*3+7] = RGB_RUN_R[AutoStep];  //R
//      AppRGB.RGB_BTN_3[Index*3+8] = RGB_RUN_B[AutoStep];  //B  
      
      //-------------------------------------------------- 
      AutoStep++;if(AutoStep>=MAX_COLOR_STEP)AutoStep=0;       
      //-------------------------------------------------- 
      
      
      for(Index=0;Index<APP_MAX_TP_CHAN;Index+=2)
      {
        RGB_Idx = (Index/2)*3*3;
        if((TPKeyCode&(1<<(Index)))!=0 || (TPKeyCode&(1<<(Index+1)))!=0)	
        {
          //-----------------------------------------------
          AppRGB.RGB_BTN_1[RGB_Idx+0] = ONBright2;  //G
          AppRGB.RGB_BTN_1[RGB_Idx+1] = ONBright1;  //R  
          AppRGB.RGB_BTN_1[RGB_Idx+2] = 0x00;       //B  
          
          AppRGB.RGB_BTN_1[RGB_Idx+3] = ONBright2;  //G
          AppRGB.RGB_BTN_1[RGB_Idx+4] = ONBright1;  //R  
          AppRGB.RGB_BTN_1[RGB_Idx+5] = 0x00;       //B
          
          AppRGB.RGB_BTN_1[RGB_Idx+6] = ONBright2;  //G
          AppRGB.RGB_BTN_1[RGB_Idx+7] = ONBright1;  //R
          AppRGB.RGB_BTN_1[RGB_Idx+8] = 0x00;       //B
          //-----------------------------------------------
        }
        else
        {
          AppRGB.RGB_BTN_1[RGB_Idx+0] = AppRGB.RGB_BTN_3[RGB_Idx+0];//Color[Index*3+1]; //R
          AppRGB.RGB_BTN_1[RGB_Idx+1] = AppRGB.RGB_BTN_3[RGB_Idx+1];//Color[Index*3+0]; //G
          AppRGB.RGB_BTN_1[RGB_Idx+2] = AppRGB.RGB_BTN_3[RGB_Idx+2];//Color[Index*3+2]; //B
          
          AppRGB.RGB_BTN_1[RGB_Idx+3] = AppRGB.RGB_BTN_3[RGB_Idx+3];//Color[Index*3+1]; //R
          AppRGB.RGB_BTN_1[RGB_Idx+4] = AppRGB.RGB_BTN_3[RGB_Idx+4];//Color[Index*3+0]; //G
          AppRGB.RGB_BTN_1[RGB_Idx+5] = AppRGB.RGB_BTN_3[RGB_Idx+5];//Color[Index*3+2]; //B
          
          AppRGB.RGB_BTN_1[RGB_Idx+6] = AppRGB.RGB_BTN_3[RGB_Idx+6];//Color[Index*3+1]; //R
          AppRGB.RGB_BTN_1[RGB_Idx+7] = AppRGB.RGB_BTN_3[RGB_Idx+7];//Color[Index*3+0]; //G
          AppRGB.RGB_BTN_1[RGB_Idx+8] = AppRGB.RGB_BTN_3[RGB_Idx+8];//Color[Index*3+2]; //B
          
//          AppRGB.RGB_BTN_1[RGB_Idx+0] = 0x00;//Color[Index*3+1]; //R
//          AppRGB.RGB_BTN_1[RGB_Idx+1] = 0x00;//Color[Index*3+0]; //G
//          AppRGB.RGB_BTN_1[RGB_Idx+2] = 0x00;//Color[Index*3+2]; //B
//          
//          AppRGB.RGB_BTN_1[RGB_Idx+3] = 0x00;//Color[Index*3+1]; //R
//          AppRGB.RGB_BTN_1[RGB_Idx+4] = 0x00;//Color[Index*3+0]; //G
//          AppRGB.RGB_BTN_1[RGB_Idx+5] = 0x00;//Color[Index*3+2]; //B
//          
//          AppRGB.RGB_BTN_1[RGB_Idx+6] = 0x00;//Color[Index*3+1]; //R
//          AppRGB.RGB_BTN_1[RGB_Idx+7] = 0x00;//Color[Index*3+0]; //G
//          AppRGB.RGB_BTN_1[RGB_Idx+8] = 0x00;//Color[Index*3+2]; //B
        }
      }
      
     
      //--------------------------------------------------         
      for(Index=0,RGB_Idx=(BTN_RGB_NUMBER/2)-1;Index<BTN_RGB_NUMBER/2;Index++,RGB_Idx--)
      {
        AppRGB.RGB_BTN_2[(RGB_Idx)*3+0] = AppRGB.RGB_BTN_1[Index*3+0];      //R
        AppRGB.RGB_BTN_2[(RGB_Idx)*3+1] = AppRGB.RGB_BTN_1[Index*3+1];      //G
        AppRGB.RGB_BTN_2[(RGB_Idx)*3+2] = AppRGB.RGB_BTN_1[Index*3+2];      //B   
      }
      RGB_Idx = (BTN_RGB_NUMBER/2)*3;
      for(Index=0;Index<BTN_RGB_NUMBER/2;Index++)
      {
        AppRGB.RGB_BTN_2[RGB_Idx+Index*3+0] = AppRGB.RGB_BTN_1[Index*3+0];  //R
        AppRGB.RGB_BTN_2[RGB_Idx+Index*3+1] = AppRGB.RGB_BTN_1[Index*3+1];  //G
        AppRGB.RGB_BTN_2[RGB_Idx+Index*3+2] = AppRGB.RGB_BTN_1[Index*3+2];  //B   
      }
      Chip2813_BTN_1_Send_Data(AppRGB.RGB_BTN_2,BTN_RGB_NUMBER*3);
      //--------------------------------------------------
      break;
    }
    default:  break;
  }
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
void App_RGB_Update(unsigned int Tick)
{
	//----------------------------------------------------------
	if(AppRGB.RGBTick!=Tick)
	{
		//--------------------------------------------------------
		AppRGB.RGBTick = Tick;
		//--------------------------------------------------------
		if(AppRGB.PwrTick>0)
		{
			AppRGB.PwrTick--;
      if(AppRGB.PwrTick==0)
      {
        AppRGB.PwrFlag  = 1;
        printf("App RGB PwrFlag:%04X,%04X\r\n",AppUser.Value,AppRGB.Mode);
      }
		}
		//--------------------------------------------------------
		else //if(AppUser.Value & KEY_CODE_11)  //IO_LED_PWR_READ
		{
			AppRGB.Speed++;
			if(AppRGB.Speed>=10)
			{
				AppRGB.Speed = 0;
				App_RGB_AutoSwitch(AppRGB.Mode);
        App_Set_IR_RGB_Value(AppUser.IR_VAL); //Version	V1.09	: (1)20241020		: IR RGB Mode
			}
		}
    
    //Version	V1.16	: (1)20250224 	: RGB Mode 
    if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_KBS)     
    {
      #ifdef  SOFT_CONIFG_KBS
        AutoTick++;
        if(AutoTick>=50)
        {
          AutoTick  = 0;
          App_RGB_AutoRunSwitch(0);
        }
      #endif
    }
    //Version	V1.17	: (1)20250227 	: HID MODE RGB Display  
    else if((ConfigMode&USBD_MODE_MAK)==USBD_MODE_HID)
    {
      #ifdef  SOFT_CONIFG_HID
      if(HID_RGB_EN == 0)  //Version	V1.17	: (1)20250227 	: HID MODE RGB Display    
      {
        AutoTick++;
        if(AutoTick>=10)
        {
          AutoTick  = 0;
          App_Set_TP_RGB_Value_ForHID();
        }
      }
      #endif
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
void App_RGB_Init(void)
{
	//----------------------------------------------------------
	memset((uint8_t *)&AppRGB,	0,sizeof(AppRGB));
  memset((uint8_t *)&AppPacket,	0,sizeof(AppPacket));
	//----------------------------------------------------------
	AppRGB.PwrTick  = 1000;		//
	//----------------------------------------------------------
  AppRGB.PwrFlag  = 0;
	AppRGB.Step 		= 0;		//
	AppRGB.Mode 		= 0;		//
//	AppRGB.Btn_Mode	= 0;		//
//  
//  AppRGB.BrightStep = 2;
//  AppRGB.Brightness = RGB_VAL;  //RGB_VAL=48
//  AppRGB.BrightCutf = 1;
  
  HID_RGB_EN  = 0;  //Version	V1.17	: (1)20250227 	: HID MODE RGB Display    
  //----------------------------------------------------------
  Chip2813_BTN_1_Send_Data(AppRGB.RGB_BTN_1,BTN_RGB_NUMBER*3);
  Chip2813_BTN_2_Send_Data(AppRGB.RGB_BTN_1,BTN_RGB_NUMBER*3);
  //----------------------------------------------------------
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


