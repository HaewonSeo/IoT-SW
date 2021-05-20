#include <FreeRTOS_AVR.h>

const int MT_P = 10;
const int MT_N = 9;
const int LeftKey = 2;
const int StopKey = 3;
const int RightKey = 21;
const int LeftSpeedKey = 20;
const int RightSpeedKey = 19;

QueueHandle_t xQueue;

// ISR, Queue로 데이터를 전달
void LeftKeyControl() {
  uint16_t sendValue = 1;
  xQueueSendFromISR(xQueue, &sendValue, 0);
}

void StopKeyControl() {
  uint16_t sendValue = 2;
  xQueueSendFromISR(xQueue, &sendValue, 0);
}

void RightKeyControl() {
  uint16_t sendValue = 3;
  xQueueSendFromISR(xQueue, &sendValue, 0);
}

void LeftSpeedControl() {
  uint16_t sendValue = 4;
  xQueueSendFromISR(xQueue, &sendValue, 0);
}

void RightSpeedControl() {
  uint16_t sendValue = 5;
  xQueueSendFromISR(xQueue, &sendValue, 0);
}

void MotorTask(void * arg) {
  uint16_t receiveValue = 0;
  const int motorSpeed[4] = {128, 192, 255, 64};
  int left = 0, right = 0;
  
  while (1) {
    // Queue로 부터 데이터를 받음
    if ( xQueueReceive(xQueue, &receiveValue, 0) ) {
      // Rotate left
      if (receiveValue == 1) {
        digitalWrite(MT_P, LOW);
        analogWrite(MT_N, motorSpeed[3]);
      }
      // Stop
      else if (receiveValue == 2) {
        digitalWrite(MT_P, LOW);
        digitalWrite(MT_N, LOW);
      }
      // Rotate right
      else if (receiveValue == 3) {
        analogWrite(MT_P, motorSpeed[3]);
        digitalWrite(MT_N, LOW);
      }
      // Rotate left(speed control)
      else if (receiveValue == 4) {
        digitalWrite(MT_P, LOW);
        analogWrite(MT_N, motorSpeed[left]);
        left = (left + 1) % 3;
      }
      // Rotate right(speed control)
      else if (receiveValue == 5) {
        analogWrite(MT_P, motorSpeed[right]);
        digitalWrite(MT_N, LOW);
        right = (right + 1) % 3;
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
  pinMode(LeftSpeedKey, INPUT);
  pinMode(RightSpeedKey, INPUT);
  
  // 인터럽트 설정, lab1-3 참고
  attachInterrupt(0, LeftKeyControl, RISING);
  attachInterrupt(1, StopKeyControl, RISING);
  attachInterrupt(2, RightKeyControl, RISING);
  attachInterrupt(3, LeftSpeedControl, RISING);
  attachInterrupt(4, RightSpeedControl, RISING);
    
  xQueue = xQueueCreate(5, sizeof(uint16_t));
  
  if (xQueue != NULL) {
    xTaskCreate(MotorTask, NULL, 200, NULL, 1, NULL);
    vTaskStartScheduler();
  }
}
void loop() {
}
