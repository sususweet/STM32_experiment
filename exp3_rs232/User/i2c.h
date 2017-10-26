#ifndef _I2C_H_
#define _I2C_H_

#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78
#define ADDR_24LC02		0xA0
#define I2C_PAGESIZE	4		/* 24C01/01Aҳ������4�� */

/* Private function prototypes -----------------------------------------------*/
void I2C_Configuration(void);
uint8_t I2C_Read(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num);
uint8_t I2C_Write(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num);

#endif

#ifndef __OLED_I2C_H
#define	__OLED_I2C_H
#define OLED_ADDRESS	0x78 
void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void DelayMs(unsigned int tt);

extern unsigned char F16x16[];
extern const unsigned char F6x8[][6];
extern const unsigned char F8X16[];


#endif
