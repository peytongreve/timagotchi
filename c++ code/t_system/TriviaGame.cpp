
#include "TriviaGame.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h> //Connect to WiFi Network

TriviaGame::TriviaGame() {
  question_timer = millis();
  right_answer = 0;
  total_questions = 0;
  total_correct = 0;
  score = 0;
  state = REST;
  selected = 0;
  flag = 0;
  //connect_to_WiFi();
}

void TriviaGame::initialize(char * u, char * wn, char * wp, TFT_eSPI tftESP, boolean mp) {
  //TODO: pass a timagotchi object as an argument
//  strcpy(username, u);
//  strcpy(wifi_name, wn);
//  strcpy(wifi_password, wp);
  tft = tftESP;
  tft.fillScreen(TFT_WHITE);
  tft.drawString("Press any button to start", 0, 0);
}

int TriviaGame::update(int b1_delta, int b2_delta) {
  flag = 0;
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
      } else if (b2_delta != 0) {
        state = QUESTION;
        getTrivia();
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
  sprintf(response, "Which of the following animals do not exist in the United States?&Alligator Snapping Turtle&Gila Monster&Jaguar&Birds&4"); // correct op to be indexed by 1
  int count = 0;
  char * p;
  p = strtok(response, "&");
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  while (p != NULL) {
    switch (count) {
      case 0:
        tft.println(p);
        break;
      case 1:
        tft.setCursor(2, (4*tft.height())/8 + 2);
        tft.print("A. ");
        tft.println(p);
        break;
      case 2:
        tft.setCursor(2, (5*tft.height())/8 + 2);
        tft.print("B. ");
        tft.println(p);
        break;
      case 3:
        tft.setCursor(2, (6*tft.height())/8 + 2);
        tft.print("C. ");
        tft.println(p);
        break;
      case 4:
        tft.setCursor(2, (7*tft.height())/8 + 2);
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
      tft.fillRect(0, ((4+i-1)*tft.height())/8, tft.width(), tft.height()/8, TFT_WHITE);
    }
  }
  drawCursor(TFT_RED);
  if (selected == right_answer) {
    total_correct++;
  }
  selected = right_answer;
  drawCursor(TFT_GREEN);
  selected = 0;
  total_questions++;
}

void TriviaGame::tempDisplay() {
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.println("long press b1 to go back to main menu");
}

void TriviaGame::displayPause() {
  tft.fillScreen(TFT_WHITE);
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.println("long press b1 to go back to main menu");
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
  int left = 0;
  int right = tft.width();
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
