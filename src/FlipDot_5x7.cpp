#include "FlipDot_5x7.h"

FlipDot_5x7::FlipDot_5x7(uint8_t xModules, uint8_t yModules) :
  // +10 is a dirty hack to allow displaying chars on last display.
  Adafruit_GFX(xModules*FLIPDOT_MODULE_WIDTH+10, yModules*FLIPDOT_MODULE_HEIGHT),
  _xModules(xModules),
  _yModules(yModules),
  _invert(false) {
  imageBuffer = (boolean*) calloc(width()*height(), sizeof(boolean));
  oldImageBuffer = (boolean*) calloc(FLIPDOT_MODULE_WIDTH*height(), sizeof(boolean));
}

FlipDot_5x7::FlipDot_5x7(uint8_t xModules, uint8_t yModules, boolean invert) :
  // +10 is a dirty hack to allow displaying chars on last display.
  Adafruit_GFX(xModules*FLIPDOT_MODULE_WIDTH+10, yModules*FLIPDOT_MODULE_HEIGHT),
  _xModules(xModules),
  _yModules(yModules),
  _invert(invert) {
  imageBuffer = (boolean*) calloc(width()*height(), sizeof(boolean));
  oldImageBuffer = (boolean*) calloc(FLIPDOT_MODULE_WIDTH*height(), sizeof(boolean));
}

int16_t FlipDot_5x7::width() const {
  return _xModules*FLIPDOT_MODULE_WIDTH;
}

void FlipDot_5x7::begin() {
}

void FlipDot_5x7::begin(uint8_t data, uint8_t clockpin, uint8_t latch) {
  _datapin = data;
  _clockpin = clockpin;
  _latch = latch;

  pinMode(_datapin, OUTPUT);
  pinMode(_clockpin, OUTPUT);
  pinMode(_latch, OUTPUT);
  digitalWrite(_latch, LOW);

  fillScreen(FLIPDOT_BLACK);
  display();
  delay(10);
}

void FlipDot_5x7::displayPixel(int16_t x, int16_t y, boolean color) {
  if (x < FLIPDOT_MODULE_WIDTH && y < FLIPDOT_MODULE_HEIGHT) {
    // DR R0 R1 R2 DC C0 C1 C2
    uint8_t data = color != _invert;
    data |= y << 1;
    data |= (color == _invert) << 4;
    data |=  x << 5;
    shiftOut(_datapin, _clockpin, LSBFIRST, data);
  }
}

void FlipDot_5x7::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if (x < 0 || y < 0 || x >= _width || y >= _height){
    return;
  }
  if (x >= width() || y >= height()) return;
  imageBuffer[y*width()+x] = (color>0);
}

void FlipDot_5x7::setPixelDelay(uint8_t pixelDelay) {
  _pixelDelay = pixelDelay;
}

void FlipDot_5x7::setRtl(boolean rtl) {
  _rtl = rtl;
}

void FlipDot_5x7::invert(boolean invert) {
  _invert = invert;
}

void FlipDot_5x7::display(void) {
  portDISABLE_INTERRUPTS();
  for (uint8_t x = 0; x < FLIPDOT_MODULE_WIDTH; x++) {
    for (uint8_t y = 0; y < FLIPDOT_MODULE_HEIGHT; y++) {
      //uint8_t dnc = 0;
      for (int8_t yModule = _yModules-1; yModule > -1; yModule--) {
        for (int8_t xModule = (yModule%2 != _rtl)?(_xModules-1):0;
          ((yModule%2 != _rtl)?-1:xModule) < ((yModule%2 != _rtl)?xModule:_xModules);
          xModule += (yModule%2 != _rtl)?-1:1) {

          uint8_t _x = (_xModules - xModule-1) * FLIPDOT_MODULE_WIDTH+x;
          uint8_t _y = yModule * FLIPDOT_MODULE_HEIGHT+y;
          //if (oldImageBuffer[y*width()+x] == imageBuffer[y*width()+x]) dnc++;
          displayPixel(x,y,imageBuffer[_y*width()+_x]);
          oldImageBuffer[_y*width()+_x] = imageBuffer[_y*width()+_x];
        }
      }
      digitalWrite(_latch, HIGH);
      delayMicroseconds(FLIPDOT_PULSE_DURATION);
      digitalWrite(_latch, LOW);
      delay(_pixelDelay);
    }
  }
  //disable
  for (uint8_t m = 0; m < _xModules*_yModules; m++) {
    shiftOut(_datapin, _clockpin, MSBFIRST, 0);
  }
  digitalWrite(_latch, HIGH);
  digitalWrite(_latch, LOW);
  portENABLE_INTERRUPTS();
}
