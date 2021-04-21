#include <MsTimer2.h>

// PIN-LED
//  5 - 1
//  6 - 2
//  7 - 9
//  8 - 10

int Led[4] = {5, 7, 6, 8}; // LED:1,9,2,10
int Led16 = 9; // LED16을 9번 digital pin에 연결

void LedControl() {
  static int counter = 0;
  static byte output = HIGH;
  static byte output16 = HIGH;

  digitalWrite(Led[counter++], output);
  if (counter > 3) {
    counter = 0;
    output = !output;
  }
  // 1초마다 LED16을 점등한다.
  if (counter % 2 == 0)
  {
    digitalWrite(Led16, output16);
    output16 = !output16; 
  }
}

void setup() {
  int i;
  for (i = 0; i < 4; i++)
    pinMode(Led[i], OUTPUT);
  pinMode(Led16, OUTPUT);
  MsTimer2::set(500, LedControl);
  MsTimer2::start();
}

void loop() {
}
