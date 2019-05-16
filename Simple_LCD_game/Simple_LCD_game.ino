//Luca Giacometti 2019

/* posizione 1 -> x=3
   posizione 2 -> x=7
   posizione 3 -> x=11
*/

#include <LiquidCrystal.h>

//costanti
const int time_ = 4000;
const int delay_ = 2000;
const int rightButton = A5;
const int centerButton = A4;
const int leftButton = A3;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//variabili
int life;
int difficulty; //1->easy; 2->normal; 4->hard
int score;

//custom char
byte hearth[] = {B00000, B00000, B01010, B11111, B11111, B01110, B00100, B00000};
byte eye[] = {B00000, B00000, B00100, B01110, B01110, B00100, B00000, B00000};
byte left_good_mouth[] = {B00000, B00000, B00000, B01000, B01000, B00100, B00011, B00000};
byte right_good_mouth[] = {B00000, B00000, B00000, B00010, B00010, B00100, B11000, B00000};
byte coin1[] = {B00000, B00000, B00000, B00111, B01000, B10001, B10010, B10010};
byte coin2[] = {B10010, B10010, B10010, B10001, B01000, B00111, B00000, B00000};
byte coin3[] = {B00000, B00000, B00000, B10000, B01000, B00100, B10100, B10100};
byte coin4[] = {B10100, B10100, B00100, B00100, B01000, B10000, B00000, B00000};


void setup() {
  // put your setup code here, to run once:
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(centerButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  score = 0;
  difficulty = 1;
  life = 4;
  lcd.begin(16, 2);
  createChars();
}

void loop() {
  // put your main code here, to run repeatedly:
  gameStart();
  while (life > 0) {
    play();
  }
  lcd.clear();
  lcd.home();
  ANIM_M1();
  for (int i = 0; i < 2; i++)
  {
    ANIM_CLOSE();
  }
  ANIM_1();
}

void gameStart() {
  greeting();
  infos();
  bool confirm = false;
  while (!confirm) {
    bool updated = false;
    if (digitalRead(rightButton) == LOW) {
      if (difficulty < 4) {
        difficulty = difficulty * 2;
        updated = true;
      }
    }
    if (digitalRead(centerButton) == LOW) {
      if (difficulty != 0) {
        confirm = true;
        confirmMessage();
      }
    }
    if (digitalRead(leftButton) == LOW) {
      if (difficulty > 1) {
        difficulty = difficulty / 2;
      }
    }
    if (updated) {
      refreshDifficulty();
      updated = false;
    }
  }
}

void confirmMessage() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Confermato!");
  delay(delay_);
}

void infos() {
  lcd.clear();
  lcd.home();
  lcd.print("Usare i bottoni per");
  lcd.setCursor(0, 1);
  lcd.print("modificare la difficolta'");
  bool left_right = true;
  for (int i = 0; i < 45; i++) {
    if (left_right) {
      lcd.scrollDisplayRight();
      delay(delay_ / 4);
    }
    else {
      lcd.scrollDisplayLeft();
      delay(delay_ / 4);
    }
    if (i % 11 == 0 && left_right) {
      left_right = false;
    }
    else if (i % 11 == 0 && !left_right) {
      left_right = true;
    }
  }
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Premi il");
  lcd.setCursor(2, 1);
  lcd.print("rosso o blu");
}

void refreshDifficulty() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("difficulty:");
  lcd.setCursor(8, 1);
  lcd.print(difficulty);
  delay(delay_);
}

void greeting()
{
  lcd.setCursor(0, 0);
  lcd.print("  Gioco arduino");
  lcd.setCursor(0, 1);
  lcd.print("    by Luca");
  delay(delay_);
  for (int i = 0; i < 17; i++)
  {
    lcd.scrollDisplayRight();
    delay(50);
  }
}

void createChars() {
  lcd.createChar(0, hearth);
  lcd.createChar(1, eye);
  lcd.createChar(2, left_good_mouth);
  lcd.createChar(3, right_good_mouth);
  lcd.createChar(4, coin1);
  lcd.createChar(5, coin2);
  lcd.createChar(6, coin3);
  lcd.createChar(7, coin4);
}

