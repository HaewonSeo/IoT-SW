#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

const int FndSelectPin[6] = {4, 17, 18, 27, 22, 23};	//S0~S5
const int FndPin[8] = {6, 12, 13, 16, 19, 20, 26, 21};	//A~H
const int FndFont[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,	//0~9
						0x6D, 0x7D, 0x07, 0x7F, 0x67};
const int Keypad[3] = {11, 9, 10};						//1~3

int time;
int startTime;
int stopTime;
int stopFlag;
int tmpTime;


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

int KeypadRead()
{
	int i;
	int keypadnum = -1;

	for (i = 0; i < 3; i++)
	{
		if (!digitalRead(Keypad[i]))
		{
			keypadnum = i;
			break;
		}
	}

	return keypadnum;
}

// ISR
void TimerStart()
{
	if (!stopFlag)
		return ;
	
	stopFlag = 0;
	startTime = millis();

}

void TimerStop()
{
	if (stopFlag)
		return ;

	stopFlag = 1;
	stopTime = time * 10;	// because 1/100s timer 

}

void TimerReset()
{
	stopFlag = 0;
	startTime = millis();
	stopTime = 0;
}

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
		digitalWrite(FndSelectPin[i], HIGH);		// FND select pin off
	}

	for (i = 0; i < 8; i++)
	{
		pinMode(FndPin[i], OUTPUT);
		digitalWrite(FndPin[i], LOW);				// FND LED off
	}

	for (i = 0; i < 3; i++)
		pinMode(Keypad[i], INPUT);
	
	// Set ISR
	if(wiringPiISR(Keypad[0], INT_EDGE_FALLING, &TimerStart) < 0)
		exit(-1);
	if(wiringPiISR(Keypad[1], INT_EDGE_FALLING, &TimerStop) < 0)
		exit(-1);
	if(wiringPiISR(Keypad[2], INT_EDGE_FALLING, &TimerReset) < 0)
		exit(-1);
}

int main()
{
	int pos;

	Init();

	// If key1 is pressed, then timer is start!
	while (KeypadRead() != 0)
	{
		startTime = millis();
	}

	while(1)
	{
		// Compute now time
		if(!stopFlag)
			time = (millis() - startTime + stopTime) / 10;
		
		// Display the FND Timer
		tmpTime = time;
		for (pos = 0; pos < 6; pos++)
		{
			FndDisplay(pos, tmpTime % 10);
			tmpTime /= 10;
			delay(1);
		}
	}

	return 0;
}
