#ifndef FUNCTIONS
#define FUNCTIONS

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


#define MQ3pin 0                          //pinA0 für MQ3 sensor
#define RELAIS_PIN 6                      //d6 für Relay steuerung
#define BUTTON_YES 4                      // d4 für yes knopf
#define BUTTON_NO 5                       // d5 für no knopf
#define LED_YES 10                        // d10 für jes LED
#define LED_NO 12                         // d12 für no LED

#define WARM_UP 10000                        // zeit die der Sensor mindestes kalibirert nach dem start
#define REFRESH_ALC_TIME 2000                // wie oft die Sensor Value auf dem Display refresed wird
#define CANCEL_TIME 2000                     // Zeit die man hat zum Abbrechen wenn Shot() startet
#define RELAIS_TIME 2800                    // wie lange der Relay an ist = wie viel eingegossen wird (20 mL)
#define SENSOR_THRESHOLD_VALUE 200           // über diesem Senor Wert geht es nich aus dem Setup
#define NO_AMOUNT 50                         // wie häufig die for schleife durchlaufen muss damit der NO Button aktiviert wird
#define NO_SHOT_LIMIT 750                     // ab welchem Wert kein Shot mehr ausgegeben wird

extern bool doubleShot;
extern float displayValue;
extern int sensorValue, sensorValueFixed, sensorValueReference;
extern unsigned long currentTime, previousTime;
extern int selected_alcohol;

void lcd_Start(LiquidCrystal_I2C);
void start_Screen(LiquidCrystal_I2C);
void calibrate_Routine(LiquidCrystal_I2C);
void ready_Message(LiquidCrystal_I2C, int);
int starting_Measurement(LiquidCrystal_I2C);
void lcd_Curse(LiquidCrystal_I2C);
int get_Highest_Value(int);
void display_Message(LiquidCrystal_I2C, int);
void shot_Or_Not(LiquidCrystal_I2C, int);
void pour_Shot(LiquidCrystal_I2C);
void shot_In_Five(LiquidCrystal_I2C);

#endif
