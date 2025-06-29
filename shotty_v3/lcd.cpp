#include "lcd.h"

LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);


/***************************************************************************************************/
void lcd_init(void)
{
  lcd.init();
  lcd.backlight();
} /* lcd_init */


/***************************************************************************************************/
void lcd_clear(void)
{
  lcd.clear();
} /* lcd_clear() */


/***************************************************************************************************/
void lcd_clear_pos(uint8_t row, uint8_t col, size_t len)
{
  lcd.setCursor(col, row);
  for(size_t i = 0; i < len; i++) {
    lcd.print(" ");
  }
} /* lcd_clear_pos */


/***************************************************************************************************/
void lcd_print_centered_string(uint8_t row, const char * str)
{
  size_t len = strlen(str);
  uint8_t mid_pos = (LCD_WIDTH / 2 - len / 2) - (len % 2);
  lcd.setCursor(mid_pos, row);
  lcd.print(str);
} /* lcd_centered_string */


/***************************************************************************************************/
void lcd_print_val(uint8_t row, uint8_t col, const char * fmt, ...)
{
  char buffer[LCD_WIDTH + 1] = "";     /* +1 for '\0' */
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);
  lcd.setCursor(col, row);
  lcd.print(buffer);
} /* lcd_print_val */


/***************************************************************************************************/
void lcd_print_float(uint8_t row, uint8_t col, float val)
{
  lcd.setCursor(col, row);
  lcd.print(val);
} /* lcd_print_float */
