#include <MsTimer2.h>

int Button[2] = {2,3};            // Button Pin
int Led[3] = {5,6,7};             // LED Pin
static byte Led16_output = HIGH;  // LED 16의 On/Off를 결정하는 변수

static int buttonState[2] = {0, };      // 버튼의 현재 상태
static int lastButtonState[2] = {0, };  // 버튼의 이전 상태
static bool button0StateFalling = false;// button0의 falling edge 여부
static bool button1StateRising = false; // button1의 rising edge 여부


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

void setup() {
  int i;

  // 버튼 2개를 pull-up input 으로 설정
  pinMode(Button[0], INPUT_PULLUP);
  pinMode(Button[1], INPUT_PULLUP);

  // LED PIN 을 출력으로 설정
  for (i=0; i<3; i++)
    pinMode(Led[i], OUTPUT);

  // TIMER 설정
  MsTimer2::set(500, LedControl);

  // TIMER 시작
  MsTimer2::start();
}


void loop() {
  int i;
  
  // 각 버튼의 현재 상태 read
  for (i=0; i<2; i++)
    buttonState[i] = digitalRead(Button[i]);

  // 버튼2의 상태가 변했고, 동시에 현재 상태가 0인 경우 falling edge
  if ((buttonState[0] != lastButtonState[0]) && (buttonState[0] == LOW))
    button0StateFalling = true;
  else
    button0StateFalling = false;

  // 버튼3의 상태가 변했고, 동시에 현재 상태가 1인 경우 rising edge
  if ((buttonState[1] != lastButtonState[1]) && (buttonState[1] == HIGH))
    button1StateRising = true;
  else
    button1StateRising = false;

  // LED16 On/Off
  if (button0StateFalling)
    digitalWrite(Led[2], Led16_output);   // LED16 On
  if (button1StateRising)
    digitalWrite(Led[2], !Led16_output);  // LED16 Off
  
  // 버튼의 이전 상태 저장
  for (i=0; i<2; i++)
    lastButtonState[i] = buttonState[i];
}
