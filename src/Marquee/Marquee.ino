
#include <SPI.h>

#include "fonts.h"
#include "config.h"

#define SWITCH_PIN 12
#define MIN_ASCII 22
#define BIT_CNT (SS_SIZE << 3)
#define NOOP 0x0
#define DECODEMODE 0x9
#define INTENSITY 0xA
#define SCANLIMIT 0xB
#define SHUTDOWN 0xC
#define DISPLAYTEST 0xF

const byte SS_SET[] = {10, 9, 8, 7, 6, 5, 4, 3};
const int INSTANCE_CNT = sizeof(DISPLAY_WORDS) / sizeof(char*);
byte buffer[SS_SIZE << 3] = {0};
int switchFlag = 1;
int instanceIdx = 0;
byte index = 0;
byte TOTAL_LEN;
byte addBlank = 0;
char *displayWord;
unsigned long prevTime = 0;

byte getNextByte() {
  byte chr, res;
  if (addBlank > 0) {
    addBlank--;
    res = 0;
  } else {
    chr = displayWord[index >> 3];
    if (chr < MIN_ASCII || chr > 128) {
      chr = ' ';
    }

    res = fonts[chr - MIN_ASCII][index & 7];
    index = (index + ((chr == ' ')? 2 : 1)) % TOTAL_LEN; // reduce the width when display space
    if (((index & 7) == 0) && chr < ' ') {
      addBlank = FONT_SPACE;
    }
  }
  return res;
}

void switchText(int idx, boolean needAnimation) {
  byte k, j, i, b, mask = 1;
  byte chr;

  index = 0; // reset the index
  displayWord = (char *)DISPLAY_WORDS[idx]; // change the string will display
  char *str = displayWord;
  while(*str) { // count the string length
    str++;
  }
  TOTAL_LEN = ((int)(str - displayWord)) << 3;

  if (needAnimation) {
    for (i = 0; i < 8; i++) {
      for (j = 0; j < BIT_CNT; j++) {
        max7219(SS_SET[j >> 3], (j & 7) + 1, buffer[j] | mask);
      }

      mask <<= 1;
      mask |= 0x01;
      delay(FLASH_INTERVAL);
    }
  
    for (k = 0; k < BIT_CNT; k++) { // assign how to diaplsy words to buffer
      buffer[k] = getNextByte();
    }

    mask = 0xFF;
    for (i = 0; i < 8; i++) {
      mask >>= 1;
      for (j = 0; j < BIT_CNT; j++) {
        max7219(SS_SET[j >> 3], (j & 7) + 1, buffer[j] | mask);
      }

      delay(FLASH_INTERVAL);
    }
  }
  prevTime = millis(); // reset the prevTime
#ifdef LED_INDICATOR
  setCurrIdx(idx + 1, prevTime);
#endif // LED_INDICATOR
}

void max7219(byte pin, byte reg, byte data) {
  digitalWrite(pin, LOW);
  SPI.transfer(reg);
  SPI.transfer(data);
  digitalWrite(pin, HIGH);
}

void setup() {
  byte k, i;

  pinMode(SWITCH_PIN, INPUT);
#ifdef LED_INDICATOR
  idxInit();
#endif // LED_INDICATOR
  SPI.begin();
  for (k = 0; k < SS_SIZE; k++) {
    pinMode(SS_SET[k], OUTPUT);
    digitalWrite(SS_SET[k], HIGH);
    max7219(SS_SET[k], SCANLIMIT, 7);
    max7219(SS_SET[k], DECODEMODE, 0);
    max7219(SS_SET[k], INTENSITY, 8);
    max7219(SS_SET[k], DISPLAYTEST, 0);
    max7219(SS_SET[k], SHUTDOWN, 1);
      
    for (i = 0; i < 8; i++) {
      max7219(SS_SET[k], i + 1, 0);
    }
  }
  
  switchText(0, false);
}

void loop() {
  byte j, chr;
  int switchInput = digitalRead(SWITCH_PIN);
  unsigned long currTime = 0;
  if (switchInput == 1 && switchInput != switchFlag) { // detech press button and prevent reproduce trigger
    switchFlag = switchInput;
    instanceIdx = (instanceIdx + 1) % INSTANCE_CNT;
    switchText(instanceIdx, true);
  } else if (switchInput != switchFlag) {
    switchFlag = switchInput;
  }

  currTime = millis();
  if (currTime - prevTime >= DELAY_INTERVAL) {
    
    for (j = 0; j < BIT_CNT - 1; j++) {
      max7219(SS_SET[j >> 3], (j & 7) + 1, buffer[j]);
      buffer[j] = buffer[j + 1];
    }
    max7219(SS_SET[j >> 3], (j & 7) + 1, buffer[j]);
    buffer[BIT_CNT - 1] = getNextByte();
    prevTime = currTime;
  }
#ifdef LED_INDICATOR
  ledBlink(currTime);
#endif // LED_INDICATOR
}
