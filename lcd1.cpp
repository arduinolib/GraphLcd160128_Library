
#include <avr/io.h>
#include <lcd1.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>    
#include <math.h>  
#include <Arduino.h>


// -------------------------------------------------------------------------------

int  Glsd160128::teststat(void)  
{
int stat;

  DDRD = 0x00;

  CD_H;        
  RD_L;        
  CE_L;        

  _delay_us(10);

  stat = PIND;      

  CE_H;         
  RD_H;         
  DDRD = 0xFF;

  return(stat);
} 

//------------------------------------------------------------------------------------------

void  Glsd160128::writeData(int byte) 
{
  do {} while ((0x03 & teststat()) != 0x03); 
  CD_L;
  RD_H;                    
  WR_L;         

  PORTD = byte;          
  CE_L;                       

  _delay_us(10);

  CE_H;                       
  WR_H;                       
} 

void Glsd160128::setAddress(unsigned int addr){
writeData(addr%256);
writeData(addr>>8);
writeCommand(SET_ADDRESS_POINTER);
}

void Glsd160128::clearGraphic()    
{
setAddress(GRAPH_BASE);     
for (int i=0;i<(BYTES_PER_ROW*128);i++) {       
    writeData(0); 
	writeCommand(DATA_WRITE_AND_INCREMENT);              
}    
} 

//----------------------------------------------------------------

void Glsd160128::clearText()
{
 setAddress(TEXT_BASE);     

 for (int i=0;i<(BYTES_PER_ROW*16);i++) {      
      writeData(0);
	  writeCommand(DATA_WRITE_AND_INCREMENT);              
 } 

} 

//-------------------------------------------------------------------------------

void Glsd160128::setPixel(int column, int row) 
{
	
int addr;       
if( (column>=XMAX) || (row>=YMAX) )
	return;
addr =  GRAPH_BASE + (row*BYTES_PER_ROW)  + (column/FONT_WIDTH);
setAddress(addr);   
writeCommand(BIT_SET | ((FONT_WIDTH-1-column%FONT_WIDTH)) ); 

}

//---------------------------------------------------------------------------------

void Glsd160128::textPos(int x, int y)  
{
int addr;

  addr = TEXT_BASE + (y * BYTES_PER_ROW) +x;
  setAddress(addr);   

} 

//------------------------------------------------------------------

void  Glsd160128::Setup()  
{

    DDRC |= (1<<PC0)|(1<<PC1)|(1<<PC2)|(1<<PC3);
    DDRB |= (1<<PB1);

    CE_H;
    RD_H;
    WR_H;
    CD_H;

    DDRD = 0xFF;

    RST_H;

    _delay_ms(15);

    RST_L;

    _delay_ms(15);

    RST_H;

} 

//---------------------------------------------------------------------------

void  Glsd160128::Initialize()  
{

 Setup();

 writeData(GRAPH_BASE%256);
 writeData(GRAPH_BASE>>8);
 writeCommand(SET_GRAPHIC_HOME_ADDRESS);       

 writeData(BYTES_PER_ROW%256);
 writeData(BYTES_PER_ROW>>8);
 writeCommand(SET_GRAPHIC_AREA);  

 writeData(TEXT_BASE%256);
 writeData(TEXT_BASE>>8);
 writeCommand(SET_TEXT_HOME_ADDRESS);       

 writeData(BYTES_PER_ROW%256);
 writeData(BYTES_PER_ROW>>8);
 writeCommand(SET_TEXT_AREA);  

 writeCommand(EXOR_MODE);  

 writeCommand(CURSOR_8_LINE);  
 writeData(0x00);
 writeData(0x00);
 writeCommand(CURSOR_1_LINE);  

 writeCommand(TEXT_ON_GRAPHIC_ON);           
         
 clearGraphic();
 clearText();

} 






//------------------------------------------------------------------------------------------

