#include "logic.h"

/* Strings */
const char * alc_types[] = {"Saure Kirsche", "Jaegermeister", "Pfeffi", "Johannisbeere", "Apfel", "Waldbeere", "Vodka", "Pflaume", "Tequila", "Wurstwasser"};
const char * cheers[] = {"Cheers", "You da best!", "Love you <3", "Goooo!!", "Yummy", "Delicious!", "Prost", "Chin chin", "Salut", "Prosit", "Cheerio", "Good choice!", "Tasty", "Lets gooo", "Sweet"};
const char * curses[] = {"Pussy", "Noob", "Little baby", "Boring!!!", "Lame!!!", "Chicken", "Meh!", "Not cool", "Weakling", "Loser", "Fuck off", "Go away!", "Easter egg ;)", "Stupid", "Lightweight",
                         "Dick", "Cunt", "Cock", "Buhh!", "Unfortunate", "Wimp", "Softy", "Shame", "U sure?", "Doofian!", "Kurva", "Fils de pute", "Cyka blyat", "Bitsh whyy?", "Whack!!", "Dumb",
                         "Foolish", "Dull", "Sad :(", "Stupid"};

int32_t alc_selected = -1;   /* Index of alc_types array. Stores the user selected alc type */


/***************************************************************************************************/
void logic_init(void)
{
  randomSeed(analogRead(RNG_PIN));
} /* logic_init */


/***************************************************************************************************/
void logic_alc_selection(void)
{
  lcd_print_centered_string(0, "Lets gooo!!!");
  lcd_print_centered_string(1, "Press Y to choose");
  lcd_print_centered_string(2, "an alcohol type.");
  lcd_print_centered_string(3, "Press N to skip");

  button_t button_pressed = button_wait_for_any();
  if(button_no == button_pressed) { /* User does not want to select an alcohol */
    lcd_clear();
    return;
  }

  /* User wants to select an alcohol */
  size_t alc_len = sizeof(alc_types)/sizeof(*alc_types);
  alc_selected = 0;

  lcd_clear();
  lcd_print_centered_string(2, "Press Y to select");
  lcd_print_centered_string(3, "Press N to change");

  while(true) {
    lcd_print_centered_string(0, alc_types[alc_selected]);  /* Print currently selected alc */  
    button_pressed = button_wait_for_any();                 /* Ask the user if they want this alc or to see the next option */
    if(button_yes == button_pressed) {                      /* User wants the currently selected alc */
      lcd_clear();
      return;
    } else {                                                /* User wants to see the next alc */
      if(++alc_selected == alc_len) {
        alc_selected = 0;
      }
    }
    lcd_clear_pos(0, 0, LCD_WIDTH);                         /* Clear previous alc from LCD */
  }
} /* logic_alc_selection */


