#ifndef InfiniteRun_h
#define InfiniteRun_h
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <mpu6050_esp32.h>
#include "Ball.h"
#include "Obstacle.h"

#define START 0
#define PLAYING 1
#define END 2

class InfiniteRun {
    uint8_t state;
    char* username;
    char* wifi_name;
    char* wifi_password;
    TFT_eSPI tft;
    int highest_score;
    int current_score;
    // Ball ball;
    // Obstacle obstacle;
    Ball ball(TFT_eSPI);
    Obstacle obstacle(TFT_eSPI, int);
    MPU6050 imu;

    public:
        InfiniteRun(TFT_eSPI tftESP, char* user, char* wifi, char* password, MPU6050 imu_arg);
        void step();
    private:
        void postScore();
        bool checkForCollision();
};

#endif
