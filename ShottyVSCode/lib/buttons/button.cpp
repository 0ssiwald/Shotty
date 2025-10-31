#include "button.h"

/***************************************************************************************************/
void button_init(void)
{
  pinMode(BUTTON_YES, INPUT_PULLUP);
  pinMode(BUTTON_NO, INPUT_PULLUP);
  pinMode(LED_YES, OUTPUT);             /* This should probably happen somewhere else, but whatever */
  pinMode(LED_NO, OUTPUT);              /* Same here */
  pinMode(RELAIS_PIN, OUTPUT);
} /* button_init */


/***************************************************************************************************/
/* Checks for user input and waits until a button is pressed. */
button_t button_wait(button_t button_wanted)
{
  while(true) {
    int pressed_yes = !digitalRead(BUTTON_YES);
    if(pressed_yes && (button_no != button_wanted)) { /* "Yes" button pressed */
      digitalWrite(LED_YES, HIGH);
      while(!digitalRead(BUTTON_YES)) {   /* Wait until user releases the button */
        delay(BUTTON_CHECK_DELAY);
      }
      digitalWrite(LED_YES, LOW);
      return button_yes;
    }
    int pressed_no = !digitalRead(BUTTON_NO);
    if(pressed_no && (button_yes != button_wanted)) {         /* "No" button pressed */
      digitalWrite(LED_NO, HIGH);
      while(!digitalRead(BUTTON_NO)) {    /* Wait until user releases the button */
        delay(BUTTON_CHECK_DELAY);
      }
      digitalWrite(LED_NO, LOW);
      return button_no;
    }
    delay(BUTTON_CHECK_DELAY);
  }
} /* button_wait_for_any */


/***************************************************************************************************/
/* Checks for user input for the given time interval. Expects delay in milliseconds. */
button_t button_wait_timed(button_t button_wanted, unsigned long delay_val)
{
  unsigned long start_time = millis();
  while(millis() < (start_time + delay_val)) {
    int pressed_yes = !digitalRead(BUTTON_YES);
    if(pressed_yes && (button_no != button_wanted)) { /* "Yes" button pressed */
      digitalWrite(LED_YES, HIGH);
      while(!digitalRead(BUTTON_YES)) {   /* Wait until user releases the button */
        delay(BUTTON_CHECK_DELAY);
      }
      digitalWrite(LED_YES, LOW);
      return button_yes;
    }
    int pressed_no = !digitalRead(BUTTON_NO);
    if(pressed_no && (button_yes != button_wanted)) { /* "No" button pressed */
      digitalWrite(LED_NO, HIGH);
      while(!digitalRead(BUTTON_NO)) {    /* Wait until user releases the button */
        delay(BUTTON_CHECK_DELAY);
      }
      digitalWrite(LED_NO, LOW);
      return button_no;
    }
    delay(BUTTON_CHECK_DELAY);
  }
  return button_none;
} /* button_wait_for_any_timed */
