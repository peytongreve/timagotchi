
#ifndef CoolButton_h
#define CoolButton_h
#include <SPI.h>
#include <TFT_eSPI.h>

#define REST 0
#define T_PRESS 1
#define SHORT 2
#define LONG 3
#define T_DOUBLE 4
#define DOUBLE 5
#define T_UNPRESS 6

class CoolButton
{
  private:
  uint32_t t_since_short;
  uint32_t t_of_button_change;    
  uint32_t debounce_time;
  uint32_t long_press_time;
  uint32_t double_click_time;
  uint8_t pin;
  uint8_t flag;
  bool button_pressed;
  uint8_t state;
  
  public:
    CoolButton(int p);
    void read();
    int update();
};
#endif
