#define LED_IDX_PIN 2
#define LED_LLONG_INT 120
#define LED_LONG_INT 120
#define LED_SHORT_INT 40
#define LED_DLEAY_INT 1000

unsigned long prevLedTime = 0;
int ledIdxMax = 0;
int currLedIdx = 0;
byte voltIdx = 0;

void idxInit() {
  pinMode(LED_IDX_PIN, OUTPUT);
}

void setCurrIdx(int idx, unsigned long currTime) {
  currLedIdx = ledIdxMax = idx;
  voltIdx = 1;
  prevLedTime = currTime;
}

void ledRealBlink(unsigned long currTime, int timeInt, int interval) {
  if (currTime - prevLedTime > timeInt) {
    digitalWrite(LED_IDX_PIN, voltIdx);
    voltIdx != voltIdx;
    if (voltIdx) {
      currLedIdx -= interval;
    }
    prevLedTime = currTime;
  }
}

void ledBlink(unsigned long currTime) {
  if (currLedIdx >= 10) { // index 10
    ledRealBlink(currTime, LED_LLONG_INT, 10);
  } else if (currLedIdx >= 5) { // index 5
    ledRealBlink(currTime, LED_LONG_INT, 5);
  } else if (currLedIdx > 0) {
    ledRealBlink(currTime, LED_SHORT_INT, 1);
  } else if (currTime - prevLedTime) {
    currLedIdx = ledIdxMax;
  }
}
