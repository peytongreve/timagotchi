#ifndef TriviaGame_h
#define TriviaGame_h
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h> //Connect to WiFi Network


TriviaGame::TriviaGame(u, wn, wp, tftESP) {
  strcpy(user, u);
  strcpy(wifi_name, wn);
  strcpy(wifi_password, wp);
  question_timer = millis();
  right_answers = 0;
  total_questions = 0;
  score = 0;
  state = REST;
  selection = 0;
}

void update(b1_delta, b2_delta) {
  // state machine
}

/*
 * should make a get request for trivia, display all the possible choices, and give the correct answer
 */
void get_trivia() {
  
}

/*
 * once the game is over, post statistics
 */
void post_stats() {
  
}
