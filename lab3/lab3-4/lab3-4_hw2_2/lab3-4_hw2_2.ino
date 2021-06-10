#include <CircularBuffer.h>
#include <FreeRTOS_AVR.h>

#define MS2TICKS(ms) (ms / portTICK_PERIOD_MS)
#define MAX_BUF 4

const int MT_P = 10;
const int MT_N = 9;
const int LeftKey = 1;
const int StopKey = 2;
const int RightKey = 21;
SemaphoreHandle_t Sem, empty, full;
int SendValue = 0;
CircularBuffer<int, MAX_BUF> Buf;

// FND의 값을 왼쪽으로 Shift하고
// 파라미터로 들어온 새로운 값(data)를 0번지에 넣는 함수
void Insert(int data)
{
	if (!Buf.isFull())
	{
		Buf.push(data);
		xSemaphoreGive(full);
	}
}

// Keypad ISR
// 전역 변수(sendValue)로 값을 전달하기 때문에 세마포로 보호
void LeftKeyControl()
{
	delay(50);
	SendValue = 1;
	xSemaphoreGive(empty);
	xSemaphoreGive(Sem);
}

void StopKeyControl()
{
	delay(50);
	SendValue = 2;
	xSemaphoreGive(empty);
	xSemaphoreGive(Sem);
}

void RightKeyControl()
{
	delay(50);
	SendValue = 3;
	xSemaphoreGive(empty);
	xSemaphoreGive(Sem);
}

void KeypadTask(void *arg)
{
	int i;
	int keypad;
	while (1)
	{
		// 세마포어를 통해 키패드가 눌렸음을 Keypad ISR로부터
		// 전달 받음
		// portMAX_DELAY: 세마포를 받기 전까지 Block
		if (xSemaphoreTake(Sem, portMAX_DELAY))
		{
			keypad = SendValue;
			if (xSemaphoreTake(empty, portMAX_DELAY))
			{
				Insert(keypad);
			}
		}
	}
}

void MotorTask(void *arg)
{
	int i, keypadnum;
	while (1)
	{
		for (i = 0; i < MAX_BUF; i++)
		{
			if (!Buf.isEmpty())
			{
				if (xSemaphoreTake(full, portMAX_DELAY))
				{
					keypadnum = Buf.pop();
					delay(1);
					if (keypadnum == 1)
					{
						digitalWrite(MT_P, LOW);
						digitalWrite(MT_N, HIGH);
					}
					// Stop
					else if (keypadnum == 2)
					{
						digitalWrite(MT_P, LOW);
						digitalWrite(MT_N, LOW);
					}
					// Rotate right
					else if (keypadnum == 3)
					{
						digitalWrite(MT_P, HIGH);
						digitalWrite(MT_N, LOW);
					}
				}
			}
		}
	}
}

void setup()
{
	pinMode(MT_P, OUTPUT);
	pinMode(MT_N, OUTPUT);
	pinMode(LeftKey, INPUT);
	pinMode(StopKey, INPUT);
	pinMode(RightKey, INPUT);

	// 인터럽트 설정, lab1-3 참고
	attachInterrupt(0, LeftKeyControl, RISING);
	attachInterrupt(1, StopKeyControl, RISING);
	attachInterrupt(2, RightKeyControl, RISING);

	vSemaphoreCreateBinary(Sem);
	empty = xSemaphoreCreateCounting(4, 4);
	full = xSemaphoreCreateCounting(4, 0);
	xTaskCreate(KeypadTask, NULL, 200, NULL, 2, NULL);
	xTaskCreate(MotorTask, NULL, 200, NULL, 1, NULL);

	vTaskStartScheduler();
}

void loop()
{
}
