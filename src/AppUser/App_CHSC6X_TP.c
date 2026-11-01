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
#include "App_RGB.h"
#include "App_SW_I2C.h"
#include "App_CHSC6X_TP.h"
#include "flash.h"						

//******************************************************************************
//
//------------- Constant  define    -------------------
//
//******************************************************************************

#define CHSC6X_I2C_ID      	(0x5c) //8bit 

#define EPERM           		(1)
#define DIRECTLY_MODE   		(0x0)
#define DEDICATE_MODE   		(0x1)
#define LEN_CMD_CHK_TX  		(10)
#define LEN_CMD_PKG_TX  		(16)
#define LEN_RSP_CHK_RX  		(8)
#define MAX_BULK_SIZE   		(1024)

#define MAX_IIC_WR_LEN			(8)
#define MAX_IIC_RD_LEN			(16)

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

static unsigned int 	g_chsc6x_cfg_ver = 0; 
static unsigned int 	g_chsc6x_boot_ver = 0;

static unsigned int 	g_mccode;    /* 1:3536 */
//static unsigned int 	g_upgrade_flag = 0; /* 0:driver init upgrade, 1:OTA upgrade */
static unsigned char 	g_i2c_addr;
struct ts_fw_infos 		*g_pfw_infos;

/* to direct memory access mode */
static unsigned char cmd_2dma_42bd[6] = { /*0x42, 0xbd, */ 0x28, 0x35, 0xc1, 0x00, 0x35, 0xae };

//============================================================================= 
unsigned char 	I2C_Raw_Buf[128];
signed short 		Tp_Raw_Buf[APP_MAX_TP_CHAN];
signed short 		Tp_Sen_Buf[APP_MAX_TP_CHAN];//Version	V1.34	: (1)20230212		: New Touch Sensitivity
signed char 	  TP_Sen_Lvl;     //Version	V1.35	: (1)20230221		: New Touch Sensitivity Config
//============================================================================= 

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


//Version	V1.34	: (1)20230212		: New Touch Sensitivity
//#define TEST_BUFEER_SIZE                 128
//#define TEST_FLASH_ADDRESS_START         (0x0800F800)
#define 	TP_DEFAULT_SEN								  300					//
#define 	TP_DEFAULT_MAX								  1600					//


uint16_t Flash_RW_Buffer[FLASH_BUFEER_SIZE];

unsigned short 	TP_SenDefault[APP_MAX_TP_CHAN]=	//34*10			
{
  //---------------------------------------------- Level1
  TP_DEFAULT_SEN, //0:
  TP_DEFAULT_SEN, //1:
  TP_DEFAULT_SEN, //2:
  TP_DEFAULT_SEN, //3:
  TP_DEFAULT_SEN, //4:
  TP_DEFAULT_SEN, //5:
  TP_DEFAULT_SEN, //6:
  TP_DEFAULT_SEN, //7:
  TP_DEFAULT_SEN, //8:
  TP_DEFAULT_SEN, //9:
  TP_DEFAULT_SEN, //10:
  TP_DEFAULT_SEN, //11:
  TP_DEFAULT_SEN, //12:
  TP_DEFAULT_SEN, //13:
  TP_DEFAULT_SEN, //14:
  TP_DEFAULT_SEN, //15:
  TP_DEFAULT_SEN, //16:
  TP_DEFAULT_SEN, //17:
  TP_DEFAULT_SEN, //18:
  TP_DEFAULT_SEN, //19:
  TP_DEFAULT_SEN, //20:
  TP_DEFAULT_SEN, //21:
  TP_DEFAULT_SEN, //22:
  TP_DEFAULT_SEN, //23:
  TP_DEFAULT_SEN, //24:
  TP_DEFAULT_SEN, //25:
  TP_DEFAULT_SEN, //26:
  TP_DEFAULT_SEN, //27:
  TP_DEFAULT_SEN, //28:
  TP_DEFAULT_SEN, //29:
  TP_DEFAULT_SEN, //30:
  TP_DEFAULT_SEN, //31:
// 200, //32:
//  200, //33:
};


unsigned short 	TP_MaxDefault[APP_MAX_TP_CHAN]=	//34*10			
{
  TP_DEFAULT_MAX, //0:
  TP_DEFAULT_MAX, //1:
  TP_DEFAULT_MAX, //2:
  TP_DEFAULT_MAX, //3:
  TP_DEFAULT_MAX, //4:
  TP_DEFAULT_MAX, //5:
  TP_DEFAULT_MAX, //6:
  TP_DEFAULT_MAX, //7:
  TP_DEFAULT_MAX, //8:
  TP_DEFAULT_MAX, //9:
  TP_DEFAULT_MAX, //10:
  TP_DEFAULT_MAX, //11:
  TP_DEFAULT_MAX, //12:
  TP_DEFAULT_MAX, //13:
  TP_DEFAULT_MAX, //14:
  TP_DEFAULT_MAX, //15:
  TP_DEFAULT_MAX, //16:
  TP_DEFAULT_MAX, //17:
  TP_DEFAULT_MAX, //18:
  TP_DEFAULT_MAX, //19:
  TP_DEFAULT_MAX, //20:
  TP_DEFAULT_MAX, //21:
  TP_DEFAULT_MAX, //22:
  TP_DEFAULT_MAX, //23:
  TP_DEFAULT_MAX, //24:
  TP_DEFAULT_MAX, //25:
  TP_DEFAULT_MAX, //26:
  TP_DEFAULT_MAX, //27:
  TP_DEFAULT_MAX, //28:
  TP_DEFAULT_MAX, //29:
  TP_DEFAULT_MAX, //30:
  TP_DEFAULT_MAX, //31:
//  500, //32:
//  500, //33:  
};


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
¸Ð¶È  = ratio,41~48(sens),49~62(sens)
-5    = 32,   5a,         46
-4    = 32,   50,         3c
-3    = 32,   46,         32
-2    = 32,   3c,         28
-1    = 32,   32,         1e
 0    = 32,   28,         14
+1    = 32,   1e,         0f
+2    = 32,   1a,         0a
+3    = 32,   17,         05
+4    = 32,   14,         01
+5    = 32,   0a,         01
******************************************************************************************/
void App_Set_TP_Ratio(unsigned char sensor,unsigned char Value) //Version	V1.35	: (1)20230221		: New Touch Sensitivity Config
{
  //---------------------------------------------------------- 
  printf("Ratio:%02X,%02X,%d \r\n",sensor,Value,TP_Sen_Lvl);
  //---------------------------------------------------------- 
}

