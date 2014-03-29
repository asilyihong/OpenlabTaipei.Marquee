
const int latchPin = 3;  // Pin connected to Pin 12 of 74HC595 (Latch)
const int dataPin  = 2;  // Pin connected to Pin 14 of 74HC595 (Data)
const int clockPin = 4;  // Pin connected to Pin 11 of 74HC595 (Clock)

const byte LEDs[] = {
                    B1111110,
                    B0110000,
                    B1101101,
                    B1111001,
                    B0110011,
                    B1011011,
                    B1011111,
                    B1110000,
                    B1111111,
                    B1111011
};


void sevenSegInit() {

  //set pins to output 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void sevenSegWrite( byte index)
{
  // Use a loop and a bitwise AND to move over each bit that makes up
  // the seven segment display (from left to right, A => G), and check
  // to see if it should be on or not
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, LEDs[index]);
  digitalWrite(latchPin, HIGH);
}
