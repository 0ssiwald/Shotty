#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "defines.h"

typedef enum button {
  button_none = 0,
  button_yes  = 1,
  button_no   = 2,
} button_t;

void button_init(void);
button_t button_wait_for_any(void);
button_t button_wait_for_any_timed(unsigned long);

#endif /* _BUTTON_H_ */