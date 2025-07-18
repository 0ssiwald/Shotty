#include "logic.h"

void setup() {
  sensor_init();
  pump_init();
  button_init();
  lcd_init();
  logic_init();

  logic_alc_selection();    /* Let the user pick an alc (or skip it) */
}

void loop() {
  logic_calibration();                                /* Calibrate sensor */
  logic_program_start();                              /* Display user the option to shot or measure */
  button_t button_pressed = button_wait_for_any();    /* Wait for user input */
  if(button_yes == button_pressed) {                  /* User wants a shot */
    logic_shot();
  } else if(button_no == button_pressed) {            /* User wants to measure */
    logic_measurement();
  }
}
