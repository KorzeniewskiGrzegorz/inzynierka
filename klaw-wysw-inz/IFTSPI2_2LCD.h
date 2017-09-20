/*
 * clockfunc.h
 *
 *  Created on: Nov 18, 2015
 *      Author: a0876236
 */



#ifndef IFTSPI2_2LCD_H_
#define IFTSPI2_2LCD_H_



#define		AllPins							GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
#define		TFTControlPins					GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
#define		TFTReset						GPIO_PIN_4
#define		LPLEDs							GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
#define		u8 								unsigned char
#define		u16 							unsigned int
#define		u32 							unsigned long
#define		MSB								GPIO_PORTB_BASE
#define		LSB								GPIO_PORTD_BASE
// SSI RELATED DEFINITIONS, ONLY FOR 2.2" LCD //
#define		SCK								GPIO_PIN_2//PORT A, SSI 0 ONLY
#define		CS								GPIO_PIN_3
#define		MISO							GPIO_PIN_4
#define		MOSI							GPIO_PIN_5
#define		DC								GPIO_PIN_4// PORT E
#define		RESET							GPIO_PIN_5// PORT E


#define		LCD_RS			GPIO_PIN_4
#define		LCD_WR			GPIO_PIN_5
#define		LCD_RD			GPIO_PIN_6
#define		LCD_CS			GPIO_PIN_7
#define		LCD_REST		GPIO_PIN_4



// Variables //
uint32_t	SysFreq;
int32_t	LCD_READ;// read_data=0;
unsigned char bitdata;
uint32_t reply[];
static u16 kkk=1 , kkkbk=0;


extern  u16 BACK_COLOR, POINT_COLOR;


extern uint32_t ClockFunction(void);
extern	void	notrequired(void);
//extern	void	useless(void);
extern void TivaLCDInit(void);
extern void Lcd_Init(void);
extern void LCD_Clear(u16 Color);
extern void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
extern void LCD_WR_DATA8(char da);
extern void LCD_WR_DATA(int da);
extern void LCD_WR_REG(char da);

extern void LCD_DrawPoint(u16 x,u16 y);
extern void LCD_DrawPixel(u16 x,u16 y,uint16_t color);
extern void LCD_DrawPoint_big(u16 x,u16 y);
extern void LCD_DrawImage(uint16_t x,uint16_t y, uint16_t w,uint16_t h, uint8_t *data);
u16  LCD_ReadPoint(u16 x,u16 y);
extern void Draw_Circle(u16 x0,u16 y0,u8 r);
extern void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
extern void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
extern void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
extern void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);
extern void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);
extern void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);
extern void LCD_ShowString(u16 x,u16 y,const u8 *p);
extern	void	notrequired(void);



#endif /* CLOCKFUNC_H_ */
