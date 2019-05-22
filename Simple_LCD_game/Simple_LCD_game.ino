//Luca Giacometti 2019

/* position 1 -> x=3
   position 2 -> x=7
   position 3 -> x=11
*/

#include <EEPROM.h>
#include <LiquidCrystal.h>

//costanti
const int time_ = 2000;
const int delay_ = 2000;
const int rightButton = A5;
const int centerButton = A4;
const int leftButton = A3;
const char alphabet [] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//struct [4 bytes]
struct player {
  char name_[3];
  int score_;
};

//variables
int life;
int difficulty;           //1->easy; 2->normal; 4->hard
int lowerScore;
player player_;           //the current player
player playerArray[2] ;     //array of previous players

//custom chars
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
  getPreviousPlayers();                             //gets previous players from EEPROM
  difficulty = 1;
  life = 4;
  lcd.begin(16, 2);
  createChars();                  //creates the custom chars
  //clearEEMPROM();               clears EEPROM
}

void loop() {
  // put your main code here, to run repeatedly:
  getName();
  gameStart();
  while (life > 0) {
    play();
  }
  ClearSetCursor(0, 0);     //clears and resets the lcd and its cursor
  endingAnimation();        //animation
  save();
  setup();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EEPROM methods
void savePlayer(int address) {
  //save player name
  EEPROM.put(address, player_.name_);
  address += (3 * (sizeof(char)));              
  //save score
  EEPROM.put(address, player_.score_);
  address++;                                    //max score saveble is 254!
}

void getPreviousPlayers() {
  bool finished = false;
  int address = 0;
  int index = 0;
  while (!finished) {
    //get the name of the player
    char name_ [3];
    for (int i = 0; i < 3; i++) {
      name_[i] = (char)EEPROM.read(address);
      address ++;
    }
    //get the score of the player
    int score_;
    score_ = EEPROM.read(address);
    address++;
    //create the struct
    player player1_;
    for (int i = 0; i < 3; i++) {
      player1_.name_[i] = name_[i];
    }
    player1_.score_ = score_;
    //put the object in the array
    playerArray[index] = player1_;
    if (index == 0) {
      index++;
      address++;
    }
    else {
      finished = true;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//main methods
void gameStart() {
  greeting();
  showChart();
  getDifficultyMessage();
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
        updated = true;
      }
    }
    if (updated) {
      refreshDifficulty();
      updated = false;
      delay(delay_ / 10);
    }
  }
}

void clearEEMPROM() {
  for (int i = 0; i < 20; i++) {
    EEPROM.write(i, 255);
  }
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
    player_.score_ += difficulty;
    correct(true);
  }
  else if (random_value % 5 != 0 && random_value % 3 == buttonPressed - 1) {
    player_.score_ += difficulty;
    correct(false);
  }
  else {
    life--;
    wrong();
  }
}

void getName() {
  getNameMessage();
  int index = 0;
  int x = 6;
  String name_ = "";
  bool finished = false;
  printLetter(x, 'A');
  while (!finished) {
    if (digitalRead(rightButton) == LOW) {
      if (index == 25) {
        index = 0;
        printLetter(x, alphabet[index]);
      }
      else {
        index++;
        printLetter(x, alphabet[index]);
      }
      delay(delay_ / 10);
    }
    if (digitalRead(centerButton) == LOW) {
      if (x == 8) {
        finished = true;
      }
      else {
        x++;
        printLetter(x, 'A');
      }
      name_ += alphabet[index];
      index = 0;
      delay(delay_ / 10);
    }
    if (digitalRead(leftButton) == LOW) {
      if (index != 0) {
        index--;
        printLetter(x, alphabet[index]);
        delay(delay_ / 10);
      }
    }
  }
  //add the name to the player struct
  for (int i = 0; i < 3; i++) {
    player_.name_[i] = name_[i];
  }
}

void showChart() {
  char name1_[3];
  char name2_[3];
  int score1_;
  int score2_;
  //get both names
  for (int i = 0; i < 3; i++) {
    name1_[i] = playerArray[0].name_[i];
    name2_[i] = playerArray[1].name_[i];
  }
  //get both scores
  score1_ = playerArray[0].score_;
  score2_ = playerArray[1].score_;

  if (score1_ == 255) {
    score1_ = 0;
  }
  if (score2_ == 255) {
    score2_ = 0;
  }

  lcd.clear();
  if (score1_ > score2_) {
    printChart(0, name1_, score1_);
    printChart(1, name2_, score2_);
    lowerScore = score2_;
  }
  else {
    printChart(0, name2_, score2_);
    printChart(1, name1_, score1_);
    lowerScore = score1_;
  }

  bool finished = false;
  while (!finished) {
    if (digitalRead(centerButton) == LOW) {
      finished = true;
      delay(delay_ / 10);
    }
  }
}

void save() {
  if (playerArray[0].score_ == 255) {
    savePlayer(0);
  }
  else if (playerArray[1].score_ == 255) {
    savePlayer(5);
  }
  else if (player_.score_ > lowerScore) {
    if (lowerScore == playerArray[0].score_) {
      savePlayer(0);
    }
    else {
      savePlayer(5);
    }
  }
}

void endingAnimation() {
  ANIM_M1();
  for (int i = 0; i < 2; i++)
  {
    ANIM_CLOSE();
  }
  ANIM_1();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//lcd methods
void confirmMessage() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Confermato!");
  delay(delay_);
}

void refreshDifficulty() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("difficulty:");
  if (difficulty == 1) {
    lcd.setCursor(6, 1);
    lcd.print("Easy");
  }
  else if (difficulty == 2) {
    lcd.setCursor(5, 1);
    lcd.print("Normal");
  }
  else {
    lcd.setCursor(6, 1);
    lcd.print("Hard");
  }
}

void greeting() {
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

void correct(bool wonNewLife) {
  if (!wonNewLife) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Corretto!");
    lcd.setCursor(7, 1);
    lcd.print(player_.score_);
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
    lcd.print(player_.score_);
    delay(delay_ / 2);
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
  lcd.print(player_.score_);
  delay(delay_ / 2);
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
  lcd.print(player_.score_);
  delay(delay_ / 2);
  for (int i = 0; i < 16; i++) {
    lcd.scrollDisplayRight();
    delay(delay_ / 50);
  }
}

bool printChart(int y, char name_[], int score_) {
  lcd.setCursor(0, y);
  if (score_ != 0) {
    if (y == 0) {
      String message1 = "1> " + (String)name_ + "  " + score_;
      lcd.print(message1);
    }
    else {
      String message1 = "2> " + (String)name_ + "  " + score_;
      lcd.print(message1);
    }
  }
  else {
    if (y == 0) {
      String message1 = "1> ---";
      lcd.print(message1);
    }
    else {
      String message1 = "2> ---";
      lcd.print(message1);
    }
  }
}

void getDifficultyMessage() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("difficulty:");
  lcd.setCursor(6, 1);
  lcd.print("Easy");
}

void getNameMessage() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Scegli un nome");
}

void ClearSetCursor(int x, int y) {
  lcd.clear();
  lcd.setCursor(x, y);
}

void printLetter(int x, char letter) {
  lcd.setCursor(x, 1);
  lcd.print(letter);
}

