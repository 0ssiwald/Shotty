#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "defines.h"

typedef enum button {
  button_none = 0,
  button_any  = 1,
  button_yes  = 2,
  button_no   = 3,
} button_t;

void button_init(void);
button_t button_wait(button_t);
button_t button_wait_timed(button_t, unsigned long);

#endif /* _BUTTON_H_ */