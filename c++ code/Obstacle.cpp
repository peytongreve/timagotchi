#include "Obstacle.h"
#include <SPI.h>
#include <TFT_eSPI.h>

Obstacle::Obstacle(TFT_eSPI tftESP, float x, float vel=5, int length=10, int height=5) {
    X_POS = x; // starting x
    y_pos = 0;
    y_vel = vel; // delta y (must be positive)
    OBSTACLE_CLR = TFT_BLACK;
    BKGND_CLR = TFT_WHITE;
    WIDTH = length;
    HEIGHT = height;
    BOTTOM = tftESP.height() - HEIGHT;
    tft = tftESP;
}

void Obstacle::step() { // call this every time step
    if (y_pos + y_vel > BOTTOM) {
        // remove the obstacle
        tft.fillRect(X_POS, y_pos, WIDTH, HEIGHT, BKGND_CLR);
    } else {
        tft.fillRect(X_POS, y_pos, WIDTH, HEIGHT, BKGND_CLR);
        y_pos += y_vel;
        tft.fillRect(X_POS, y_pos, WIDTH, HEIGHT, OBSTACLE_CLR);
    }
}