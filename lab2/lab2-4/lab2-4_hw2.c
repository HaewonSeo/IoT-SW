#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

const int FndSelectPin[6] = {4, 17, 18, 27, 22, 23};	//S0~S5
const int FndPin[8] = {6, 12, 13, 16, 19, 20, 26, 21};	//A~H
const int FndFont[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,	//0~9
						0x6D, 0x7D, 0x07, 0x7F, 0x67};

void Init()
{
	int i;

	if (wiringPiSetupGpio() == -1)
	{
		printf("wiringPiSetupGpio() error\n");
		exit(-1);
	}

	for (i = 0; i < 6; i++)
	{
		pinMode(FndSelectPin[i], OUTPUT);
		digitalWrite(FndSelectPin[i], HIGH);	// select pin off
	}

	for (i = 0; i < 8; i++)
	{
		pinMode(FndPin[i], OUTPUT);
		digitalWrite(FndPin[i], LOW);		// LED off
	}
}

void FndSelect(int pos)
{
	int i;

	for (i = 0; i < 6; i++)
	{
		if (i == pos)
			digitalWrite(FndSelectPin[i], LOW);		// select pin on
		else
			digitalWrite(FndSelectPin[i], HIGH);	// select pin off
	}
}

void FndDisplay(int pos, int num)
{
	int i;
	int flag = 0;
	int shift = 0x01;

	for (i = 0; i < 8; i++)
	{
		flag = (FndFont[num] & shift);
		digitalWrite(FndPin[i], flag);
		shift <<= 1;
	}
	if (pos == 2)						// pos2 decimal point on
		digitalWrite(FndPin[7], 1);

	FndSelect(pos);
}

int main()
{
	int pos;
	int startTime;
	int time;

	Init();

	startTime = millis();
	while(1)
	{
		time = (millis() - startTime) / 10;		 // 1/100s timer
		for (pos = 0; pos < 6; pos++)			 // print time
		{
			FndDisplay(pos, time % 10);
			time /= 10;
			delay(1);
		}
	}

	return 0;
}
