#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

#define BLACK 0x0000
#define BLUE 0x0C3A
#define RED 0xF800


extern unsigned char beaver[];
extern unsigned char energy_bar[];
extern unsigned char empty_bar[];
extern unsigned char fifth_bar[];
extern unsigned char two_fifth_bar[];
extern unsigned char three_fifth_bar[];
extern unsigned char four_fifth_bar[];
extern unsigned char full_bar[];
extern unsigned char eating_icon[];
extern unsigned char gaming_icon[];
extern unsigned char  newbeaver[];
extern unsigned char  book_icon[];
//extern unsigned char  ;

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_WHITE);
  drawGamingIcon();
  
}

void loop() {

}
void drawBeaverAndEnergyBar(){
  tft.drawBitmap(0,0, beaver, 69,104, BLACK);
  tft.drawBitmap(69, 84, energy_bar, 40,20, BLACK);
  tft.drawBitmap(69, 0, empty_bar, 40,84, BLACK);
}

void drawEatingIcon(){
  tft.drawBitmap(29,0, eating_icon, 70,70, BLACK);
}

void drawGamingIcon(){
  tft.drawBitmap(29,0, gaming_icon, 70,70, BLACK);
}

void drawNewBeaver(){
  tft.drawBitmap(29,0, newbeaver, 70,70, BLACK);
}

void drawBookIcon(){
  tft.drawBitmap(29,0, book_icon, 70,70, BLACK);
}