int  Glsd160128::readData(void)      
{
int _byte;

  do {} while ((0x03 & teststat()) != 0x03); 
  DDRD = 0x00;
  
  WR_H;   
  CD_L;   
  RD_L;   
  CE_L;   

 _delay_us(10);

  _byte = PIND;  

  CE_H;   
  RD_H;   
  DDRD = 0xFF; 

  return(_byte);
} 

//----------------------------------------------------------------------------------------

void  Glsd160128::writeCommand(int byte) 
{
  do {} while ((0x03 & teststat()) != 0x03); 

  PORTD = byte;  

  CD_H;         
  RD_H;         
  WR_L;         
  CE_L;         
  CE_H;         
  WR_H;         

} 

//--------------------------------------------------------------------------------

void Glsd160128:: printStr(int x,int y, const char *string)  
{
  int i;
  int c;
  textPos(x,y);
  for (i=0;string[i]!=0;i++) {
      c = string[i] - 0x20;     
      if (c<0) c=0;
      writeData(c);
      writeCommand(DATA_WRITE_AND_INCREMENT);               
  } 

} 

//--------------------------------------------------------------------------------
void   Glsd160128::printInt(int val)
{
    char bufor[17];
    printStr(8,7,( itoa(val, bufor, 10) ) );
   
}
//*********************************************************


//  ----------------------------     Draws a line      -----------------

void  Glsd160128::Line(int x1, int y1, int x2, int y2)    
{
	
  int x,y,jumpx, jumpy, bit;
  
 x =x2 -x1;
  y = y2 - y1;
  
  if (x >= 0)
  {
	jumpx = 1;  
  }
  else
  {
   x = -x;
    jumpx = -1;
  }
  
  if (y >= 0)
  {
    jumpy = 1;
  }
  else
  {
    y = -y;
    jumpy = -1;
  }
  
 x <<= 1;
  y <<= 1;
   
  setPixel(x1,y1); 
  
 //-------------- 
 
  if (x <= y)
  {
    bit =x - (y >> 1);
    while (y1 != y2)
    {
      if (bit >= 0)
      {
       x1 += jumpx;
        bit -= y;
      }
      y1 += jumpy;
      bit +=x;
      setPixel(x1,y1); 
    }
  }
  else
    {
	  bit = y - (x >> 1);
      while (x1 !=x2)
      {
      if (bit >= 0)
      {
        y1 += jumpy;
        bit -=x;
      }
     x1 += jumpx;
      bit += y;
      setPixel(x1,y1); 
      }
    }
  
}

 // ------------       Draws a circle     ------------------
 
 void   Glsd160128::Circle(int x0, int y0 ,int r) {
	
	   int x, y, xchange, ychange, rError;

	   x = r;
	   y = 0;
	   xchange = 1 - 2*r;
	   ychange = 1;
	   rError = 0;
	   while(x >= y ) {
		    setPixel(x0+x, y0+y);
			setPixel(x0-x, y0+y);
			setPixel(x0-x, y0-y);
			setPixel(x0+x, y0-y);
			setPixel(x0+y, y0+x);
			setPixel(x0-y, y0+x);
			setPixel(x0-y, y0-x);
			setPixel(x0+y, y0-x);

	        y++;
	        rError += ychange;
	        ychange+=2;
	        if ( 2*rError +xchange > 0 ) {
	             x--;
	              rError+=xchange;
	             xchange+=2;
	        }
	   }
}


