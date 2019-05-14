//Luca Giacometti 2019

#include <LiquidCrystal.h>

//costanti
const int delay_ = 2000;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//custom char
byte hearth[] = {B00000,B00000,B01010,B11111,B11111,B01110,B00100,B00000};
byte half_hearth[] = {B00000,B00000,B01000,B11100,B11000,B01100,B00100,B00000};

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  createChars();
  lcd.home();
  prepareScreen();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void createChars() {
    lcd.createChar(1, hearth);
    lcd.createChar(2, half_hearth);
}

void prepareScreen() {
  lcd.write(1);
  lcd.setCursor(1,0);
  lcd.write(2);
}

