#include "lcd.h"

LiquidCrystal_I2C lcd(0x27,20,4);


/***************************************************************************************************/
void lcd_init(void)
{
  //lcd.begin();  TODO: Falsche library ?
  lcd.init();
  lcd.backlight();
} /* lcd_init */


/***************************************************************************************************/
void lcd_clear(void)
{
  lcd.clear();
  lcd.setCursor(0,0);
} /* lcd_clear() */


/***************************************************************************************************/
void lcd_alc_selection_start(void)
{
  lcd.setCursor(0,0);
  lcd.print("    Lets gooo!!!");
  lcd.setCursor(0,2);
  lcd.print(" Press Y to choose");
  lcd.setCursor(0,3);
  lcd.print("  an alcohol type");
} /* lcd_alc_selection_start */


/***************************************************************************************************/
void lcd_alc_selection(void)
{
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print(" Press N to change");
  lcd.setCursor(0,3);
  lcd.print(" Press Y to select");
} /* lcd_alc_selection*/


/***************************************************************************************************/
void lcd_centered_string(char * str, unsigned short row)
{
  size_t len = strlen(str);
  unsigned int mid_pos = (10 - len / 2) - (len % 2);
  lcd.setCursor(mid_pos, row);
  lcd.print(str);
} /* lcd_centered_string */


/***************************************************************************************************/
void lcd_cheer(void)
{
  char * cheer_str = random(sizeof(cheers)/sizeof(*cheers));
  lcd_centered_string(cheer_str, 0);
} /* lcd_cheer */


/***************************************************************************************************/
void lcd_curse(void)
{
  lcd.clear();
  char * curse_str = random(sizeof(curses)/sizeof(*curses));
  lcd_centered_string(curse_str, 2);
  delay(LCD_CURSE_TIME);
} /* lcd_curse */


/***************************************************************************************************/
void lcd_ready_message(char * alc_str)
{
  lcd.clear();
  lcd.setCursor(0,0);
  if('\0' == alc_str[0]) {
    lcd.print("  Shotty is ready");
  } else {
    lcd.print("Shotty is ready with");
    lcd_centered_string(alc_str, 1);
  }
  lcd.setCursor(2,2);
  lcd.print("Press Y for shot");
  lcd.setCursor(1,3);
  lcd.print("Press N to measure");
} /* lcd_ready_message */


/***************************************************************************************************/
void lcd_shot_countdown_setup(void)
{
  lcd.setCursor(0,1);
  lcd.print("Shot glass in place?");
  lcd.setCursor(5,2);
  lcd.print("Shot in");
  lcd.setCursor(1,3);
  lcd.print("Press N to cancel");
} /* lcd_shot_countdown_setup */


/***************************************************************************************************/
void lcd_shot_countdown_update(unsigned short time)
{
  lcd.setCursor(13,1);
  lcd.print("    ");
  lcd.setCursor(13,1);
  lcd.print(time);
} /* lcd_shot_countdown_update */


/***************************************************************************************************/
void lcd_double_shot(void)
{
  lcd.clear();
  lcd.setCursor(4,0); 
  lcd.print("Double Shot?");
  lcd.setCursor(9,1); 
  lcd.print(";)");
  lcd.setCursor(8,3);
  lcd.print("Y/N");
} /* lcd_double_shot */


/***************************************************************************************************/
void lcd_measurement_start(void)
{
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Starting measurement");
  lcd.setCursor(2,2); 
  lcd.print("Blow me for 5s");
  lcd.setCursor(5,3);           /* Already set cursor to position to print the countdown */
} /* lcd_measurement_start */


/***************************************************************************************************/
void lcd_measurement_update(int time)
{
  lcd.print(time);
  lcd.print(" ");
} /* lcd_measurement_update */


/***************************************************************************************************/
void lcd_alc_percentage(float val)
{
  lcd.setCursor(0,0);
  lcd.print("Alcohol: ");
  lcd.print(val);
  lcd.print(" %.");
} /* lcd_alc_percentage */


/***************************************************************************************************/
void lcd_alc_level(lcd_alc_level_t alc_level)
{
  lcd.setCursor(1,1);
  switch(alc_level) {
    case alc_level_none : lcd.print(" No alcohol deteced"); lcd.setCursor(1,2); lcd.print("A shot will help!"); break;
    case alc_level_one  : lcd.print(" Meh, pretty sober!"); lcd.setCursor(1,2); lcd.print("You need a drink!"); break;
    case alc_level_two  : lcd.print("   A little drunk");   lcd.setCursor(1,2); lcd.print("A shot is needed!"); break;
    case alc_level_three: lcd.print("   Good level!!!");    lcd.setCursor(4,2); lcd.print("Keep it up!");       break;
    case alc_level_max  : lcd.print(" You are wasted!!!");  lcd.setCursor(2,2); lcd.print("You had enough!");   break;
    default             : lcd.print("Something went wrong"); break;
  }
  if(alc_level_max == alc_level) {
    lcd.setCursor(0,3);
    lcd.print("Press N for no shot");
  } else {
    lcd.setCursor(0,3);
    lcd.print("Press Y for a shot!");
  }
} /* lcd_alc_level */


/***************************************************************************************************/
void lcd_good_boy(bool is_good)
{
  lcd.clear();
  lcd.setCursor(5,0);
  if(is_good)
    lcd.print("Good boy!"); 
  else
    lcd.print("Bad boy!");
  delay(LCD_CURSE_TIME);
} /* lcd_is_good_boy */


/***************************************************************************************************/
void lcd_calibration_setup(void)
{
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Sensor calibrating");
  lcd.setCursor(1,1);
  lcd.print("Current value: ");
  lcd.setCursor(0,2);
  lcd.print("Y for a warmup shot");
  lcd.setCursor(0,3);
  lcd.print("N to disable calibr.");
} /* lcd_calibration_setup */


/***************************************************************************************************/
void lcd_calibration_update(int val)
{
  lcd.setCursor(16, 1);
  lcd.print("    ");
  lcd.setCursor(16, 1);
  lcd.print(val);
} /* lcd_calibration_update */
