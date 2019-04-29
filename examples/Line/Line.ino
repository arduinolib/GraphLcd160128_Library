/*
 * The library was built based on the documentation of the 
 * WG160128B-TFH display with the T6963C controller.
 * https://www.unisystem.pl/pl/products/mono-lcm/graphic/wg160128b-tfh-tz.html
 *
 * This example code is in the public domain.
*/

/*     
Interface Pin Function  
WG160128B-TFH - T6963C Controller                                Arduino UNO Pin
Pin No. Symbol Level Description

1 FG  Frame ground                                                     
2 Vss 0V Ground                                                   GND        _
3 VDD 5.0V Power supply for logic                                 +5V  -----| |
4 Vadj  Power supply for dupa contrast adjustment                       ---->| | pot 10k - 20k
5 Vee  Negative voltage Output                                         -----|_|
6 /WR L Data write. Write data into T6963C when /WR = L           Pin A0       
7 /RD L Data read. Read data from T6963C when RD = L              Pin A1
8 /CE L Chip enable the controller T6963C                         Pin A2
9 C/D H / L WR=L , C/D=H : Command Write  C/D=L: Data write       Pin A3
RD=L , C/D=H : Status Read  C/D=L: Data read                      
10 /HALT L Clock operating stop signal                            
11 /RESET L Reset signal                                          Pin 9

12 DB0 H / L Data bus line                                        Pin 0
13 DB1 H / L Data bus line                                        Pin 1
14 DB2 H / L Data bus line                                        Pin 2
15 DB3 H / L Data bus line                                        Pin 3
16 DB4 H / L Data bus line                                        Pin 4
17 DB5 H / L Data bus line                                        Pin 5
18 DB6 H / L Data bus line                                        Pin 6
19 DB7 H / L Data bus line                                        Pin 7
20 NC  NC 

*/ 


#include <lcd1.h>
Glsd160128 Lcd ;

void setup() {
  
Lcd.Initialize();

}

void loop() {
  
    Lcd.clearGraphic();
    Lcd.clearText();

    Lcd.printStr(0,5,"    Draws a Line    ");
    Lcd.printStr(0,7,"   WG160128B-TFH    ");
    Lcd.printStr(0,9," T6963C Controller  ");
	delay(5000);
	Lcd.clearText();

    Lcd.Line(80, 0, 80, 128);
    delay(2000);
    Lcd.clearGraphic();
	
    Lcd.Line(0, 64, 160, 64);
    delay(2000);
    Lcd.clearGraphic();
	
	int a=0,b=128;
	for(int i=0; i<=35; i++)  
    {
      
    int y1=i*8;
     if(y1>=160){
      y1=160;
      a += 8;
     }
    int y2=160-i*8;
    if(y2<=0){
      y2=0; 
      b -= 8;
    }
		Lcd.Line(y1, a,y2 , b);
		delay(100);
	}		   
    delay(2000);
    Lcd.clearGraphic(); 
    
    }
