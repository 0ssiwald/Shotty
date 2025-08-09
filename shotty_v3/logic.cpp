#include "logic.h"

/* Strings */
const char * selection[] = {"Shot", "Measure", "Akinator", "Timer Game", "Select Alc"};
const char * alc_types[] = {"Mystery", "Saure Kirsche", "Jaegermeister", "Pfeffi", "Johannisbeere", "Apfel", "Waldbeere", "Vodka", "Pflaume", "Tequila", "Wurstwasser"};
const char * cheers[] = {"Cheers", "You da best!", "Love you <3", "Goooo!!", "Yummy", "Delicious!", "Prost", "Chin chin", "Salut", "Prosit", "Cheerio", "Good choice!", "Tasty", "Lets gooo", "Sweet"};
const char * curses[] = {"Pussy", "Noob", "Little baby", "Boring!!!", "Lame!!!", "Chicken", "Meh!", "Not cool", "Weakling", "Loser", "Fuck off", "Go away!", "Easter egg ;)", "Stupid", "Lightweight",
                         "Dick", "Cunt", "Cock", "Buhh!", "Unfortunate", "Wimp", "Softy", "Shame", "U sure?", "Doofian!", "Kurva", "Fils de pute", "Cyka blyat", "Bitsh whyy?", "Whack!!", "Dumb",
                         "Foolish", "Dull", "Sad :(", "Stupid"};

size_t alc_selected = 0;   /* Index of alc_types array. Stores the user selected alc type */


/***************************************************************************************************/
void logic_init(void)
{
  randomSeed(analogRead(RNG_PIN));
} /* logic_init */


/***************************************************************************************************/
void logic_alc_selection(void)
{
  static bool first_selection = true;
  const size_t alc_types_len = sizeof(alc_types)/sizeof(*alc_types);

  lcd_clear();
  if(first_selection) {
    first_selection = false;
    lcd_print_centered_string(0, F("Lets gooo!!!"));
    lcd_print_centered_string(1, F("Choose an alc type"));
    lcd_print_val(3, 0, "%s", "Y-Okay        Skip-N");

    button_t button_pressed = button_wait(button_any);
    if(button_no == button_pressed) { /* User does not want to select an alcohol */
      return;
    }
  }

  /* User wants to select an alcohol */
  lcd_clear();
  lcd_print_val(0, 0, "%s", "-> ");
  lcd_print_val(3, 0, "Y-Select      Next-N");

  while(true) {
    lcd_print_val(0, 3, "%s", alc_types[alc_selected]);      /* Print currently selected alc */
    size_t display_ix = alc_selected;
    for(int i = 0; i < 2; i++) {
      if(++display_ix >= alc_types_len) { display_ix = 0; }
      lcd_print_val(i + 1, 3, "%s", alc_types[display_ix]);
    }  
    button_t button_pressed = button_wait(button_any);      /* Ask the user if they want this alc or to see the next option */
    if(button_yes == button_pressed) {                      /* User wants the currently selected alc */
      lcd_clear();
      return;
    } else {                                                /* User wants to see the next alc */
      if(++alc_selected >= alc_types_len) { alc_selected = 0; }
    }
    for(uint8_t i = 0; i < 3; i++) {                        /* Clear previous alc selection */
      lcd_clear_pos(i, 3, LCD_WIDTH - 3);
    }
  }
} /* logic_alc_selection */


/***************************************************************************************************/
/* Returns whether or not the shot was canceled */
bool _logic_pour_shot(void)
{
  pump_start();
  button_t button_pressed = button_wait_timed(button_no, PUMP_SHOT_TIME);  /* Check for user cancelation and time the shot */
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
void _logic_shot(void)
{
  /* Screen setup for shot */
  lcd_clear();
  size_t ix = random(sizeof(cheers)/sizeof(*cheers));
  lcd_print_centered_string(0, cheers[ix]);
  lcd_print_centered_string(1, F("Shot glass in place?"));
  lcd_print_val(2, 5, "Shot in ");  /* Not centered so we can print the countdown next to it */
  lcd_print_centered_string(3, F("Press N to cancel"));

  /* Countdown to first shot */
  for(uint8_t seconds_left = SHOT_CANCEL_TIME; seconds_left > 0; seconds_left--) {
    lcd_print_val(2, 13, "%hhu", seconds_left);                   /* Update LCD with seconds left */
    button_t button_pressed = button_wait_timed(button_no, 1000); /* Wait 1s for user input */
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
  lcd_print_centered_string(1, F("Double Shot? :)"));
  lcd_print_centered_string(2, F("Y/N"));
  button_t button_pressed = button_wait(button_any);
  if(button_no == button_pressed) {   /* No double shot */
    _logic_curse();
    return;
  }

  /* Pour double shot */
  lcd_print_centered_string(3, cheers[ix]);
  (void)_logic_pour_shot();   /* Pour shot without checking for cancelation */
} /* _logic_shot */


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
    case 0   ... 25                : lcd_print_centered_string(1, F("No alcohol deteced")); lcd_print_centered_string(2, F("A shot will help!")); break;
    case 26  ... 250               : lcd_print_centered_string(1, F("Meh, pretty sober!")); lcd_print_centered_string(2, F("You need a drink!")); break;
    case 251 ... 500               : lcd_print_centered_string(1, F("A little drunk"));     lcd_print_centered_string(2, F("A shot is needed!")); break;
    case 501 ... NO_SHOT_LIMIT     : lcd_print_centered_string(1, F("Good level!!!"));      lcd_print_centered_string(2, F("Keep it up!"));       break;
    case NO_SHOT_LIMIT + 1 ... 1100: lcd_print_centered_string(1, F("You are wasted!!!"));  lcd_print_centered_string(2, F("You had enough!")); too_drunk = true; break;
    default: break;
  }

  if(too_drunk) {
    lcd_print_centered_string(3, F("Press N for no shot"));
  } else {
    lcd_print_centered_string(3, F("Press Y for a shot!"));
  }

  button_t button_pressed = button_wait(button_any);
  lcd_clear();
  if(too_drunk) {                         /* No more shots for the user */
    if(button_no == button_pressed) {     /* Pressed N like a good boy */
      lcd_print_centered_string(1, F("Good Boy!"));
    } else {                              /* Pressed Y like a bad boy */
      lcd_print_centered_string(1, F("Bad Boy!"));
      delay(LCD_CURSE_TIME);
    } 
  } else {                                /* User may have a shot */
    if(button_no == button_pressed) {     /* No shot */
      _logic_curse();
    } else {                              /* Shot */
      _logic_shot();
    }
  }
} /* _logic_shot_or_not */



