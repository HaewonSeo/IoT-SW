#include <FreeRTOS_AVR.h>
#define MS2TICKS(ms) (ms / portTICK_PERIOD_MS)
QueueHandle_t xQueue;
QueueHandle_t xQueue2;

const int Led[8] = {3, 4, 5, 6, 7, 8, 9, 10};
const int FndSelectPin[6] = {22, 23, 24, 25, 26, 27};
const int FndPin[8] = {30, 31, 32, 33, 34, 35, 36, 37};
const int FndFont[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,
						 0x6D, 0x7D, 0x07, 0x7F, 0x67};
int SendValue = 0;

void LedTask(void *arg)
{
	int receive_eachTime = 0;
	int second_sum = 0;
	int i = 0;
	int j = 0;
	int sendReset = 1;

	while (1)
	{
		if (xQueueReceive(xQueue, &receive_eachTime, 0))
		{
			second_sum += receive_eachTime;

			//LED ON
			if (second_sum / 10 >= i + 1)
			{
				digitalWrite(Led[i], HIGH);
				i++;
			}

			//Reset LED & send sendReset to FndTask
			if (i == 8)
			{
				second_sum = 0;
				i = 0;

				//Reset LED
				for (j = 0; j < 8; j++)
					digitalWrite(Led[j], LOW);

				//Reset FND
				xQueueSend(xQueue2, &sendReset, 0);
				vTaskDelay(MS2TICKS(100));
			}
		}
	}
}

void FndSelect(int pos)
{
	int i;

	for (i = 0; i < 6; i++)
	{
		if (i == pos)
		{
			digitalWrite(FndSelectPin[i], LOW);
		}
		else
		{
			digitalWrite(FndSelectPin[i], HIGH);
		}
	}
}

void FndDisplay(int pos, int num)
{
	int i;
	int flag = 0;
	int shift = 0x01;

	FndSelect(pos);
	for (i = 0; i < 8; i++)
	{
		flag = (FndFont[num] & shift);
		digitalWrite(FndPin[i], flag);
		shift <<= 1;
	}
	if (pos == 2) // pos2 decimal point on
		digitalWrite(FndPin[7], 1);
}

void FndTask(void *arg)
{
	int pos = 0;
	int startTime = 0;
	unsigned long time = 0; /* int(2bytes) -> unsigned long(4bytes)로 변경 */
	int tmpTime = 0;
	int eachTime = 1;
	int receiveReset = 0;

	startTime = millis();

	while (1)
	{
		time = (millis() - startTime) / 10; // 1/100s timer

		//Send eachTime to LedTask every second
		if ((time / 100) == eachTime)
		{
			xQueueSend(xQueue, &eachTime, 0);
			eachTime++;
		}

		//Print time
		tmpTime = time;
		for (pos = 0; pos < 6; pos++)
		{
			FndDisplay(pos, tmpTime % 10);
			tmpTime /= 10;
			vTaskDelay(MS2TICKS(1));
		}

		//Reset FND
		if (xQueueReceive(xQueue2, &receiveReset, 0))
		{
			if (receiveReset)
			{
				startTime = millis();
				time = 0;
				tmpTime = 0;
				eachTime = 1;
				receiveReset = 0;
			}
			vTaskDelay(MS2TICKS(10));
		}

	}
}

void setup()
{
	int i;

	for (i = 0; i < 6; i++)
	{
		pinMode(FndSelectPin[i], OUTPUT);
		digitalWrite(FndSelectPin[i], HIGH); // select pin off
	}

	for (i = 0; i < 8; i++)
	{
		pinMode(Led[i], OUTPUT);
		digitalWrite(Led[i], LOW);

		pinMode(FndPin[i], OUTPUT);
		digitalWrite(FndPin[i], LOW);
	}

	xQueue = xQueueCreate(1, sizeof(int));
	xQueue2 = xQueueCreate(1, sizeof(int));

	if (xQueue != NULL && xQueue2 != NULL)
	{
		xTaskCreate(FndTask, NULL, 200, NULL, 2, NULL);
		xTaskCreate(LedTask, NULL, 200, NULL, 1, NULL);
		vTaskStartScheduler();
	}
}

void loop()
{
}

/*

문제 상황
3번 반복까지는 정상적인데, 그 이후로는 Timer 초기화시 time이 60000 정도의 값을 갖는 문제
모든 LED는 정상적으로 OFF 된다. 그러나 Timer의 시간만이 비정상
Why?


해결 방법 1 : 자료형 크기 변경
millis()로 인한 오류였다.
현재 millis()를 int형에 저장했기 때문에 int의 범위를 초과하여 나타난 문제
unsigned long로 수정하여 해결할 수 있다. -> timer를 50일까지 사용가능
참고로 아두이노의 int/long은 C와 범위가 다르다.
아두이노 우노 : int(2bytes), long(4bytes)
참고 : https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=darknisia&logNo=221436915696

해결 방법 2 : delay()를 이용한 timer 계산
-> Final_solution.ino

*/
