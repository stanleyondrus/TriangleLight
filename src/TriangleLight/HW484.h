#ifndef LIB_HW484_HW484_H_
#define LIB_HW484_HW484_H_

#include "Arduino.h"
#include "stdint.h"

#define HW484_PIN 13
#define SAMPLING_PERIOD 250

class HW484 {
public:
	void init();
	boolean triggered() ;
private:
	uint32_t timer = millis();
};

#endif /* LIB_HW484_HW484_H_ */