/***************************************************************************************************/
void logic_program_start(void)
{
  lcd_clear();
  if(-1 == alc_selected) {
    lcd_print_centered_string(0, "Shotty is ready");
  } else {
    lcd_print_centered_string(0, "Shoty is ready with");
    lcd_print_centered_string(1, alc_types[alc_selected]);
  }

  lcd_print_centered_string(2, "Press Y for shot");
  lcd_print_centered_string(3, "Press N to measure");
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
void _logic_curse(void)
{
  digitalWrite(LED_NO, HIGH);
  lcd_clear();
  size_t ix = random(sizeof(curses)/sizeof(*curses));
  lcd_print_centered_string(1, curses[ix]);
  delay(LCD_CURSE_TIME);
  digitalWrite(LED_NO, LOW);
} /* _logic_curse */


/***************************************************************************************************/
void logic_shot(void)
{
  /* Screen setup for shot */
  lcd_clear();
  size_t ix = random(sizeof(cheers)/sizeof(*cheers));
  lcd_print_centered_string(0, cheers[ix]);
  lcd_print_centered_string(1, "Shot glass in place?");
  lcd_print_val(2, 5, "Shot in ");  /* Not centered so we can print the countdown next to it */
  lcd_print_centered_string(3, "Press N to cancel");

  /* Countdown to first shot */
  for(uint8_t seconds_left = SHOT_CANCEL_TIME; seconds_left > 0; seconds_left--) {
    lcd_print_val(2, 13, "%hhu", seconds_left);                   /* Update LCD with seconds left */
    button_t button_pressed = button_wait_for_any_timed(1000);    /* Wait 1s for user input */
    if(button_no == button_pressed) {                             /* Shot canceled */
      _logic_curse();
      return;
    }
  }
  lcd_print_val(2, 13, "%hhu", 0);  /* Show a 0 on the countdown */

  /* Pour the shot and check for cancelation */
  if(_logic_pour_shot()) {    
    _logic_curse();     /* Shot canceled */
    return;
  }

  /* Ask for double shot */
  lcd_clear();
  lcd_print_centered_string(0, "Double Shot? :)");
  lcd_print_centered_string(1, "Y/N");
  button_t button_pressed = button_wait_for_any();
  if(button_no == button_pressed) {   /* No double shot */
    _logic_curse();
    return;
  }

  /* Pour double shot */
  lcd_print_centered_string(3, cheers[ix]);
  (void)_logic_pour_shot();   /* Pour shot without checking for cancelation */
} /* logic_shot */


/***************************************************************************************************/
void _logic_shot_or_not(int16_t sensor_val)
{
  /* Print BAC */
  float display_val = (sensor_val < 25) ? 0.0 : (float)sensor_val / 500.0;
  lcd_clear();
  lcd_print_val(0, 3, "Alcohol:");
  lcd_print_float(0, 12, display_val);
  lcd_print_val(0, 16, "%s", "%.");

  /* Determine drunkenness */
  bool too_drunk = false;
  switch(sensor_val) {
    case 0   ... 25                : lcd_print_centered_string(1, "No alcohol deteced"); lcd_print_centered_string(2, "A shot will help!"); break;
    case 26  ... 250               : lcd_print_centered_string(1, "Meh, pretty sober!"); lcd_print_centered_string(2, "You need a drink!"); break;
    case 251 ... 500               : lcd_print_centered_string(1, "A little drunk");     lcd_print_centered_string(2, "A shot is needed!"); break;
    case 501 ... NO_SHOT_LIMIT     : lcd_print_centered_string(1, "Good level!!!");      lcd_print_centered_string(2, "Keep it up!");       break;
    case NO_SHOT_LIMIT + 1 ... 1100: lcd_print_centered_string(1, "You are wasted!!!");  lcd_print_centered_string(2, "You had enough!"); too_drunk = true; break;
    default: break;
  }

  if(too_drunk) {
    lcd_print_centered_string(3, "Press N for no shot");
  } else {
    lcd_print_centered_string(3, "Press Y for a shot!");
  }

  button_t button_pressed = button_wait_for_any();
  lcd_clear();
  if(too_drunk) {                         /* No more shots for the user */
    if(button_no == button_pressed) {     /* Pressed N like a good boy */
      lcd_print_centered_string(1, "Good Boy!");
    } else {                              /* Pressed Y like a bad boy */
      lcd_print_centered_string(1, "Bad Boy!");
      delay(LCD_CURSE_TIME);
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
  lcd_clear();
  lcd_print_centered_string(0, "Starting measurement");
  lcd_print_centered_string(1, "Blow me for 5s");
  delay(MEASUREMENT_DELAY);

  /* Determine alc level */
  int16_t sensor_val_ref = sensor_measure();
  int16_t sensor_val_highest = 0;
  for(uint8_t seconds_left = 5; seconds_left > 0; seconds_left--) {
    lcd_print_val(3, 9, "%hhu", seconds_left);                  /* Update the screen with seconds left */
    button_t button_pressed = button_wait_for_any_timed(1000);  /* Wait 1s for user cancelation and time the measurement */
    if(button_no == button_pressed) {                           /* User canceled the measurement */
      _logic_curse();
      return;
    }
    int16_t sensor_val_new = sensor_measure();
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

  int16_t sensor_val = sensor_measure();

  /* Sensor already calibrated */
  if(millis() > WARM_UP_TIME && sensor_val < SENSOR_THRESHOLD) {
    return;
  }

  /* Display calibration text */
  lcd_clear();
  lcd_print_centered_string(0, "Sensor calibrating");
  lcd_print_val(1, 0, "Current value: ");
  lcd_print_centered_string(2, "Y for a warmup shot");
  lcd_print_centered_string(3, "N to disable calibr.");

  /* Start sensor calibration routine */
  while(millis() < WARM_UP_TIME || sensor_val > SENSOR_THRESHOLD) {
    lcd_clear_pos(1, 15, 4);
    lcd_print_val(1, 15, "%d", sensor_val);
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
