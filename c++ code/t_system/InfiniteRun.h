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
    MPU6050 imu;
    float ball_x_pos;
    float ball_Y_POS;
    float ball_x_vel;
    float ball_x_accel;
    int BALL_CLR;
    int BKGND_CLR;
    int MASS;
    int RADIUS;
    int LEFT_LIMIT;
    int RIGHT_LIMIT;
    float K_SPRING;
    int DT;
    float obstacle_X_POS; // starting point of obstacle
    float obstacle_y_pos;
    float obstacle_y_vel;
    int OBSTACLE_CLR;
    int WIDTH;
    int HEIGHT;
    int BOTTOM_LIMIT;

    public:
        InfiniteRun(TFT_eSPI tftESP, char* user, char* wifi, char* password, MPU6050 imu_arg);
        void step();
        void obstacleStep();
        void ballStep(float x_force);
        void ballReset();
    private:
        void postScore();
        bool checkForCollision();
        void moveBall();
};

#endif
