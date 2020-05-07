#include "InfiniteRun.h"
#include "Ball.h"
#include "Obstacle.h"
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <stdlib.h>
#include <mpu6050_esp32.h>
#include <time.h>

char ho[] = "756bdc48.ngrok.io";
const int RESPONSE_TIMEOUT = 6000; //ms to wait for respons from hos
const uint16_t OUT_BUFFER_SIZE = 1000; //size of buffer to hold HTTP respons
char respons[OUT_BUFFER_SIZE]; //char array buffer to hold HTTP request

InfiniteRun::InfiniteRun(TFT_eSPI tftESP, char* user, char* wifi, char* password, MPU6050 imu_arg) {
    state = 0;
    username = user;
    wifi_name = wifi;
    wifi_password = password;
    tft = tftESP;
    imu = imu_arg;
    ball_x_pos = tftESP.width()/2;
    ball_Y_POS = 100;
    ball_x_vel = 0;
    ball_x_accel = 0;
    BALL_CLR = TFT_RED; // colors for ball and background
    BKGND_CLR = TFT_WHITE;
    MASS = 1;
    RADIUS = 4;
    LEFT_LIMIT = RADIUS;
    RIGHT_LIMIT = tftESP.width() - RADIUS;
    K_SPRING = 0.1; // spring coefficient
    DT = 25; // loop speed
    obstacle_X_POS = 0; // starting x
    obstacle_y_pos = 0;
    obstacle_y_vel = 0.07; // delta y (must be positive)
    OBSTACLE_CLR = TFT_BLACK;
    WIDTH = 25;
    HEIGHT = 5;
    BOTTOM_LIMIT = tftESP.height() - HEIGHT;
}

void InfiniteRun::step(float x_force) {
    switch (state) {
        case START:
            ballReset();
            ballStep(x_force); // get ball moving
            state = PLAYING;
            current_score = 0;
            break;
        case PLAYING:
            // float x = -imu.accelCount[1] * imu.aRes;
            // Serial.println(imu.accelCount[1]);
            // ballStep(-imu.accelCount[1] * imu.aRes * 1000);
            ballStep(x_force);
            obstacleStep();
            // Serial.println(ball_x_pos);
            // check for collision
            // else current_score += 1
            if (checkForCollision()) {
                state = END;
            } else {
                current_score += 1;
            }
            break;
        case END:
            if (current_score > highest_score) {
                highest_score = current_score;
                postScore();
            }
            state = START;
            break;
    }
}

void InfiniteRun::ballStep(float x_force) {
    ball_x_accel = x_force/MASS;
    ball_x_vel = ball_x_vel + 0.001*DT*ball_x_accel;
    tft.fillCircle(ball_x_pos, ball_Y_POS, RADIUS, BKGND_CLR);
    moveBall();
    tft.fillCircle(ball_x_pos, ball_Y_POS, RADIUS, BALL_CLR);
}

void InfiniteRun::moveBall() {
    float x_move = 0.001*DT*ball_x_vel;
    if (ball_x_pos + x_move > RIGHT_LIMIT) {
      if (x_move - 2*(RIGHT_LIMIT - ball_x_pos) > 0) {
        x_move -= 2*(RIGHT_LIMIT - ball_x_pos);
      } else {
        x_move -= RIGHT_LIMIT - ball_x_pos;
        ball_x_pos = RIGHT_LIMIT;
      }
      x_move *= -K_SPRING;
      ball_x_vel *= -K_SPRING;
    } else if (ball_x_pos + x_move < LEFT_LIMIT) {
      if (x_move - 2*(ball_x_pos - LEFT_LIMIT) > 0) {
        x_move -= 2*(ball_x_pos - LEFT_LIMIT);
      } else {
        x_move -= ball_x_pos - LEFT_LIMIT;
        ball_x_pos = LEFT_LIMIT;
      }
      x_move *= -K_SPRING;
      ball_x_vel *= -K_SPRING;
    }
    ball_x_pos += x_move;
}

void InfiniteRun::ballReset() {
    ball_x_pos = tft.width() / 2;
    ball_x_vel = 0;
}

