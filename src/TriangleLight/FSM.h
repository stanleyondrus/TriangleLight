#ifndef LIB_FSM_H_
#define LIB_FSM_H_
#include "stdint.h"
#include "stddef.h"
#include <avr/io.h>
#include <util/delay.h>

typedef enum {
	Startup,
	Manual,
	Auto,
	Audio,
	Sleep
} state_t;

class FSM {
public:
	state_t currentState = Startup; // Holds the state MCU is IN
	state_t nextState = Startup; // Holds the state MCU goes TO next cycle
private:

};

#endif /* LIB_FSM_H_ */
