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
  right_answer = 0;
  total_questions = 0;
  score = 0;
  state = REST;
  selection = 0;
}

void TriviaGame::update(b1_delta, b2_delta) {
  // state machine
}

/*
 * should make a get request for trivia, display all the possible choices, and give the correct answer
 * 
 */
void TriviaGame::get_trivia() {
  // request to proxy
  
  // &question&option1&option2&option3&option4&correctChoice
  char response[200];
  sprintf(response, "question&answer1&answer2&answer3&answer4&correctOp"); // correct op to be indexed by 1
  count = 0;
  char * p;
  while (p != NULL) {
    if (count <= 4) {
      tft.println(p);
    } else {
      accuracy = atoi(p);
    }
    count++;
    p = strtok (NULL, "&");
  }
  // ...
}

void TriviaGame::eraseCursor() {
  int top = ((4+selected-1)*tft.height())/(4*2);
  int bottom = ((4+selected)*tft.height())/(4*2);
  int left = 10;
  int right = tft.width() - 10;
  tft.drawLine(left, top, right, top, TFT_WHITE);
  tft.drawLine(left, bottom, right, bottom, TFT_WHITE);
  tft.drawLine(left, top, left, bottom, TFT_WHITE);
  tft.drawLine(right, top, right, bottom, TFT_WHITE);
}
void TriviaGame::moveCursor(int increment) {
  eraseCursor();
  selected = selected + increment;
  if (selected > 4) {
    selected = selected % 4;
  } else if (selected == 0) {
    selected = 4;
  } else if (selected < 1) {
    selected = (-1*selected-4) % 4;
  }
  drawCursor();
}

/*
 * puts green cursor around right answer
 */
void TriviaGame::buzzer() {
  
}

void TriviaGame::tempDisplay() {
  
}

void TriviaGame::displayPause() {
  
}

/*
 * once the user presses pause, post statistics
 */
void TriviaGame::post_stats() {
  
}
