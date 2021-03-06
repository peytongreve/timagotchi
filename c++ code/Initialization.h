#ifndef Initialization_H
#define Initialization_H
#include <SPI.h>
#include <TFT_eSPI.h>

#define START 0
#define NAME 1
#define COURSE 2
#define END 4
#define MAINMENU

class Initialization
{
  private:
  uint8_t state;
  TFT_eSPI tft;

public:
  Initialization(TFT_eSPI tftESP);
  void update(int b1_flag, int b2_flag);
  void drawStart();
  void drawName();
  void drawCourse();
  void drawEnd();
  void drawMainMenu();
  void do_http_request(char* host, char* request, char* response, uint16_t response_size, uint16_t response_timeout, uint8_t serial);
  uint8_t char_append(char* buff, char c, uint16_t buff_size);
};

#endif
