#ifndef Initialization_H
#define Initialization_H
#include <SPI.h>
#include <TFT_eSPI.h>

#define START 0
#define NAME 1
#define CHECK 2
#define COURSE 3
#define END 4
#define MAINMENU

class Initialization
{
  private:
  uint8_t state;
  uint8_t flag;
  TFT_eSPI tft;

public:
  Initialization(TFT_eSPI tftESP);
  int update(int b1_flag, int b2_flag);
  void drawStart();
  void drawName();
  void drawCourse();
  void drawCheck();
  void drawEnd();
  void drawMainMenu();
  void do_http_request(char* host, char* request, char* response, uint16_t response_size, uint16_t response_timeout, uint8_t serial);
  uint8_t char_append(char* buff, char c, uint16_t buff_size);
  char* getUsername();
};

#endif
