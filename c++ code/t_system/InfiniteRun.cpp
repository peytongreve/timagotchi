#include "InfiniteRun.h"
#include "Ball.h"
#include "Obstacle.h"
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <stdlib.h>
#include <mpu6050_esp32.h>

InfiniteRun::InfiniteRun(TFT_eSPI* tftESP, char* user, char* wifi, char* password, MPU6050 imu_arg) {
    state = 0;
    username = user;
    wifi_name = wifi;
    wifi_password = password;
    tft = tftESP;
    imu = imu_arg;
    ball = Ball(tftESP);
    // obstacle = new Obstacle(tft, 0); // first obstacle is at x = 0
}

InfiniteRun::step() {
    switch (state) {
        case START:
            ball.reset();
            state = PLAYING;
            obstacle = new Obstacle(tft, 0);
            current_score = 0;
            break;
        case PLAYING:
            float x = -imu.accelCount[1] * imu.aRes
            ball.step(x * 1000);
            obstacle.step();
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

InfiniteRun::checkForCollision() {
    int min_x = obstacle.X_POS;
    int max_x = obstacle.X_POS + width;
    int min_y = obstacle.y_pos;
    int max_y = obstacle.y_pos + height;

    int left = ball.x_pos - ball.RADIUS;
    int right = ball.x_pos + ball.RADIUS;

    if (ball.x_pos > min_x && ball.x_pos < max_x && 
            ball.Y_POS > min_y && ball.Y_POS < max_y) { // middle
        return true;
    } else if (left > min_x && left < max_x && 
            ball.Y_POS > min_y && ball.Y_POS < max_y) { // left edge
        return true;
    } else if (right > min_x && right < max_x && 
            ball.Y_POS > min_y && ball.Y_POS < max_y) { // right edge
        return true;
    }
    return false;
}

InfiniteRun::postScore() {
    // post the `highest_score` to server for `username`
}
