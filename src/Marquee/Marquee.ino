
#include <SPI.h>

#include "fonts.h"

#define DELAY_INTERVAL 160
char DisplayWord[] = "HELLO!!";
const byte SS_SIZE = 2;
const byte SS_SET[] = {10, 9, 8, 7, 6, 5, 4, 3, 2};

const byte NOOP = 0x0;
const byte DECODEMODE = 0x9;
const byte INTENSITY = 0xA;
const byte SCANLIMIT = 0xB;
const byte SHUTDOWN = 0xC;
const byte DISPLAYTEST = 0xF;

byte index = 0;
byte TOTAL_LEN;

void max7219(byte pin, byte reg, byte data) {
  digitalWrite(pin, LOW);
  SPI.transfer(reg);
  SPI.transfer(data);
  digitalWrite(pin, HIGH);
}

void setup() {
  byte k, i;
  char *str = DisplayWord;
  while(*str) {
    str++;    
  }
  TOTAL_LEN = ((int)(str - DisplayWord)) << 3;

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
}

void loop() {
  byte j, k, offset;
  int idx = 0;
  for (k = 0; k < SS_SIZE; k++) {
    offset = k << 3;
    for (j = 0; j < 8; j++) {
      idx = (index + offset + j) % TOTAL_LEN;
      max7219(SS_SET[k], j + 1, fonts[(int)(DisplayWord[idx >> 3] - 32)][idx & 7]);
    }
  }
  delay(DELAY_INTERVAL);
  index = (index + 1) % TOTAL_LEN;
}