void  Glsd160128::filledCircle(int x0, int y0, int r)
{

int f = 1 - r;
int ddF_x = 1;
int ddF_y = -2 * r;
int x = 0;
int y = r;

	
	Line(x0, y0-r,x0, y0+r);

	while(x < y)
	{
   
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

        
		Line(x0+x, y0+y,x0+x, y0-y);
		Line(x0-x, y0+y,x0-x, y0-y);
		Line(x0+y, y0+x, y+x0, y0-x);
		Line(x0-y, y0+x,x0-y, y0-x);
  	}
}         
void  Glsd160128::halfCircle(int x, int y, int r)
{
  int xc = 0;
  int yc ;
  int p ,x1,x2;
 
  yc=r;
  p = 3 - (r<<1);
  while (xc <= yc)
  {

    setPixel(x +xc, y - yc );
    setPixel(x -xc, y - yc);
    setPixel(x + yc, y -xc);
    setPixel(x - yc, y -xc);
	
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
  x1 = x - r;
  x2 = x + r;
  Line(x1, y, x2, y);
}
void  Glsd160128::IquarterCircle(int x, int y, int r)
{
	 int xc = 0;
  int yc ;
  int p ,x1,x2;
 
  yc=r;
  p = 3 - (r<<1);
  while (xc <= yc)
  {
    setPixel(x -xc, y - yc);
    setPixel(x - yc, y -xc);
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
}
void  Glsd160128::IIquarterCircle(int x, int y, int r)
{
	 int xc = 0;
  int yc ;
  int p ,x1,x2;
 
  yc=r;
  p = 3 - (r<<1);
  while (xc <= yc)
  {
    setPixel(x +xc, y - yc );
    setPixel(x + yc, y -xc);
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
}
void  Glsd160128::IIIquarterCircle(int x, int y, int r)
{
	 int xc = 0;
  int yc ;
  int p ,x1,x2;
 
  yc=r;
  p = 3 - (r<<1);
  while (xc <= yc)
  {
    setPixel(x +xc, y + yc);
    setPixel(x + yc, y +xc);
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
}
void  Glsd160128::IVquarterCircle(int x, int y, int r)
{
	 int xc = 0;
  int yc ;
  int p ,x1,x2;
 
  yc=r;
  p = 3 - (r<<1);
  while (xc <= yc)
  {
    setPixel(x -xc, y + yc);
    setPixel(x - yc, y +xc);
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
}
//   ----------------  Draws a  Box     -----------------------

void  Glsd160128::Box(int x1, int y1, int x2, int y2) 
{
Line(x1,y1,x2,y1);  
Line(x1,y2,x2,y2);  
Line(x2,y1,x2,y2);  
Line(x1,y1,x1,y2);  
}

void  Glsd160128::circleBox(int x1, int y1, int x2, int y2, int r) 
{
	int x1L = x1+r;
	int x2R = x2-r;
	int y1U = y1+r;
	int y2D = y2-r;
Line(x1L,y1,x2R,y1);  
Line(x1L,y2,x2R,y2);  
Line(x2,y1U,x2,y2D);  
Line(x1,y1U,x1,y2D); 

IquarterCircle(x1L, y1U, r); 
IIquarterCircle(x2R, y1U, r);
IIIquarterCircle(x2R, y2D, r);
IVquarterCircle(x1L, y2D, r);
}

void  Glsd160128::filledBox(int x1, int y1, int x2, int y2) 
{
Line(x1,y1,x2,y1);  
Line(x1,y2,x2,y2);  
Line(x2,y1,x2,y2);  
Line(x1,y1,x1,y2); 
 for(int i=0;i<(y2-y1);i++){
     Line(x1,(y1+i) ,x2 ,(y1+i) );
}
}

// ------------       Draws a Triangle     ------------------

void  Glsd160128::Triangle(int x1, int y1,int x2, int y2,int x3, int y3 )
{

	Line(x1, y1,x2,y2 );
    Line(x2, y2,x3,y3 );
    Line(x1, y1,x3,y3 );
  	
}          

void  Glsd160128::filledTriangle(int x1, int y1,int x2, int y2,int x3, int y3 )
{

	Line(x1, y1,x2,y2 );
    Line(x2, y2,x3,y3 );
    Line(x1, y1,x3,y3 );
	for(int i=x1;i<=x2;i++)
  	Line(i, y1,x3,y3 );
}           


