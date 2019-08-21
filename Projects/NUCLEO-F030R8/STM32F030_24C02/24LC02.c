/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_i2c.h"
#include "24LC02.h"

// Initialize I2C
void InitI2C1(void)
{
	I2C_InitTypeDef I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitB;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
	
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_1);
  GPIO_InitB.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitB.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitB.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitB.GPIO_OType = GPIO_OType_OD;
	GPIO_InitB.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
  GPIO_Init(GPIOB, &GPIO_InitB);
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x01;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  
  I2C_InitStructure.I2C_Timing = 0x40B22536;     // SCL 100KHz @ HSI PLL 48MHz
	
  I2C_Init(I2C1, &I2C_InitStructure);
  I2C_Cmd(I2C1, ENABLE);
	
}

// Write one Byte to WordAddress, WordAddress from 0 to 255
void AT24C02_WriteByte(uint8_t WordAddress,uint8_t Data)
{
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);            //IF BUSY
  I2C_TransferHandling(I2C1,AT24C02_Base_Address ,2,I2C_AutoEnd_Mode,I2C_Generate_Start_Write);
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);            //If Write OK
  I2C_SendData(I2C1,WordAddress);
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);            //If Write OK
  I2C_SendData(I2C1,Data);
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET);
}


// Read one Byte from WordAddress, WordAddress from 0 to 255
uint8_t AT24C02_ReadByte(uint8_t WordAddress)
{
	uint8_t Recev = 0x00;
  
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);            //IF BUSY
  I2C_TransferHandling(I2C1,AT24C02_Base_Address ,1,I2C_SoftEnd_Mode,I2C_Generate_Start_Write);
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);            //If Write OK
  I2C_SendData(I2C1,WordAddress);
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TC) == RESET);
  I2C_TransferHandling(I2C1,AT24C02_Base_Address ,1,I2C_AutoEnd_Mode,I2C_Generate_Start_Read);
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET);
  Recev = I2C_ReceiveData(I2C1);
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET);
  return Recev;
	
}


// For AT24C02 page size is 8 Bytes
// Bits 7 to 3 is page address, bits 2 to 0 will be increased automatically in page writting
void AT24C02_PageWrite(uint8_t WordAddress,uint8_t Length,uint8_t* Data)
{
	uint8_t i = 0;

  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);            //IF BUSY
  I2C_TransferHandling(I2C1,AT24C02_Base_Address,Length + 1,I2C_AutoEnd_Mode,I2C_Generate_Start_Write);
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);            //If Write OK
  I2C_SendData(I2C1,WordAddress);
  for(i = 0;i < Length; i++)
  {
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);          //If Write OK
    I2C_SendData(I2C1,Data[i]);
  }
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET);
}


// For AT24C02 page size is 8 Bytes
// Bits 7 to 3 is page address, bits 2 to 0 will be increased automatically in page reading
void AT24C02_SequentialRead(uint8_t WordAddress, uint8_t length , uint8_t* p)
{
	uint8_t i;
  
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);            //IF BUSY
  I2C_TransferHandling(I2C1,AT24C02_Base_Address ,1,I2C_SoftEnd_Mode,I2C_Generate_Start_Write);
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);            //If Write OK
  I2C_SendData(I2C1,WordAddress);
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TC) == RESET);
  I2C_TransferHandling(I2C1,AT24C02_Base_Address ,length,I2C_AutoEnd_Mode,I2C_Generate_Start_Read);
  for(i = 0;i < length;i++)
  {
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET);
    p[i] = I2C_ReceiveData(I2C1);
  }
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET);
}






