
#include "CoolButton.h"
#include <SPI.h>
#include <TFT_eSPI.h>

CoolButton::CoolButton(int p) {
  flag = 0;  
  state = 0;
  pin = p;
  t_since_short = millis(); //init
  t_of_button_change = millis(); //init
  debounce_time = 10;
  long_press_time = 1000;
  double_click_time = 300;
  button_pressed = 0;
}
void CoolButton::read() {
  uint8_t button_state = digitalRead(pin);  
  button_pressed = !button_state;
}
int CoolButton::update() {
  read();
  switch(state) {
    ////////////////////////
    case REST:
      if (button_pressed) {
        state = T_PRESS;
        t_of_button_change = millis();
      }
      break;
    ////////////////////////
    case T_PRESS:
      if (button_pressed) {
        if (millis() - t_of_button_change >= debounce_time) {
          state = SHORT;
          t_since_short = millis();
        }
      } else {
        state = REST;
        t_of_button_change = millis();
      }
      break;
    ////////////////////////
    case SHORT:
      if (button_pressed) {
        if (millis() - t_since_short >= long_press_time) {
          state = LONG;
          flag = 2;
        }
      } else {
        state = T_DOUBLE;
        t_of_button_change = millis();
      }
      break;
    ////////////////////////
    case LONG:
      if (!button_pressed) {
        state = T_UNPRESS;
        t_of_button_change = millis();
      }
      break;
    ///////////////////////
    case T_DOUBLE:
      if (button_pressed) {
        if (millis() - t_of_button_change >= debounce_time) {
          state = DOUBLE;
          t_of_button_change = millis();
          flag = 3;
        }
      } else if (millis() - t_of_button_change >= double_click_time) {
        state = T_UNPRESS;
        flag = 1;
      }
      break;
    ////////////////////////
    case DOUBLE:
      if (!button_pressed) {
        state = T_UNPRESS;
        t_of_button_change = millis();
      }
      break;
    ///////////////////////
    case T_UNPRESS:
      if (button_pressed) {
        if (flag == 2) {
          state - LONG;
        } else if (flag == 3) {
          state = DOUBLE;
        }
        t_of_button_change = millis();
      } else if (millis() - t_of_button_change >= debounce_time) {
        state = REST;
        flag = 0;
      }
      break;
  }
  return flag;
}
