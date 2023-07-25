#include "functions.h"

const char * cheers[] = {"Cheers", "You da best!", "Love you <3", "Goooo!!", "Yummy", "Delicious!", "Prost", "Chin chin", "Salut", 
"Prosit", "Cheerio", "Good choice!", "Tasty", "<3333"};
const char *pCheer;                                   // Variable for the selected curse
String sCheer;                                        // casting because lcd.print only accepts Strigs 
const char *curses[] = {"Pussy", "Noob", "Little baby", "Boring!!!", "Lame!!!", "Chicken", "Meh!", "Not cool", "Weakling", "Loser", 
"Fuck off", "Go away!", "Easter egg ;)", "Stupid", "Lightweight", "Dick", "Cunt", "Cock", "Buhh!", "Unfortunate",
"Wimp", "Softy", "Shame", "U sure?", "Doofian!", "Kurva", "Fils de pute", "Cyka blyat", "Bitsh whyy?", "Whack!!", "Dumb",
"Foolish", "Dull", "Lel", "Sad :(", "Stupid", "Traitor"};
const char *pCurse;                                   // Variable for the selected curse
String sCurse;                                        // casting because lcd.print only accepts Strigs 

String alcohol_types[] = {"Saure Kirsche", "Jaegermeister", "Pfeffi", "Johannisbeere", "Apfel", "Waldbeere", "Vodka"};

void lcd_Start(LiquidCrystal_I2C lcd){             
  lcd.begin();                                   // LCD einschalten 
  lcd.backlight();
}

void start_Screen(LiquidCrystal_I2C lcd){ 
  currentTime = millis();
  lcd.setCursor(0,0);
  lcd.print("Lets gooo!!!");
  lcd.setCursor(0,2);
  lcd.print("Press Y to choose");
  lcd.setCursor(0,3);
  lcd.print("an alcohol type");
  while(1) {
    if (!digitalRead(BUTTON_NO)) {
      digitalWrite(LED_NO, HIGH);
      while(!digitalRead(BUTTON_NO)) {delay(200);}       // to wait that the botton is unpressed
      digitalWrite(LED_NO, LOW);
      lcd.clear();
      return;
    }
    if (!digitalRead(BUTTON_YES)) {
      digitalWrite(LED_YES, HIGH);
      while(!digitalRead(BUTTON_YES)) {delay(200);}       // to wait that the botton is unpressed
      selected_alcohol = 0;                               // is initilized with -1 for no selection
      lcd.clear();
      lcd.setCursor(0,2);
      lcd.print("Press N to change");
      lcd.setCursor(0,3);
      lcd.print("Press Y to select");
      digitalWrite(LED_YES, LOW);
      while(1) {
        lcd.setCursor(0,0);
        lcd.print(alcohol_types[selected_alcohol]);
        if (!digitalRead(BUTTON_NO)) {
          digitalWrite(LED_NO, HIGH);
          while(!digitalRead(BUTTON_NO)) {delay(200);}       // to wait that the botton is unpressed
          digitalWrite(LED_NO, LOW);
          lcd.setCursor(0,0);
          lcd.print("                    ");
          if (selected_alcohol < sizeof(alcohol_types)/sizeof(alcohol_types[0]) - 1)
            selected_alcohol += 1;  
          else 
            selected_alcohol = 0;  
        }
        if (!digitalRead(BUTTON_YES)) {
           digitalWrite(LED_YES, HIGH);
           while(!digitalRead(BUTTON_YES)) {delay(200);}       // to wait that the botton is unpressed
           lcd.clear();
           digitalWrite(LED_YES, LOW);
           return;
        }
      }
    }
  }
}

void calibrate_Routine(LiquidCrystal_I2C lcd){
  currentTime = millis(); 
  lcd.clear(); // f1                                        
  while (currentTime <= WARM_UP || sensorValue >= SENSOR_THRESHOLD_VALUE) {     // läuft bis warming Up Zeit um ist oder sensorValue unter SENSOR_THRESHOLD_VALUE
    if (currentTime - previousTime >= REFRESH_ALC_TIME) {
      lcd.setCursor(0,0);
      lcd.print("Sensor calibrating  ");                                       //two spaces are improtant to clear display
      lcd.setCursor(0,1);
      lcd.print("Current value:");
      lcd.setCursor(15,1);
      lcd.print("    ");                                                         //spaces to clear display
      lcd.setCursor(15,1);                        
      lcd.print(sensorValue);
      lcd.setCursor(0,3);
      lcd.print("Warmup shot? Y/N");
      previousTime = currentTime;
    }
    if (!digitalRead(BUTTON_YES))
      shot_In_Five(lcd); 
    else if (!digitalRead(BUTTON_NO))
      lcd_Curse(lcd);
  currentTime = millis();                           
  sensorValue = analogRead(MQ3pin);                                              // das muss auch jedes mal in der while schleife aktualisiert werden für sensorValue >= 200
  }
  lcd.clear();                                                                    
}