void App_Set_TP_Sensi(unsigned char sensor,unsigned char Value) //Version	V1.35	: (1)20230221		: New Touch Sensitivity Config
{
  //---------------------------------------------------------- 
  if (sensor < 0x41 | sensor > 0x62) {
    return;
  }
  //---------------------------------------------------------- 
  // A1=0x41 ~ A8=0x48
  else if (sensor <= 0x48) 
  { 
    if(Value>=0x5a)     TP_Sen_Lvl = -5;
    else if(Value>=0x50)TP_Sen_Lvl = -4;
    else if(Value>=0x46)TP_Sen_Lvl = -3;
    else if(Value>=0x3c)TP_Sen_Lvl = -2;
    else if(Value>=0x32)TP_Sen_Lvl = -1;
    else if(Value>=0x28)TP_Sen_Lvl =  0;
    else if(Value>=0x1e)TP_Sen_Lvl = +1;
    else if(Value>=0x1a)TP_Sen_Lvl = +2;
    else if(Value>=0x17)TP_Sen_Lvl = +3;
    else if(Value>=0x14)TP_Sen_Lvl = +4;
    else if(Value>=0x0a)TP_Sen_Lvl = +5;
  }
  //---------------------------------------------------------- 
  //B1=0x49 ~ B8=0x50
  //C1=0x51   C2=0x52
  //D1=0x53 ~ D8=0x5A
  //E1=0x5B ~ E8=0x62
  //----------------------------------------------------------   
  else 
  { 
    if(Value>=0x46)     TP_Sen_Lvl = -5;
    else if(Value>=0x3c)TP_Sen_Lvl = -4;
    else if(Value>=0x32)TP_Sen_Lvl = -3;
    else if(Value>=0x28)TP_Sen_Lvl = -2;
    else if(Value>=0x1e)TP_Sen_Lvl = -1;
    else if(Value>=0x14)TP_Sen_Lvl =  0;
    else if(Value>=0x0f)TP_Sen_Lvl = +1;
    else if(Value>=0x0a)TP_Sen_Lvl = +2;
    else if(Value>=0x05)TP_Sen_Lvl = +3;
    else if(Value>=0x01)TP_Sen_Lvl = +4;
    else if(Value>=0x01)TP_Sen_Lvl = +5;
  }
  //---------------------------------------------------------- 
  printf("Sensi:%02X,%02X,%d \r\n",sensor,Value,TP_Sen_Lvl);
  //---------------------------------------------------------- 
}


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
void App_Get_TP_Sens_Cofnig(void)
{
	//----------------------------------------------------------
	uint32_t i;
	uint16_t CS=0;
	//----------------------------------------------------------
  // read data from flash 
  flash_read(FLASH_DATA_ADDR_SENSI, Flash_RW_Buffer, FLASH_BUFEER_SIZE);
	printf("FlashR:%08X,%d \r\n",FLASH_DATA_ADDR_SENSI,FLASH_BUFEER_SIZE);
	for(i=0;i<(APP_MAX_TP_CHAN+2);i++)
	{
		printf("%04X",Flash_RW_Buffer[i]);
		if((i!=0)&&(((i+1)%8)==0))printf("\r\n");
	}
	printf("\r\n");
	//----------------------------------------------------------
	CS = UPDATE_CRC_16((unsigned char *)Flash_RW_Buffer,(34+1)*2);
	if(Flash_RW_Buffer[0]==0xA55A && Flash_RW_Buffer[34+1]==CS)
	{
		printf("Load Flash TP Sens:CS=%04X\r\n",Flash_RW_Buffer[34+1]);
		for(i=0;i<APP_MAX_TP_CHAN;i++)
		{
			Tp_Sen_Buf[i]=Flash_RW_Buffer[i+1];
      //if(i==0x03)Tp_Sen_Buf[i] += 200;  //Version	V1.26	: (1)20250401 	: Touch Chan 3 Sensitivity ADJ 200  //Version	V1.12	: (2)20250102 	: Touch Chan 3 Sensitivity ADJ
		}		
	}
	else
	{
		printf("Load Default TP Sens:%04X,%04X\r\n",Flash_RW_Buffer[0],Flash_RW_Buffer[34+1]);
		for(i=0;i<APP_MAX_TP_CHAN;i++)
		{
			Tp_Sen_Buf[i]=TP_SenDefault[i];
      //if(i==0x03)Tp_Sen_Buf[i] += 200;  //Version	V1.26	: (1)20250401 	: Touch Chan 3 Sensitivity ADJ 200  //Version	V1.12	: (2)20250102 	: Touch Chan 3 Sensitivity ADJ
		}		
	}
  //----------------------------------------------------------
  TP_Sen_Lvl  = 0;     //Version	V1.35	: (1)20230221		: New Touch Sensitivity Config
	//----------------------------------------------------------
}



