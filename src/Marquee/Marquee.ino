
#include <SPI.h>

#include "fonts.h"

#define DELAY_INTERVAL 160
#define FLASH_INTERVAL 400
#define FLASH_COUNT 2
char *DisplayWords[] = {"HELLO!!", "Openlab.Taipei", "Honki!!"};
const int INSTANCE_CNT = 3;
const byte SS_SIZE = 3;
const byte SS_SET[] = {10, 9, 8, 7, 6, 5, 4, 3};

const byte NOOP = 0x0;
const byte DECODEMODE = 0x9;
const byte INTENSITY = 0xA;
const byte SCANLIMIT = 0xB;
const byte SHUTDOWN = 0xC;
const byte DISPLAYTEST = 0xF;

int switchFlag = 0;
int instanceIdx = 0;
byte index = 0;
byte TOTAL_LEN;
char *DisplayWord;
unsigned long prevTime = 0;

void switchText(int idx) {
  byte k, j, i;
  for (i = 0; i < FLASH_COUNT; i++) {
    for (k = 0; k < SS_SIZE; k++) {
      for (j = 0; j < 8; j++) {
        max7219(SS_SET[k], j + 1, 0xFF);
      }
    }
    delay(FLASH_INTERVAL);
    for (k = 0; k < SS_SIZE; k++) {
      for (j = 0; j < 8; j++) {
        max7219(SS_SET[k], j + 1, 0x00);
      }
    }
    delay(FLASH_INTERVAL);
  }
  DisplayWord = DisplayWords[idx];
  char *str = DisplayWord;
  while(*str) {
    str++;
  }
  TOTAL_LEN = ((int)(str - DisplayWord)) << 3;
  index = 0;
}

void max7219(byte pin, byte reg, byte data) {
  digitalWrite(pin, LOW);
  SPI.transfer(reg);
  SPI.transfer(data);
  digitalWrite(pin, HIGH);
}

void setup() {
  byte k, i;

  pinMode(2, INPUT);
  for (k = 0; k < SS_SIZE; k++) {
    pinMode(SS_SET[k], OUTPUT);
    digitalWrite(SS_SET[k], HIGH);
  }

  SPI.begin();
  for (k = 0; k < SS_SIZE; k++) {
    max7219(SS_SET[k], SCANLIMIT, 7);
    max7219(SS_SET[k], DECODEMODE, 0);
    max7219(SS_SET[k], INTENSITY, 4);
    max7219(SS_SET[k], DISPLAYTEST, 0);
    max7219(SS_SET[k], SHUTDOWN, 1);
      
    for (i = 0; i < 8; i++) {
      max7219(SS_SET[k], i + 1, 0);
    }
  }
  
  switchText(0);
}

void loop() {
  byte j, k, offset;
  int idx = 0, switchInput = digitalRead(2);
  unsigned long currTime = 0;
  if (switchInput == 1 && switchInput != switchFlag) {
    switchFlag = switchInput;
    instanceIdx = (instanceIdx + 1) % INSTANCE_CNT;
    switchText(instanceIdx);
  } else if (switchInput != switchFlag) {
    switchFlag = switchInput;
  }
  for (k = 0; k < SS_SIZE; k++) {
    offset = k << 3;
    for (j = 0; j < 8; j++) {
      idx = (index + offset + j) % TOTAL_LEN;
      max7219(SS_SET[k], j + 1, fonts[(int)(DisplayWord[idx >> 3] - 32)][idx & 7]);
    }
  }

  currTime = millis();
  if (currTime - prevTime >= DELAY_INTERVAL) {
    index = (index + 1) % TOTAL_LEN;
    prevTime = currTime;
  }
}

