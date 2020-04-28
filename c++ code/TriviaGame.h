
#ifndef TriviaGame_h
#define TriviaGame_h
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h> //Connect to WiFi Network

#define REST
#define QUESTION
#define ANSWER
#define PAUSE
#define EXIT

class TriviaGame
{
  private:
  const char username[20];
  const char wifi_name[50];
  const char wifi_password[50];
  uint32_t question_timer;
  uint32_t right_answer;
  uint32_t total_questions;
  uint32_t score;
  uint8_t state;
  uint8_t selection;
  TFT_eSPI tft;
  
  public:
    TriviaGame(user, wifi_name, wifi_password, tftESP);
    void update(int b1_delta, int b2_delta);
    void get_trivia();
    void post_stats();
};
#endif
