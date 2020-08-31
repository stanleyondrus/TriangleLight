#include "TriangleLight.h"

Adafruit_NeoPixel side_1(TRIANGLE_SIDE_1_COUNT, TRIANGLE_SIDE_1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel side_2(TRIANGLE_SIDE_2_COUNT, TRIANGLE_SIDE_2_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel side_3(TRIANGLE_SIDE_3_COUNT, TRIANGLE_SIDE_3_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel center(TRIANGLE_CENTER_COUNT, TRIANGLE_CENTER_PIN, NEO_GRB + NEO_KHZ800);

void TriangleLight::init() {
	side_1.begin();
	side_2.begin();
	side_3.begin();
	center.begin();
	randomSeed(analogRead(A4));
}

void TriangleLight::rainbow(Adafruit_NeoPixel& strip) {
	firstPixelHue = (firstPixelHue < 1*65536) ? firstPixelHue += firstPixelHueIncrement : 0;
	for(uint16_t i=0; i<strip.numPixels(); i++) {
		int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
		strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
	}
	strip.show();
}

void TriangleLight::color(Adafruit_NeoPixel& strip, uint32_t color) {
	for(uint16_t i=0; i<strip.numPixels(); i++) {
		strip.setPixelColor(i, color);
	}
	strip.show();
}

void TriangleLight::strobe(uint16_t dly, uint8_t randomColor) {
	if (millis() - timer >= dly) {
		flag ^= 1UL << 1;
		uint8_t c1 = flag?randomColor?random(20,brightness+1):brightness:0;
		uint8_t c2 = flag?randomColor?random(20,brightness+1):brightness:0;
		uint8_t c3 = flag?randomColor?random(20,brightness+1):brightness:0;
		color(side_1, side_1.Color(c1, c2, c3));
		color(side_2, side_2.Color(c3, c1, c2));
		color(side_3, side_3.Color(c2, c3, c1));
		color(center, center.Color(c1, c1, c1));
		timer = millis();
	}
}

void TriangleLight::theaterChaseRainbow(uint16_t dly) {
	if (millis() - timer >= dly) {
		side_1.clear();
		side_2.clear();
		side_3.clear();
		center.clear();

		firstPixelHue = (firstPixelHue < 1*65536) ? firstPixelHue += firstPixelHueIncrement : 0;
		int hue = firstPixelHue + var * 65536L / center.numPixels();
		uint32_t color = center.gamma32(center.ColorHSV(hue));

		side_1.setPixelColor(var, color);
		side_2.setPixelColor(var, color);
		side_3.setPixelColor(var, color);
		center.setPixelColor(var, color);
		center.setPixelColor(var+3, color);

		side_1.show();
		side_2.show();
		side_3.show();
		center.show();
		firstPixelHue += 65536 / 90;
		var = var<2 ? var+1 : 0;
		timer = millis();
	}
}

void TriangleLight::update() {
	switch (mode) {
	case 0:
		firstPixelHueIncrement = 8;
		rainbow(side_1);
		rainbow(side_2);
		rainbow(side_3);
		rainbow(center);
		break;
	case 1:
		firstPixelHueIncrement = 16;
		rainbow(side_1);
		rainbow(side_2);
		rainbow(side_3);
		rainbow(center);
		break;
	case 2:
		firstPixelHueIncrement = 125;
		rainbow(side_1);
		rainbow(side_2);
		rainbow(side_3);
		rainbow(center);
		break;
	case 3:
		strobe(50, 0);
		break;
	case 4:
		strobe(50, 1);
		break;
	case 5:
		firstPixelHueIncrement = 64;
		theaterChaseRainbow(75);
		break;
	case 6:
		firstPixelHueIncrement = 64;
		theaterChaseRainbow(100);
		break;
	}
}

void TriangleLight::randomChangeState() {
	mode = random(0, MODE_COUNT+1);
}

void TriangleLight::randomChangeStatePeriodic() {
	if (millis() - randomChangeStateTimer >= randomChangeStatePeriod) {
		randomChangeState();
		randomChangeStatePeriod = random(100, 1001);
		randomChangeStateTimer = millis();
	}
}
