/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_i2c.h"

#define AT24C02_Base_Address 0xA0
void AT24C02_WriteByte(uint8_t WordAddress,uint8_t Data);
uint8_t AT24C02_ReadByte(uint8_t WordAddress);
void AT24C02_PageWrite(uint8_t WordAddress,uint8_t Length,uint8_t* Data);
void AT24C02_SequentialRead(uint8_t WordAddress, uint8_t length , uint8_t* p);
void InitI2C1(void);

