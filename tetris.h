#include <LiquidCrystal.h>
#include <avr/wdt.h>
#include <LedControl.h>
#include "pitches.h"

#define btn1 A0
#define btn2 A1
#define btn3 A2
#define btn4 A3
#define btn5 A4
#define btnReset A5
#define btn6 A7

#define maxBlock    6
#define maxRotations 4
#define melodyPin    22

#define FND1 30
#define FND2 31
#define FND3 32
#define FND4 33
#define FND5 34
#define FND6 35
#define FND7 36
#define FND8 37

#define FND_A 38
#define FND_B 39
#define FND_C 40
#define FND_D 41
#define FND_E 42
#define FND_F 43
#define FND_G 44
#define FND_DP 45

#define  A_BIT  0x01
#define  B_BIT  0x02
#define  C_BIT  0x04
#define  D_BIT  0x08
#define  E_BIT  0x10
#define  F_BIT  0x20
#define  G_BIT  0x40
#define  DP_BIT 0x80

#define FND_1_SEL    0x01
#define FND_2_SEL    0x02
#define FND_3_SEL    0x04
#define FND_4_SEL    0x08
#define FND_5_SEL    0x10
#define FND_6_SEL    0x20

#define MAX_FND_NUM 6
#define MAX_FND 8

#define DATA_IN 12
#define CLK 11
#define CS 10

extern LiquidCrystal lcd;
extern LedControl lc;

extern boolean matrix[16][4];

extern int currentX, currentY, currentRot, currentShape, prevKey, gameSpeed, score, highScore, melodyNote;
extern unsigned long timeToMove, melodyDelay, lastDisplayUpdate;
extern boolean melodyOn;

extern const int FndSelectTable[6];
extern const int FndPinTable[8];
extern const int FndNumberTable[10];

extern const int melody[];
extern const int noteDurations[];

extern byte cfull[];
extern byte ctop[];
extern byte cbottom[];
extern byte Blocks[6][4][3];

void FndInit();
void FndSelect(int Position);
void FndOff();
void FndData(char text);
void DrawTextFnd(int Position, char text);
void drawScreen();
void newBlock();
void putBlock(boolean visible, int shape, int rot, int x, int y);
void drawBlock();
void clearBlcok();
void clearDisplay();
boolean onScreen(int x, int y);
boolean isValid(int rot, int x, int y);
boolean isValid(int x, int y);
boolean moveDown();
boolean moveLeft();
boolean moveRight();
boolean rotate();
void flashLine(int y);
int clearLines();
int getKey();
int getNote(int k);
int getNoteDuration(int k);
void playNote();
void initialize();
void displayScore();
void displayLC_Block();