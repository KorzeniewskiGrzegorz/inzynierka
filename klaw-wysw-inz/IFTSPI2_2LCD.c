#include <stdint.h>
#include <stdbool.h>
#include "IFT_LCD_PenColor.h"
#include "IFT_LCD_font.h"
#include "IFTSPI2_2LCD.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"



uint32_t ClockFunction(void)
{

	uint32_t ui32SysClock;

	ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                SYSCTL_CFG_VCO_480), 120000000);

	return(ui32SysClock);
}

void	notrequired(void)
{

}

void LCD_Writ_Bus(char da)   //Parallel Data Write function
{
		char bitdata;
		bitdata=da;
/*		LCD_SDI=bit7;LCD_SCK=0;LCD_SCK=1;
		LCD_SDI=bit6;LCD_SCK=0;LCD_SCK=1;
		LCD_SDI=bit5;LCD_SCK=0;LCD_SCK=1;
		LCD_SDI=bit4;LCD_SCK=0;LCD_SCK=1;
		LCD_SDI=bit3;LCD_SCK=0;LCD_SCK=1;
		LCD_SDI=bit2;LCD_SCK=0;LCD_SCK=1;
		LCD_SDI=bit1;LCD_SCK=0;LCD_SCK=1;
		LCD_SDI=bit0;LCD_SCK=0;LCD_SCK=1;
		*/
		SSIDataPut(SSI0_BASE,bitdata);
		while(SSIBusy(SSI0_BASE));

}
void LCD_WR_DATA8_SSI(char da) //Send -8 bit parameter data  // CONVERTED TO CCS
{
    //DC=1;
    GPIOPinWrite(GPIO_PORTE_BASE, DC,0x10); //Pulses the dc line
	LCD_Writ_Bus(da);
}
 void LCD_WR_DATA(int da) // CONVERTED TO CCS
{
	    //DC=1;
	    GPIOPinWrite(GPIO_PORTE_BASE, DC,0x10); //Pulses the dc line
	    LCD_Writ_Bus(da>>8);
	    LCD_Writ_Bus(da);
}
void LCD_WR_REG(char da)	 // CONVERTED TO CCS
{
    //DC=0;
    GPIOPinWrite(GPIO_PORTE_BASE, DC,0); //Pulses the WR line
	LCD_Writ_Bus(da);
}
// void LCD_WR_REG_DATA(int reg,int da) // CONVERTED TO CCS
//{
//    LCD_WR_REG(reg);
//	LCD_WR_DATA(da);
//}
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)  // CONVERTED TO CCS
{
	   LCD_WR_REG(0x2a);
	   LCD_WR_DATA8_SSI(x1>>8);
	   LCD_WR_DATA8_SSI(x1);
	   LCD_WR_DATA8_SSI(x2>>8);
	   LCD_WR_DATA8_SSI(x2);

	   LCD_WR_REG(0x2b);
	   LCD_WR_DATA8_SSI(y1>>8);
	   LCD_WR_DATA8_SSI(y1);
	   LCD_WR_DATA8_SSI(y2>>8);
	   LCD_WR_DATA8_SSI(y2);

	   LCD_WR_REG(0x2C);
}
///////////////////////////newly imported///////////////////////////////
//    void Write_Command(unsigned int Wcommand)
//    {
////      TFT_RD = 1;
//		GPIOPinWrite(GPIO_PORTC_BASE, LCD_RD,LCD_RD);
////      TFT_RS = 0;
//    	GPIOPinWrite(GPIO_PORTC_BASE, LCD_RS,LCD_RS);
////      TFT_DP_Hi = wcommand >> 8;
//    	GPIOPinWrite(MSB, AllPins,(Wcommand >>8));
////      TFT_DP_Lo = wcommand ;
//    	GPIOPinWrite(LSB, AllPins,Wcommand);
//    	//      TFT_WR = 0;
//    	GPIOPinWrite(GPIO_PORTC_BASE, LCD_WR,0);
//    	//      TFT_WR = 1 ;
//    	GPIOPinWrite(GPIO_PORTC_BASE, LCD_WR,LCD_WR);
//    }
//
//    void Write_Data(unsigned int Wdata)
//    {
////      TFT_RD = 1;
//    	GPIOPinWrite(GPIO_PORTC_BASE, LCD_RD,LCD_RD);
////      TFT_RS = 1 ;
//    	GPIOPinWrite(GPIO_PORTC_BASE, LCD_RS,LCD_RS);
////      TFT_DP_Hi = Wdata >>8 ;
//    	GPIOPinWrite(MSB, AllPins,(Wdata >>8));
////      TFT_DP_Lo = wdata;
//    	GPIOPinWrite(LSB, AllPins,Wdata);
////      TFT_WR = 0;
//    	GPIOPinWrite(GPIO_PORTC_BASE, LCD_WR,0);
////      TFT_WR = 1 ;
//    	GPIOPinWrite(GPIO_PORTC_BASE, LCD_WR,LCD_WR);
//
//    }
//    void Write_Command_Data(unsigned int Wcommand,unsigned int Wdata)
//    {
//       Write_Command(Wcommand);
//       Write_Data(Wdata);
//    }
void Lcd_Init(void)
{
	GPIOPinWrite(GPIO_PORTA_BASE, CS,CS);
    //TFT_RST=1;
	GPIOPinWrite(GPIO_PORTE_BASE, RESET,0x20);
    //delay_ms(5);
	SysCtlDelay(SysCtlClockGet()/2);// 5 ms delay
    //TFT_RST=0;
	GPIOPinWrite(GPIO_PORTE_BASE, RESET,0);
    //delay_ms(15);
	SysCtlDelay(SysCtlClockGet()/2);// 15 ms delay
    //TFT_RST=1;
	GPIOPinWrite(GPIO_PORTE_BASE, RESET,0x20);
    //delay_ms(15);
	SysCtlDelay(SysCtlClockGet()/2);// 15 ms delay
    //TFT_CS =0;
	GPIOPinWrite(GPIO_PORTA_BASE, CS,0x08);
	SysCtlDelay(SysCtlClockGet()/100);// 15 ms delay
	GPIOPinWrite(GPIO_PORTA_BASE, CS,0);

	LCD_WR_REG(0xCB);
    LCD_WR_DATA8_SSI(0x39);
    LCD_WR_DATA8_SSI(0x2C);
    LCD_WR_DATA8_SSI(0x00);
    LCD_WR_DATA8_SSI(0x34);
    LCD_WR_DATA8_SSI(0x02);

    LCD_WR_REG(0xCF);
    LCD_WR_DATA8_SSI(0x00);
    LCD_WR_DATA8_SSI(0XC1);
    LCD_WR_DATA8_SSI(0X30);

    LCD_WR_REG(0xE8);
    LCD_WR_DATA8_SSI(0x85);
    LCD_WR_DATA8_SSI(0x00);
    LCD_WR_DATA8_SSI(0x78);

    LCD_WR_REG(0xEA);
    LCD_WR_DATA8_SSI(0x00);
    LCD_WR_DATA8_SSI(0x00);

    LCD_WR_REG(0xED);
    LCD_WR_DATA8_SSI(0x64);
    LCD_WR_DATA8_SSI(0x03);
    LCD_WR_DATA8_SSI(0X12);
    LCD_WR_DATA8_SSI(0X81);

    LCD_WR_REG(0xF7);
    LCD_WR_DATA8_SSI(0x20);

    LCD_WR_REG(0xC0);    //Power control
    LCD_WR_DATA8_SSI(0x23);   //VRH[5:0]

    LCD_WR_REG(0xC1);    //Power control
    LCD_WR_DATA8_SSI(0x10);   //SAP[2:0];BT[3:0]

    LCD_WR_REG(0xC5);    //VCM control
    LCD_WR_DATA8_SSI(0x3e);
    LCD_WR_DATA8_SSI(0x28);

    LCD_WR_REG(0xC7);    //VCM control2
    LCD_WR_DATA8_SSI(0x86);  //--

    LCD_WR_REG(0x36);    // Memory Access Control
    LCD_WR_DATA8_SSI(0x48);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA8_SSI(0x55);

    LCD_WR_REG(0xB1);
    LCD_WR_DATA8_SSI(0x00);
    LCD_WR_DATA8_SSI(0x18);

    LCD_WR_REG(0xB6);    // Display Function Control
    LCD_WR_DATA8_SSI(0x08);
    LCD_WR_DATA8_SSI(0x82);
    LCD_WR_DATA8_SSI(0x27);

    LCD_WR_REG(0xF2);    // 3Gamma Function Disable
    LCD_WR_DATA8_SSI(0x00);

    LCD_WR_REG(0x26);    //Gamma curve selected
    LCD_WR_DATA8_SSI(0x01);

    LCD_WR_REG(0xE0);    //Set Gamma
    LCD_WR_DATA8_SSI(0x0F);
    LCD_WR_DATA8_SSI(0x31);
    LCD_WR_DATA8_SSI(0x2B);
    LCD_WR_DATA8_SSI(0x0C);
    LCD_WR_DATA8_SSI(0x0E);
    LCD_WR_DATA8_SSI(0x08);
    LCD_WR_DATA8_SSI(0x4E);
    LCD_WR_DATA8_SSI(0xF1);
    LCD_WR_DATA8_SSI(0x37);
    LCD_WR_DATA8_SSI(0x07);
    LCD_WR_DATA8_SSI(0x10);
    LCD_WR_DATA8_SSI(0x03);
    LCD_WR_DATA8_SSI(0x0E);
    LCD_WR_DATA8_SSI(0x09);
    LCD_WR_DATA8_SSI(0x00);

    LCD_WR_REG(0XE1);    //Set Gamma
    LCD_WR_DATA8_SSI(0x00);
    LCD_WR_DATA8_SSI(0x0E);
    LCD_WR_DATA8_SSI(0x14);
    LCD_WR_DATA8_SSI(0x03);
    LCD_WR_DATA8_SSI(0x11);
    LCD_WR_DATA8_SSI(0x07);
    LCD_WR_DATA8_SSI(0x31);
    LCD_WR_DATA8_SSI(0xC1);
    LCD_WR_DATA8_SSI(0x48);
    LCD_WR_DATA8_SSI(0x08);
    LCD_WR_DATA8_SSI(0x0F);
    LCD_WR_DATA8_SSI(0x0C);
    LCD_WR_DATA8_SSI(0x31);
    LCD_WR_DATA8_SSI(0x36);
    LCD_WR_DATA8_SSI(0x0F);

    LCD_WR_REG(0x11);    //Exit Sleep
 //   delayms(120);
	SysCtlDelay(SysCtlClockGet()/2);// 200 ms delay

    LCD_WR_REG(0x29);    //Display on
    LCD_WR_REG(0x2c);

    SysCtlDelay(SysCtlClockGet()/10);

   // LCD_Clear(BLACK);
	BACK_COLOR=BLUE;
	POINT_COLOR=WHITE;

}

