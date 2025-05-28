#include "tetris.h"

LiquidCrystal lcd(4, 6, 18, 19, 20, 21);
LedControl lc = LedControl(DATA_IN, CLK, CS, 1);

boolean matrix[16][4];

int Block_X, Block_Y, Block_Rot, Block_shape, prevKey, gameSpeed, score, melodyNote;
unsigned long timeToMove, melodyDelay, lastDisplayUpdate;
boolean melodyOn;

const int FndSelectTable[6] = {
    FND1, FND2, FND3, FND4, FND5, FND6
};
const int FndPinTable[8] = {
    FND_A, FND_B, FND_C, FND_D, FND_E, FND_F, FND_G, FND_DP
};
const int FndNumberTable[10] = {
    A_BIT | B_BIT | C_BIT | D_BIT | E_BIT | F_BIT, // '0'
    B_BIT | C_BIT, // '1'
    A_BIT | B_BIT | D_BIT | E_BIT | G_BIT, // '2'
    A_BIT | B_BIT | C_BIT | D_BIT | G_BIT, // '3'
    B_BIT | C_BIT | F_BIT | G_BIT, // '4'
    A_BIT | C_BIT | D_BIT | F_BIT | G_BIT, // '5'
    C_BIT | D_BIT | E_BIT | F_BIT | G_BIT, // '6'
    A_BIT | B_BIT | C_BIT, // '7'
    A_BIT | B_BIT | C_BIT | D_BIT | E_BIT | F_BIT | G_BIT, // '8'
    A_BIT | B_BIT | C_BIT | F_BIT | G_BIT // '9'
};

static const int PROGMEM melody[] = {
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0, 
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0,
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_A3, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS3, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3, 
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_E4, NOTE_E3, NOTE_A4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0, 
  NOTE_E5, NOTE_E3, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, 
  NOTE_B4, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_A3, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E3, NOTE_E5,
  NOTE_E3, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_A4, NOTE_A3, NOTE_B2, 
  NOTE_C3, NOTE_D3, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_C5, NOTE_C5, NOTE_G5, 
  NOTE_F5, NOTE_E5, NOTE_C3, 0, NOTE_C5, NOTE_E5, NOTE_A4, NOTE_G4, NOTE_D5,
  NOTE_C5, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_E5, 
  NOTE_G4, NOTE_C5, NOTE_E4, NOTE_A4, NOTE_E3, NOTE_A4, 0,
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_A3, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS3, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3, 
  NOTE_E4, NOTE_E3, NOTE_A2, NOTE_E3, NOTE_C4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_D4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_B3, NOTE_E3, NOTE_GS2, NOTE_E3,
  NOTE_C4, NOTE_E3, NOTE_E4, NOTE_E3, NOTE_A4, NOTE_E3, NOTE_A2, NOTE_E3, 
  NOTE_GS4, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3, NOTE_GS2, NOTE_E3,
};

static const int PROGMEM noteDurations[] = {
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,4,8,8,16,16,8,8,8,8,8,8,8,16,16,8,8,8,8,8,8,8,8,8,8,8,8,8,8,4,4,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
};

byte cfull[] = {B11111, B11111, B11111, B00000, B11111, B11111, B11111, B11111};
byte ctop[] = {B11111, B11111, B11111, B11111, B00000, B00000, B00000, B00000};
byte cbottom[] = {B00000, B00000, B00000, B00000, B11111, B11111, B11111, B11111};
byte Blocks[6][4][3] = {
  {{B111, B000, B000}, {B100, B100, B100}, {B111, B000, B000}, {B100, B100, B100}}, //I
  {{B010, B110, B000}, {B100, B110, B000}, {B110, B100, B000}, {B110, B010, B000}}, //J
  {{B110, B011, B000}, {B010, B110, B100}, {B110, B011, B000}, {B010, B110, B100}}, //z
  {{B011, B110, B000}, {B100, B110, B010}, {B011, B110, B000}, {B100, B110, B010}}, //s
  {{B110, B110, B000}, {B110, B110, B000}, {B110, B110, B000}, {B110, B110, B000}}, //O
  {{B010, B111, B000}, {B010, B011, B010}, {B000, B111, B010}, {B010, B110, B010}}  //T
};