/***************************************************************************************************/
void _logic_measurement(void)
{
  /* Display the measurement message */
  lcd_clear();
  lcd_print_centered_string(0, F("Starting measurement"));
  lcd_print_centered_string(1, F("Blow me for 5s"));
  delay(MEASUREMENT_DELAY);

  /* Determine alc level */
  int16_t sensor_val_ref = sensor_measure();
  int16_t sensor_val_highest = 0;
  for(uint8_t seconds_left = 5; seconds_left > 0; seconds_left--) {
    lcd_print_val(3, 9, "%hhu", seconds_left);                    /* Update the screen with seconds left */
    button_t button_pressed = button_wait_timed(button_no, 1000); /* Wait 1s for user cancelation and time the measurement */
    if(button_no == button_pressed) {                             /* User canceled the measurement */
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
} /* _logic_measurement */


/***************************************************************************************************/
void logic_program_start(void)
{
  const size_t selection_len = sizeof(selection) / sizeof(*selection);
  size_t selection_ix = 0;

  char alc_str[20] = "Alc: ";
  strncat(alc_str, alc_types[alc_selected], 15);

  lcd_clear();
  lcd_print_centered_string(0, alc_str);
  lcd_print_val(1, 0, "%s", "->");
  lcd_print_val(1, 3, "%s", selection[selection_ix]);
  lcd_print_val(2, 3, "%s", selection[selection_ix + 1]);
  lcd_print_val(3, 3, "%s", selection[selection_ix + 2]);
  lcd_print_centered_string(3, F("Y-Select      Next-N"));

  while(true) {
    button_t button_pressed = button_wait(button_any);
    if(button_yes == button_pressed) {
      break;
    }
    lcd_clear_pos(1, 3, LCD_WIDTH - 3);   /* Clear previous */
    lcd_clear_pos(2, 3, LCD_WIDTH - 3);   /* selection      */
    if(++selection_ix >= selection_len) { selection_ix = 0; } /* Increment selection index */
    lcd_print_val(1, 3, "%s", selection[selection_ix]);       /* Display the first next option */
    size_t display_ix = selection_ix + 1;
    if(display_ix >= selection_len) { display_ix = 0; }
    lcd_print_val(2, 3, "%s", selection[display_ix]);         /* Display the second next option */
  }

  bool shot = false;
  switch(selection_ix) {
    case 0: _logic_shot(); break;
    case 1: _logic_measurement(); break;
    case 2: games_akinator(); break;
    case 3: shot = games_timer(); break;
    case 4: logic_alc_selection(); break;
    default: break;
  }
  if(shot) {
    _logic_shot();
  }
} /* logic_program_start */


/***************************************************************************************************/
void _logic_calibration_text(void)
{
  lcd_clear();
  lcd_print_centered_string(0, F("Sensor calibrating"));
  lcd_print_val(1, 0, "Current value: ");
  lcd_print_centered_string(2, F("Y for a warmup shot"));
  lcd_print_centered_string(3, F("N to disable calibr."));
} /* _logic_calibration_text */


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
  _logic_calibration_text();

  /* Start sensor calibration routine */
  while(millis() < WARM_UP_TIME || sensor_val > SENSOR_THRESHOLD) {
    lcd_clear_pos(1, 15, 4);
    lcd_print_val(1, 15, "%d", sensor_val);
    button_t button_pressed = button_wait_timed(button_any, CALIBRATION_INTERVAL);
    if(button_yes == button_pressed) {  /* Warmup shot */
      _logic_shot();
      _logic_calibration_text();
    }
    if(button_no == button_pressed) {   /* User disabled calibration */
      calibration_enabled = false;
      return;
    }
    sensor_val = sensor_measure();
  }
} /* logic_calibration */
