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
				vTaskDelay(MS2TICKS(10));
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
	int time = 0;
	int eachTime = 1;
	int tmpTime = 0;
	int receiveReset = 0;


	// 1/100s timer
	while (1)
	{
		//Print time
		tmpTime = time;
		for (pos = 0; pos < 6; pos++)
		{
			FndDisplay(pos, tmpTime % 10);
			tmpTime /= 10;
			vTaskDelay(MS2TICKS(1));
		}

		//10ms delay overall!
		vTaskDelay(MS2TICKS(4));

		//Increase time (+10ms)
		time++;

		//Send eachTime to LedTask every second
		if ((time / 100) == eachTime)
		{
			xQueueSend(xQueue, &eachTime, 0);
			eachTime++;
		}

		//Reset FND
		if (xQueueReceive(xQueue2, &receiveReset, 0))
		{
			if (receiveReset)
			{
				time = 0;
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