void InfiniteRun::obstacleStep() {
    if (obstacle_y_pos + obstacle_y_vel > BOTTOM_LIMIT) {
        // remove the obstacle
        tft.fillRect(obstacle_X_POS, obstacle_y_pos, WIDTH, HEIGHT, BKGND_CLR);
        // add new one
        obstacle_X_POS = rand() % tft.width(); // random number
        obstacle_y_pos = 0;
    } else {
        tft.fillRect(obstacle_X_POS, obstacle_y_pos, WIDTH, HEIGHT, BKGND_CLR);
        obstacle_y_pos += obstacle_y_vel;
        tft.fillRect(obstacle_X_POS, obstacle_y_pos, WIDTH, HEIGHT, OBSTACLE_CLR);
    }
}

bool InfiniteRun::checkForCollision() {
    int min_x = obstacle_X_POS;
    int max_x = obstacle_X_POS + WIDTH;
    int min_y = obstacle_y_pos;
    int max_y = obstacle_y_pos + HEIGHT;

    int left = ball_x_pos - RADIUS;
    int right = ball_x_pos + RADIUS;

    if (ball_x_pos > min_x && ball_x_pos < max_x && 
            ball_Y_POS > min_y && ball_Y_POS < max_y) { // middle
        return true;
    } else if (left > min_x && left < max_x && 
            ball_Y_POS > min_y && ball_Y_POS < max_y) { // left edge
        return true;
    } else if (right > min_x && right < max_x && 
            ball_Y_POS > min_y && ball_Y_POS < max_y) { // right edge
        return true;
    }
    return false;
}

void InfiniteRun::postScore() {
    char thing[30];
    sprintf(thing, "name=%s&score=%d", username, current_score);
    char request[500];
    sprintf(request, "POST /infinite_run? HTTP/1.1\r\n");
    sprintf(request + strlen(request), "Host: %s\r\n", ho);
    strcat(request, "Content-Type: application/x-www-form-urlencoded\r\n");
    sprintf(request + strlen(request), "Content-Length: %d\r\n\r\n", strlen(thing));
    strcat(request, thing);
    do_http_request(ho, request, respons, OUT_BUFFER_SIZE, RESPONSE_TIMEOUT, true);
    Serial.println(respons);
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
uint8_t InfiniteRun::char_append(char* buff, char c, uint16_t buff_size) {
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
      char* respons: char-array used as output for function to contain respons
      uint16_t respons_size: size of respons buffer (in bytes)
      uint16_t respons_timeout: duration we'll wait (in ms) for a respons from server
      uint8_t serial: used for printing debug information to terminal (true prints, false doesn't)
   Return value:
      void (none)
*/
void InfiniteRun::do_http_request(char* hos, char* request, char* respons, uint16_t respons_size, uint16_t respons_timeout, uint8_t serial) {
  WiFiClient client; //instantiate a client object
  if (client.connect(hos, 80)) { //try to connect to hos on port 80
    if (serial) Serial.print(request);//Can do one-line if statements in C without curly braces
    Serial.println("\n\n");
    client.print(request);
    memset(respons, 0, respons_size); //Null out (0 is the value of the null terminator '\0') entire buffer
    uint32_t count = millis();
    while (client.connected()) { //while we remain connected read out data coming back
      client.readBytesUntil('\n', respons, respons_size);
      if (serial) Serial.println(respons);
      if (strcmp(respons, "\r") == 0) { //found a blank line!
        break;
      }
      memset(respons, 0, respons_size);
      if (millis() - count > respons_timeout) break;
    }
    memset(respons, 0, respons_size);
    count = millis();
    while (client.available()) { //read out remaining text (body of respons)
      char_append(respons, client.read(), OUT_BUFFER_SIZE);
    }
    if (serial) Serial.println(respons);
    client.stop();
    if (serial) Serial.println("-----------");
  } else {
    if (serial) Serial.println("connection failed :/");
    if (serial) Serial.println("wait 0.5 sec...");
    client.stop();
  }
}
