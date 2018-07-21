#include <FlipDot_5x7.h>

//ESP32
#define CLOCK 18//SH_CP
#define DATA  23//DS
#define LATCH 19//ST_CP

FlipDot_5x7 flipdot(6, 1, false);

const char* text = "Hello, World!!";

void setup() {
  flipdot.begin(DATA, CLOCK, LATCH);
  flipdot.setTextColor(FLIPDOT_YELLOW, FLIPDOT_BLACK);
}

void loop() {
  for (int16_t i = flipdot.width(); i > (int16_t)(strlen(text))*-6; i--) {
    flipdot.setCursor(i,0);
    flipdot.print(text);
    flipdot.display();
    delay(100);
  }
}
