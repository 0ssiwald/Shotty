#ifndef _LCD_H_
#define _LCD_H_

#include "defines.h"
#include <stdarg.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

typedef enum lcd_alc_level{
  alc_level_none  = 0,
  alc_level_one   = 1,
  alc_level_two   = 2,
  alc_level_three = 3,
  alc_level_max   = 4,
} lcd_alc_level_t;

void lcd_init(void);
void lcd_clear(void);
void lcd_clear_pos(uint8_t row, uint8_t col, size_t len);
void lcd_print_centered_string(uint8_t row, const char * str);

/* Can be used like printf. Example: lcd_print_val(1, 3, "%s %d", str, int); */
void lcd_print_val(uint8_t row, uint8_t col, const char * fmt, ...)  __attribute__((format(printf, 3, 4)));

/* ChatGPT said, that printf (used in lcd_print_val) doesn't support floats on Arduino Nano. Use this function instead */
void lcd_print_float(uint8_t row, uint8_t col, float val);

#endif /* _LCD_H_ */