/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
void App_Set_TP_Sens_Cofnig(void)
{
	//----------------------------------------------------------
	uint32_t i;
  error_status err_status;
	flash_status_type status = FLASH_OPERATE_DONE;
	//----------------------------------------------------------
	flash_unlock();
	status = flash_sector_erase(FLASH_DATA_ADDR_SENSI);
	flash_lock();
	printf("Erase Flash: %08X\r\n",status);
	//----------------------------------------------------------
  // fill buffer_write data to test 
	Flash_RW_Buffer[0]=0xA55A;
  for(i = 0; i < APP_MAX_TP_CHAN; i++)
  {
    Flash_RW_Buffer[i+1] = Tp_Sen_Buf[i];
  }	
	Flash_RW_Buffer[34+1] = UPDATE_CRC_16((unsigned char *)Flash_RW_Buffer,(34+1)*2);
	//----------------------------------------------------------
	printf("FlashW:%08X,%02X,%d \r\n",FLASH_DATA_ADDR_SENSI,err_status,FLASH_BUFEER_SIZE);
	for(i=0;i<(APP_MAX_TP_CHAN+2);i++)
	{
		printf("%04X",Flash_RW_Buffer[i]);
		if((i!=0)&&(((i+1)%8)==0))printf("\r\n");
	}
	printf("\r\n");	
	//----------------------------------------------------------
	// write data to flash 
  err_status = flash_write(FLASH_DATA_ADDR_SENSI, Flash_RW_Buffer, FLASH_BUFEER_SIZE);
	//----------------------------------------------------------
//  // read data from flash 
//  flash_read(FLASH_DATA_ADDR_SENSI, buffer_read, FLASH_BUFEER_SIZE);
//	//----------------------------------------------------------
//	printf("FlashR:%08X,%d \r\n",FLASH_DATA_ADDR_SENSI,FLASH_BUFEER_SIZE);
//	for(i=0;i<(34+2);i++)
//	{
//		printf("%04X",buffer_read[i]);
//		if((i!=0)&&(((i+1)%8)==0))printf("\r\n");
//	}
//	printf("\r\n");
	//----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
unsigned char	TP_CMD_BUF[128];		//Version	V1.34	: (1)20230212		: New Touch Sensitivity
void App_TP_Sens_Command_Report(unsigned char *CMDBuffer,unsigned char CMDLen,unsigned char *RSPBuffer,unsigned char RSPLen)//Version	V1.34	: (1)20230212		: New Touch Sensitivity
{
	//----------------------------------------------------------------
	unsigned char 	i;
	unsigned char 	CRC_U8;
	//----------------------------------------------------------------
	//TX Head: 52 45 41 44 00
	//RX Head: 52 52 53 50 00
	//	 Data: 03 20 03 20 03 20 0320 0320 0320 0320 0320 0320 0320 0320 0320 0320 0320 0320 0320
	//   CS	 : 96	
  //TX Head: 52 45 41 44 01
	//RX Head: 52 52 53 50 01
	//	 Data: 03 20 03 20 03 20 0320 0320 0320 0320 0320 0320 0320 0320 0320 0320 0320 0320 0320
	//   CS	 : 96	
  //----------------------------------------------------------------
  //for(i=0; i<RSPLen; i++)
  //{
  //  RSPBuffer[i] = 0x00;
  //}
	//----------------------------------------------------------------
	if(	CMDBuffer[0]==0x52 &&		//R
			CMDBuffer[1]==0x45 && 	//E
			CMDBuffer[2]==0x41 && 	//A
			CMDBuffer[3]==0x44)			//D
	{
    printf("Read Tp Sens \r\n");
    if(CMDBuffer[4]==0x00)
    {
      printf("Read Tp Sens 1-16 : ");
      for(i=0;i<(APP_MAX_TP_CHAN/2);i++)
      {
        printf("%04X",Tp_Sen_Buf[i]);
      }
      printf("\r\n");
      
      RSPBuffer[0]=0x52;				  //R
      RSPBuffer[1]=0x52;				  //R
      RSPBuffer[2]=0x53;				  //S
      RSPBuffer[3]=0x50;				  //P
      RSPBuffer[4]=CMDBuffer[4];  //Index
      for(i=0;i<(APP_MAX_TP_CHAN/2);i++)
      {
        RSPBuffer[HEAD_LEN+(i*2+0)]= (Tp_Sen_Buf[i]>>8)&0xff;
        RSPBuffer[HEAD_LEN+(i*2+1)]= (Tp_Sen_Buf[i]>>0)&0xff;
      }
      RSPBuffer[HEAD_LEN+(APP_MAX_TP_CHAN/2)*2]=UPDATE_CRC(RSPBuffer,HEAD_LEN+(APP_MAX_TP_CHAN/2)*2);
      //App_USB_CDC_SendPacket(TP_CMD_BUF,4+34*2+1);//App_User_USB_SendPacket(TP_CMD_BUF,4+34*2+1);   
    }
    else
    {
      printf("Read Tp Sens 17-32 : ");
      for(i=0;i<(APP_MAX_TP_CHAN/2);i++)
      {
        printf("%04X",Tp_Sen_Buf[(APP_MAX_TP_CHAN/2)+i]);
      }
      printf("\r\n");

      RSPBuffer[0]=0x52;				  //R
      RSPBuffer[1]=0x52;				  //R
      RSPBuffer[2]=0x53;				  //S
      RSPBuffer[3]=0x50;				  //P
      RSPBuffer[4]=CMDBuffer[4];  //Index
      for(i=0;i<(APP_MAX_TP_CHAN/2);i++)
      {
        RSPBuffer[HEAD_LEN+(i*2+0)]= (Tp_Sen_Buf[(APP_MAX_TP_CHAN/2)+i]>>8)&0xff;
        RSPBuffer[HEAD_LEN+(i*2+1)]= (Tp_Sen_Buf[(APP_MAX_TP_CHAN/2)+i]>>0)&0xff;
      }
      RSPBuffer[HEAD_LEN+(APP_MAX_TP_CHAN/2)*2]=UPDATE_CRC(RSPBuffer,HEAD_LEN+(APP_MAX_TP_CHAN/2)*2);
    }
	}
	//----------------------------------------------------------------
	//TX Head: 57 52 49 54 00
	//	 Data: 03 21 03 22 03 23 03 24 03 25 03 26 03 27 03 28 03 29 03 2A 03 2B 03 2C 03 2D 03 2E 03 2F 03 30  
	//   CS	 : B9
	//RX Head: 57 4F 4B B9 
	//TX Head: 57 52 49 54 01 
	//	 Data: 03 31 03 32 03 33 03 34 03 35 03 36 03 37 03 38 03 39 03 3A 03 3B 03 3C 03 3D 03 3E 03 3F 03 40  
	//   CS	 : A0			
	//RX Head: 57 4F 4B A0
	//----------------------------------------------------------------
  else if(CMDBuffer[0]==0x57 &&
					CMDBuffer[1]==0x52 && 
					CMDBuffer[2]==0x49 && 
					CMDBuffer[3]==0x54 &&
					CMDLen>=38)						//5Bytes Head+16*2Bytes Data + 1Byte CS  
	{
    //--------------------------------------------------------------
		for(i=0;i<(HEAD_LEN+(APP_MAX_TP_CHAN/2)*2+1);i++)
		{
			TP_CMD_BUF[i]= CMDBuffer[i];
		}
		CRC_U8=UPDATE_CRC(TP_CMD_BUF,(HEAD_LEN+(APP_MAX_TP_CHAN/2)*2));		
		//--------------------------------------------------------------
		if(CRC_U8==TP_CMD_BUF[(HEAD_LEN+(APP_MAX_TP_CHAN/2)*2)])
		{
			if(CMDBuffer[4]==0x00)
			{
				printf("Write Tp Sens 1-16 \r\n");
				for(i=0;i<(APP_MAX_TP_CHAN/2);i++)
				{
					Tp_Sen_Buf[i] = ((TP_CMD_BUF[HEAD_LEN+(i*2+0)]<<8)|(TP_CMD_BUF[HEAD_LEN+(i*2+1)]));
				}
				//App_Set_TP_Sens_Cofnig();
        RSPBuffer[0]=0x57;
				RSPBuffer[1]=0x4F;	      //O
				RSPBuffer[2]=0x4B;	      //K
				RSPBuffer[3]=CRC_U8;
				RSPBuffer[4]=CMDBuffer[4];
			}
			else
			{
				printf("Write Tp Sens 16-32 \r\n");
				for(i=0;i<(APP_MAX_TP_CHAN/2);i++)
				{
					Tp_Sen_Buf[(APP_MAX_TP_CHAN/2)+i] = ((TP_CMD_BUF[HEAD_LEN+(i*2+0)]<<8)|(TP_CMD_BUF[HEAD_LEN+(i*2+1)]));
				}
				App_Set_TP_Sens_Cofnig();
        RSPBuffer[0]=0x57;
				RSPBuffer[1]=0x4F;	      //O
				RSPBuffer[2]=0x4B;	      //K
				RSPBuffer[3]=CRC_U8;
				RSPBuffer[4]=CMDBuffer[4];
			}
		}
		//--------------------------------------------------------------
		else
		{
      printf("Write Tp Sens CS:%02X-%02X\r\n",CRC_U8,TP_CMD_BUF[HEAD_LEN+(APP_MAX_TP_CHAN/2)*2]);
			RSPBuffer[0]=0x57;
			RSPBuffer[1]=0x46;    //F
			RSPBuffer[2]=0x49;    //I
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
RETURN:0->pass else->fail
******************************************************************************************/
int chsc6x_read_bytes_u16addr_sub(unsigned char id, unsigned short adr, unsigned char *rxbuf, unsigned short lenth)
{
	//----------------------------------------------------------
	//int ret = i2cReadPacket(id,adr,lenth,rxbuf);
	int ret = SW_I2C_ReadWrite_ShortRegAddr(id,adr,READ_CMD, rxbuf,lenth);
	//int ret = HW_I2C_ReadWrite_ShortRegAddr(id,adr,READ_CMD, rxbuf,lenth);
	//----------------------------------------------------------
	if(ret == 1)	{	return 0;	}
	else					{	return -1;}
	//----------------------------------------------------------
}

//RETURN:0->pass else->fail 
int chsc6x_write_bytes_u16addr_sub(unsigned char id, unsigned short adr, unsigned char *txbuf, unsigned short lenth)
{
	//----------------------------------------------------------
	//int ret = i2cSendPacket(id,adr,lenth,txbuf);
	int ret = SW_I2C_ReadWrite_ShortRegAddr(id,adr,WRITE_CMD, txbuf,lenth);
	//int ret = HW_I2C_ReadWrite_ShortRegAddr(id,adr,WRITE_CMD, txbuf,lenth);
	//----------------------------------------------------------
	if(ret == 1)	{	return 0;	}
	else					{	return -1;}
	//----------------------------------------------------------
}


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
 RETURN:0->pass else->fail 
******************************************************************************************/
static int chsc6x_read_bytes_u16addr(unsigned char id, unsigned short adr, unsigned char *rxbuf, unsigned short lenth)
{
	//----------------------------------------------------------
	int ret = 0;
	int retry;
	unsigned short ofs_adr;
	int len = lenth;
	int rd_len = 0;
	int offset = 0;
	//----------------------------------------------------------
	while (len > 0) 
	{
		//----------------------------------------------------------
		ofs_adr = adr + offset;
		if (len > MAX_IIC_RD_LEN) 
		{
			rd_len 	= MAX_IIC_RD_LEN;
			len 	 -= MAX_IIC_RD_LEN;
		} 
		else 
		{
			rd_len 	= len;
			len 		= 0;
		}
		//----------------------------------------------------------
		retry = 0;
		while (chsc6x_read_bytes_u16addr_sub(id, ofs_adr, &rxbuf[offset], rd_len) != 0) 
		{
			if (retry++ == 3) 
			{
				ret = -1;
				break;
			}
		}
		//----------------------------------------------------------
		offset += MAX_IIC_RD_LEN;
		if (ret < 0) 
		{
			break;
		}
		//----------------------------------------------------------
	}
	//----------------------------------------------------------
	return ret;
	//----------------------------------------------------------
}

// RETURN:0->pass else->fail 
static int chsc6x_write_bytes_u16addr(unsigned char id, unsigned short adr, unsigned char *rxbuf, unsigned short lenth)
{
	//----------------------------------------------------------
	int ret = 0;
	int retry;
	unsigned short ofs_adr;
	int len = lenth;
	int wr_len = 0;
	int offset = 0;
	//----------------------------------------------------------
	while (len > 0) 
	{
		//--------------------------------------------------------
		ofs_adr = adr + offset;
		if (len > MAX_IIC_WR_LEN) 
		{
			wr_len 	= MAX_IIC_WR_LEN;
			len 	 -= MAX_IIC_WR_LEN;
		} 
		else 
		{
			wr_len 	= len;
			len 		= 0;
		}
		//--------------------------------------------------------
		retry = 0;
		while (chsc6x_write_bytes_u16addr_sub(id, ofs_adr, &rxbuf[offset], wr_len) != 0) 
		{
			if (retry++ == 3) 
			{
				ret = -1;
				break;
			}
		}
		//----------------------------------------------------------
		offset += MAX_IIC_WR_LEN;
		if (ret < 0) 
		{
			break;
		}
		//----------------------------------------------------------
	}
	//----------------------------------------------------------
	return ret;
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
void chsc6x_tp_reset(void)
{
	//----------------------------------------------------------
	IO_TP_RST_LOW();
	App_BSP_Delay_MS(30);//30ms
	//----------------------------------------------------------
	IO_TP_RST_HIGH();
	App_BSP_Delay_MS(30);//30ms
	//----------------------------------------------------------
}

void chsc6x_tp_reset_ex(void)
{
	//----------------------------------------------------------
	IO_TP_RST_LOW();
	//----------------------------------------------------------
	App_BSP_Delay_MS(30);//30ms
	//----------------------------------------------------------
	IO_TP_RST_HIGH();
	//----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
 <0 : i2c error 
  0 : direct address mode 
  1 : protect mode 
******************************************************************************************/
static int chsc6x_get_i2cmode(void)
{
	//----------------------------------------------------------
	unsigned char regData[4];
	//----------------------------------------------------------
	if (chsc6x_read_bytes_u16addr(g_i2c_addr, 0x01, regData, 3)) 
	{
		return -EPERM;
	}
	//----------------------------------------------------------
	if ((g_i2c_addr == regData[0]) && (regData[2] == 0X01)) 
	{
		return DIRECTLY_MODE;
	}
	//----------------------------------------------------------
	return DEDICATE_MODE;
	//----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
int chsc6x_set_dd_mode(void)
{
	//----------------------------------------------------------
	int mod = -1;
	int retry = 0;
	int ret = 0;
	//----------------------------------------------------------
	chsc6x_tp_reset();
	ret = chsc6x_get_i2cmode();
	if (ret < 0) {
		return ret;
	}
	if (ret == DIRECTLY_MODE) {
		return 0;
	}
	//----------------------------------------------------------
	while (retry++ < 5) 
	{
		App_BSP_Delay_MS(20);
		chsc6x_write_bytes_u16addr(g_i2c_addr, 0x42bd, cmd_2dma_42bd, 6);
		App_BSP_Delay_MS(30);
		mod = chsc6x_get_i2cmode();
		if (mod == DIRECTLY_MODE) {break;}
	}
	//----------------------------------------------------------
	if (mod == DIRECTLY_MODE) 
	{
		return 0;
	} else {
		return -EPERM;
	}
	//----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
void chsc6x_tp_mccode(void)
{
	//----------------------------------------------------------
	unsigned int tmp[3];
	//----------------------------------------------------------
	g_mccode = 0xff;
	//----------------------------------------------------------
	if (chsc6x_read_bytes_u16addr(g_i2c_addr, 0x8000, (unsigned char *) tmp, 12)) 
	{
		printf("chsc6x: read 0x8000-12 fail! \r\n");
		return;
	}
	//----------------------------------------------------------
	if ( 0x544c4e4b == tmp[2] )  //  boot code 
	{   
		if (tmp[0] == 0x35368008) 
		{
			g_mccode = 1;
			g_chsc6x_boot_ver = tmp[1] & 0xffff;
			g_pfw_infos->chsc6x_boot_version = g_chsc6x_boot_ver;
			printf("chsc6x: read 0x8000-12 OK! \r\n");
		}
	} 
	else if ( 0x544c4e00 == (tmp[2]&0xffffff00) ) 
	{
		if (tmp[0] == 0x35368008) 
		{
			g_mccode = 1;
			g_chsc6x_boot_ver = 0;
			g_pfw_infos->chsc6x_boot_version = g_chsc6x_boot_ver;
			printf("chsc6x: IC run flag error! \r\n");
		}
	}
	else // none code
	{     
		tmp[0] = 0;
		if (chsc6x_read_bytes_u16addr(g_i2c_addr, 0x09, (unsigned char *) tmp, 3)) 
		{
			printf("chsc6x: read 0x09-3 fail! \r\n");
			return;
		}
		if (tmp[0] == 0x5c5c5c)	//empty ic
		{ 
			g_mccode = 1;
			printf("chsc6x: empty ic! \r\n");
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
int chsc6x_bulk_down_check(unsigned char *pbuf, unsigned short addr, unsigned short len)
{
	//----------------------------------------------------------
	unsigned int j, k, retry;
	unsigned char rback[128];
	//----------------------------------------------------------
	while (len) 
	{
		k = (len < 128) ? len : 128;
		retry = 0;
		do 
		{
			rback[k - 1] = pbuf[k - 1] + 1;
			chsc6x_write_bytes_u16addr(g_i2c_addr, addr, pbuf,  k);
			chsc6x_read_bytes_u16addr (g_i2c_addr, addr, rback, k);
			for (j = 0; j < k; j++) 
			{
				if (pbuf[j] != rback[j]) {	break;	}
			}
			if (j >= k) {	break;}    // match
		} while (++retry < 3);

		if (j < k) {	break;	}

		addr += k;
		pbuf += k;
		len -= k;
	}
	//----------------------------------------------------------
	return (int)len;
	//----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
******************************************************************************************/
unsigned short chsc6x_checksum_u16(unsigned short *buf, unsigned short length)
{
	//----------------------------------------------------------
	unsigned short sum, len, i;
	//----------------------------------------------------------
	sum = 0;
	len = length >> 1;
	for (i = 0; i < len; i++) {
		sum += buf[i];
	}
	//----------------------------------------------------------
	return sum;
	//----------------------------------------------------------
}

int is_valid_cfg_data(unsigned short *ptcfg)
{
	//----------------------------------------------------------
	if (ptcfg == 0) {	return 0;	}
	//----------------------------------------------------------
	if ((unsigned char) (ptcfg[53] & 0xff) != g_i2c_addr) {
		printf("chsc6x: cfg iic adr error !\r\n");
		return 0;
	}
	//----------------------------------------------------------
	if (chsc6x_checksum_u16(ptcfg, 204)) {
		printf("chsc6x: cfg chk_sum error !\r\n");
		return 0;
	}
	//----------------------------------------------------------
	return 1;
	//----------------------------------------------------------
}

int chsc6x_get_running_cfg(unsigned short *ptcfg, unsigned short addr)
{
	//----------------------------------------------------------
	int retry, err_type;
	retry = 0;
	err_type = 0;
	//----------------------------------------------------------
	chsc6x_set_dd_mode();
	//----------------------------------------------------------
	while (++retry < 5) 
	{
		err_type = 0;
		if (chsc6x_read_bytes_u16addr(g_i2c_addr, addr, (unsigned char *) ptcfg, 204)) 
		{
			App_BSP_Delay_MS(20);
			err_type = 2;    // i2c error 
			continue;
		}

		if (is_valid_cfg_data(ptcfg) == 0) 
		{
			chsc6x_set_dd_mode();
			err_type = 1;    // data error or no data 
			App_BSP_Delay_MS(20);
			continue;
		}
		break;
	}
	//----------------------------------------------------------
	return err_type;
	//----------------------------------------------------------	
}

int chsc6x_find_ver(void)
{
	//----------------------------------------------------------
//	unsigned short buf_tmpcfg[102];
//	//----------------------------------------------------------
//	if (chsc6x_download_ramcode(fw_fcode_burn, sizeof(fw_fcode_burn))) {
//        chsc6x_err("chsc6x: find factory cfg error:ram-code error!\r\n");
//        return -EPERM;
//	}
//	//----------------------------------------------------------
//	if (chsc6x_read_burn_space((unsigned char *) buf_tmpcfg, 0xf000, 204)) {
//        chsc6x_err("chsc6x: read factory cfg fail !\r\n");
//        return -EPERM;
//	}
//	//----------------------------------------------------------
//	if (is_valid_cfg_data(buf_tmpcfg)) {
//        chsc6x_info("chsc6x: read factory cfg OK !\r\n");
//        g_chsc6x_cfg_ver = (unsigned int)buf_tmpcfg[1];
//        g_chsc6x_cfg_ver = (g_chsc6x_cfg_ver<<16) + (unsigned int)buf_tmpcfg[0];
//        g_chsc6x_cfg_ver = g_chsc6x_cfg_ver&0x3ffffff;
//        g_pfw_infos->chsc6x_cfg_version = 0;
//        g_pfw_infos->chsc6x_vendor_id = (g_chsc6x_cfg_ver>>9)&0x7F;
//        g_pfw_infos->chsc6x_project_id = g_chsc6x_cfg_ver&0x01FF;
//	}
  //----------------------------------------------------------  
	return 0;	
	//----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:
FUNC In your systerm init process,Must call this interface function to detec if the TP IC is Chipsemi corp'. 
PARM pfw_infos: to get top 5 fw info in struct ts_fw_infos.
PARM update_ret_flag: point value=1 update succeed; point value=0 update failed, If opend CHSC6X_AUTO_UPGRADE macro.
RETURN 1:is chsc chip, 0:is not chsc chip
******************************************************************************************/
int chsc6x_tp_dect(struct ts_fw_infos *pfw_infos, unsigned char *update_ret_flag)
{
	//----------------------------------------------------------
	//int ret = -1;
	int try_cnt;
	unsigned char dwr=0x05;
	unsigned short buf_tmpcfg[102]; 
	//----------------------------------------------------------
	g_mccode 		= 0xff;        /* default */
	g_i2c_addr 	= CHSC6X_I2C_ID;
	g_pfw_infos = pfw_infos;
	//----------------------------------------------------------
	for(try_cnt=0; try_cnt<5; try_cnt++) 
	{
		if(chsc6x_set_dd_mode()) 
		{
			chsc6x_tp_reset_ex();
			if (chsc6x_bulk_down_check(&dwr, 0x0602, 1) == 0) 
			{    
				dwr = 0x00;
				chsc6x_bulk_down_check(&dwr, 0x0643, 1);    
			} 
			else 
			{
				printf("chsc6x: write 0x0602 failed \r\n");
			}
		} 
		else 
		{
			break;
		}
	}
	//----------------------------------------------------------
	if(try_cnt >= 5) 
	{
		printf("chsc6x: chsc6x_set_dd_mode failed \r\n");
	}
	//printf("chsc6x: chsc_boot size=%d \r\n", sizeof(chsc_boot));
	//----------------------------------------------------------
	chsc6x_tp_mccode();    			// MUST: call this function there!!! 
	printf("chsc6x: g_mccode is 0x%x \r\n",g_mccode);    
	if(g_mccode == 0xff) 
	{
		printf("chsc6x: get mccode fail! \r\n");
		return 0;
	}
	//----------------------------------------------------------
	/*try to get running time tp-cfg. if fail : wrong boot? wrong rom-cfg?*/
	if (chsc6x_get_running_cfg(buf_tmpcfg, 0x9e00) == 0) 
	{
		printf("chsc6x: get_running_cfg pass !\r\n");
		g_chsc6x_cfg_ver = (unsigned int)buf_tmpcfg[1];
		g_chsc6x_cfg_ver = (g_chsc6x_cfg_ver<<16) + (unsigned int)buf_tmpcfg[0];

		g_pfw_infos->chsc6x_cfg_version = g_chsc6x_cfg_ver>>26;
		g_pfw_infos->chsc6x_vendor_id 	=	(g_chsc6x_cfg_ver>>9)&0x7F;
		g_pfw_infos->chsc6x_project_id 	= g_chsc6x_cfg_ver&0x01FF;
		g_pfw_infos->chsc6x_rpt_lcd_x 	= buf_tmpcfg[38];
		g_pfw_infos->chsc6x_rpt_lcd_y 	= buf_tmpcfg[39];
		g_pfw_infos->chsc6x_chip_id 		= buf_tmpcfg[53]&0xff;
		g_pfw_infos->chsc6x_chip_type 	= (buf_tmpcfg[53]>>8)&0xf;
		printf("chsc6x: vid=%d,pid=%d,boot_ver=0x%x,cfg_ver=%d,chip_id=0x%x\r\n", \
		g_pfw_infos->chsc6x_vendor_id,g_pfw_infos->chsc6x_project_id,g_pfw_infos->chsc6x_boot_version, \
		g_pfw_infos->chsc6x_cfg_version,g_pfw_infos->chsc6x_chip_id \
			);
	} 
	else 
	{
		if(0 == buf_tmpcfg[2] && 0 == buf_tmpcfg[3]) 
		{
			g_chsc6x_boot_ver = 0;
		}
		chsc6x_find_ver();
	}
	//----------------------------------------------------------
	if (g_chsc6x_cfg_ver == 0) 
	{
		printf("chsc6x: get tp-info fail! \r\n");
		return 0;
	}
	//----------------------------------------------------------
	chsc6x_tp_reset();
	//----------------------------------------------------------
	return 1;
	//----------------------------------------------------------
}


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:	change to mode 2
******************************************************************************************/
unsigned char App_CHSC6X_TP_Config(void)  //Version	V2.01:(1)20230429		: TP Timeout RST
{
	//----------------------------------------------------------
	int ret = -1;
	int try_cnt = 5;
	//----------------------------------------------------------
	unsigned char fw_update_ret_flag = 0; //1:update OK, !0 fail
	struct ts_fw_infos fw_infos;

	//----------------------------------------------------------
	IO_TP_INT_INPUT();
	IO_TP_RST_OUTPUT();
	//----------------------------------------------------------
	App_Get_TP_Sens_Cofnig();		//Version	V1.34	: (1)20230212		: New Touch Sensitivity
	printf("TP Sens RAW:\r\n");
	for(try_cnt=0;try_cnt<APP_MAX_TP_CHAN;try_cnt++)
	{
		printf("%04X ",Tp_Sen_Buf[try_cnt]);
	}
	printf("\r\n");
	
	//App_Set_TP_Sens_Cofnig();
	//----------------------------------------------------------
	ret = chsc6x_tp_dect(&fw_infos, &fw_update_ret_flag);
	//----------------------------------------------------------
  if(ret==1)
  { 
    for(; try_cnt>0; try_cnt--) 
    {
      g_i2c_addr = CHSC6X_I2C_ID;
      ret = chsc6x_set_dd_mode();
      if(DIRECTLY_MODE == ret) 
      {
        unsigned short 	addr 	= 0xff01;	//dif;
        unsigned char 	size 	= 61;			//valid short size;
        unsigned char 	sync 	= 1;			//
        chsc6x_write_bytes_u16addr(g_i2c_addr, 0x9f22, (unsigned char *)&addr, 2);
        chsc6x_write_bytes_u16addr(g_i2c_addr, 0x9f20, (unsigned char *)&size, 1);
        chsc6x_write_bytes_u16addr(g_i2c_addr, 0x9f24, (unsigned char *)&sync, 1);
        ret = 1;
        break;
      }
      else
      {
        ret = 0;
      }
    }
  }
  return ret;//Version	V2.01:(1)20230429		: TP Timeout RST
	//----------------------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:	
******************************************************************************************/
//unsigned char App_HW_TP_Get_Index(unsigned char Index)
//{
//	//----------------------------------------------------------
//	if(Index==15)			{return 0;	}	//A1
//	else if(Index==13){return 1;	}	//A2
//	else if(Index==11){return 2;	}	//A3
//	else if(Index== 9){return 3;	}	//A4
//	else if(Index== 7){return 4;	}	//A5
//                               
//	else if(Index== 5){return 5;	}	//A6
//	else if(Index== 3){return 6;	}	//A7
//	else if(Index== 1){return 7;	}	//A8
//	else if(Index==31){return 8;	}	//B1
//	else if(Index==30){return 9;	}	//B2
//                     
//	else if(Index==29){return 10;	}	//B3
//	else if(Index==28){return 11;	}	//B4
//	else if(Index==27){return 12;	}	//B5
//	else if(Index==26){return 13;	}	//B6
//	else if(Index==25){return 14;	}	//B7
//                   
//	else if(Index==24){return 15;	}	//B8
//	else if(Index==33){return 16;	}	//C1
//	else if(Index==32){return 17;	}	//C2
//	else if(Index== 0){return 18;	}	//D1
//	else if(Index==14){return 19;	}	//D2
//                   
//	else if(Index==12){return 20;	}	//D3
//	else if(Index==10){return 21;	}	//D4
//	else if(Index== 8){return 22;	}	//D5
//	else if(Index== 6){return 23;	}	//D6
//	else if(Index== 4){return 24;	}	//D7
//                            
//	else if(Index== 2){return 25;	}	//D8
//	else if(Index==16){return 26;	}	//E1
//	else if(Index==23){return 27;	}	//E2
//	else if(Index==22){return 28;	}	//E3
//	else if(Index==21){return 29;	}	//E4
//                            
//	else if(Index==20){return 30;	}	//E5
//	else if(Index==19){return 31;	}	//E6
//	else if(Index==18){return 32;	}	//E7
//	else if(Index==17){return 33;	}	//E8	
//	//----------------------------------------------------------
//	return 0;
//	//----------------------------------------------------------
//}


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe:	change to mode 2
******************************************************************************************/
unsigned char 	ChanMapping[34]=	//34*10			
{
  1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,
  32,33,
};

void App_CHSC6X_TP_Update(void)
{
	//----------------------------------------------------------
	int i,j;
	int ret = -1;
	unsigned char NewVal=0;
  signed int  SensSDJ = 0;    
  signed int  Sensitivity=0;  
  signed int  MaxSensitiv=0;
  //----------------------------------------------------------
  unsigned char status=0;
  static unsigned char ResetEN=0;        
  //static unsigned int TPTimeTick;   //Version	V1.08	: (1)20241016		: Brightness For RGB  
  //static unsigned int TPTimeTOut;   //Version	V1.08	: (1)20241016		: Brightness For RGB  
  static unsigned int TPTimeTCnt;
  static unsigned int TP_RAW_Val = 0;
	//----------------------------------------------------------
	if(IO_TP_INT_READ()==0 && (ResetEN==0))
	{
    //TPTimeTOut = 0;                 //Version	V1.08	: (1)20241016		: Brightness For RGB    //Version	V2.01:(1)20230429		: TP Timeout RST
		ret = chsc6x_get_i2cmode();
		if (ret == DIRECTLY_MODE) 
		{
      TPTimeTCnt = 0; //Version	V2.01:(1)20230429		: TP Timeout RST
			if (chsc6x_read_bytes_u16addr_sub(g_i2c_addr, 0x9f26, (unsigned char *) I2C_Raw_Buf, 70)==0) 
			{
        //----------------------------------------------------
				//App_BSP_LogBuf((unsigned char*)"TP",I2C_Raw_Buf,72);
				//for(i=0;i<34;i++)
        for(i=0;i<APP_MAX_TP_CHAN;i++)
				{
          j = ChanMapping[i];
					Tp_Raw_Buf[j] = ((I2C_Raw_Buf[i*2+1]<<8)|(I2C_Raw_Buf[i*2+0]));
//					printf("%08X ",Tp_Raw_Buf[i]);
          //------------------------------------------------- Val = Sen+(Lvl*5*Sen/100)
          SensSDJ     = TP_Sen_Lvl*5;   //-5 ~ +5
          SensSDJ    *= Tp_Sen_Buf[i];
          SensSDJ    /= 100;
          Sensitivity = Tp_Sen_Buf[i]+SensSDJ;
          
          //Version	V1.26	: (1)20250401 	: Touch Chan 3 Sensitivity ADJ 200  
          if(i==0x03)Sensitivity += 200;
          //------------------------------------------------- 
					if(Tp_Raw_Buf[i]>Sensitivity)														                //Version	V1.35	: (1)20230221		: New Touch Sensitivity Config
					{
						NewVal      = 1;
            //----------------------------------------------- Val = Max+(Lvl*5*Max/100)
            SensSDJ     = TP_Sen_Lvl*5;   //-5 ~ +5
            SensSDJ    *= (TP_MaxDefault[i]+Tp_Sen_Buf[i]);
            SensSDJ    /= 100;
            MaxSensitiv = (TP_MaxDefault[i]+Tp_Sen_Buf[i])+SensSDJ;
            //----------------------------------------------- Val = (Raw-Min)*255/(Max-Min)
            TP_RAW_Val     = Tp_Raw_Buf[i] - Sensitivity;
            TP_RAW_Val    *= 255;
            TP_RAW_Val    /= (MaxSensitiv - Sensitivity);
            //----------------------------------------------- 
            //Version	V1.24	: (1)20250327 	: IO4 MODE Touch Issue           
            //if(TP_RAW_Val>0xfe)TP_RAW_Val=0xfe;     //Version	V1.15	: (1)20250222 	: Touch Crush Issue
            //if(TP_RAW_Val>=0xff)TP_RAW_Val=0xfd;
            //if(TP_RAW_Val==0xfd)TP_RAW_Val--;
            //Version	V1.25	: (1)20250328 	: IO4 MODE Touch Issue
            if(TP_RAW_Val>=0xff)TP_RAW_Val=0xff;
            //-----------------------------------------------
            AppUser.TPRAW[i] = TP_RAW_Val;
            //-----------------------------------------------
            AppUser.TPValue |= (1<<i);
						//Index = App_HW_TP_Get_Index(i);
						//App_Game_TP_KNote_SET(i,Game_TP_Data);
						//printf("%d,%04d-%04d,%d,%03d-%04X ",i,Tp_Raw_Buf[i],Tp_Sen_Buf[i],TP_Sen_Lvl,AppUser.TPRAW[i],AppUser.TPValue);	//Version	V1.35	: (1)20230221		: New Touch Sensitivity Config
            //printf("%d,%04d-%04d,%d,%03d-%04X ",i,Tp_Raw_Buf[i],Tp_Sen_Buf[i],TP_Sen_Lvl,AppUser.TPRAW[i],Sensitivity);     //Version	V1.26	: (1)20250401 	: Touch Chan 3 Sensitivity ADJ 200  
            //-----------------------------------------------
					}
					else
					{
            AppUser.TPRAW[i] = 0;
            AppUser.TPValue &= ~(1<<i);
						//Index = App_HW_TP_Get_Index(i);
						//App_Game_TP_KNote_CLR(i,Game_TP_Data);
					}
				}
        //----------------------------------------------------
				if(NewVal!=0)
				{
          printf("TP: %08X\r\n",AppUser.TPValue);
          //Version	V1.12	: (2)20250102 	: Touch Chan 3 Sensitivity ADJ
//          printf("%04d,%04d,%04d,%04d,%04d,%04d,%04d,%04d - %04d,%04d,%04d,%04d,%04d,%04d,%04d,%04d - %04d,%04d,%04d,%04d,%04d,%04d,%04d,%04d - %04d,%04d,%04d,%04d,%04d,%04d,%04d,%04d\r\n",
//          Tp_Raw_Buf[ 0],Tp_Raw_Buf[ 1],Tp_Raw_Buf[ 2],Tp_Raw_Buf[ 3],Tp_Raw_Buf[ 4],Tp_Raw_Buf[ 5],Tp_Raw_Buf[ 6],Tp_Raw_Buf[ 7],
//          Tp_Raw_Buf[ 8],Tp_Raw_Buf[ 9],Tp_Raw_Buf[10],Tp_Raw_Buf[11],Tp_Raw_Buf[12],Tp_Raw_Buf[13],Tp_Raw_Buf[14],Tp_Raw_Buf[15],
//          Tp_Raw_Buf[16],Tp_Raw_Buf[17],Tp_Raw_Buf[18],Tp_Raw_Buf[19],Tp_Raw_Buf[20],Tp_Raw_Buf[21],Tp_Raw_Buf[22],Tp_Raw_Buf[23],
//          Tp_Raw_Buf[24],Tp_Raw_Buf[25],Tp_Raw_Buf[26],Tp_Raw_Buf[27],Tp_Raw_Buf[28],Tp_Raw_Buf[29],Tp_Raw_Buf[30],Tp_Raw_Buf[31]);	
          
					//printf("TP: %02X %02X %02X %02X %02X %02X %02X \r\n",Game_TP_Data[1],Game_TP_Data[2],Game_TP_Data[3],Game_TP_Data[4],Game_TP_Data[5],Game_TP_Data[6],Game_TP_Data[7]);
				}
        //----------------------------------------------------
			}
		}
		else
		{
      printf("mode err :%d\r\n",TPTimeTCnt);
      TPTimeTCnt++; 
      if(TPTimeTCnt>=25)
      {
        TPTimeTCnt = 0;
        ResetEN    = 1;
        printf("mode err RST\r\n");
      }
		}
	}
  //----------------------------------------------------------
  else
  {
//    if(TPTimeTick!=AppUser.SysTimeTick) 
//    {
//      TPTimeTick = AppUser.SysTimeTick;
//      TPTimeTOut++;
//      if(ResetEN==0)
//      {
//        if(TPTimeTOut>=200)
//        {
//          TPTimeTOut = 0;
//          ResetEN    = 1;
//          printf("TP INT TO1 \r\n");
//        }
//      }
//      else
//      {
//        if(TPTimeTOut>=2000)
//        {
//          TPTimeTOut = 0;
//          ResetEN    = 1;
//          printf("TP INT TO2 \r\n");
//        }
//      }
//    }
  }
  //----------------------------------------------------------
  if(ResetEN==1)
  {
    ResetEN = 2;
    printf("TP Err,Soft RST --------------- \r\n");
    chsc6x_tp_reset();
    status = App_CHSC6X_TP_Config();
    printf("TP Soft RST Ststus:%02X --------------- \r\n",status);
    if(status==0x01)
    {
      ret = chsc6x_get_i2cmode();
      if (ret == DIRECTLY_MODE) 
      {
        ResetEN = 0;
        printf("TP Soft Mode Chk OK  \r\n");
      }
    }
  }
	//----------------------------------------------------------
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&





