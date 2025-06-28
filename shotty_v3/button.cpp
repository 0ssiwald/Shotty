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
button_t button_wait_for_any(void)
{
  while(true) {
    if(!digitalRead(BUTTON_YES)) {        /* "Yes" button pressed */
      digitalWrite(LED_YES, HIGH);
      while(!digitalRead(BUTTON_YES)) {   /* Wait until user releases the button */
        delay(100);
      }
      digitalWrite(LED_YES, LOW);
      return button_yes;
    }
    if(!digitalRead(BUTTON_NO)) {         /* "No" button pressed */
      digitalWrite(LED_NO, HIGH);
      while(!digitalRead(BUTTON_NO)) {    /* Wait until user releases the button */
        delay(100);
      }
      digitalWrite(LED_NO, LOW);
      return button_no;
    }
    delay(BUTTON_CHECK_DELAY);
  }
} /* button_wait_for_any */


/***************************************************************************************************/
/* Checks for user input for the given time interval. Expects delay in milliseconds. */
button_t button_wait_for_any_timed(unsigned long delay_val)
{
  unsigned long start_time = millis();
  while(millis() < (start_time + delay_val)) {
    if(!digitalRead(BUTTON_YES)) {        /* "Yes" button pressed */
      digitalWrite(LED_YES, HIGH);
      while(!digitalRead(BUTTON_YES)) {   /* Wait until user releases the button */
        delay(100);
      }
      digitalWrite(LED_YES, LOW);
      return button_yes;
    }
    if(!digitalRead(BUTTON_NO)) {         /* "No" button pressed */
      digitalWrite(LED_NO, HIGH);
      while(!digitalRead(BUTTON_NO)) {    /* Wait until user releases the button */
        delay(100);
      }
      digitalWrite(LED_NO, LOW);
      return button_no;
    }
    delay(BUTTON_CHECK_DELAY);
  }
  return button_none;
} /* button_wait_for_any_timed */