//void TFT_Set_Address(unsigned int PX1,unsigned int PY1,unsigned int PX2,unsigned int PY2)
//{
//  Write_Command_Data(68,(PX2 << 8) + PX1 );  //Column address start2
//  Write_Command_Data(69,PY1);      //Column address start1
//  Write_Command_Data(70,PY2);  //Column address end2
//  Write_Command_Data(78,PX1);      //Column address end1
//  Write_Command_Data(79,PY1);  //Row address start2
//  Write_Command(34);
//}
//
//void TFT_Fill(unsigned int color)
//{
//  unsigned int i,j;
////  TFT_CS  = 0;
//  	  GPIOPinWrite(GPIO_PORTC_BASE, LCD_CS,0);
//  TFT_Set_Address(0,0,239,319);
//
//  Write_Data(color);
//
//  for(i = 0; i <= 319; i++)
//  {
//    for(j = 0; j <= 239; j++)
//    {
//        //TFT_WR = 0;
//    	GPIOPinWrite(GPIO_PORTC_BASE, LCD_WR,0);
//        //TFT_WR = 1;
//    	GPIOPinWrite(GPIO_PORTC_BASE, LCD_WR,LCD_WR);
//    }
//  }

//  TFT_CS  = 1;
//  	  GPIOPinWrite(GPIO_PORTC_BASE, LCD_CS,LCD_CS);
//}
//
//// Clear screen function
////Color:To clear the screen filled with color
void LCD_Clear(u16 Color) // CONVERTED TO CCS
{
	xSemaphoreTake(  semaphore_lcd, portMAX_DELAY) ;
	u8 VH,VL;
	u16 i,j;
	int x=1,y=1;
	VH=Color>>8;
	VL=Color;
//	kkk	= 100;
//	kkkbk =0
	Address_set(0,0,LCD_W,LCD_H);

    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++) // LCD_H
	   	{

        	 LCD_WR_DATA8_SSI(VH);
			 LCD_WR_DATA8_SSI(VL);
	    }
	  }

