#ifndef FLIPDOT_5X7_H
#define FLIPDOT_5X7_H

#include <Adafruit_GFX.h>

#define FLIPDOT_BLACK          0
#define FLIPDOT_YELLOW         1

#define FLIPDOT_PULSE_DURATION 475
// 475 = 16.625ms(35pixel)
#define FLIPDOT_I2C_OFFSET     8

#define FLIPDOT_MODULE_WIDTH   5
#define FLIPDOT_MODULE_HEIGHT  7

class FlipDot_5x7 : public Adafruit_GFX {
  private:
    uint8_t
      _clockpin,
      _datapin,
      _latch;

    boolean *imageBuffer = NULL;
    boolean *oldImageBuffer = NULL;
    boolean _invert;
    boolean _serialMode = false;
    boolean _rtl = false;
    uint8_t _xModules, _yModules,
      _pixelDelay = 0;

    void displayPixel(int16_t, int16_t, boolean);

  public:
    void begin(void);
    void begin(uint8_t, uint8_t, uint8_t);
    void drawPixel(int16_t, int16_t, uint16_t);
    void display(void);
    void setPixelDelay(uint8_t);
    void setRtl(boolean);
    void invert(boolean);
    int16_t width(void) const;
    FlipDot_5x7(uint8_t, uint8_t);
    FlipDot_5x7(uint8_t, uint8_t, boolean);
};

#endif //FLIPDOT_5X7_H
