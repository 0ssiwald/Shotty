#include "logic.h"

size_t alc_selected = -1;   /* Index of alc_types array. Stores the user selected alc type */


/***************************************************************************************************/
void logic_init(void)
{
  randomSeed(analogRead(RNG_PIN));
} /* logic_init */


/***************************************************************************************************/
void logic_alc_selection(void)
{
  lcd_alc_selection_start();

  button_t button_pressed = button_wait_for_any();
  if(button_no == button_pressed) { /* User does not want to select an alcohol */
    lcd_clear();
    return;
  }

  /* User wants to select an alcohol */
  size_t alc_len = sizeof(alc_types)/sizeof(*alc_types);
  alc_selected = 0;
  while(true) {
    lcd_clear();
    lcd_centered_string(alc_types[alc_selected], 1);  /* Print currently selected alc */
    button_pressed = button_wait_for_any();           /* Ask the user if they want this alc or to see the next option */
    if(button_yes == button_pressed) {                /* User wants the currently selected alc */
      lcd_clear();
      return;
    } else {                                          /* User wants to see the next alc */
      if(++alc_selected == alc_len) {
        alc_selected = 0;
      }
    }
  }
} /* logic_alc_selection */


/***************************************************************************************************/
void logic_program_start(void)
{
  char * alc_str = (-1 == alc_selected) ? "" : alc_types[alc_selected];
  lcd_ready_message(alc_str);
} /* logic_program_start */


/***************************************************************************************************/
/* Returns whether or not the shot was canceled */
bool _logic_pour_shot(void)
{
  pump_start();
  button_t button_pressed = button_wait_for_any_timed(PUMP_SHOT_TIME);  /* Check for user cancelation and time the shot */
  pump_stop();
  if(button_no == button_pressed) {   /* User canceled the shot */
    return true;
  }
  return false;
} /* _logic_pour_shot */


/***************************************************************************************************/
void _logic_cheer(void)
{
  lcd_cheer();
} /* _logic_cheer */


/***************************************************************************************************/
void _logic_curse(void)
{
  digitalWrite(LED_NO, HIGH);
  lcd_curse();
  digitalWrite(LED_NO, LOW);
} /* _logic_curse */


/***************************************************************************************************/
void logic_shot(void)
{
  lcd_clear();
  _logic_cheer();

  /* Countdown to first shot */
  lcd_shot_countdown_setup();
  for(unsigned short seconds_left = SHOT_CANCEL_TIME; seconds_left > 0; seconds_left--) {
    lcd_shot_countdown_update(seconds_left);                      /* Update LCD with seconds left */
    button_t button_pressed = button_wait_for_any_timed(1000);    /* Wait 1s for user input */
    if(button_no == button_pressed) {                             /* Shot canceled */
      _logic_curse();
      return;
    }
  }

  /* Pour the shot and check for cancelation */
  if(_logic_pour_shot()) {    
    _logic_curse();         /* Shot canceled */
    return;
  }

  /* Ask for double shot */
  lcd_double_shot();
  button_t button_pressed = button_wait_for_any();
  if(button_no == button_pressed) {   /* No double canceled */
    _logic_curse();
    return;
  }

  /* Pour double shot */
  _logic_cheer();
  (void)_logic_pour_shot();   /* Pour shot without checking for cancelation */
} /* logic_shot */


/***************************************************************************************************/
void _logic_shot_or_not(int sensor_val)
{
  float display_val = (sensor_val < 25) ? 0.0 : (float)sensor_val / 500.0;
  lcd_clear();
  lcd_alc_percentage(display_val);
  lcd_alc_level_t alc_level = alc_level_none;
  switch(sensor_val) {
    case 0   ... 25                : alc_level = alc_level_none;  break;
    case 26  ... 250               : alc_level = alc_level_one;   break;
    case 251 ... 500               : alc_level = alc_level_two;   break;
    case 501 ... NO_SHOT_LIMIT     : alc_level = alc_level_three; break;
    case NO_SHOT_LIMIT + 1 ... 9999: alc_level = alc_level_max;   break;
    default: break;
  }
  lcd_alc_level(alc_level);

  button_t button_pressed = button_wait_for_any();
  if(alc_level_max == alc_level) {        /* No more shots for the user */
    if(button_no == button_pressed) {     /* Pressed N like a good boy */
      lcd_good_boy(true);
    } else {                              /* Pressed Y like a bad boy */
      lcd_good_boy(false);
    } 
  } else {                                /* User may have a shot */
    if(button_no == button_pressed) {     /* No shot */
      _logic_curse();
    } else {                              /* Shot */
      logic_shot();
    }
  }
} /* _logic_shot_or_not */


/***************************************************************************************************/
void logic_measurement(void)
{
  /* Display the measurement message */
  lcd_measurement_start();

  /* Determine alc level */
  int sensor_val_ref = sensor_measure();
  int sensor_val_highest = 0;
  for(int seconds_left = 5; seconds_left > 0; seconds_left--) {
    lcd_measurement_update(seconds_left);                       /* Update the screen with seconds left */
    button_t button_pressed = button_wait_for_any_timed(1000);  /* Wait 1s for user cancelation and time the measurement */
    if(button_no == button_pressed) {                           /* User canceled the measurement */
      _logic_curse();
      return;
    }
    int sensor_val_new = sensor_measure();
    if(sensor_val_highest < sensor_val_new) {
      sensor_val_highest = sensor_val_new;
    }
  }

  sensor_val_highest = (sensor_val_highest < sensor_val_ref) ? 0 : sensor_val_highest - sensor_val_ref;
  _logic_shot_or_not(sensor_val_highest);
} /* logic_measurement */


/***************************************************************************************************/
void logic_calibration(void)
{
  static bool calibration_enabled = true;

  if(!calibration_enabled) {
    return;
  }

  int sensor_val = sensor_measure();

  /* Sensor already calibrated */
  if(millis() > WARM_UP_TIME && sensor_val < SENSOR_THRESHOLD) {
    return;
  }

  /* Start sensor calibration routine */
  lcd_calibration_setup();
  while(millis() < WARM_UP_TIME || sensor_val > SENSOR_THRESHOLD) {
    lcd_calibration_update(sensor_val);
    button_t button_pressed = button_wait_for_any_timed(CALIBRATION_INTERVAL);
    if(button_yes == button_pressed) {  /* Warmup shot */
      logic_shot();
    }
    if(button_no == button_pressed) {   /* User disabled calibration */
      calibration_enabled = false;
      return;
    }
    sensor_val = sensor_measure();
  }
} /* logic_calibration */
