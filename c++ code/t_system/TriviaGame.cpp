#ifndef TriviaGame_h
#define TriviaGame_h
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h> //Connect to WiFi Network


TriviaGame::TriviaGame(char * u, char * wn, char * wp, TFT_eSPI tftESP) {
  strcpy(user, u);
  strcpy(wifi_name, wn);
  strcpy(wifi_password, wp);
  question_timer = millis();
  right_answer = 0;
  total_questions = 0;
  score = 0;
  state = REST;
  selection = 0;
  tft = tftESP;
  
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

/*
 * puts green cursor around right answer
 */
void TriviaGame::buzzer() {
  selection = right_answer;
  drawCursor(TFT_GREEN);
}

void TriviaGame::tempDisplay() {
  tft.fillScreen(TFT_WHITE);
}

void TriviaGame::displayPause() {
  tft.fillScreen(TFT_WHITE);
}

/*
 * once the user presses pause, post statistics
 */
void TriviaGame::post_stats() {
  
}

void TriviaGame::eraseCursor() {
  drawCursor(TFT_WHITE);
}

void TriviaGame::drawCursor(uint32_t color) {
  int top = ((num_items+selected-1)*tft.height())/(num_items*2);
  int bottom = ((num_items+selected)*tft.height())/(num_items*2);
  int left = 10;
  int right = tft.width() - 10;
  tft.drawLine(left, top, right, top, color);
  tft.drawLine(left, bottom, right, bottom, color);
  tft.drawLine(left, top, left, bottom, color);
  tft.drawLine(right, top, right, bottom, color);
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
  drawCursor(TFT_RED);
}
