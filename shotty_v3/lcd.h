#ifndef _LCD_H_
#define _LCD_H_

#include "defines.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

typedef enum lcd_alc_level{
  alc_level_none  = 0,
  alc_level_one   = 1,
  alc_level_two   = 2,
  alc_level_three = 3,
  alc_level_max   = 4,
} lcd_alc_level_t;

static const char * cheers[] = {"Cheers", "You da best!", "Love you <3", "Goooo!!", "Yummy", "Delicious!", "Prost", "Chin chin", "Salut", "Prosit", "Cheerio", "Good choice!", "Tasty", "Lets gooo", "Sweet"};
static const char * curses[] = {"Pussy", "Noob", "Little baby", "Boring!!!", "Lame!!!", "Chicken", "Meh!", "Not cool", "Weakling", "Loser", "Fuck off", "Go away!", "Easter egg ;)", "Stupid", "Lightweight",
                                "Dick", "Cunt", "Cock", "Buhh!", "Unfortunate", "Wimp", "Softy", "Shame", "U sure?", "Doofian!", "Kurva", "Fils de pute", "Cyka blyat", "Bitsh whyy?", "Whack!!", "Dumb",
                                "Foolish", "Dull", "Lel", "Sad :(", "Stupid"};

void lcd_init(void);
void lcd_clear(void);
void lcd_alc_selection_start(void);
void lcd_alc_selection(void);
void lcd_centered_string(char *, unsigned short);
void lcd_cheer(void);
void lcd_curse(void);
void lcd_ready_message(char *);
void lcd_shot_countdown_setup(void);
void lcd_shot_countdown_update(unsigned short);
void lcd_double_shot(void);
void lcd_measurement_start(void);
void lcd_measurement_update(int);
void lcd_alc_percentage(float);
void lcd_alc_level(lcd_alc_level_t);
void lcd_good_boy(bool);
void lcd_calibration_setup(void);
void lcd_calibration_update(int);

#endif /* _LCD_H_ */