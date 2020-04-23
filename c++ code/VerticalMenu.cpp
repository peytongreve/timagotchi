#include "VerticalMenu.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include <pgmspace.h>

VerticalMenu::VerticalMenu(TFT_eSPI tftESP) {
  state = HOME;
  selected = 1;
  tft = tftESP;
}
void VerticalMenu::update(int b1_delta, int b2_delta) {
  switch(state) {
    case HOME:
      if (b1_delta == -1) {
        moveCursor(-1);
      } else if (b2_delta == -1) {
        moveCursor(1);
      } else if (b2_delta == -3) {
        eraseCursor();
        int old_selected = selected;
        selected = 1;
        switch (old_selected) {
          case 1:
            state = GRIND;
            displayGrind();
            break;
          case 2:
            state = EAT;
            displayEat();
            break;
          case 3:
            state = PLAY;
            displayPlay();
            break;
          case 4:
            state = RESUME;
            displayResume();
            break;
        }
      }
      break;
    ////////////////////////////////
    case GRIND:
      if (b1_delta == -1) {
        moveCursor(-1);
      } else if (b2_delta == -1) {
        moveCursor(1);
      } else if (b1_delta == -3) {
        state = HOME;
        eraseCursor();
        selected = 1;
        displayHome();
      }
      break;
    ////////////////////////////////
    case EAT:
      if (b1_delta == -1) {
        moveCursor(-1);
      } else if (b2_delta == -1) {
        moveCursor(1);
      } else if (b1_delta == -3) {
        state = HOME;
        eraseCursor();
        selected = 1;
        displayHome();
      }
      break;
    ////////////////////////////////
    case PLAY:
      if (b1_delta == -1) {
        moveCursor(-1);
      } else if (b2_delta == -1) {
        moveCursor(1);
      } else if (b1_delta == -3) {
        state = HOME;
        eraseCursor();
        selected = 1;
        displayHome();
      }
      break;
    ////////////////////////////////
    case RESUME:
      if (b1_delta == -1) {
        moveCursor(-1);
      } else if (b2_delta == -1) {
        moveCursor(1);
      } else if (b1_delta == -3) {
        state = HOME;
        eraseCursor();
        selected = 1;
        displayHome();
      }
      break;
  }
}
void VerticalMenu::displayHome() {
  num_items = 4;
  tft.fillScreen(TFT_WHITE);
  drawNewBeaver();
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("Grind", tft.width()/2, (4*tft.height()/(num_items*2)) + 10, 1);
  tft.drawString("Eat", tft.width()/2, (5*tft.height()/(num_items*2)) + 10, 1);
  tft.drawString("Play", tft.width()/2, (6*tft.height()/(num_items*2)) + 10, 1);
  tft.drawString("Resume", tft.width()/2, (7*tft.height()/(num_items*2)) + 10, 1);
  drawCursor();
}
void VerticalMenu::drawCursor() {
  int top = ((num_items+selected-1)*tft.height())/(num_items*2);
  int bottom = ((num_items+selected)*tft.height())/(num_items*2);
  int left = 10;
  int right = tft.width() - 10;
  tft.drawLine(left, top, right, top, TFT_RED);
  tft.drawLine(left, bottom, right, bottom, TFT_RED);
  tft.drawLine(left, top, left, bottom, TFT_RED);
  tft.drawLine(right, top, right, bottom, TFT_RED);
}
void VerticalMenu::eraseCursor() {
  int top = ((num_items+selected-1)*tft.height())/(num_items*2);
  int bottom = ((num_items+selected)*tft.height())/(num_items*2);
  int left = 10;
  int right = tft.width() - 10;
  tft.drawLine(left, top, right, top, TFT_WHITE);
  tft.drawLine(left, bottom, right, bottom, TFT_WHITE);
  tft.drawLine(left, top, left, bottom, TFT_WHITE);
  tft.drawLine(right, top, right, bottom, TFT_WHITE);
}
void VerticalMenu::moveCursor(int increment) {
  eraseCursor();
  selected = selected + increment;
  if (selected > num_items) {
    selected = selected % num_items;
  } else if (selected == 0) {
    selected = num_items;
  } else if (selected < 1) {
    selected = (-1*selected-num_items) % num_items;
  }
  drawCursor();
}

void VerticalMenu::displayGrind() {
  num_items = 3;
  tft.fillScreen(TFT_WHITE);
  drawBookIcon();
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("Pset", tft.width()/2, (3*tft.height()/(num_items*2)) + 10, 1);
  tft.drawString("Midterm", tft.width()/2, (4*tft.height()/(num_items*2)) + 10, 1);
  tft.drawString("Labs", tft.width()/2, (5*tft.height()/(num_items*2)) + 10, 1);
  drawCursor();
}
void VerticalMenu::displayEat() {
  num_items = 3;
  tft.fillScreen(TFT_WHITE);
  drawEatingIcon();
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("Dining Hall", tft.width()/2, (3*tft.height()/(num_items*2)) + 10, 1);
  tft.drawString("La Verdes", tft.width()/2, (4*tft.height()/(num_items*2)) + 10, 1);
  tft.drawString("Restaurant", tft.width()/2, (5*tft.height()/(num_items*2)) + 10, 1);
  drawCursor();
}
void VerticalMenu::displayPlay() {
  // also want to display people online in top corner
  num_items = 3;
  tft.fillScreen(TFT_WHITE);
  drawGamingIcon();
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1.8);
  tft.drawString("Infinite Run", tft.width()/2, (3*tft.height()/(num_items*2)) + 10, 1);
  tft.drawString("[some other game?...]", tft.width()/2, (4*tft.height()/(num_items*2)) + 10, 1);
  tft.drawString("View Score", tft.width()/2, (5*tft.height()/(num_items*2)) + 10, 1);
  drawCursor();
}
void VerticalMenu::displayResume() {
  num_items = 3;
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1);
  tft.drawString("[To be integrated...]", tft.width()/2, (3*tft.height()/(num_items*2)) + 10, 1);
  drawCursor();
}
void VerticalMenu::drawEatingIcon() {
  tft.drawBitmap(29,0, eating_icon, 70, 70, BLACK);
}
void VerticalMenu::drawGamingIcon() {
  tft.drawBitmap(29,0, gaming_icon, 70,70, BLACK);
}
void VerticalMenu::drawNewBeaver() {
  tft.drawBitmap(29,0, newbeaver, 70,70, BLACK);
}
void VerticalMenu::drawBookIcon() {
  tft.drawBitmap(29,0, book_icon, 70,70, BLACK);
}
