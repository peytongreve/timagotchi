
#include "TriviaGame.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h> //Connect to WiFi Network

TriviaGame::TriviaGame() {
  question_timer = millis();
  right_answer = 0;
  total_questions = 0;
  score = 0;
  state = REST;
  selected = 0;
  flag = 0;
}

void TriviaGame::initialize(char * u, char * wn, char * wp, TFT_eSPI tftESP, boolean mp) {
  //TODO: pass a timagotchi object as an argument
//  strcpy(username, u);
//  strcpy(wifi_name, wn);
//  strcpy(wifi_password, wp);
  tft = tftESP;
}

int TriviaGame::update(int b1_delta, int b2_delta) {
  switch (state) {
    case REST:
      if (b1_delta != 0 or b2_delta != 0) {
        state = QUESTION;
        getTrivia();
      }
      break;
    case QUESTION:
      if (b1_delta == -1) {
        moveCursor(-1);
      } else if (b2_delta == -1) {
        moveCursor(1);
      } else if (b2_delta == -3) {
        state = ANSWER;
        buzzer();
      }
      break;
    case ANSWER:
      if (b2_delta == -1) {
        state = QUESTION;
        getTrivia();
      } else if (b1_delta == -3) {
        state = PAUSE;
        tempDisplay();
      }
      break;
    case PAUSE:
      if (b1_delta == -2) {
        flag = 1;
        // code to save everything, update timagotchi, all that jazz
      }
      break;
    case EXIT: // not sure if we really need this state
      flag = 1;
      break;
  }
  return flag;
}

/*
 * should make a get request for trivia, display all the possible choices, and give the correct answer
 * 
 */
void TriviaGame::getTrivia() {
  // request to proxy
  
  // &question&option1&option2&option3&option4&correctChoice
  char response[200];
  sprintf(response, "question&answer1&answer2&answer3&answer4&correctOp"); // correct op to be indexed by 1
  int count = 0;
  char * p;
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0);
  while (p != NULL) {
    switch (count) {
      case 0:
        tft.println(p);
        tft.setCursor(0, 64);
        break;
      case 1:
        tft.print("A. ");
        tft.println(p);
        break;
      case 2:
        tft.print("B. ");
        tft.println(p);
        break;
      case 3:
        tft.print("C. ");
        tft.println(p);
        break;
      case 4:
        tft.print("D. ");
        tft.println(p);
        break;
      case 5:
        right_answer = atoi(p);
        break;
    }
    count++;
    p = strtok (NULL, "&");
  }
}

/*
 * puts green cursor around right answer
 */
void TriviaGame::buzzer() {
  for (int i = 1; i < 5; i++) {
    if (i != selected and i != right_answer) {
      tft.fillRect(10, ((4+i-1)*tft.height())/8, tft.width()-20, tft.height()/8, TFT_WHITE);
    }
  }
  drawCursor(TFT_RED);
  selected = right_answer;
  drawCursor(TFT_GREEN);
  selected = 0;
}

void TriviaGame::tempDisplay() {
  tft.fillScreen(TFT_WHITE);
}

void TriviaGame::displayPause() {
  tft.fillScreen(TFT_WHITE);
}

/*
 * once the user presses pause (or exits), post statistics to the server and update timagotchi
 */
void TriviaGame::post_stats() {
  
}

void TriviaGame::eraseCursor() {
  drawCursor(TFT_WHITE);
}

void TriviaGame::drawCursor(uint32_t color) {
  int top = ((4+selected-1)*tft.height())/8;
  int bottom = ((4+selected)*tft.height())/8;
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

void TriviaGame::connect_to_WiFi() {
  WiFi.begin(wifi_name, wifi_password); //attempt to connect to wifi
  uint8_t count = 0; //count used for Wifi check times
  Serial.print("Attempting to connect to ");
  Serial.println(wifi_name);
  while (WiFi.status() != WL_CONNECTED && count < 12) {
    delay(500);
    Serial.print(".");
    count++;
  }
  delay(2000);
  if (WiFi.isConnected()) { //if we connected then print our IP, Mac, and SSID we're on
    Serial.println("CONNECTED!");
    Serial.printf("%d:%d:%d:%d (%s) (%s)\n", WiFi.localIP()[3], WiFi.localIP()[2],
                  WiFi.localIP()[1], WiFi.localIP()[0],
                  WiFi.macAddress().c_str() , WiFi.SSID().c_str());
    delay(500);
  } else { //if we failed to connect just Try again.
    Serial.println("Failed to Connect :/  Going to restart");
    Serial.println(WiFi.status());
    ESP.restart(); // restart the ESP (proper way)
  }
}
