#ifdef LED_INDICATOR

unsigned long prevLedTime = 0;
int ledIdxMax = 0;
int currLedIdx = 0;
byte voltIdx = LOW;

void idxInit() {
  pinMode(LED_IDX_PIN, OUTPUT);
}

void setCurrIdx(int idx, unsigned long currTime) {
  digitalWrite(LED_IDX_PIN, LOW);
  currLedIdx = ledIdxMax = idx;
  voltIdx = HIGH;
  prevLedTime = currTime;
}

void ledRealBlink(unsigned long currTime, int timeInt, int interval) {
  if (currTime - prevLedTime > timeInt) {
    digitalWrite(LED_IDX_PIN, voltIdx);
    voltIdx = (voltIdx == HIGH)? LOW : HIGH;
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
  } else if (currTime - prevLedTime > LED_DLEAY_INT) {
    currLedIdx = ledIdxMax;
  }
}
#endif // LED_INDICATOR
