#include "akinator.h"

typedef uint32_t possible_people_t;
typedef struct question {
  const char * line_one;
  const char * line_two;
} question_t;

#define NUM_PEOPLE    10
#define NUM_QUESTIONS 5

typedef struct person {
  const char * name;
  union {
    struct {
      bool male     : 1;
      bool glasses  : 1;
      bool bachelor : 1;
      bool weed     : 1;
      bool uni      : 1;
    };
    uint8_t answers;
  };
} person_t;

const person_t people[] = {/* name    , male, glasses, bachelor, weed, uni */
                           { "Hermine",  0  ,    0   ,    0    ,  0  ,  0 },
                           { "Paul"   ,  1  ,    0   ,    1    ,  1  ,  0 },
                           { "Lino"   ,  1  ,    1   ,    1    ,  1  ,  0 },
                           { "Hannah" ,  0  ,    0   ,    1    ,  0  ,  1 },
                           { "Freddi" ,  1  ,    1   ,    0    ,  0  ,  0 },
                           { "Ossi"   ,  1  ,    0   ,    1    ,  1  ,  1 },
                           { "Cheru"  ,  1  ,    1   ,    0    ,  1  ,  0 },   /* Passi */
                           { "Hanna"  ,  0  ,    1   ,    1    ,  0  ,  1 },
                           { "Chrissi",  1  ,    1   ,    0    ,  0  ,  0 },
                           { "Passi"  ,  1  ,    1   ,    0    ,  1  ,  0 }    /* Cheru */
};

/* BEWARE: Questions have to be in the same order as the answers in person_t */
const question_t questions[] = {{"Ist deine Person"   , "ein Mann?"      },
                                {"Traegt deine Person", "eine Brille?"   },
                                {"Hat deine Person"   , "einen Bachelor?"},
                                {"Kifft deine Person" , "regelmaessig?"  },
                                {"Studiert deine"     , "Person noch?"   }};

static_assert(NUM_PEOPLE == (sizeof(people) / sizeof(*people)), "Number of people in array 'people' isnt't the same as NUM_PEOPLE");
static_assert(NUM_QUESTIONS == (sizeof(questions) / sizeof(*questions)), "Number of questions in array 'questions' isn't the same as NUM_QUESTIONS");

/***************************************************************************************************/
void akinator_start()
{
  possible_people_t possible_people = pow(2, NUM_PEOPLE) - 1;   /* To store possible people */

  for(size_t question_ix = 0; question_ix < NUM_QUESTIONS; question_ix++) {
    lcd_clear();
    lcd_print_centered_string(1, questions[question_ix].line_one);
    lcd_print_centered_string(2, questions[question_ix].line_two);
    uint8_t answer_mask = 1 << question_ix;
    button_t button_pressed = button_wait(button_any);
    for(size_t person_ix = 0; person_ix < NUM_PEOPLE; person_ix++) {
      if((button_yes == button_pressed) && (0 == (people[person_ix].answers & answer_mask))) {  /* Player answered yes, but the answer for the checked person is no */
        possible_people &= ~(1 << person_ix);                                                   /* Remove person from possible solution */
      } else
      if((button_no == button_pressed) && (1 == (people[person_ix].answers & answer_mask))) {   /* Player answered no, but the answer for the checked person is yes */
        possible_people &= ~(1 << person_ix);                                                   /* Remove person from possible solution */
      }
    }
    if(0 == (possible_people & (possible_people - 1))) {      /* Only one possible person remains */
      lcd_clear();
      if(0 != possible_people) {
        int32_t solution_ix = __builtin_ctz(possible_people); /* Get index of solution */
        lcd_print_centered_string(1, people[solution_ix].name);
        delay(AKINATOR_SOLUTION_TIME);
        return;
      }
      break;
    }
  }

  /* If there are no more questions but we didn't find the answer */
  lcd_clear();
  lcd_print_centered_string(1, "I am confusion :(");
  delay(AKINATOR_SOLUTION_TIME);
} /* akinator_start */