void ready_Message(LiquidCrystal_I2C lcd, int sensorValue){ 
  lcd.setCursor(0,0);
  lcd.print("Shotty is ready");
  if(selected_alcohol != -1) {
    lcd.print(" with");
    lcd.setCursor(0,1);
    lcd.print(alcohol_types[selected_alcohol]);
  }
  //lcd.setCursor(0,1);
  //lcd.print("Current value:");
  //lcd.setCursor(15,1);
  //lcd.print("    ");                                // wenn er von 100 auf 99 springt da nicht 990 steht weil die 0 stehen bleibt
  //lcd.setCursor(15,1);
  //lcd.print(sensorValue);
  lcd.setCursor(0,2);
  lcd.print("Press Y for shot");
  lcd.setCursor(0,3);
  lcd.print("Press N to measure");
}

int starting_Measurement(LiquidCrystal_I2C lcd){
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print("Starting measurement");
    lcd.setCursor(0,2); 
    lcd.print("Blow me for 5 s");
    lcd.setCursor(0,3);
    sensorValueFixed = analogRead(MQ3pin);
    sensorValueReference = sensorValueFixed;
    for(int i = 5; i > 0; i--) {                         // es wird runtergezählt von 5 in der zeit in der man pusten soll
      lcd.print(i);                                  
      lcd.print(" ");
      sensorValueFixed = get_Highest_Value(sensorValueFixed);
      digitalWrite (LED_YES, LOW);
      if (!digitalRead(BUTTON_NO)) {
           digitalWrite(LED_YES, LOW);
           digitalWrite(LED_NO, LOW);
           lcd_Curse(lcd);
           return -1;                                  // -1 because then display_Message is not called 
       } 
    }
    lcd.setCursor(0,3);
    lcd.print("                 "); 
    lcd.setCursor(0,3);
    lcd.print("Harder Daddy!!!"); 
    sensorValueFixed = get_Highest_Value(sensorValueFixed);
    sensorValueFixed = get_Highest_Value(sensorValueFixed);
    digitalWrite(LED_YES, LOW);
    digitalWrite(LED_NO, LOW);
    lcd.clear();
    if(sensorValueFixed <= sensorValueReference)
      return 0;
    else
      return sensorValueFixed - sensorValueReference;
}

void lcd_Curse(LiquidCrystal_I2C lcd) {
  int next_curse = random(sizeof(curses)/sizeof(curses[0]));
  pCurse = curses[next_curse];
  sCurse = pCurse;                                              // casting because lcd.print only accepts Strigs 
  digitalWrite(LED_NO, HIGH);
  lcd.clear();
  int mid_of_screen = 10 - strlen(curses[next_curse]) /2;
  lcd.setCursor(mid_of_screen,1); 
  lcd.print(sCurse);
  delay(3000);
  digitalWrite(LED_NO, LOW);
  digitalWrite(LED_YES, LOW);
  lcd.setCursor(0,1);
  lcd.print("                    ");
}

int get_Highest_Value( int sensorValueFixed){
  digitalWrite(LED_YES, HIGH);
  digitalWrite(LED_NO, LOW);
  delay (500);
  digitalWrite(LED_YES, LOW);
  digitalWrite(LED_NO, HIGH);
  delay (500);
  sensorValue = analogRead(MQ3pin);
    if (sensorValue >= sensorValueFixed)
        return sensorValue;
    else
      return sensorValueFixed;
}

void display_Message(LiquidCrystal_I2C lcd, int sensorValue){
  lcd.clear();
  displayValue = (float) sensorValue / 500.0;
  lcd.setCursor(0,1);
  lcd.print ("Alcohol: ");
  lcd.print(displayValue);
  lcd.print(" %.");
  lcd.setCursor(0,0);
  switch(sensorValue) {                   // is like that because switch dont work with float 250 == 0.5 dispalyValue                                           
      case   0 ... 250:                   lcd.print("Meh, pretty sober!"); 
                                          lcd.setCursor(0,2); lcd.print("You need a drink!");
                                          lcd.setCursor(0,3); lcd.print("Press Y for shot!"); 
                                          break;
      case 251 ... 500:                   lcd.print("A little drunk");
                                          lcd.setCursor(0,2); lcd.print("A shot is needed!");
                                          lcd.setCursor(0,3); lcd.print("Press Y for shot!");
                                          break;
      case 501 ... NO_SHOT_LIMIT:         lcd.print("Good level!!!");
                                          lcd.setCursor(0,2); lcd.print("Keep it up!");
                                          lcd.setCursor(0,3); lcd.print("Press Y for shot!"); 
                                          break;
      case NO_SHOT_LIMIT+1 ... 1500:      lcd.print("You are wasted!!!");
                                          lcd.setCursor(0,2); lcd.print("You had enough!");
                                          lcd.setCursor(0,3); lcd.print("Press N for no shot");
      default:                            lcd.print("Thats weird!"); 
                                          lcd.setCursor(0,2); lcd.print("Something went wrong");
                                          break;
  }
  shot_Or_Not(lcd, sensorValueFixed);
}

