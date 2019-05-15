//Luca Giacometti 2019

#include <LiquidCrystal.h>

//costanti
const int confirmButton = 6;
const int delay_ = 2000;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//custom char
byte hearth[] = {B00000,B00000,B01010,B11111,B11111,B01110,B00100,B00000};
byte half_hearth[] = {B00000,B00000,B01000,B11100,B11000,B01100,B00100,B00000};
byte bad_eye[] = {B00000,B00000,B00100,B01110,B01110,B00100,B00000,B00000};
byte left_bad_mouth[] = {B00000,B00000,B00000,B00011,B00100,B01000,B01000,B00000};
byte right_bad_mouth[] = {B00000,B00000,B00000,B11000,B00100,B00010,B00010,B00000};

void setup() {
  // put your setup code here, to run once:
  pinMode(confirmButton, INPUT_PULLUP);
  lcd.begin(16, 2);
  createChars();
  lcd.home();
  prepareScreen();
  createBadFace();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void createChars() {
    lcd.createChar(1, hearth);
    lcd.createChar(2, half_hearth);
    lcd.createChar(3, bad_eye);
    lcd.createChar(4, left_bad_mouth);
    lcd.createChar(5, right_bad_mouth);
}

void prepareScreen() {
  lcd.write(1);
  lcd.setCursor(0,1);
  lcd.write(1);
  lcd.setCursor(15,0);
  lcd.write(1);
  lcd.setCursor(15,1);
  lcd.write(1);
}

void play() {
  
}

void createBadFace() {
  lcd.setCursor(5,0);
  lcd.write(3);
  lcd.write(3);
  lcd.setCursor(5,1);
  lcd.write(4);
  lcd.write(5);
}

