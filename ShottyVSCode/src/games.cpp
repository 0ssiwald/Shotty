#include "games.h"

typedef uint32_t possible_people_t;
typedef struct question {
  const char * line_one;
  const char * line_two;
} question_t;

#define NUM_PEOPLE    14
#define NUM_QUESTIONS 7

typedef struct person {
  const char * name;
  union {
    struct {
      bool male     : 1;
      bool glasses  : 1;
      bool bachelor : 1;
      bool weed     : 1;
      bool uni      : 1;
      bool year     : 1;
      bool flat     : 1;
    };
    uint8_t answers;
  };
} person_t;

const person_t people[] = {/* name    , male, glasses, bachelor, weed, uni, 98er, flat */
                           { "Hermine",  0  ,    0   ,    0    ,  0  ,  0 ,  1  ,  0  },
                           { "Paul"   ,  1  ,    0   ,    1    ,  1  ,  0 ,  1  ,  1  },
                           { "Lino"   ,  1  ,    1   ,    1    ,  1  ,  0 ,  1  ,  1  },
                           { "Hannah" ,  0  ,    0   ,    1    ,  0  ,  1 ,  1  ,  1  },
                           { "Freddi" ,  1  ,    1   ,    0    ,  0  ,  1 ,  0  ,  0  },
                           { "Ossi"   ,  1  ,    0   ,    1    ,  1  ,  1 ,  1  ,  1  },  
                           { "Cheru"  ,  1  ,    1   ,    0    ,  1  ,  0 ,  1  ,  0  },
                           { "Hanna"  ,  0  ,    1   ,    1    ,  0  ,  1 ,  1  ,  1  },
                           { "Chrissi",  1  ,    1   ,    0    ,  0  ,  0 ,  0  ,  0  },
                           { "Passi"  ,  1  ,    1   ,    0    ,  1  ,  0 ,  0  ,  0  },
                           { "Anton"  ,  1  ,    0   ,    1    ,  0  ,  1 ,  1  ,  0  },
                           { "Dilara" ,  0  ,    0   ,    1    ,  0  ,  0 ,  0  ,  1  },
                           { "Enya"   ,  0  ,    0   ,    1    ,  0  ,  0 ,  1  ,  1  },
                           { "Che"    ,  1  ,    0   ,    0    ,  0  ,  1 ,  0  ,  1  }
};

/* BEWARE: Questions have to be in the same order as the answers in person_t */
const question_t questions[] = {{"Ist deine Person"   , "ein Mann?"           },
                                {"Traegt deine Person", "eine Brille?"        },
                                {"Hat deine Person"   , "einen Bachelor?"     },
                                {"Kifft deine Person" , "regelmaessig?"       },
                                {"Studiert deine"     , "Person noch?"        },
                                {"Ist deine Person"   , "98er Jahrgang?"      },
                                {"Wohnt deine Person" , "mit dem Partner zsm?"}};

static_assert(NUM_PEOPLE == (sizeof(people) / sizeof(*people)), "Number of people in array 'people' isnt't the same as NUM_PEOPLE");
static_assert(NUM_QUESTIONS == (sizeof(questions) / sizeof(*questions)), "Number of questions in array 'questions' isn't the same as NUM_QUESTIONS");

/***************************************************************************************************/
void games_akinator()
{
  possible_people_t possible_people = pow(2, NUM_PEOPLE) - 1;   /* To store possible people */

  /* Generate random questions order */
  size_t random_ix[NUM_QUESTIONS];
  for(size_t i = 0; i < NUM_QUESTIONS; i++) {
    random_ix[i] = i;
  }
  for(size_t i = NUM_QUESTIONS - 1; i > 0; i--) {
    size_t j = random(i + 1);
    size_t tmp = random_ix[i];
    random_ix[i] = random_ix[j];
    random_ix[j] = tmp;
  } 

  for(size_t i = 0; i < NUM_QUESTIONS; i++) {
    lcd_clear();
    lcd_print_centered_string(1, questions[random_ix[i]].line_one);
    lcd_print_centered_string(2, questions[random_ix[i]].line_two);
    uint16_t answer_mask = 1 << random_ix[i];
    button_t button_pressed = button_wait(button_any);
    for(size_t person_ix = 0; person_ix < NUM_PEOPLE; person_ix++) {
      if((button_yes == button_pressed) && (0 == (people[person_ix].answers & answer_mask))) {  /* Player answered yes, but the answer for the checked person is no */
        possible_people &= ~(1 << person_ix);                                                   /* Remove person from possible solution */
      } else
      if((button_no == button_pressed) && (0 < (people[person_ix].answers & answer_mask))) {    /* Player answered no, but the answer for the checked person is yes */
        possible_people &= ~(1 << person_ix);                                                   /* Remove person from possible solution */
      }
    }
    if(0 == (possible_people & (possible_people - 1))) {      /* Only one possible person remains */
      lcd_clear();
      if(0 != possible_people) {
        int32_t solution_ix = __builtin_ctz(possible_people); /* Get index of solution */
        lcd_print_centered_string(1, F("Du denkst an..."));
        delay(1000);
        lcd_print_centered_string(2, people[solution_ix].name);
        delay(AKINATOR_SOLUTION_TIME);
        return;
      }
      break;
    }
  }

  /* If there are no more questions but we didn't find the answer */
  lcd_clear();
  lcd_print_centered_string(1, F("I am confusion :("));
  delay(AKINATOR_SOLUTION_TIME);
} /* akinator_start */


bool games_timer()
{
  lcd_clear();
  lcd_print_centered_string(0, F("Druecke nach genau"));
  char lcd_str[20] = "";
  snprintf(lcd_str, 2, "%d", TIMER_TARGET_TIME);
  strncat(lcd_str, " Sekunden", 10);
  lcd_print_centered_string(1, lcd_str);
  lcd_print_centered_string(3, F("00.00 s"));
  delay(1000);

  unsigned long start_time = millis();
  unsigned long current_time = start_time;
  while(current_time <= start_time + TIMER_MAX_TIME) {
    button_t button_pressed = button_wait_timed(button_any, random(5));
    if(button_none != button_pressed) { 
      unsigned long difference = labs(current_time - start_time - TIMER_TARGET_TIME*1000);
      lcd_clear_pos(0, 0, LCD_WIDTH);
      lcd_clear_pos(1, 0, LCD_WIDTH);
      bool shot = false;
      if(difference < 20) {
        shot = true;
        lcd_print_centered_string(0, F("Perfect!"));
        lcd_print_centered_string(1, F("You get a shot :)"));
      }
      else if(difference < 60) {
        lcd_print_centered_string(0, F("Pretty good, but"));
        lcd_print_centered_string(1, F("not quite"));
      }
      else if(difference < 100){
        lcd_print_centered_string(1, F("Meh"));
      } else {
        lcd_print_centered_string(0, F("Were you even"));
        lcd_print_centered_string(1, F("trying?"));
      }
      delay(TIMER_RESULT_TIME);
      return shot;
    }
    current_time = millis();
    unsigned long timer   = current_time - start_time;
    uint8_t       seconds = (uint8_t) (timer / 1000);
    uint16_t      milli   = (uint16_t)(timer % 1000)/10;
    lcd_print_val(3, 6, "%02hhu.%02hu", seconds, milli);
  }

  return false;
}