void setup() {
  pinMode(A0, INPUT);
  digitalWrite(A0, INPUT_PULLUP);
  pinMode(A1, INPUT);
  digitalWrite(A1, INPUT_PULLUP);
  pinMode(A2, INPUT);
  digitalWrite(A2, INPUT_PULLUP);
  pinMode(A3, INPUT);
  digitalWrite(A3, INPUT_PULLUP);
  pinMode(A4, INPUT);
  digitalWrite(A4, INPUT_PULLUP);
  pinMode(btnReset, INPUT);
  digitalWrite(btnReset, INPUT_PULLUP);
  pinMode(melodyPin, OUTPUT);
  melodyOn = true;
  lcd.begin(16, 2);
  lcd.createChar(0, ctop);
  lcd.createChar(1, cbottom);
  lcd.createChar(2, cfull);
  Serial.begin(9600);
  randomSeed(analogRead(A9));
  initialize();
  lcd.clear();
  lcd.setCursor(1, 0);
  FndInit();
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  while(1) {
    lcd.setCursor(0, 0);
    lcd.print("Game start");
    if (getKey() == btn6) {
      lcd.clear();
      break;
    }
  }
  delay(2000);
}

void loop() {
  newBlock();
  drawBlock();
  drawScreen();
  while (moveDown()) {
    while (millis() < timeToMove) {
      playNote();
      int k = getKey();
      if (k != prevKey) {
        Serial.println(analogRead(A0));
        switch (k) {
          case btn1:
            while(moveDown()) { playNote(); }
            break;
          case btn2:
            moveLeft();
            break;
          case btn3:
            moveRight();
            break;
          case btn4:
            rotate();
            break;
          case btn5:
            melodyOn = !melodyOn;
            break;
          case btnReset:
            wdt_enable(WDTO_15MS);
            while (1) {}
            break;
        }
        prevKey = k;
      }
      displayScore();
    }
    timeToMove = millis() + gameSpeed;
  }
  int cleared = clearLines();
  score = score + (10 * (cleared + 1));

  if (Block_Y == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    while (1) {
      displayScore();
      if (getKey() == btnReset) {
        wdt_enable(WDTO_15MS);
      }
    }
  }
}

void FndInit() {
    for (int i = 0; i < MAX_FND; i++) {
        pinMode(FndSelectTable[i], OUTPUT);
        pinMode(FndPinTable[i], OUTPUT);
    }
}

void FndSelect(int Position) {
    for (int i = 0; i < MAX_FND_NUM; i++) {
        if (Position & (1 << i)) {
            digitalWrite(FndSelectTable[i], LOW);
        } else {
            digitalWrite(FndSelectTable[i], HIGH);
        }
    }
}

void FndOff() {
    for (int i = 0; i < MAX_FND; i++) {
        digitalWrite(FndSelectTable[i], HIGH);
    }
    delay(50);
}

void FndData(char text) {
    for (int i = 0; i < MAX_FND; i++) {
        if (FndNumberTable[text - '0'] & (1 << i)) {
            digitalWrite(FndPinTable[i], HIGH);
        } else {
            digitalWrite(FndPinTable[i], LOW);
        }
    }
    delay(1);
}

void DrawTextFnd(int Position, char text) {
    FndSelect(Position);
    FndData(text);
}

void drawScreen() {
    for (int i = 0; i < 16; i++) {
        lcd.setCursor(i, 0);
        if (matrix[i][0] && matrix[i][1]) lcd.write(byte(2));
        else if (matrix[i][0]) lcd.write(byte(0));
        else if (matrix[i][1]) lcd.write(byte(1));
        else lcd.print(' ');
        lcd.setCursor(i, 1);
        if (matrix[i][2] && matrix[i][3]) lcd.write(byte(2));
        else if (matrix[i][2]) lcd.write(byte(0));
        else if (matrix[i][3]) lcd.write(byte(1));
        else lcd.print(' ');
    }
}

void newBlock() {
    Block_X = 1;
    Block_Y = 0;
    Block_Rot = 0;
    Block_shape = random(0, maxBlock);
    displayLC_Block();
}

void putBlock(boolean visible, int shape, int rot, int x, int y) {
    for (int i = 0; i < 3; i++) {
        if ((Blocks[shape][rot][i] >> 2) & 1) matrix[y + i][x] = visible;
        if ((Blocks[shape][rot][i] >> 1) & 1) matrix[y + i][x + 1] = visible;
        if (Blocks[shape][rot][i] & 1) matrix[y + i][x + 2] = visible;
    }
}

void drawBlock() {
    putBlock(true, Block_shape, Block_Rot, Block_X, Block_Y);
}

void clearBlock() {
    putBlock(false, Block_shape, Block_Rot, Block_X, Block_Y);
}

void clearDisplay() {
    for (int i = 0; i < 16; i++) {
        for (int n = 0; n < 4; n++) {
            matrix[i][n] = 0;
        }
    }
}

boolean onScreen(int x, int y) {
    if (x < 0) return false;
    if (x > 3) return false;
    if (y < 0) return false;
    if (y > 15) return false;
    return true;
}

