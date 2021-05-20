#include <FreeRTOS_AVR.h>
#define MS2TICKS(ms) (ms / portTICK_PERIOD_MS)

const int MT_P = 10;
const int MT_N = 9;
const int LeftKey = 2;
const int StopKey = 3;
const int RightKey = 21;

SemaphoreHandle_t Sem;
int SendValue = 0;

// Keypad ISR
// 전역 변수(sendValue)로 값을 전달하기 때문에 세마포로 보호
void LeftKeyControl() {
  SendValue = 1;
  xSemaphoreGive(Sem);
}

void StopKeyControl() {
  SendValue = 2;
  xSemaphoreGive(Sem);
}

void RightKeyControl() {
  SendValue = 3;
  xSemaphoreGive(Sem);
}

void MotorTask(void* arg) {
  int receiveValue = 0;
  
  while (1) {
    // 세마포어를 통해 키패드가 눌렸음을 Keypad ISR로부터 전달 받음
    // portMAX_DELAY: 세마포를 받기 전까지 Block
    if (xSemaphoreTake(Sem, portMAX_DELAY)) {
      receiveValue = SendValue;
      // Rotate left
      if (receiveValue == 1) {
        digitalWrite(MT_P, LOW);
        digitalWrite(MT_N, HIGH);
      }
      // Stop
      else if (receiveValue == 2) {
        digitalWrite(MT_P, LOW);
        digitalWrite(MT_N, LOW);
      }
      // Rotate right
      else if (receiveValue == 3) {
        digitalWrite(MT_P, HIGH);
        digitalWrite(MT_N, LOW);
      }
    }
  }
}

void setup() {

  pinMode(MT_P, OUTPUT);
  pinMode(MT_N, OUTPUT);
  pinMode(LeftKey, INPUT);
  pinMode(StopKey, INPUT);
  pinMode(RightKey, INPUT);
  
  attachInterrupt(0, LeftKeyControl, RISING);
  attachInterrupt(1, StopKeyControl, RISING);
  attachInterrupt(2, RightKeyControl, RISING);

  vSemaphoreCreateBinary(Sem);
  xTaskCreate(MotorTask, NULL, 200, NULL, 1, NULL);
  vTaskStartScheduler();
}

void loop() {
}
