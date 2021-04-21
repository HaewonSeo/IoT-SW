#include <MsTimer2.h>

// PIN-LED
//  5 - 1
//  6 - 2
//  7 - 9
//  8 - 10

int Led[4] = {5,7,6,8}; // LED:1,9,2,10

void LedControl() {
 static int counter = 0;
 static byte output = HIGH;
 
 // LED1 – LED9 – LED2 – LED10의 순서로 ON/OFF
 digitalWrite(Led[counter++], output);
 if(counter > 3){
    counter = 0;
    output = !output;
 }
}

void setup() {
  int i;
  for(i=0;i<4;i++)
    pinMode(Led[i], OUTPUT);
  MsTimer2::set(500, LedControl);
  MsTimer2::start();
}

void loop() {
}
