#include <MsTimer2.h>

#define FALLING LOW
#define RISING  HIGH

int Button[2] = {2, 3};                       // Button Pin
int Led[3] = {5, 6, 7};                       // LED Pin
byte Led16Output;                             // LED16의 On/Off를 결정하는 변수
const int EdgeTrigger[2] = {FALLING, RISING}; // 버튼 edge trigger
const byte LedState[2] = {HIGH, LOW};         // 버튼 입력 시 수행할 동작
int ButtonState[2];                           // 버튼의 현재 상태, toggle 확인을 위해 사용
int LastButtonState[2];                       // 버튼의 이전 상태, debounce 확인을 위해 사용
unsigned long LastDebounceTime[2];            // 마지막으로 버튼의 입력 상태가 바뀐 시간
unsigned long DebounceDelay = 50;             // debounce time

// Timer function
void LedControl() {
  static int counter = 0;
  static byte output = HIGH;

  digitalWrite(Led[counter++], output);    // LED(1, 9) ON/OFF 수행

  if (counter > 1) {
    counter = 0;
    output = !output;
  }
}

// Debounce function
void Debounce(int buttonNum, int edgeTrigger, byte ledState) {
  int reading;

  reading = digitalRead(Button[buttonNum]);

  // 버튼의 입력상태가 변했을 경우 debounceTimer 초기화
  if (reading != LastButtonState[buttonNum])
    LastDebounceTime[buttonNum] = millis();

  // debounceTimer가 debounceDelay 이상일 경우, 버튼의 입력 변화로 인한 동작 수행
  if ((millis() - LastDebounceTime[buttonNum]) > DebounceDelay) {
    // 버튼의 상태가 toggle 되었다면, 버튼 상태 갱신 및 edgeTrigger 확인
    if (reading != ButtonState[buttonNum]) {
      ButtonState[buttonNum] = reading;
      // edgeTrigger 일치하면, Led16 출력값 설정
      if (ButtonState[buttonNum] == edgeTrigger)
        Led16Output = ledState;
    }
  }
  // LED16 On/Off
  digitalWrite(Led[2], Led16Output);
  // 버튼의 debounce를 확인하기 위해, 현재 입력 상태 저장
  LastButtonState[buttonNum] = reading;
}

void setup() {
  int i;

  // 버튼 2개를 pull-up input 으로 설정
  for (i = 0; i < 2; i++)
    pinMode(Button[i], INPUT_PULLUP);

  // LED PIN 을 출력으로 설정
  for (i = 0; i < 3; i++)
    pinMode(Led[i], OUTPUT);

  // TIMER 설정
  MsTimer2::set(500, LedControl);

  // TIMER 시작
  MsTimer2::start();
}

void loop() {
  int i;

  for (i = 0; i < 2; i++)
    Debounce(i, EdgeTrigger[i], LedState[i]);
}
