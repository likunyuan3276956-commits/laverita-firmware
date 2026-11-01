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

#include    <stdio.h>
#include    "at32f415_clock.h"
#include    "App_SW_I2C.h"
#include    "App_BSP.h"

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


//******************************************************************************
//
//------------- Constant  define    -------------------
//
//******************************************************************************

 

//******************************************************************************
//
//-------------  Variable  define   --------------------
//
//******************************************************************************
//uint32_t	SW_I2C_SCL_PIN;
//uint32_t	SW_I2C_SDA_PIN;
//uint32_t	SW_I2C_INT_PIN;
 

//******************************************************************************
//
//	SubProgram List																		
//
//******************************************************************************	
//#define SW_I2C_SCL_OUTPUT() 					{nrf_gpio_cfg_output(SW_I2C_SCL_PIN);}
//#define SW_I2C_SCL_HIGH() 						{nrf_gpio_cfg_output(SW_I2C_SCL_PIN);nrf_gpio_pin_set(SW_I2C_SCL_PIN);}
//#define SW_I2C_SCL_LOW() 							{nrf_gpio_cfg_output(SW_I2C_SCL_PIN);nrf_gpio_pin_clear(SW_I2C_SCL_PIN);}

//#define SW_I2C_SDA_OUTPUT() 					{nrf_gpio_cfg_output(SW_I2C_SDA_PIN);}
//#define SW_I2C_SDA_HIGH() 						{nrf_gpio_pin_set(SW_I2C_SDA_PIN);  nrf_gpio_cfg_output(SW_I2C_SDA_PIN);}
//#define SW_I2C_SDA_LOW() 							{nrf_gpio_pin_clear(SW_I2C_SDA_PIN);nrf_gpio_cfg_output(SW_I2C_SDA_PIN);}
//#define SW_I2C_SDA_INPUT() 						{nrf_gpio_cfg_input(SW_I2C_SDA_PIN,NRF_GPIO_PIN_PULLUP);}
//#define SW_I2C_SDA_READ() 						nrf_gpio_pin_read(SW_I2C_SDA_PIN)


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void I2C_Delay(unsigned char Pause)
{
	unsigned char i;
	for(i=0;i<Pause;i++);
}
/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void SW_I2C_Config(void)
{	
	SW_I2C_SDA_OUTPUT(); 
	SW_I2C_SDA_OUTPUT(); 
	SW_I2C_SCL_OUTPUT();
	SW_I2C_SCL_OUTPUT(); 	
	SW_I2C_Stop();
}
/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void SW_I2C_Start(void)
{
	SW_I2C_SDA_OUTPUT(); 
	SW_I2C_SDA_OUTPUT(); 
	SW_I2C_SCL_OUTPUT();
	SW_I2C_SCL_OUTPUT();
	
	SW_I2C_SDA_HIGH();
	I2C_Delay(5);
	SW_I2C_SCL_HIGH();
	I2C_Delay(5);
	SW_I2C_SDA_LOW();
	I2C_Delay(5);
	SW_I2C_SCL_LOW();
}
/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void SW_I2C_Stop(void)
{
	SW_I2C_SDA_OUTPUT(); 
	SW_I2C_SDA_OUTPUT(); 
	SW_I2C_SCL_OUTPUT();
	SW_I2C_SCL_OUTPUT();
	
	SW_I2C_SDA_LOW();
	//I2C_Delay(5);				//
	SW_I2C_SCL_HIGH();
	//I2C_Delay(5);				//
	SW_I2C_SDA_HIGH();
	//I2C_Delay(5);				//
	SW_I2C_SCL_HIGH();	
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void SW_I2C_Send_ACK(void)
{
	SW_I2C_SDA_OUTPUT();	//SDA output
	SW_I2C_SDA_OUTPUT();
	SW_I2C_SDA_LOW();			//ACK	
	I2C_Delay(5);
	SW_I2C_SCL_HIGH();
	I2C_Delay(5);					//
	SW_I2C_SCL_LOW();
}
/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
void SW_I2C_Send_NACK(void)
{
	SW_I2C_SDA_OUTPUT();	//SDA output
	SW_I2C_SDA_OUTPUT();
	SW_I2C_SDA_HIGH();		//NACK	
	I2C_Delay(5);					//
	SW_I2C_SCL_HIGH();
	I2C_Delay(5);					//
	SW_I2C_SCL_LOW();
}
/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
unsigned char SW_I2C_Read_AByte(void)
{
	//-----------------------------------------------
	unsigned char read_data, i;
	//-----------------------------------------------
	SW_I2C_SDA_INPUT();				//SDA input
	//-----------------------------------------------
	for(i=0;i<8;i++)
	{
		read_data <<= 1;
		//I2C_Delay(15);
		SW_I2C_SCL_HIGH();
		//I2C_Delay(3);					//
		if(SW_I2C_SDA_READ()!=0)
		{
			read_data |= 0x01;
		}
		else
		{
			read_data &= ~0x01;
		}
		//I2C_Delay(15);
		SW_I2C_SCL_LOW();
		I2C_Delay(5);						//
	}
	//-----------------------------------------------
	return read_data;	
	//-----------------------------------------------
}
/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
unsigned char SW_I2C_Write_AByte(char write_buf)
{
	//-----------------------------------------------
	unsigned char i,I2C_ACK_Flag=NACK;
	//-----------------------------------------------
	SW_I2C_SDA_LOW();
	SW_I2C_SDA_OUTPUT();					//SDA output
	SW_I2C_SDA_OUTPUT();
	for(i=0;i<8;i++)
	{
		if(write_buf&0x80)
		{
			SW_I2C_SDA_HIGH();
		}
		else
		{
			SW_I2C_SDA_LOW();
		}
		I2C_Delay(4);
		SW_I2C_SCL_HIGH();
		I2C_Delay(8);
		SW_I2C_SCL_LOW();
		write_buf <<= 1;
	}
	//-----------------------------------------------ACK_IN
	//I2C_Delay(10);							//
	SW_I2C_SDA_INPUT();						//SDA input 
	I2C_Delay(10);
	SW_I2C_SCL_HIGH();
	I2C_Delay(5);
	//-----------------------------------------------
	if(SW_I2C_SDA_READ()!=0)
		I2C_ACK_Flag = NACK;	//nack 
	else 
		I2C_ACK_Flag = ACK;		//ack
	//-----------------------------------------------
	SW_I2C_SCL_LOW();
	//I2C_Delay(10);							//
	//SW_I2C_SDA_LOW();
	//SW_I2C_SDA_OUTPUT();				//SDA output
	//-----------------------------------------------
	return I2C_ACK_Flag;
	//-----------------------------------------------
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
unsigned char SW_I2C_CurrentRead(unsigned int I2CAddr, unsigned char *Buffer,unsigned char Len)
{ 
	//---------------------------------------------------------
	unsigned char i=0;
	//---------------------------------------------------------	
	SW_I2C_Start();	
	//I2C_Delay(150);
	if(SW_I2C_Write_AByte(I2CAddr)==ACK)
	{
		for(i=0;i<Len;i++)
		{
			I2C_Delay(150);
			Buffer[i] = SW_I2C_Read_AByte();
			if(i<Len-1)
			{
				SW_I2C_Send_ACK();
			}
			else
			{
				SW_I2C_Send_NACK();
			}
		}
		//I2C_Delay(250);
		SW_I2C_Stop();
	}
	else
	{
		//I2C_Delay(50);
		SW_I2C_Stop();
		return 0;
	}
	//---------------------------------------------------------	
	return 1;
	//---------------------------------------------------------	
}

/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
unsigned char SW_I2C_ReadWrite(unsigned int I2CAddr,unsigned int RegAddr,unsigned char RW, unsigned char *Buffer,unsigned char Len)
{ 
	//---------------------------------------------------------
	unsigned char i=0;
	//---------------------------------------------------------	Read
	if(RW == READ_CMD)
	{
		SW_I2C_Start();	
		//I2C_Delay(15);		//
		if(SW_I2C_Write_AByte(I2CAddr+WRITE_CMD)==ACK)
		{
			//I2C_Delay(5);		//
			SW_I2C_Write_AByte(RegAddr);
			//I2C_Delay(5);		//
			SW_I2C_Start();		
			if(SW_I2C_Write_AByte(I2CAddr+READ_CMD)==ACK)
			{
				for(i=0;i<Len;i++)
				{
					//I2C_Delay(5);//
					Buffer[i] = SW_I2C_Read_AByte();
					if(i<Len-1)
					{
						SW_I2C_Send_ACK();
					}
					else
					{
						SW_I2C_Send_NACK();
					}
					
				}
				//I2C_Delay(15);
				SW_I2C_Stop();
			}
			else
			{
				//I2C_Delay(15);
				SW_I2C_Stop();
				return 0;
			}
		}
		else
		{
			//I2C_Delay(15);
			SW_I2C_Stop();
			return 0;
		}	
	}
	//---------------------------------------------------------	
	else
	{
		SW_I2C_Start();	
		//I2C_Delay(15);
		if(SW_I2C_Write_AByte(I2CAddr+WRITE_CMD)==ACK)
		{
			//I2C_Delay(5);			//
			SW_I2C_Write_AByte(RegAddr);	
			for(i=0;i<Len;i++)
			{
				//I2C_Delay(5);		//
				SW_I2C_Write_AByte(Buffer[i]);
			}								
			//I2C_Delay(15);
			SW_I2C_Stop();
		}
		else
		{
			//I2C_Delay(15);
			SW_I2C_Stop();
			return 0;
		}	
	}
	//---------------------------------------------------------	
	return 1;
	//---------------------------------------------------------	
}



/******************************************************************************************
Name		:  
Input   :  
Output  :  
describe: 
******************************************************************************************/
unsigned char SW_I2C_ReadWrite_ShortRegAddr(unsigned char I2CAddr,unsigned short RegAddr,unsigned char RW, unsigned char *Buffer,unsigned char Len)
{ 
	//---------------------------------------------------------
	unsigned char i=0;
	//---------------------------------------------------------	Read
	if(RW == READ_CMD)
	{
		SW_I2C_Start();	
		//I2C_Delay(15);
		if(SW_I2C_Write_AByte(I2CAddr+WRITE_CMD)==ACK)
		{
			I2C_Delay(15);
			SW_I2C_Write_AByte(RegAddr>>8);
			I2C_Delay(15);
			SW_I2C_Write_AByte(RegAddr&0xff);
			I2C_Delay(15);
			SW_I2C_Start();		
			if(SW_I2C_Write_AByte(I2CAddr+READ_CMD)==ACK)
			{
				for(i=0;i<Len;i++)
				{
					I2C_Delay(15);
					Buffer[i] = SW_I2C_Read_AByte();
					if(i<Len-1)
					{
						SW_I2C_Send_ACK();
					}
					else
					{
						SW_I2C_Send_NACK();
					}
					
				}
				//I2C_Delay(15);
				SW_I2C_Stop();
			}
			else
			{
				//I2C_Delay(15);
				SW_I2C_Stop();
				return 0;
			}
		}
		else
		{
			//I2C_Delay(15);
			SW_I2C_Stop();
			return 0;
		}	
	}
	//---------------------------------------------------------	
	else
	{
		SW_I2C_Start();	
		//I2C_Delay(15);
		if(SW_I2C_Write_AByte(I2CAddr+WRITE_CMD)==ACK)
		{
			I2C_Delay(15);
			SW_I2C_Write_AByte(RegAddr>>8);
			I2C_Delay(15);
			SW_I2C_Write_AByte(RegAddr&0xff);
			I2C_Delay(15);
			for(i=0;i<Len;i++)
			{
				//I2C_Delay(15);
				SW_I2C_Write_AByte(Buffer[i]);
			}								
			//I2C_Delay(15);
			SW_I2C_Stop();
		}
		else
		{
			//I2C_Delay(15);
			SW_I2C_Stop();
			return 0;
		}	
	}
	//---------------------------------------------------------	
	return 1;
	//---------------------------------------------------------	
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&



