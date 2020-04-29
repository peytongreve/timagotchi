#include "Ball.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <mpu6050_esp32.h>

Ball::Ball(TFT_eSPI tftESP, MPU6050 imu_arg) {
    imu = imu_arg;
    x_pos = tftESP.width()/2;
    Y_POS = (3/4)*tftESP.height();
    x_vel = 0;
    x_accel = 0;
    BALL_CLR = TFT_RED; // colors for ball and background
    BKGND_CLR = TFT_WHITE;
    MASS = 1;
    RADIUS = 4;
    LEFT_LIMIT = RADIUS;
    RIGHT_LIMIT = tftESP.width() - RADIUS;
    K_SPRING = 0.9; // spring coefficient
    DT = 25; // loop speed
    tft = tftESP;
}

void Ball::step(float x_force=0) { // use the force from the imu
    x_accel = x_force/MASS;
    x_vel = x_vel + 0.001*DT*x_accel;
    tft.drawCircle(x_pos, Y_POS, RADIUS, BKGND_CLR);
    moveBall();
    tft.drawCircle(x_pos, Y_POS, RADIUS, BALL_CLR);
}

void Ball::reset() {
    x_pos = tftESP.width() / 2;
    x_vel = 0;
}

void Ball::moveBall() {
    float x_move = 0.001*DT*x_vel;
    if (x_pos + x_move > RIGHT_LIMIT) {
      if (x_move - 2*(RIGHT_LIMIT - x_pos) > 0) {
        x_move -= 2*(RIGHT_LIMIT - x_pos);
      } else {
        x_move -= RIGHT_LIMIT - x_pos;
        x_pos = RIGHT_LIMIT;
      }
      x_move *= -K_SPRING;
      x_vel *= -K_SPRING;
    } else if (x_pos + x_move < LEFT_LIMIT) {
      if (x_move - 2*(x_pos - LEFT_LIMIT) > 0) {
        x_move -= 2*(x_pos - LEFT_LIMIT);
      } else {
        x_move -= x_pos - LEFT_LIMIT;
        x_pos = LEFT_LIMIT;
      }
      x_move *= -K_SPRING;
      x_vel *= -K_SPRING;
    }
    x_pos += x_move;
}
