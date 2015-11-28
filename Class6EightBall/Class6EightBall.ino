// include the necessary libraries
#include <SPI.h>
#include <SDqs.h>  // Modified to reduce clock speed
#include <TFT18.h> // Modified Hardware-specific library
#include <Wire.h>

// pin definition for the Uno
#define SD_CS  10
#define LCD_CS 9
#define DC    8
#define RESET    7  
#define BACKLIGHT 6
#define THRESHOLD 2000

TFT myScreen = TFT(LCD_CS, DC, RESET);
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

// this variable represents the image to be drawn on screen
PImage image;


void setup() {
  pinMode(BACKLIGHT, OUTPUT);   digitalWrite(BACKLIGHT,1); // Turn on the backlight
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  // initialize the serial port
  Serial.begin(9600);

  // try to access the SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
//  if (!card.init(SPI_QUARTER_SPEED, SD_CS)) {
    Serial.println("failed!");
    return;
  }
  Serial.println("OK!");

  // initialize and clear the LCD screen
  myScreen.begin();
  myScreen.background(255, 255, 255);

  // load the image from the SD card
  image = myScreen.loadImage("LCD.bmp");

  // check if the image loaded properly
  if (image.isValid() != true) {
    Serial.println("error while loading bmp");
  }

  //write the image on screen
  myScreen.image(image, 0, 0);
}

void loop(){
  char filename[16]= "00.bmp";
  static int lastX, seedAcc, tempRND; 
  static long lastMillis;
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,2,true);  // request a total of 2 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     

  if (((lastX + THRESHOLD) < AcX) or ((lastX - THRESHOLD) > AcX)){
    digitalWrite(BACKLIGHT, 0);
    Serial.print(AcX); Serial.print(" "); Serial.print(lastX);
    Serial.println("Shake!");
    lastX = AcX;
    lastMillis = millis();
  }
  if (lastMillis){
    if((lastMillis+250)<millis()){
      randomSeed(seedAcc); //re-seed random number generator
      tempRND = random(1,21);
      filename[0] = 48 + (tempRND /10);
      filename[1] = 48 + (tempRND %10);
      image = myScreen.loadImage(filename);
      //write the image on screen
      myScreen.image(image, 0, 0);
      digitalWrite(BACKLIGHT, 1);
      lastMillis = 0;
    }
  }
  seedAcc += AcX;
  delay(1);
  }
