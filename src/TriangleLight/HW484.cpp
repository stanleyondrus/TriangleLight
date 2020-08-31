#include "HW484.h"

void HW484::init() {
	pinMode(HW484_PIN, INPUT);
}

boolean HW484::triggered() {
	if (millis() - timer >= SAMPLING_PERIOD) {
		timer = millis();
		return digitalRead(HW484_PIN);
	}
	return 0;
}
