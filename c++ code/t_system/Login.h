#ifndef Login_h
#define Login_h
#include <SPI.h>
#include <TFT_eSPI.h>

#define START 0
#define NAME 1
#define COURSE 2
#define END 4
#define MAINMENU

const int RESPONSE_TIMEOUT = 6000; //ms to wait for response from hos
const uint16_t OUT_BUFFER_SIZE = 1000; //size of buffer to hold HTTP response

class Login
{
  private:
  uint8_t state;
  uint8_t flag;
  TFT_eSPI tft;
  char login_username[11];
  int user_index = 0;
  char letters[27] = "abcdefghijklmnopqrstuvwxyz";
  int letter_index = 0;
  char prompt[100];
  //Timagotchi tim;
  bool draw = true;
  

public:
  Login(TFT_eSPI tftESP);
  int update(int b1_flag, int b2_flag);
  void drawStart();
  void drawName();
  void drawEnd();
  void do_http_request(char* host, char* request, char* response, uint16_t response_size, uint16_t response_timeout, uint8_t serial);
  uint8_t char_append(char* buff, char c, uint16_t buff_size);
};

#endif
