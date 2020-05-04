#include "InfiniteRun.h"
#include "Ball.h"
#include "Obstacle.h"
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <stdlib.h>
#include <mpu6050_esp32.h>
#include <time.h>

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
    // post the `highest_score` to server for `username`
}
