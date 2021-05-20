#include <FreeRTOS_AVR.h>
#define   MS2TICKS(ms) (ms / portTICK_PERIOD_MS)
#define   MAX_BUF  3

typedef struct  {
  int buf[MAX_BUF];
  int in;
  int out;
} CircularQueueType;
CircularQueueType C_Queue;

const int MT_P = 10;
const int MT_N = 9;
const int LeftKey = 2;
const int StopKey = 3;
const int RightKey = 21;

SemaphoreHandle_t Mutex, Empty, Full;
int SendValue;

// Keypad ISR
// 전역 변수(SendValue)로 값을 변경
void LeftKeyControl() {
  SendValue = 1;
}

void StopKeyControl() {
  SendValue = 2;
}

void RightKeyControl() {
  SendValue = 3;
}

// Producer
void KeypadTask() {
  int lastSendValue = 0;

  while (1) {
    //Serial.print("producer\n");
    //delay(1)을 주지 않으면 ISR로 SendValue가 변경되었음에도 불구하고
    //if문 내의 코드가 실행되지 않는다.
    delay(1);
    if (SendValue != lastSendValue) {
      if (xSemaphoreTake(Empty, portMAX_DELAY)) {     //wait(Empty);
        if (xSemaphoreTake(Mutex, portMAX_DELAY)) {   //wait(Mutex);
          //Serial.print("Producer: Producing an item.....  SendValue : ");
          //Serial.println(SendValue);
          C_Queue.buf[C_Queue.in] = SendValue;
          C_Queue.in = (C_Queue.in + 1) % MAX_BUF;
          lastSendValue = SendValue;

          xSemaphoreGive(Mutex); //signal(Mutex);
          xSemaphoreGive(Full);  //signal(Full);
        }
      }
    }
  }
}

// Consumer
void MotorTask() {
  int receiveValue = 0;

  while (1) {
    //Serial.print("consumer\n");
    if (xSemaphoreTake(Full, portMAX_DELAY)) {        //wait(Full);
      if (xSemaphoreTake(Mutex, portMAX_DELAY)) {     //wait(Mutex);
        receiveValue = C_Queue.buf[C_Queue.out];
        C_Queue.out = (C_Queue.out + 1) % MAX_BUF;
        //Serial.print("Consumer: Consuming an item.....   receiveValue : ");
        //Serial.println(receiveValue);
        
        if (receiveValue == 1) {
          digitalWrite(MT_P, LOW);
          digitalWrite(MT_N, HIGH);
        }
        else if (receiveValue == 2) {
          digitalWrite(MT_P, LOW);
          digitalWrite(MT_N, LOW);
        }
        else if (receiveValue == 3) {
          digitalWrite(MT_P, HIGH);
          digitalWrite(MT_N, LOW);
        }
        xSemaphoreGive(Mutex);  //signal(Mutex);
        xSemaphoreGive(Empty);  //signal(Empty);
      }
    }
  }
}

void setup() {
  //Serial.begin(9600);
  // pinMode 설정
  pinMode(MT_P, OUTPUT);
  pinMode(MT_N, OUTPUT);
  pinMode(LeftKey, INPUT);
  pinMode(StopKey, INPUT);
  pinMode(RightKey, INPUT);

  // Interrupt 설정
  attachInterrupt(0, LeftKeyControl, RISING);
  attachInterrupt(1, StopKeyControl, RISING);
  attachInterrupt(2, RightKeyControl, RISING);

  // Semaphore 생성
  // SemaphoreHandle_t xSemaphoreCreateBinary( void );
  // SemaphoreHandle_t xSemaphoreCreateCounting( UBaseType_t uxMaxCount,
  //                                          UBaseType_t uxInitialCount);
  // Mutex semaphore의 초기값을 1으로 설정
  // Empty semaphore의 초기값을 MAX_BUF으로 설정
  // Full semaphore의 초기값을 0으로 설정
  Mutex = xSemaphoreCreateBinary();
  xSemaphoreGive(Mutex);
  Empty = xSemaphoreCreateCounting(MAX_BUF, MAX_BUF);
  Full = xSemaphoreCreateCounting(MAX_BUF, 0);

  // Task 생성
  xTaskCreate(KeypadTask, NULL, 200, NULL, 1, NULL);
  xTaskCreate(MotorTask, NULL, 200, NULL, 2, NULL);
  vTaskStartScheduler();
}

void loop() {
}
