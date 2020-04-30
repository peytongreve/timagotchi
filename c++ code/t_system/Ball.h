#ifndef Ball_h
#define Ball_h
#include <SPI.h>
#include <TFT_eSPI.h>
#include <mpu6050_esp32.h>

class Ball 
{
    
    MPU6050 imu; //imu object called, appropriately, imu
    float x_pos;
    float Y_POS;
    float x_vel;
    float x_accel;
    int BALL_CLR;
    int BKGND_CLR;
    int MASS;
    int RADIUS;
    int LEFT_LIMIT;
    int RIGHT_LIMIT;
    float K_SPRING;
    int DT;
    TFT_eSPI tft;

    public:
        Ball(TFT_eSPI tftESP, MPU6050 imu_arg);
        void step(float x_force);
        void reset();
        void moveBall();
    private:
};
#endif
