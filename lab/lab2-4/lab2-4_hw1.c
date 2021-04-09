#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

const int FndSelectPin[6] = {4, 17, 18, 27, 22, 23};	//S0~S5
const int FndPin[8] = {6, 12, 13, 16, 19, 20, 26, 21};	//A~H
const int FndFontHello[4] = {0x76, 0x79, 0x38, 0x3f};	//H,E,L,O 
const int TimerDelay = 500;

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
		flag = (FndFontHello[num] & shift);
		digitalWrite(FndPin[i], flag);
		shift <<= 1;
	}

	FndSelect(pos);	
}

int main()
{
	int i;
	int hello_pos[5] = {4, 3, 2, 1, 0};		// initialize HELLO position
	const int hello[5] = {0, 1, 2, 2, 3};	// HELLO
	int timer;

	Init();

	timer = millis();							// initialize timer
	while(1)
	{
		for (i = 0; i < 5; i++)
		{
			FndDisplay(hello_pos[i], hello[i]);
			delay(1);
		}
		if (millis() - timer > TimerDelay)		// every 500ms 
		{
			for (i = 0; i < 5; i++)				// shift HELLO to the left
				hello_pos[i] = (hello_pos[i] + 1) % 6;
			timer = millis();					// update timer
		}
	}

	return 0;
}
