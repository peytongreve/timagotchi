#include "CoolButton.h"
#include "VerticalMenu.h"
#include "TriviaGame.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h> //Connect to WiFi Network
#include <string.h>
TFT_eSPI tft = TFT_eSPI();

#define IDLE 0
#define NAV 1
#define TRIVIA 2
#define INFINITE 3

const int single_trivia_flag = 10;
const int multi_trivia_flag = 11;
const int infinite_run_flag = 30;

char network[] = "kumba17";
char password[] = "jsjnetworkyekumba17";

const int BUTTON_PIN_1 = 16;
const int BUTTON_PIN_2 = 5;

CoolButton button1(BUTTON_PIN_1);
CoolButton button2(BUTTON_PIN_2);

int old_button1_flag, old_button2_flag, button1_flag, button2_flag, button1_delta, button2_delta;
int flag;
int state;

VerticalMenu menu = VerticalMenu(tft);
TriviaGame trivia;
//InfiniteRun infinite;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);               // Set up serial port
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  tft.init();
  tft.setRotation(2);
  tft.setTextSize(1);
  tft.fillScreen(TFT_WHITE);
  old_button1_flag = 0;
  old_button2_flag = 0;
  button1_flag = button1.update();
  button2_flag = button2.update();
  connect_to_WiFi();
  pinMode(14, OUTPUT); //controlling TFT with hardware power management
  digitalWrite(14, HIGH);
  state = 1;
}

void loop() {
  // put your main code here, to run repeatedly:
  button1_flag = button1.update();
  button2_flag = button2.update();
  button1_delta = button1_flag - old_button1_flag;
  button2_delta = button2_flag - old_button2_flag;
  // those deltas determine what kind of press was made
  if (button1_delta != 0 or button2_delta != 0) {
    fsm(button1_delta, button2_delta);
  }
  old_button1_flag = button1_flag;
  old_button2_flag = button2_flag;
}

void fsm(int b1_delta, int b2_delta) {
  switch (state) {
    case IDLE:
      state = NAV;
      menu.displayHome();
      break;
    case NAV:
      flag = menu.update(b1_delta, b2_delta);
      if (flag == single_trivia_flag) {
        state = TRIVIA;
        //trivia = TriviaGame(single);
      } else if (flag == multi_trivia_flag) {
        state = TRIVIA;
        //trivia = TriviaGame(multi);
      } else if (flag == infinite_run_flag) {
        state = INFINITE;
        //infinite = InfiniteRun();
      }
      break;
    case TRIVIA:
      flag = trivia.update(b1_delta, b2_delta);
      if (flag != 0) {
        state = NAV;
        menu.displayHome();
      }
      break;
    case INFINITE:
      flag = infinite.update(b1_delta, b2_delta);
      if (flag != 0) {
        state = NAV;
        menu.displayHome();
      }
      break;
  }
}

////////////////////////////////////////////////////////////////////

void connect_to_WiFi() {
  WiFi.begin(network, password); //attempt to connect to wifi
  uint8_t count = 0; //count used for Wifi check times
  Serial.print("Attempting to connect to ");
  Serial.println(network);
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
