
#ifndef LCD1_H_
#define LCD1_H_


#include <avr/io.h>
#include <Arduino.h>

                                   
	
#define FONT_WIDTH 8            
#define BYTES_PER_ROW 20        
#define GRAPH_BASE 0x0C00                 
#define TEXT_BASE 0x0000           

#define XMAX 160        
#define XMIN 0
#define YMAX 128
#define YMIN 0

#define WR_H         PORTC  |=  (1<<PC0)
#define WR_L         PORTC  &= ~(1<<PC0)

#define RD_H         PORTC  |=  (1<<PC1)
#define RD_L         PORTC  &= ~(1<<PC1)

#define CE_H         PORTC  |=  (1<<PC2)
#define CE_L         PORTC  &= ~(1<<PC2)

#define CD_H         PORTC  |=  (1<<PC3)
#define CD_L         PORTC  &= ~(1<<PC3)

#define RST_H        PORTB  |=  (1<<PB1)
#define RST_L        PORTB  &= ~(1<<PB1)



//**********************************************************************************************************

//    COMMAND DEFINITIONS 

//******************************************************************************************************

//   REGISTERS SETTING 
#define SET_CURSOR_POINTER       0x21
#define SET_OFFSET_REGISTER      0x22
#define SET_ADDRESS_POINTER      0x24

//   SET CONTROL WORD 
#define SET_TEXT_HOME_ADDRESS    0x40 
#define SET_TEXT_AREA            0x41 
#define SET_GRAPHIC_HOME_ADDRESS 0x42
#define SET_GRAPHIC_AREA         0x43
  
//   MODE SET
#define MODE_SET                 0x80    //  OR mode  
#define EXOR_MODE                0x81    //  EXOR mode 

//   DISPLAY MODE 
#define DISPLAY_MODE             0x90    //  Display off 
#define TEXT_ON_GRAPHIC_ON       0x9C    //  Text on, graphic on 

//   CURSOR PATTERN SELECT 
#define CURSOR_1_LINE            0xA0    //  1-line cursor 
#define CURSOR_2_LINE            0xA1    //  2-line cursor
#define CURSOR_3_LINE            0xA2    //  3-line cursor
#define CURSOR_4_LINE            0xA3    //  4-line cursor
#define CURSOR_5_LINE            0xA4    //  5-line cursor
#define CURSOR_6_LINE            0xA5    //  6-line cursor
#define CURSOR_7_LINE            0xA6    //  7-line cursor
#define CURSOR_8_LINE            0xA7    //  8-line cursor

//   DATA AUTO READ/WRITE 
#define DATA_AUTO_READ           0xB1    //  Set Data Auto Read 
#define DATA_AUTO_WRITE          0xB0    //  Set Data Auto Write 
#define AUTO_RESET               0xB2    //  Auto Reset 

//   DATA READ/WRITE 
#define DATA_WRITE_AND_INCREMENT    0xC0 //  Data Write and Increment ADP 
#define DATA_READ_AND_INCREMENT     0xC1 //  Data Read and Increment ADP
#define DATA_WRITE_AND_DECREMENT    0xC2 //  Data Write and Decrement ADP
#define DATA_READ_AND_DECREMENT     0xC3 //  Data Read and Decrement ADP 
#define DATA_WRITE_AND_NON_VARIABLE 0xC4 //  Data Write and Non-variable ADP                    
#define DATA_READ_AND_NON_VARIABLE  0xC5 //  Data Read and Non-variable ADP

//   SCREEN PEEK 
#define SCREEN_PEEK                 0xE0 //  Screen Peek 

//  SCREEN COPY 
#define SCREEN_COPY                 0xE8 //  Screen Copy 

//  BIT SET/RESET 
#define BIT_SET                     0xF8 //   Bit Set

class Glsd160128	
{
	private:
    void setAddress(unsigned int addr);
	int  teststat(void);               // check LCD display status pbrt
	void writeData(int byte);              // write data byte to LCD module
    int  readData(void);                 // get data byte from LCD module
    
    void writeCommand(int byte);             // write command byte to LCD module
    void Setup(void);                      // make sure control lines are at correct leve
    
    void textPos(int x, int y);          // set memory pointer to (x,y) position (text)
    void setPixel(int column, int row);//,char show1
    void IquarterCircle(int x, int y, int r);
    void IIquarterCircle(int x, int y, int r);
    void IIIquarterCircle(int x, int y, int r);
    void IVquarterCircle(int x, int y, int r);

  
    public:
    void Initialize(void);  // initialize LCD memory and display modes
    void clearGraphic(void);             // clear graphics memory of LCD
    void clearText(void);           // clear text memory of LCD

    void printStr(int x,int y,const char *string);       // send string of characters to LCD
    void printInt(int val);

    void Line(int x1, int y1, int x2, int y2); //, unsigned char show

    void Circle(int x, int y, int r);  //, unsigned char show
    void halfCircle(int x, int y, int r);
    void filledCircle(int x0, int y0, int r);

    void Box(int x1, int y1, int x2, int y2);  //, unsigned char show
    void circleBox(int x1, int y1, int x2, int y2, int r);
    void filledBox(int x1, int y1, int x2, int y2);
	
	void  Triangle(int x1, int y1,int x2, int y2,int x3, int y3 );
    void  filledTriangle(int x1, int y1,int x2, int y2,int x3, int y3 );
};



#endif  /* LCD1_H_ */

