#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "button.h"
#include "lcd.h"
#include "pump.h"
#include "sensor.h"

void logic_init(void);
void logic_alc_selection(void);
void logic_program_start(void);
void logic_shot(void);
void logic_measurement(void);
void logic_calibration(void);

#endif /* _LOGIC_H_ */