#ifndef Ball_h
#define Ball_h
#include <SPI.h>
#include <TFT_eSPI.h>

class Ball 
{
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
        Ball(TFT_eSPI tftESP);
        void step(float x_force);
        void reset();
        void moveBall();
    private:
};
#endif
