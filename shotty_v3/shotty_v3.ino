#include "logic.h"
#include "akinator.h"

void setup() {
  sensor_init();
  pump_init();
  button_init();
  lcd_init();
  logic_init();

  logic_alc_selection();
}

void loop() {
  logic_calibration();                                /* Calibrate sensor */
  logic_program_start();                              /* Display user the option to shot or measure */
}
