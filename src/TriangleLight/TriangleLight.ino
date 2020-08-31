#include "Arduino.h"
#include "HW484.h"
#include "TriangleLight.h"
#include "FSM.h"

#define SERIAL_BAUD_RATE 9600
#define SERIAL_START_CHAR 'X'
#define SERIAL_STOP_CHAR 'Y'
#define BTN_PIN 12

HW484 mic;
TriangleLight tl;
FSM fsm;

void setup() {
	Serial.begin(SERIAL_BAUD_RATE);
	pinMode(BTN_PIN, INPUT_PULLUP);
	mic.init();
	tl.init();
}

void loop() {
	switch (fsm.currentState) {
	case Startup: {
		fsm.nextState = Manual;
		break;
	}
	case Manual: {
		break;
	}
	case Auto: {
		tl.randomChangeStatePeriodic();
		break;
	}
	case Audio: {
		if (mic.triggered()) {
			tl.randomChangeState();
		}
		break;
	}
	case Sleep: {
		break;
	}
	}
	periodicOp();
}

void periodicOp() {
	String msg = "";
	if (Serial.peek() == SERIAL_START_CHAR) {
		while (Serial.available() && Serial.peek() != SERIAL_STOP_CHAR) { msg += Serial.readString();}
		msg = msg.substring(1, msg.length() - 1); // unwrap from START/STOP chars
		if (msg.charAt(0) == 'M') {
			tl.mode = msg.charAt(1) - '0';;
			fsm.nextState = Manual;
		} else if (msg.charAt(0) == 'A') {
			uint8_t mode = msg.charAt(1) - '0';
			if (!mode) {
				//				tl.randomChangeStatePeriod = msg.substring(2,msg.length()-1).toInt();
				fsm.nextState = Auto;
			}
			else fsm.nextState = Audio;
		}
	} else {
		Serial.flush();
	}

	if (!digitalRead(BTN_PIN)) {
		uint8_t flag = 0;
		uint32_t t = millis();
		while (!digitalRead(BTN_PIN)) {
			if (millis() - t >= 1000) {
				if (fsm.currentState == Manual) fsm.nextState = Auto;
				else if (fsm.currentState == Auto) fsm.nextState = Audio;
				else if (fsm.currentState == Audio) fsm.nextState = Manual;
				flag = 1;
				break;
			}
		}
		if (!flag) tl.mode = (tl.mode+1) % MODE_COUNT;
	}

	tl.update();
	if (fsm.currentState != fsm.nextState) handleFsmSwitch();
}

// execute actions on FSM transitions
void handleFsmSwitch() {
	switch (fsm.nextState) {
	case Startup: {
		break;
	}
	case Manual: {
		Serial.println("MANUAL");
		break;
	}
	case Auto: {
		Serial.println("AUTO");
		break;
	}
	case Audio: {
		Serial.println("AUDIO");
		break;
	}
	case Sleep: {
		break;
	}
	}
	fsm.currentState = fsm.nextState; // change the current state to the selected next state
}

String getValue(String data, char separator, int index) {
	int found = 0;
	int strIndex[] = {0, -1};
	int maxIndex = data.length()-1;

	for(int i=0; i<=maxIndex && found<=index; i++){
		if(data.charAt(i)==separator || i==maxIndex){
			found++;
			strIndex[0] = strIndex[1]+1;
			strIndex[1] = (i == maxIndex) ? i+1 : i;
		}
	}

	return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