void play() {
  lcd.clear();
  refreshLife();
  int random_value = displayTurn();
  int buttonPressed = 0;
  int startTime = (int)millis();
  while ((int)millis() - startTime < (int)(time_ / difficulty) && buttonPressed == 0) {
    if (digitalRead(leftButton) == LOW) {
      buttonPressed = 1;
    }
    else if (digitalRead(centerButton) == LOW) {
      buttonPressed = 2;
    }
    else if (digitalRead(rightButton) == LOW) {
      buttonPressed = 3;
    }
  }
  check(random_value, buttonPressed);
}

void refreshLife() {
  if (life == 1) {
    createOneLife(0, 0);
  }
  else if (life == 2) {
    createOneLife(0, 0);
    createOneLife(0, 1);
  }
  else if (life == 3) {
    createOneLife(0, 0);
    createOneLife(0, 1);
    createOneLife(15, 0);
  }
  else {
    createOneLife(0, 0);
    createOneLife(0, 1);
    createOneLife(15, 0);
    createOneLife(15, 1);
  }
}

void createOneLife(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(byte(0));
}

void createBadFace(int x) {
  lcd.setCursor(x, 0);
  lcd.write(1);
  lcd.write(1);
  lcd.setCursor(x, 1);
  lcd.print("--");
}

void createGoodFace(int x) {
  lcd.setCursor(x, 0);
  lcd.write(1);
  lcd.write(1);
  lcd.setCursor(x, 1);
  lcd.write(2);
  lcd.write(3);
}

void createCoin(int x) {
  lcd.setCursor(x, 0);
  lcd.write(4);
  lcd.write(6);
  lcd.setCursor(x, 1);
  lcd.write(5);
  lcd.write(7);
}

void randomGoodFace(int value) {
  if (value % 3 == 0) {
    createGoodFace(3);
  }
  else if (value % 3 == 1) {
    createGoodFace(7);
  }
  else {
    createGoodFace(11);
  }
}

void randomLife(int value) {
  if (value % 3 == 0) {
    createCoin(3);
  }
  else if (value % 3 == 1) {
    createCoin(7);
  }
  else {
    createCoin(11);
  }
}

int displayTurn() {
  createBadFace(3);
  createBadFace(7);
  createBadFace(11);
  int random_value = analogRead(A0);    //0 to 1023
  if (random_value % 5 == 0) {
    randomLife(random_value);
  }
  else {
    randomGoodFace(random_value);
  }
  return random_value;
}

void check(int random_value, int buttonPressed) {
  if (buttonPressed == 0)
  {
    life--;
    tooSlow();
  }
  else if (random_value % 5 == 0 && random_value % 3 == buttonPressed - 1) {
    if (life < 4) {
      life++;
    }
    score += difficulty;
    correct(true);
  }
  else if (random_value % 5 != 0 && random_value % 3 == buttonPressed - 1) {
    score += difficulty;
    correct(false);
  }
  else {
    life--;
    wrong();
  }
}

void correct(bool wonNewLife) {
  if (!wonNewLife) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Corretto!");
    lcd.setCursor(7, 1);
    lcd.print(score);
    delay(delay_ / 2);
    for (int i = 0; i < 16; i++) {
      lcd.scrollDisplayRight();
      delay(delay_ / 50);
    }
  }
  else {
    lcd.clear();
    lcd.home();
    lcd.print("Corretto: +1UP!");
    lcd.setCursor(7, 1);
    lcd.print(score);
    delay(delay_);
    for (int i = 0; i < 16; i++) {
      lcd.scrollDisplayRight();
      delay(delay_ / 50);
    }
  }
}

void wrong() {
  lcd.clear();
  lcd.home();
  lcd.print("Sbagliato: -1UP!");
  lcd.setCursor(7, 1);
  lcd.print(score);
  delay(delay_);
  for (int i = 0; i < 16; i++) {
    lcd.scrollDisplayRight();
    delay(delay_ / 50);
  }
}

void tooSlow() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Lento!! -1UP!");
  lcd.setCursor(7, 1);
  lcd.print(score);
  delay(delay_);
  for (int i = 0; i < 16; i++) {
    lcd.scrollDisplayRight();
    delay(delay_ / 50);
  }
}

