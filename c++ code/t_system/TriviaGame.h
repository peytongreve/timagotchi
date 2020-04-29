
#ifndef TriviaGame_h
#define TriviaGame_h
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h> //Connect to WiFi Network

#define REST 0
#define QUESTION 1
#define ANSWER 2
#define PAUSE 3
#define EXIT 4

class TriviaGame
{
  private:
    char * username;
    char * wifi_name;
    char * wifi_password;
    uint32_t question_timer;
    uint32_t right_answer;
    uint32_t total_questions;
    uint32_t total_correct;
    uint32_t score;
    uint8_t state;
    uint8_t selected;
    uint8_t flag;
    TFT_eSPI tft;

    void connect_to_WiFi();
  
  public:
    TriviaGame();
    void initialize(char * user, char * wifi_name, char * wifi_password, TFT_eSPI tftESP, boolean multiplayer);
    int update(int b1_delta, int b2_delta);
    void getTrivia();
    void buzzer();
    void tempDisplay();
    void displayPause();
    void post_stats();
    void eraseCursor();
    void drawCursor(uint32_t color);
    void moveCursor(int increment);
    
};
#endif
