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
#define SCORES 7

const int multi_trivia_flag = 10;
const int single_trivia_flag = 11;
const int infinite_run_flag = 30;
const int scores_flag = 40;

//char * network = "kumba20";
//char * password = "yekumba20!";
//char* network = "Homebase";
//char* password = "StevenEdgar";
char* network = "TheAllens2.4";
char* password = "magnolia";
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

char host[] = "756bdc48.ngrok.io";
const int RESPONSE_TIMEOU = 6000; //ms to wait for response from hos
const uint16_t OUT_BUFFER_SIZ = 1000; //size of buffer to hold HTTP response
char response[OUT_BUFFER_SIZ]; //char array buffer to hold HTTP request

bool drawScoresBool = false;

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
  drawLandingScreen();
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
      if (flag == 1) {
        state = NAV;
        menu.displayHome();
        username = login.getUsername();
      } else if (flag == 2) {
        state = LANDING;
        drawLandingScreen();
      }
      break;
    case INIT:
      flag = timCreator.update(b1_delta, b2_delta);
      if (flag != 0) {
        username = timCreator.getUsername();
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
      } else if (flag == scores_flag) {
        state = SCORES;
        drawScoresBool = true;
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
    case SCORES:
      flag = 0;
      if(drawScoresBool){
        drawScores();
        drawScoresBool = false;
      }
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

void drawLandingScreen() {
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

void drawScores() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);      
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  char thing[30];
  char request[500];
  sprintf(request, "GET /infinite_run?name=%s HTTP/1.1\r\n", username);
  sprintf(request + strlen(request), "Host: %s\r\n", host);
  strcat(request, "Content-Type: application/x-www-form-urlencoded\r\n");
  sprintf(request + strlen(request), "Content-Length: %d\r\n\r\n", strlen(thing));
  strcat(request, thing);
  do_http_request(host, request, response, OUT_BUFFER_SIZ, RESPONSE_TIMEOU, true);
  Serial.println(response);
  char yours[40];
  sprintf(yours, "Your score: %s", response);
  tft.drawString(yours, tft.width()/2, tft.height()/2-60, 1);
  char thin[30];
  char reques[500];
  sprintf(reques, "GET /infinite_run HTTP/1.1\r\n");
  sprintf(reques + strlen(reques), "Host: %s\r\n", host);
  strcat(reques, "Content-Type: application/x-www-form-urlencoded\r\n");
  sprintf(reques + strlen(reques), "Content-Length: %d\r\n\r\n", strlen(thin));
  strcat(reques, thin);
  do_http_request(host, reques, response, OUT_BUFFER_SIZ, RESPONSE_TIMEOU, true);
  Serial.println(response);
  char user1[11];
  char score1[40];
  char user2[11];
  char score2[40];
  char user3[11];
  char score3[40];
  int curVal = 0;
  int curIndex = 0;
  for(int i = 1; i < strlen(response); i++){
    if(response[i] == '&'){
      if(curVal == 0){
        user1[curIndex] = '\0';
      }
      if(curVal == 1){
        score1[curIndex] = '\0';
      }
      if(curVal == 2){
        user2[curIndex] = '\0';
      }
      if(curVal == 3){
        score2[curIndex] = '\0';
      }
      if(curVal == 4){
        user3[curIndex] = '\0';
      }
      curVal += 1;
      curIndex = 0;
    }
    else{
      if(curVal == 0){
        user1[curIndex] = response[i];
        curIndex += 1;
      }
      if(curVal == 1){
        score1[curIndex] = response[i];
        curIndex += 1;
      }
      if(curVal == 2){
        user2[curIndex] = response[i];
        curIndex += 1;
      }
      if(curVal == 3){
        score2[curIndex] = response[i];
        curIndex += 1;
      }
      if(curVal == 4){
        user3[curIndex] = response[i];
        curIndex += 1;
      }
      if(curVal == 5){
        if(response[i] != '0' && response[i] != '1' && response[i] != '2' && response[i] != '3' && response[i] != '4' && response[i] != '5' && response[i] != '6' && response[i] != '7' && response[i] != '8' && response[i] != '9') {
          score3[curIndex] = '\0';
          break;
        }
        score3[curIndex] = response[i];
        curIndex += 1;
      }
    }
  }
  char line1[60];
  char line2[60];
  char line3[60];
  sprintf(line1, "%s: %s", user1, score1);
  sprintf(line2, "%s: %s", user2, score2);
  sprintf(line3, "%s: %s", user3, score3);
  tft.drawString("Top Scores:", tft.width()/2, tft.height()/2-40, 1);
  tft.drawString(line1, tft.width()/2, tft.height()/2-20, 1);
  tft.drawString(line2, tft.width()/2, tft.height()/2, 1);
  tft.drawString(line3, tft.width()/2, tft.height()/2+20, 1);
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








/*----------------------------------
  char_append Function:
  Arguments:
     char* buff: pointer to character array which we will append a
     char c:
     uint16_t buff_size: size of buffer buff

  Return value:
     boolean: True if character appended, False if not appended (indicating buffer full)
*/
uint8_t char_append(char* buff, char c, uint16_t buff_size) {
  int len = strlen(buff);
  if (len > buff_size) return false;
  buff[len] = c;
  buff[len + 1] = '\0';
  return true;
}

/*----------------------------------
   do_http_request Function:
   Arguments:
      char* hos: null-terminated char-array containing hos to connect to
      char* request: null-terminated char-arry containing properly formatted HTTP request
      char* response: char-array used as output for function to contain response
      uint16_t response_size: size of response buffer (in bytes)
      uint16_t response_timeout: duration we'll wait (in ms) for a response from server
      uint8_t serial: used for printing debug information to terminal (true prints, false doesn't)
   Return value:
      void (none)
*/
void do_http_request(char* hos, char* request, char* response, uint16_t response_size, uint16_t response_timeout, uint8_t serial) {
  WiFiClient client; //instantiate a client object
  if (client.connect(hos, 80)) { //try to connect to hos on port 80
    if (serial) Serial.print(request);//Can do one-line if statements in C without curly braces
    Serial.println("\n\n");
    client.print(request);
    memset(response, 0, response_size); //Null out (0 is the value of the null terminator '\0') entire buffer
    uint32_t count = millis();
    while (client.connected()) { //while we remain connected read out data coming back
      client.readBytesUntil('\n', response, response_size);
      if (serial) Serial.println(response);
      if (strcmp(response, "\r") == 0) { //found a blank line!
        break;
      }
      memset(response, 0, response_size);
      if (millis() - count > response_timeout) break;
    }
    memset(response, 0, response_size);
    count = millis();
    while (client.available()) { //read out remaining text (body of response)
      char_append(response, client.read(), OUT_BUFFER_SIZ);
    }
    if (serial) Serial.println(response);
    client.stop();
    if (serial) Serial.println("-----------");
  } else {
    if (serial) Serial.println("connection failed :/");
    if (serial) Serial.println("wait 0.5 sec...");
    client.stop();
  }
}
