#include <MsTimer2.h>

// PIN-LED
//  5 - 1
//  6 - 2
//  7 - 9
//  8 - 10

int Led[4] = {5, 7, 6, 8}; // LED:1,9,2,10

void LedControl() {
  static int counter = 0;
  static byte output = HIGH;
  static int half_cycle = 0;
  static int cycle = 0;

  // LED1 – LED9 – LED2 – LED10의 순서로 ON/OFF
  digitalWrite(Led[counter++], output);
  if (counter > 3) {
    counter = 0;
    output = !output;
    half_cycle++;                   // half_cycle 계산
    if ((half_cycle % 2) == 0) {
      cycle++;                      // cycle 계산                                      
      if (cycle == 4)
        cycle = 0;                  // 4번째 cycle마다 cycle을 0으로 초기화
      // cycle에 따라 delay_time 변경하여 timer 재시작
      MsTimer2::stop();
      MsTimer2::set(500 * (cycle + 1), LedControl);
      MsTimer2::start();
    }
  }
}

void setup() {
  int i;
  for (i = 0; i < 4; i++)
    pinMode(Led[i], OUTPUT);
  MsTimer2::set(500, LedControl);
  MsTimer2::start();
}

void loop() {
}
