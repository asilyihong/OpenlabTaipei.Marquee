
#include <SPI.h>

#include "fonts.h"

#define CHAR_LEN 7
#define SS2 9
#define SS3 6
const int DisplayWord[] = {'H', 'E', 'L', 'L', 'O', '!', '!'};

const byte SS_SET[] = {SS, SS2, SS3};
const byte SS_SIZE = 3;
const byte TOTAL_LEN = CHAR_LEN << 3;

const byte NOOP = 0x0;
const byte DECODEMODE = 0x9;
const byte INTENSITY = 0xA;
const byte SCANLIMIT = 0xB;
const byte SHUTDOWN = 0xC;
const byte DISPLAYTEST = 0xF;
byte index = 0;

void max7219(byte pin, byte reg, byte data) {
  digitalWrite(pin, LOW);
  SPI.transfer(reg);
  SPI.transfer(data);
  digitalWrite(pin, HIGH);
}

void setup() {
  byte k, i;
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
    offset = k * 8;
    for (j = 0; j < 8; j++) {
      idx = (index + offset + j) % TOTAL_LEN;
      max7219(SS_SET[k], j + 1, fonts[DisplayWord[idx >> 3]][idx % 8]);
    }
  }
  delay(160);
  index = (index + 1) % TOTAL_LEN;
}

