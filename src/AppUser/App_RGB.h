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

#ifndef __APP_RGB_H__
#define __APP_RGB_H__	

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&



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
#define		MAX_RGB_NUMBER	 				70 

#define		BTN_RGB_NUMBER	 				47*2 //Version	V1.04	: (1)20240718		: Switch To RGBx25

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
	unsigned int		PwrTick;				//Version	V1.12	: (1)20220827		: RGB Out
	unsigned char		PwrFlag;				//Version	V1.12	: (1)20220827		: RGB Out 
	unsigned char		Btn_Mode;				//Version	V1.12	: (1)20220827		: RGB Out 
//	unsigned char		BtnRGB[32*3];		//Version	V1.12	: (1)20220827		: RGB Out 	
	unsigned int		BtnIdle;				//Version	V1.15	: (2)20220915		: RGB Power Sync
  unsigned char		BrightStep;     //Version	V1.36	: (2)20230228		: New Btn For 3,7,*,Brightness
  unsigned char		Brightness;     //Version	V1.36	: (2)20230228		: New Btn For 3,7,*,Brightness
  unsigned char		BrightCutf;     //Version	V1.36	: (2)20230228		: New Btn For 3,7,*,Brightness
	//------------------------------------------------------------
	unsigned char		Mode;
	unsigned char		Step;
	unsigned short	Speed;
	unsigned int		RGBTick;
  //------------------------------------------------------------
  unsigned char		Color_R;
  unsigned char		Color_G;
  unsigned char		Color_B;
//	unsigned char		NextRGB[MAX_RGB_NUMBER*3];    //Version	V1.36	: (2)20230228		: New Btn For 3,7,*,Brightness
//  unsigned char		NorRGB[MAX_RGB_NUMBER*3];

	unsigned char		NextR[MAX_RGB_NUMBER];
	unsigned char		NextG[MAX_RGB_NUMBER];
	unsigned char		NextB[MAX_RGB_NUMBER];
  unsigned char		Col_R[MAX_RGB_NUMBER];
	unsigned char		Col_G[MAX_RGB_NUMBER];
	unsigned char		Col_B[MAX_RGB_NUMBER];
  unsigned char		RGB_BUF_L[MAX_RGB_NUMBER*3];
  unsigned char		RGB_BUF_R[MAX_RGB_NUMBER*3];
  
  unsigned char		RGB_BTN_1[BTN_RGB_NUMBER*3];  //Version	V1.04	: (1)20240718		: Switch To RGBx25
  unsigned char		RGB_BTN_2[BTN_RGB_NUMBER*3];
  unsigned char		RGB_BTN_3[BTN_RGB_NUMBER*3];
  unsigned char		RGB_BTN_4[BTN_RGB_NUMBER*3];
	//------------------------------------------------------------
}AppRGB_t;

//#define     IAP_LEN            64
//typedef union _IAP_CMD
//{
//    struct
//    {
//        unsigned char    CMDCode;          		/* 命令码 0x81 */
//        unsigned char    Length;             	/* 后续数据长度 */
//        unsigned char    Addr[2];           	/* 擦除地址 */
//    } Erase;                            /* 擦除命令 */
//    struct
//    {
//        unsigned char    CMDCode;          		/* 命令码 0x83 */
//        unsigned char    Length;             	/* 后续数据长度 */
//        unsigned char    Status[2];          	/* 两字节状态，保留 */
//    } Exit;                            	/* 结束命令 */
//    struct
//    {
//        unsigned char    CMDCode;            	/* 命令码 0x82 */
//        unsigned char    Length;             	/* 后续数据长度 */
//        unsigned char    Addr[2];           	/* 校验地址 */
//        unsigned char    Buffer[IAP_LEN-4];  	/* 校验数据 */
//    } Verify;                           /* 校验命令 */
//    struct
//    {
//        unsigned char    CMDCode;           	/* 命令码 0x80 */
//        unsigned char    Length;             	/* 后续数据长度 */
//        unsigned char    Addr[2];           	/* 地址 */
//        unsigned char    Buffer[IAP_LEN-4];  	/* 后续数据 */
//    } Program;                          /* 编程命令 */
//    struct
//    {
//        unsigned char    Buffer[IAP_LEN];    	/* 接收数据包*/
//    } RAW;
//} Packet_t;


enum {
  LedGs8Bit           = 0x31,//49
  LedGs8BitMulti      = 0x32,//50
  LedGs8BitMultiFade  = 0x33,//51
  LedFet              = 0x39,//57
  LedGsUpdate         = 0x3C,//60
  LedDirect           = 0x82,
};

typedef union {
  unsigned char base[64];
  struct {
    struct {
      unsigned char dstNodeID;
      unsigned char srcNodeID;
      unsigned char length;
      unsigned char cmd;
    };
    union {
      struct { //39
        unsigned char color[11][3];//CRGB
      };
      struct { //9
        unsigned char BodyLed;
        unsigned char ExtLed;
        unsigned char SideLed;
      };
      struct { //LedGs8Bit
        unsigned char index;
        unsigned char r;
        unsigned char g;
        unsigned char b;
      };
      struct { //LedGs8BitMulti,LedGs8BitMultiFade
        unsigned char start;
        unsigned char end;//length
        unsigned char skip;
        unsigned char mr;
        unsigned char mg;
        unsigned char mb;
        unsigned char speed;
      };
    };
  };
} Packet_t;

//******************************************************************************
//
//-------------  Variable  define   --------------------
//
//******************************************************************************


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//--------------------------------------------------------------- 
//extern void App_User_ReSet_Btn_RGB(void);	
//extern void App_User_Set_Btn_RGB(unsigned char Index,unsigned char Status);
//extern void App_User_Out_Btn_RGB(void);		

//--------------------------------------------------------------- 
extern void App_RGB_Brightness_Update(void);	
extern void App_RGB_CC_PowerUpdate(unsigned int Value);	
//--------------------------------------------------------------- 
extern void App_Set_IR_RGB_Value(unsigned short KeyCode);
extern void App_Set_TP_RGB_Value(unsigned int   TPKeyCode);
extern void App_Set_HID_RGB_Value(unsigned char *LED_BUF,unsigned char Len);
extern void App_Set_IO4_RGB_Value(unsigned char *LED_BUF,unsigned char Len);  //Version	V1.19	: (1)20250309 	: IO4 MODE RGB Game Ctrl Support
extern void App_Set_TP_RGB_TEST(unsigned int TPKeyCode);
extern void App_RGB_Update(unsigned int Tick);
extern void App_RGB_Init(void);


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

#endif

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
