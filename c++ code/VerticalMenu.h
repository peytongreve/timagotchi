#ifndef Menu_h
#define Menu_h
#include <SPI.h>
#include <TFT_eSPI.h>

#define HOME 0
#define GRIND 1
#define EAT 2
#define PLAY 3
#define RESUME 4

class VerticalMenu
{
  private:
  uint8_t state;
  uint32_t num_items;
  
  TFT_eSPI tft;
  
  public:
    uint32_t selected;
    VerticalMenu(TFT_eSPI tftESP);
    void update(int b1_delta, int b2_delta);
    void drawCursor();
    void moveCursor(int increment);
    void eraseCursor();
    void displayHome();
    void displayGrind();
    void displayEat();
    void displayPlay();
    void displayResume();
};
#endif
