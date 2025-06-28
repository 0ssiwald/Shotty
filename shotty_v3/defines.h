#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <Arduino.h>

/* Hardware defines */
#define MQ3pin      0     // A0  MQ3 sensor
#define BUTTON_YES  4     // D4  Yes button
#define BUTTON_NO   5     // D5  No button
#define LED_YES     10    // D10 Yes LED
#define LED_NO      12    // D12 No LED
#define RELAIS_PIN  6     // D6 for pump relais
#define SENSOR_PIN  0     // Pin A0 for MQ3 sensor
#define RNG_PIN     1     // Pin A1 is not connected for RNG

/* User changeable defines */
#define LCD_CURSE_TIME        2000    // (ms) How long curses are displayed
#define NO_SHOT_LIMIT         750     //      Sensor value above which the user won't get any more shots
#define SHOT_CANCEL_TIME      3       // (s)  How many seconds the user is able to cancel the shot

/* System defines that should not be changed by the user */
#define BUTTON_CHECK_DELAY      50      // (ms) Delay between checking the button states
#define LOOP_REFRESH_TIME       1000    // (ms) How long to delay between loop() iterations 
#define PUMP_SHOT_TIME          2800    // (ms) How long the pump has to be on for 20 ml
#define WARM_UP_TIME            10000   // (ms) How long the sensor has to warm up after startup
#define SENSOR_THRESHOLD        200     //      The sensor has to fall below this value after startup
#define CALIBRATION_INTERVAL    500     // (ms) Time between sensor updates during calibration

#endif /* _DEFINES_H_ */