#include "Initialization.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h> //Connect to WiFi netw


#define START 0
#define NAME 1
#define CHECK 2
#define COURSE 3
#define END 4
#define MAINMENU 5


char init_username[11];
int user_index = 0;
char letters[27] = "abcdefghijklmnopqrstuvwxyz";
int letter_index = 0;
char prompt[100];
int course_num = 1;
//Timagotchi tim;
bool draw = true;
bool check = false;

char netw[] = "kumba20";  //SSID CHANGE!!
char passw[] = "yekumba20!"; //passw for WiFi CHANGE!!!
char hos[] = "756bdc48.ngrok.io";

//Some constants and some resources:
const int RESPONSE_TIMEOUT = 6000; //ms to wait for respon from hos
const uint16_t OUT_BUFFER_SIZE = 1000; //size of buffer to hold HTTP respon
char respon[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP request

Initialization::Initialization(TFT_eSPI tftESP) {
  state = START;
  tft = tftESP;
  flag = 0;
}

int Initialization::update(int b1_flag, int b2_flag){
  flag = 0;
  switch(state){
    case START:
      if(draw){
        drawStart();
        draw = false;
      }
      if(b1_flag != 0 || b2_flag != 0){
        state = NAME;
        draw = true;
      }
      break;
    case NAME:
      if(draw){
        drawName();
        draw = false;
      }
      if(b2_flag == 1){
        letter_index = (letter_index + 1)%26;
        draw = true;
      }
      else if(b2_flag == 3){
        init_username[user_index] = letters[letter_index];
        user_index = user_index + 1;
        if(user_index == 10){
          state = CHECK;
          check = true;
          draw = false;
        }
        else{
          draw = true;
        }
      }
      else if(b1_flag == 1){
        state = CHECK;
        check = true;
        draw = false;
      }
      break;
    case CHECK:
      if(check){
        char thing[30];
        char request[500];
        sprintf(request, "GET /timagochi_exists?name=%s HTTP/1.1\r\n", init_username);
        sprintf(request + strlen(request), "Host: %s\r\n", hos);
        strcat(request, "Content-Type: application/x-www-form-urlencoded\r\n");
        sprintf(request + strlen(request), "Content-Length: %d\r\n\r\n", strlen(thing));
        strcat(request, thing);
        do_http_request(hos, request, respon, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
        Serial.println(respon);
        check = false;
        if(respon[0] == 'f'){
          state = COURSE;
        }
        draw = true;
      }
      else if(draw){
        drawCheck();
        draw = false;
      }
      if(b2_flag == 1 || b1_flag == 1){
        for(int i = 0; i < 10; i++){
          init_username[i] = 0;
        }
        user_index = 0;
        letter_index = 0;
        state = NAME;
        draw = true;
      }
      break;
    case COURSE:
      if(draw){
        drawCourse();
        draw = false;
      }
      if(b2_flag == 1){
        course_num = ((course_num + 1)%25);
        if(course_num == 0){ course_num = 1; }
        draw = true;
      }
      else if(b1_flag == 1){
        state = END;
        draw = true;
      }
      break;
    case END:
      if(draw){
        char thing[30];
        sprintf(thing, "name=%s&major=%d", init_username, course_num);
        char request[500];
        sprintf(request, "POST /get_timagochi? HTTP/1.1\r\n");
        sprintf(request + strlen(request), "Host: %s\r\n", hos);
        strcat(request, "Content-Type: application/x-www-form-urlencoded\r\n");
        sprintf(request + strlen(request), "Content-Length: %d\r\n\r\n", strlen(thing));
        strcat(request, thing);
        do_http_request(hos, request, respon, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
        Serial.println(respon);
        drawEnd();
        draw = false;
      }
      else if(b2_flag != 0 || b1_flag != 0){
        state = MAINMENU;
        draw = true;
      }
      flag = 1;
      break;
    case MAINMENU:
      if(draw){
        drawMainMenu();
        draw = false;
      }
      break;
  }
  return flag;
}

char* Initialization::getUsername(){
  return init_username;
}

void Initialization::drawStart(){
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("Welcome Timagatchi!", tft.width()/2, tft.height()/2-10, 1);
  tft.drawString("Press any button", tft.width()/2, tft.height()/2+10, 1);
  tft.drawString("to start!", tft.width()/2, tft.height()/2+20, 1);
}

void Initialization::drawName(){
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("Make your username!", tft.width()/2, tft.height()/2-60, 1);
  tft.drawString("Click right button to", tft.width()/2, tft.height()/2-40, 1);
  tft.drawString("choose a new letter", tft.width()/2, tft.height()/2-30, 1);
  tft.drawString("Double click right", tft.width()/2, tft.height()/2-10, 1);
  tft.drawString("to confirm letter", tft.width()/2, tft.height()/2, 1);
  tft.drawString("Click left button", tft.width()/2, tft.height()/2+20, 1);
  tft.drawString("to move on", tft.width()/2, tft.height()/2+30, 1);
  sprintf(prompt, "Currently choosing: %c", letters[letter_index]);
  tft.drawString(prompt, tft.width()/2, tft.height()/2+50, 1);
  tft.drawString(init_username, tft.width()/2, tft.height()/2+60, 1);
}

void Initialization::drawCourse(){
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("What is your course?", tft.width()/2, tft.height()/2-50, 1);
  tft.drawString("Click right button to", tft.width()/2, tft.height()/2-30, 1);
  tft.drawString("increase course number", tft.width()/2, tft.height()/2-20, 1);
  tft.drawString("Click left button", tft.width()/2, tft.height()/2, 1);
  tft.drawString("to move on", tft.width()/2, tft.height()/2+10, 1);
  sprintf(prompt, "Currently choosing: %d", course_num);
  tft.drawString(prompt, tft.width()/2, tft.height()/2+30, 1);
}

void Initialization::drawEnd(){
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("Thank you!", tft.width()/2, tft.height()/2-10, 1);
  tft.drawString("Press any button to", tft.width()/2, tft.height()/2, 1);
  tft.drawString("go to main menu!", tft.width()/2, tft.height()/2+10, 1);
}

void Initialization::drawCheck(){
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("Sorry, that username", tft.width()/2, tft.height()/2-30, 1);
  tft.drawString("is already in use.", tft.width()/2, tft.height()/2-20, 1);
  tft.drawString("Press any button to", tft.width()/2, tft.height()/2, 1);
  tft.drawString("create new!", tft.width()/2, tft.height()/2+10, 1);
}


void Initialization::drawMainMenu(){
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("MAIN MENU", tft.width()/2, tft.height()/2, 1);
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
uint8_t Initialization::char_append(char* buff, char c, uint16_t buff_size) {
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
      char* respon: char-array used as output for function to contain respon
      uint16_t respon_size: size of respon buffer (in bytes)
      uint16_t respon_timeout: duration we'll wait (in ms) for a respon from server
      uint8_t serial: used for printing debug information to terminal (true prints, false doesn't)
   Return value:
      void (none)
*/
void Initialization::do_http_request(char* hos, char* request, char* respon, uint16_t respon_size, uint16_t respon_timeout, uint8_t serial) {
  WiFiClient client; //instantiate a client object
  if (client.connect(hos, 80)) { //try to connect to hos on port 80
    if (serial) Serial.print(request);//Can do one-line if statements in C without curly braces
    Serial.println("\n\n");
    client.print(request);
    memset(respon, 0, respon_size); //Null out (0 is the value of the null terminator '\0') entire buffer
    uint32_t count = millis();
    while (client.connected()) { //while we remain connected read out data coming back
      client.readBytesUntil('\n', respon, respon_size);
      if (serial) Serial.println(respon);
      if (strcmp(respon, "\r") == 0) { //found a blank line!
        break;
      }
      memset(respon, 0, respon_size);
      if (millis() - count > respon_timeout) break;
    }
    memset(respon, 0, respon_size);
    count = millis();
    while (client.available()) { //read out remaining text (body of respon)
      char_append(respon, client.read(), OUT_BUFFER_SIZE);
    }
    if (serial) Serial.println(respon);
    client.stop();
    if (serial) Serial.println("-----------");
  } else {
    if (serial) Serial.println("connection failed :/");
    if (serial) Serial.println("wait 0.5 sec...");
    client.stop();
  }
}
