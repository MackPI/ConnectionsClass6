#include <SPI.h>
//#include "ST7735.h"
#define MaxRate 1000000
#define LCD_CS 9
#define BACKLIGHT 6
#define LCD_RS 8
#define LCD_RST 7

void setup() {
  int ramIndex;

  pinMode(LCD_CS, OUTPUT);  digitalWrite(LCD_CS, 0);
  pinMode(BACKLIGHT, OUTPUT);   digitalWrite(BACKLIGHT,1); // Turn on the backlight
  pinMode(LCD_RS, OUTPUT);   digitalWrite(LCD_RS,0); // SPI sends commands
  pinMode(LCD_RST, OUTPUT);

  digitalWrite(LCD_RST,0);
  delay(1); 
  digitalWrite(LCD_RST,1);// Reset LCD
  delay(1);
  
  Serial.begin(9600);
  SPI.begin();
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  initializeLCD();
  sendLCDCommand(0b00101100); //RAMWR
  for (ramIndex = 0; ramIndex < 20480; ramIndex++){ // fill Screen with red
    sendLCDData(255); // Red Channel
    sendLCDData(0); // Green Channel
    sendLCDData(0); // Blue Channel
  }
  SPI.endTransaction();
}
void initializeLCD(void){
  sendLCDCommand(0b00000001); //SWRESET
  delay(200);
  sendLCDCommand(0b00010001); //SLPOUT
  delay(200);
  sendLCDCommand(0b00101001); //DISPON
}

void sendLCDCommand(byte Command){
  digitalWrite(LCD_CS, 0); // tell LCD to Listen
  digitalWrite(LCD_RS,0);  // SPI sends commands
  SPI.transfer(Command);
  digitalWrite(LCD_CS, 1); // tell LCD to release bus
  
}
void sendLCDData(byte data){
  digitalWrite(LCD_CS, 0);  // tell LCD to Listen
  digitalWrite(LCD_RS,1);   // SPI sends data
  SPI.transfer(data);
  digitalWrite(LCD_CS, 1);  // tell LCD to release bus
  
}
void loop() {
  // put your main code here, to run repeatedly:

}

