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
      Obstacle(TFT_eSPI tftESP, float x);
      Obstacle(TFT_eSPI tftESP, float x, float vel, int length, int height);
      void step();
      void clear();
};

#endif