boolean isValid(int rot, int x, int y) {
    boolean okay = true;
    for (int i = 0; ((i < 3) && (okay)); i++) {
        if ((Blocks[Block_shape][rot][i] >> 2) & 1) {
            if (!onScreen(x, y + i) || matrix[y + i][x]) okay = false;
        }

        if ((Blocks[Block_shape][rot][i] >> 1) & 1) {
            if (!onScreen(x + 1, y + i) || matrix[y + i][x + 1]) okay = false;
        }

        if (Blocks[Block_shape][rot][i] & 1) {
            if (!onScreen(x + 2, y + i) || matrix[y + i][x + 2]) okay = false;
        }
    }
    return okay;
}

boolean isValid(int x, int y) {
    return isValid(Block_Rot, x, y);
}

boolean moveDown() {
    clearBlock();
    if (isValid(Block_X, Block_Y + 1)) {
        Block_Y++;
        drawBlock();
        drawScreen();
        return true;
    }
    drawBlock();
    return false;
}

boolean moveLeft() {
    clearBlock();
    if (isValid(Block_X - 1, Block_Y)) {
        Block_X--;
        drawBlock();
        drawScreen();
        return true;
    }
    drawBlock();
    return false;
}

boolean moveRight() {
    clearBlock();
    if (isValid(Block_X + 1, Block_Y)) {
        Block_X++;
        drawBlock();
        drawScreen();
        return true;
    }
    drawBlock();
    return false;
}

boolean rotate() {
    int r;
    clearBlock();
    if (Block_Rot == maxRotations - 1) r = 0;
    else r = Block_Rot + 1;
    if (isValid(r, Block_X, Block_Y)) {
        Block_Rot = r;
        drawBlock();
        drawScreen();
        displayLC_Block();
        return true;
    }
    drawBlock();
    return false;
}

void flashLine(int y) {
    for (int t = 0; t < 3; t++) {
        for (int i = 0; i < 4; i++) matrix[y][i] = 1;
        drawScreen();
        tone(melodyPin, NOTE_E5, 50);
        for (int i = 0; i < 4; i++) matrix[y][i] = 0;
        drawScreen();
        tone(melodyPin, NOTE_C5, 50);
    }
}

int clearLines() {
    int lineCount = 0;
    boolean tmpmatrix[16][4];
    for (int i = 0; i < 16; i++) {
        for (int n = 0; n < 4; n++) {
            tmpmatrix[i][n] = 0;
        }
    }
    int tmpY = 15;
    boolean found = false;
    for (int y = 15; y >= 0; y--) {
        if (matrix[y][0] && matrix[y][1] && matrix[y][2] && matrix[y][3]) {
            flashLine(y);
            lineCount++;
        } else {
            for (int x = 0; x < 4; x++) tmpmatrix[tmpY][x] = matrix[y][x];
            tmpY--;
        }
    }
    if (lineCount > 0) {
        for (int i = 0; i < 16; i++) {
            for (int n = 0; n < 4; n++) {
                matrix[i][n] = tmpmatrix[i][n];
            }
        }
    }
    return lineCount;
}

int getKey() {
    if (digitalRead(btn1) == LOW) return btn1;
    else if (digitalRead(btn2) == LOW) return btn2;
    else if (digitalRead(btn3) == LOW) return btn3;
    else if (digitalRead(btn4) == LOW) return btn4;
    else if (digitalRead(btn5) == LOW) return btn5;
    else if (digitalRead(btnReset) == LOW) return btnReset;
    else if (digitalRead(btn6) == LOW) return btn6;
    else return -1;
    delay(8);
}

int getNote(int k) {
    return pgm_read_word_near(melody + k);
}

int getNoteDuration(int k) {
    return pgm_read_word_near(noteDurations + k);
}

void playNote() {
    if (!melodyOn) return;
    if (millis() > melodyDelay) {
        int noteDuration = 1000 / getNoteDuration(melodyNote);
        tone(melodyPin, getNote(melodyNote), noteDuration);
        if (melodyNote < (sizeof(melody) / sizeof(melody[0])) - 1) melodyNote++;
        else melodyNote = 0;
        int pauseBetweenNotes = noteDuration * 1.30;
        melodyDelay = millis() + pauseBetweenNotes;
    }
}

void initialize() {
    newBlock();
    clearDisplay();
    gameSpeed = 600;
    score = 0;
    timeToMove = millis() + gameSpeed;
    melodyDelay = 0;
    melodyNote = 0;
}

void displayScore() {
    String scoreString = String(score);
    int length = scoreString.length();
    for (int i = 0; i < length; i++) {
        DrawTextFnd(FND_1_SEL << (length - i - 1), scoreString[i]);
    }
}

void displayLC_Block() {
    lc.clearDisplay(0);
    int offsetX = 2;
    int offsetY = 2;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if ((Blocks[Block_shape][Block_Rot][i] >> (2 - j)) & 1) {
                lc.setLed(0, j + offsetY, i + offsetX, true);
            }
        }
    }
}
