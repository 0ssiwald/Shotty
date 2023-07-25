#include "functions.h"
#include <Wire.h>                                              //???
#include <LiquidCrystal_I2C.h>                                 //???

LiquidCrystal_I2C lcd(0x27,20,4);

bool doubleShot = true;
int sensorValue;                                    // variable to store sensor value
int sensorValueFixed;                               // variable to store the sensor Value für determining the Alkohollevel
int sensorValueReference;                           // variable to store the current value before mesurement
float displayValue;                                 // to display a value that is hopefully close to the promille value
unsigned long currentTime;                          // notwendig für die millis() funktion
unsigned long previousTime = 0;                     // notwendig für die millis() funktion
int selected_alcohol = -1;                          // -1 == no selection

void setup() {
  pinMode(BUTTON_YES, INPUT_PULLUP);             //Button for Yes
  pinMode(BUTTON_NO, INPUT_PULLUP);              //Button for No
  pinMode (LED_YES, OUTPUT);
  pinMode (LED_NO, OUTPUT);
  pinMode(RELAIS_PIN, OUTPUT);
  randomSeed(analogRead(1));                     // analog pin 1 is not connected for RNG of curses[]

  lcd_Start(lcd);                            
  start_Screen(lcd);                        // displays StartMessage
}

void loop() {
  sensorValue = analogRead(MQ3pin);  
  currentTime = millis();
  if (currentTime <= WARM_UP || sensorValue >= SENSOR_THRESHOLD_VALUE + 10) {       // +10 versuchter fix1
    while (!digitalRead(BUTTON_NO) || !digitalRead(BUTTON_YES)) {}                   // versuchter fix2
    calibrate_Routine(lcd);  
    }
    
  if (currentTime - previousTime >= REFRESH_ALC_TIME) {  
    ready_Message(lcd, sensorValue);
    previousTime = currentTime;
  }
  if (!digitalRead(BUTTON_NO)) {
    sensorValueFixed = starting_Measurement(lcd);
    if(sensorValueFixed != -1)
      display_Message(lcd, sensorValueFixed);
  }
  if (!digitalRead(BUTTON_YES)) {
    shot_In_Five(lcd);                                 
  }  
}
