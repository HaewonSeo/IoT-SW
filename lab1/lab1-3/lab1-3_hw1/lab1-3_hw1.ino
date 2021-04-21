#include <MsTimer2.h>

int Button[2] = {2,3};            // Button Pin
int Led[3] = {5,6,7};             // LED Pin
static byte Led16_output = HIGH;  // LED 16의 On/Off를 결정하는 변수
 
// Timer function
void LedControl(){
   static int counter = 0;
   static byte output = HIGH;

   digitalWrite(Led[counter++], output);    // LED(1, 9) ON/OFF 수행

   if (counter > 1){
      counter = 0;
      output = !output;
   }
}

// ISR
void ButtonLedControl(){
  digitalWrite(Led[2], Led16_output);     // LED 16 On
}

void ButtonLedControl2(){
  digitalWrite(Led[2], !Led16_output);    // LED 16 Off
}

void setup() {
  int i;

  // 버튼 2개를 pull-up input 으로 설정
  for (i=0; i<2; i++)
    pinMode(Button[i], INPUT_PULLUP);

  // LED PIN 을 출력으로 설정
  for (i=0; i<3; i++)
    pinMode(Led[i], OUTPUT);

  // 인터럽트 설정
  attachInterrupt(0, ButtonLedControl, FALLING);
  attachInterrupt(1, ButtonLedControl2, RISING);

  // TIMER 설정
  MsTimer2::set(500, LedControl);

  // TIMER 시작
  MsTimer2::start();
}

void loop() {
}
