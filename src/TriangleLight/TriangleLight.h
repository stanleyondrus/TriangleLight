#ifndef LIB_TRIANGLELIGHT_TRIANGLELIGHT_H_
#define LIB_TRIANGLELIGHT_TRIANGLELIGHT_H_

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

#define TRIANGLE_SIDE_1_PIN A0
#define TRIANGLE_SIDE_2_PIN A1
#define TRIANGLE_SIDE_3_PIN A2
#define TRIANGLE_CENTER_PIN A3

#define TRIANGLE_SIDE_1_COUNT 3
#define TRIANGLE_SIDE_2_COUNT 3
#define TRIANGLE_SIDE_3_COUNT 3
#define TRIANGLE_CENTER_COUNT 9

#define MODE_COUNT 7

class TriangleLight {
public:
	void init();
	void test();
	void update();
	void randomChangeState();
	void randomChangeStatePeriodic();
	uint8_t brightness = 255;
	uint8_t mode = 0;
	uint16_t randomChangeStatePeriod = 250;
private:
	void rainbow(Adafruit_NeoPixel& strip);
	void color(Adafruit_NeoPixel& strip, uint32_t color);
	void strobe(uint16_t dly, uint8_t randomColor);
	void theaterChaseRainbow(uint16_t dly);
	long firstPixelHue = 0;
	uint32_t firstPixelHueIncrement = 8;
	uint32_t timer = millis();
	uint32_t randomChangeStateTimer = millis();
	uint8_t var = 0;
	uint8_t flag = 0;
};

#endif /* LIB_TRIANGLELIGHT_TRIANGLELIGHT_H_ */
