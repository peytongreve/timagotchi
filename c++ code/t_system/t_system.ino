#include "CoolButton.h"
#include "VerticalMenu.h"
#include "TriviaGame.h"
#include "Initialization.h"
#include "Login.h"
#include "InfiniteRun.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h> //Connect to WiFi Network
#include <string.h>
#include <mpu6050_esp32.h>

TFT_eSPI tft = TFT_eSPI();

#define LANDING 0
#define LOGIN 1
#define INIT 2
#define NAV 3
#define SP_TRIVIA 4
#define MP_TRIVIA 5
#define INFINITE 6

const int multi_trivia_flag = 10;
const int single_trivia_flag = 11;
const int infinite_run_flag = 30;

// char * network = "kumba20";
// char * password = "yekumba20!";
char* network = "Homebase";
char* password = "StevenEdgar";
char * username = "julian";

const int BUTTON_PIN_1 = 16;
const int BUTTON_PIN_2 = 5;

CoolButton button1(BUTTON_PIN_1);
CoolButton button2(BUTTON_PIN_2);

int old_button1_flag, old_button2_flag, button1_flag, button2_flag, button1_delta, button2_delta;
int flag;
int state;
boolean imu_activated;

VerticalMenu menu = VerticalMenu(tft);
TriviaGame sp_trivia;
MPU6050 imu;
Initialization timCreator = Initialization(tft);
Login login = Login(tft);
//TriviaGame mp_trivia = TriviaGame(username, network, password, tft, false);
InfiniteRun infinite = InfiniteRun(tft, username, network, password, imu);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);               // Set up serial port
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);

//  if (imu.setupIMU(1)) {
//    Serial.println("IMU Connected!");
//  } else {
//    Serial.println("IMU Not Connected :/");
//    Serial.println("Restarting");
//    ESP.restart(); // restart the ESP (proper way)
//  }
  
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
  state = LANDING;
  Serial.println("STARTING");
  imu_activated = false;
  if (imu.setupIMU(1)) {
      Serial.println("IMU Connected!");
    } else {
      Serial.println("IMU Not Connected :/");
      Serial.println("Restarting");
      ESP.restart(); // restart the ESP (proper way)
    }
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("Welcome to Timagochi!", tft.width()/2, tft.height()/2-60, 1);
  tft.drawString("Click left button to", tft.width()/2, tft.height()/2-40, 1);
  tft.drawString("log in!", tft.width()/2, tft.height()/2-30, 1);
  tft.drawString("Click right button to", tft.width()/2, tft.height()/2-10, 1);
  tft.drawString("create new!", tft.width()/2, tft.height()/2, 1);
  tft.drawString("Enjoy!", tft.width()/2, tft.height()/2+20, 1);
}

void loop() {
  if(state == INIT){
    button1.read();
    button2.read();
  }
  button1_flag = button1.update();
  button2_flag = button2.update();
  button1_delta = button1_flag - old_button1_flag;
  button2_delta = button2_flag - old_button2_flag;
  // those deltas determine what kind of press was made
  //if (button1_delta != 0 or button2_delta != 0) {
  fsm(button1_delta, button2_delta);
  //}
  old_button1_flag = button1_flag;
  old_button2_flag = button2_flag;
}

void fsm(int b1_delta, int b2_delta) {
  switch (state) {
    case LANDING:
      if (b1_delta == -1) {
        state = LOGIN;
      } else if (b2_delta == -1) {
        state = INIT;
      }
      break;
    case LOGIN:
      flag = login.update(b1_delta, b2_delta);
      if (flag != 0) {
        state = NAV;
        menu.displayHome();
      }
      break;
    case INIT:
      flag = timCreator.update(b1_delta, b2_delta);
      if (flag != 0) {
        state = NAV;
        menu.displayHome();
      }
      break;
    case NAV:
      flag = menu.update(b1_delta, b2_delta);
      if (flag == single_trivia_flag) {
        state = SP_TRIVIA;
        Serial.println("Problem?");
        sp_trivia.initialize(username, network, password, tft, false);
      } else if (flag == multi_trivia_flag) {
        state = MP_TRIVIA;
        //trivia = TriviaGame(multi);
      } else if (flag == infinite_run_flag) {
        state = INFINITE;
        imu_activated = true;
        tft.fillScreen(TFT_WHITE);
        infinite = InfiniteRun(tft, username, network, password, imu);
        infinite.step(10);
      }
      break;
    case SP_TRIVIA:
      flag = sp_trivia.update(b1_delta, b2_delta);
      if (flag != 0) {
        state = NAV;
        menu.displayHome();
      }
      break;
    case MP_TRIVIA:
      //flag = mp_trivia.update(b1_delta, b2_delta);
      if (flag != 0) {
        state = NAV;
        menu.displayHome();
      }
      break;
    case INFINITE:
      flag = 0;
      // float timer = millis();
      // while (millis() - timer < 1000);
      imu.readAccelData(imu.accelCount);
      infinite.step(float(-imu.accelCount[1] * imu.aRes) * 10);
      //flag = infinite.update(b1_delta, b2_delta);
      if (flag != 0 or b1_delta != 0) {
        state = NAV;
        imu_activated = false;
        menu.displayHome();
      }
      break;
  }
  if (flag != 0) {
    Serial.println(flag);
  }
  flag = 0;
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