//    kkkbk = kkk+1;
//    kkk += 1;
//    if(kkk > 240)
//    	{
//    		kkk = 1;
//    		kkkbk = 0;
//    	}
    xSemaphoreGive(  semaphore_lcd) ;
}
////Dotted
////POINT_COLOR:The color of this point
void LCD_DrawPoint(u16 x,u16 y)
{

	Address_set(x,y,x,y);//Setting the cursor position
	LCD_WR_DATA(POINT_COLOR);

}

////Dotted
////POINT_COLOR:The color of this point
void LCD_DrawPixel(u16 x,u16 y,uint16_t color)
{

	Address_set(x,y,x,y);//Setting the cursor position
	LCD_WR_DATA(color);

}

void LCD_DrawImage(uint16_t x,uint16_t y, uint16_t w,uint16_t h, uint8_t *data){
	xSemaphoreTake(  semaphore_lcd, portMAX_DELAY) ;
	  uint16_t color;
	  uint8_t msb, lsb;
	int i, j;

	for (i=0; i< h; i++)
	  {

	    for (j=0; j<w; j++)
	    	{
	    		msb = *data++;     // blue
			  lsb = *data++;     // green
			  color =  (msb<<8)|lsb;     // red




	    	 LCD_DrawPixel( j,i,color);


	    	}

	  }
	xSemaphoreGive(  semaphore_lcd) ;
}
//// Draw a big point
////POINT_COLOR:The color of this point
//void LCD_DrawPoint_big(u16 x,u16 y) // CONVERTED TO CCS
//{
//	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
//}
// Fill in the designated area specified color
// Size of the area:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color) // CONVERTED TO CCS
{
	xSemaphoreTake(  semaphore_lcd, portMAX_DELAY) ;
	u16 i,j;
	Address_set(xsta,ysta,xend,yend);      //Setting the cursor position
	for(i=ysta;i<=yend;i++)
	{
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//Setting the cursor position
	}
	xSemaphoreGive(  semaphore_lcd) ;
}
// Draw the line
//x1,y1:Starting point coordinates
//x2,y2:End coordinates
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2) // CONVERTED TO CCS
{
	xSemaphoreTake(  semaphore_lcd, portMAX_DELAY) ;
	u16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;

	delta_x=x2-x1; //Calculate the coordinates of the incremental
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1; //Set single-step directions
	else if(delta_x==0)incx=0;//Vertical line
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//Level
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //Select the basic incremental axis
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )//Drawing a line output
	{
		LCD_DrawPoint(uRow,uCol);//Dotted
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
	xSemaphoreGive(  semaphore_lcd) ;
}
//Draw a rectangle
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2) // CONVERTED TO CCS
{
	xSemaphoreTake(  semaphore_lcd, portMAX_DELAY) ;
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
	xSemaphoreGive(  semaphore_lcd) ;
}
//A circle the size of the appointed position draw
//(x,y):The center
//r    :Radius
void Draw_Circle(u16 x0,u16 y0,u8 r)  // CONVERTED TO CCS
{
	xSemaphoreTake(  semaphore_lcd, portMAX_DELAY) ;
	int a,b;
	int di;
	a=0;b=r;
	di=3-(r<<1);             //Judgment flag next point position
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3
		LCD_DrawPoint(x0+b,y0-a);             //0
		LCD_DrawPoint(x0-a,y0+b);             //1
		LCD_DrawPoint(x0-b,y0-a);             //7
		LCD_DrawPoint(x0-a,y0-b);             //2
		LCD_DrawPoint(x0+b,y0+a);             //4
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6
		LCD_DrawPoint(x0-b,y0+a);
		a++;
		//Using the Bresenham algorithm Circle
		if(di<0)di +=4*a+6;
		else
		{
			di+=10+4*(a-b);
			b--;
		}
		LCD_DrawPoint(x0+a,y0+b);
	}
	xSemaphoreGive(  semaphore_lcd) ;
}
////Displays a character at the specified position
//
//// num "" ---> "~"
//// mode: overlay mode (1) or non-overlapping mode (0)
//// Display a character at the specified location
//
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode) // CONVERTED TO CCS
{

    u8 temp;
    u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;
    if(x>LCD_W-16||y>LCD_H-16)
    	{
    		return;
    	}
	//Settings window
	num=num-' ';//Obtained after the offset value
	Address_set(x,y,x+8-1,y+16-1);      //Setting the cursor position
	if(!mode) //Non-overlapping mode
	{
		for(pos=0;pos<16;pos++)
		{
			temp=asc2_1608[(u16)num*16+pos];		 //Call 1608 fonts
			for(t=0;t<8;t++)
		    {
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WR_DATA(POINT_COLOR);
				temp>>=1;
				x++;
		    }
			x=x0;
			y++;
		}
	}else//Superimposition
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //Call 1608 fonts
			for(t=0;t<8;t++)
		    {
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//Draw a point
		        temp>>=1;
		    }
		}
	}
	POINT_COLOR=colortemp;

}
// m ^ n function
u32 mypow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)result*=m;
	return result;
}
// Show two figures
// x, y: starting point coordinates
// len: Digits
// color: color
// num: value (0 to 4294967295);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len) // CONVERTED TO CCS
{
	xSemaphoreTake(  semaphore_lcd, portMAX_DELAY) ;
	u8 t,temp;
	u8 enshow=0;
	num=(u16)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1;

		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0);
	}
	xSemaphoreGive(  semaphore_lcd) ;
}
// Show two figures
// x, y: starting point coordinates
// num: number (0 to 99);
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len) // CONVERTED TO CCS
{
	xSemaphoreTake(  semaphore_lcd, portMAX_DELAY) ;
	u8 t,temp;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0);
	}
	  xSemaphoreGive(  semaphore_lcd) ;
}
// Display the string
// x, y: starting point coordinates
// * p: string starting address
// With 16 fonts
void LCD_ShowString(u16 x,u16 y,const u8 *p) // CONVERTED TO CCS
{

	xSemaphoreTake(  semaphore_lcd, portMAX_DELAY) ;
    while(*p!='\0')
    {
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16)
        {
        	y=x=0;
        	LCD_Clear(RED);
        }
        LCD_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }
    xSemaphoreGive(  semaphore_lcd) ;
}
//
//// Display a character (32 * 33 size) at the specified location
//// dcolor content color, gbcolor for Beijing color
//void show_char(unsigned int x,unsigned int y,unsigned char index)  // CONVERTED TO CCS
//{
//	unsigned char i,j;
//	unsigned char *temp=symbol;
//    Address_set(x,y,x+31,y+31); //Settings area
//	temp+=index*128;
//	for(j=0;j<128;j++)
//	{
//		for(i=0;i<8;i++)
//		{
//		 	if((*temp&(1<<i))!=0)
//			{
//				LCD_WR_DATA(POINT_COLOR);
//			}
//			else
//			{
//				LCD_WR_DATA(BACK_COLOR);
//			}
//		}
//		temp++;
//	 }
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////ENABLING PORTS//////////////////////////////////////////////
void	TivaLCDInit(void)
{
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // PE4 = D/C' , PE5 = RESET
	    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOE);
	    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // LEDs for debugging only, not requierd in final
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);// SSI pins
	    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOA);
	    SysCtlDelay(SysCtlClockGet()/10);// 100mS delay
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_SSI0);

//	    never_declared = 35qr;

	   // GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LPLEDs);

/////////////////////OTHER CONTROL LINES////////////////////////////////////////////
	    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, CS);//
	    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, DC|RESET);

/////////////////SSI CONFIG HERE//////////////////////////////////////////////////////

	    SSIDisable(SSI0_BASE);	// disables th SSI module as required for init
		GPIOPinConfigure(GPIO_PA2_SSI0CLK);
		GPIOPinConfigure(GPIO_PA4_SSI0RX);
		GPIOPinConfigure(GPIO_PA5_SSI0TX);

		GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_4|GPIO_PIN_5);

		SSIClockSourceSet(SSI0_BASE, SSI_CLOCK_SYSTEM); // sets the system clock as the source of clock

		SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_2,SSI_MODE_MASTER, 20000000, 8);// defines base, System clk, Mode 0 = SPH = SPO = 0,Master, 400 KHz, no. of bits = 8 = 1 byte transfer

		SSIEnable(SSI0_BASE); // enables SSI

////////////////////// SSI CONFIG ENDS/////////////////////////////////////////
}
/////