void shot_Or_Not(LiquidCrystal_I2C lcd, int sensorValue){
  while(1) {
    if (sensorValue <= NO_SHOT_LIMIT){
       if (!digitalRead(BUTTON_YES)) {
          shot_In_Five(lcd);
          return;                                                         
        }  else if (!digitalRead(BUTTON_NO)) {
          lcd_Curse(lcd);
          return;
        }
    }
    else{
     if (!digitalRead(BUTTON_YES)) {
        digitalWrite(LED_NO, HIGH);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Forget it"); 
        delay(3000);
        digitalWrite(LED_NO, LOW);
        lcd.clear();
        return;                                                         
        }  else if(!digitalRead(BUTTON_NO)) {
        digitalWrite (LED_NO, HIGH);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Good boy!"); 
        delay(3000);
        digitalWrite(LED_NO, LOW);
        lcd.clear();
        return;
        }
    }
  }
}

void shot_In_Five(LiquidCrystal_I2C lcd){
  digitalWrite(LED_YES, HIGH); 
  digitalWrite(LED_NO, LOW);  
  lcd.clear();
  lcd.setCursor(0,0); 
  pCheer = cheers[random(sizeof(cheers)/sizeof(cheers[0]))];
  sCheer = pCheer;                                              // casting because lcd.print only accepts Strigs 
  lcd.print(sCheer);
  lcd.setCursor(0,1); 
  lcd.print("Shot in ");
  lcd.print (CANCEL_TIME / 1000);
  lcd.setCursor(0,2);
  lcd.print("Shot glass in place?");
  lcd.setCursor(0,3);
  lcd.print("Press N to cancel"); 
    for(int i=CANCEL_TIME/1000; i>=0; i--) {      
       lcd.setCursor(0,1);
       lcd.print("Shot in     "); 
       lcd.setCursor(8,1);
       lcd.print(i);
       lcd.print(" s"); 
       int y = CANCEL_TIME/1000;
       if (y >= i+1) {                                                             // das macht die LED nach 1s aus
          digitalWrite (LED_YES, LOW);
       }
       currentTime = millis();
       previousTime = currentTime;                              
       while(currentTime - previousTime <= 1000){
        if (!digitalRead(BUTTON_NO)) {
           lcd_Curse(lcd);
           return;
       } 
     currentTime = millis(); 
     }
    // currentTime = millis(); 
   }
 pour_Shot(lcd);
 lcd.clear();
}

void pour_Shot(LiquidCrystal_I2C lcd) {
  while (!digitalRead(BUTTON_NO) || !digitalRead(BUTTON_YES)) {}                    //versuchter fix2
  digitalWrite(RELAIS_PIN, HIGH);
   currentTime = millis();
   previousTime = currentTime;
  while (currentTime - previousTime <= RELAIS_TIME) {
    if (!digitalRead(BUTTON_NO)) {                                            
       for (int i = 0; i <= NO_AMOUNT; i++) {                                      // for loop is nececarry because active relayPin leads to wrong readings at the bottons
        if (digitalRead(BUTTON_NO) == 1) 
          break;
        if (i == NO_AMOUNT) {
          digitalWrite(RELAIS_PIN, LOW); // f1
          lcd_Curse(lcd);
          return;
        }
       } 
    }
    currentTime = millis();
  } 
  lcd.clear();
  digitalWrite(RELAIS_PIN, LOW);
  while (!digitalRead(BUTTON_NO) || !digitalRead(BUTTON_YES)) {}                    //versuchter fix2
  while (doubleShot == true) {
    lcd.setCursor(0,0); 
    lcd.print("Double Shot? ;)");
    lcd.setCursor(0,3);
    lcd.print("Y/N");
    if (!digitalRead(BUTTON_YES)) {
      digitalWrite (LED_YES, HIGH);
      lcd.clear();
      lcd.setCursor(0,0); 
      lcd.print("Cheers!");
      doubleShot = false;
      pour_Shot(lcd);
      break;  
    }
    if (!digitalRead(BUTTON_NO)) {
       for (int i = 0; i <= NO_AMOUNT; i++) {                                      // for loop is nececarry because active relayPin leads to wrong readings at the bottons
        if (digitalRead(BUTTON_NO) == 1) 
          break;
       if (i == NO_AMOUNT) {
          digitalWrite(RELAIS_PIN, LOW); // f1
          lcd_Curse(lcd);
          return;
       }
     } 
   }
  }
 doubleShot = true;
}
