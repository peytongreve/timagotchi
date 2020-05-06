#include "Login.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h> //Connect to WiFi netw

char login_response[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP request
char ntw[] = "kumba20";  //SSID CHANGE!!
char pwd[] = "yekumba20!"; //passw for WiFi CHANGE!!!
char hst[] = "756bdc48.ngrok.io";
char letter[10];

Login::Login(TFT_eSPI tftESP) {
  state = START;
  tft = tftESP;
  flag = 0;
}

int Login::update(int b1_delta, int b2_delta) {
  flag = 0;
  switch(state){
    case START: // give option to log in or create a new timagotchi
      if(draw){
        drawStart();
        draw = false;
      }
      if(b1_delta != 0 || b2_delta != 0){
        state = NAME;
        draw = true;
        letter_index = 0;
      }
      break;
    case NAME:
      if(draw){
        drawName();
        draw = false;
      }
      if(b2_delta == -1){
        letter_index = (letter_index + 1)%26;
        draw = true;
      } else if (b1_delta == -1) {
        if (letter_index > 0) {
          letter_index--;
        } else {
          letter_index = 25-abs(letter_index);
        }
        draw = true;
      }
      else if(b2_delta == -3){
        login_username[user_index] = letters[letter_index];
        user_index = user_index + 1;
        if(user_index == 10) {
          state = END;
        }
        draw = true;
      }
      else if(b2_delta == -2){
        state = END;
        draw = true;
      } else if (b1_delta == -3) {
        memset(login_username, 0, 11);
        draw = true;
        user_index = 0;
        letter_index = 0;
      } else if (b1_delta == -2) {
        flag = 2;
      }
      break;
    case END:
      if(draw){
        char thing[30];
        char request[500];
        sprintf(request, "GET /timagochi_exists?name=%s HTTP/1.1\r\n", login_username);
        sprintf(request + strlen(request), "Host: %s\r\n", hst);
        strcat(request, "Content-Type: application/x-www-form-urlencoded\r\n");
        sprintf(request + strlen(request), "Content-Length: %d\r\n\r\n", strlen(thing));
        strcat(request, thing);
        do_http_request(hst, request, login_response, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
        Serial.println(login_response);
        draw = false;
        if(login_response[0] == 'f'){
          Serial.println("username doesn't exist");
          state = DNE;
          tft.fillScreen(TFT_WHITE);
          tft.setTextSize(1);
          tft.setTextColor(TFT_RED);
          tft.drawString("username not found :(", tft.width()/2, tft.height()/3);
          tft.setTextSize(1);
          tft.drawString("press LB to go BACK / press RB to try again", tft.width()/2, (2*tft.height())/3);
        } else {
          flag = 1;
          drawEnd();
        }
        draw = false;
      }
      else if (b1_delta == -2){
        state = START;
        draw = true;
      }
      break;
    case DNE:
      if (b1_delta != 0) {
        Serial.println("issue");
        state = START;
        drawStart();
        draw = false;
      } else if (b2_delta != 0) {
        state = NAME;
        memset(login_username, 0, 11);
        user_index = 0;
        letter_index = 0;
        draw = true;
      }
      break;
  }
  return flag;
}

void Login::drawStart(){
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("Welcome Timagatchi!", tft.width()/2, tft.height()/2-10, 1);
  tft.drawString("Press any button", tft.width()/2, tft.height()/2+10, 1);
  tft.drawString("to start!", tft.width()/2, tft.height()/2+20, 1);
}

void Login::drawName(){
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_LIGHTGREY);
  tft.setTextSize(0.5);
  tft.drawString("LB long to go BACK", tft.width()/2, 0);
  tft.drawString("LB double to clear", tft.width()/2, 10);
  tft.drawString("RB long to ENTER", tft.width()/2, 20);
  tft.drawString("RB double to choose", tft.width()/2, 30);
  tft.setTextSize(1.5);
  if (letter_index > 0) tft.drawChar(letters[letter_index-1], 10, (3*tft.height())/4);
  if (letter_index < 25) tft.drawChar(letters[letter_index+1], tft.width()-10, (3*tft.height())/4);
  tft.drawString("Username:", tft.width()/2, tft.height()/3);
  
  tft.setTextDatum(TC_DATUM);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString(login_username, tft.width()/2, tft.height()/2, 1);
  tft.setTextColor(TFT_BLUE);
  tft.drawChar(letters[letter_index], tft.width()/2, (3*tft.height())/4, 1);
  
}

void Login::drawEnd(){
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("Thank you!", tft.width()/2, tft.height()/2-10, 1);
  tft.drawString("Press any button to", tft.width()/2, tft.height()/2, 1);
  tft.drawString("go to main menu!", tft.width()/2, tft.height()/2+10, 1);
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
uint8_t Login::char_append(char* buff, char c, uint16_t buff_size) {
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
void Login::do_http_request(char* hos, char* request, char* response, uint16_t response_size, uint16_t response_timeout, uint8_t serial) {
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
      char_append(response, client.read(), OUT_BUFFER_SIZE);
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
