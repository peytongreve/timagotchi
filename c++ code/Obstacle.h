#ifndef Obstacle_h
#define Obstacle_h
#include <SPI.h>
#include <TFT_eSPI.h>

class Obstacle {
    float X_POS; // starting point of obstacle
    float y_pos;
    float y_vel;
    int OBSTACLE_CLR;
    int BKGND_CLR;
    int WIDTH;
    int HEIGHT;
    int BOTTOM;
    TFT_eSPI tft;

    public:
        void step();
}

#endif