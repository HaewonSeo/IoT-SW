#include <wiringPi.h>

const int LedRed[8] = {4, 17, 18, 27, 22, 23, 24, 25};
const int Keypad[8] = {6, 12, 13, 16, 19};				// key6,2,3,4,5
int INT_Flag;	// To break loop

//ISR
void LEDStop()
{
	int i;

	for (i = 0; i < 8; i++)
		digitalWrite(LedRed[i], LOW);
	
	INT_Flag = 1;		// Set Interrupt Flag
}

int KeypadRead()
{
	int i, keypadnum = -1;
	
	for (i = 0; i < 5; i++)
	{
		if (!digitalRead(Keypad[i]))
		{	
			keypadnum = i;
			break;
		}
	}
	return keypadnum;
}

void LEDControl(int keypadnum)
{
	int i;
	
	for (i = 0; i < 8; i++)
	{
		if (i == keypadnum)
			digitalWrite(LedRed[i], HIGH);
		else
			digitalWrite(LedRed[i], LOW);
	}
}

void LEDUp()
{
	int i;

	for (i = 0; i < 8; i++)
	{
		if (INT_Flag)
			break;
		LEDControl(i);
		delay(500);
	}
	digitalWrite(LedRed[i-1], LOW);
}

void LEDDown()
{
	int i;

	for (i = 7; i >= 0; i--)
	{
		if (INT_Flag)
			break;
		LEDControl(i);
		delay(500);
	}
	digitalWrite(LedRed[i+1], LOW);
}

int main()
{
	int i, keypadnum = -1;

	// Error Handling
	if (wiringPiSetupGpio() == -1)
		return 1;

	// Setup
	if (wiringPiISR(Keypad[2], INT_EDGE_FALLING, &LEDStop) < 0)
		return 1;

	for (i = 0; i < 8; i++)
	{
		pinMode(LedRed[i], OUTPUT);
		digitalWrite(LedRed[i], LOW);
	}

	for (i = 0; i < 5; i++)
		pinMode(Keypad[i], INPUT);
	
	// Loop
	while (1)
	{
		keypadnum = KeypadRead();
		INT_Flag = 0;

		if (keypadnum == 0)
		{
			while(1)
			{
				LEDUp();
				if (INT_Flag)
					break;
			}
		}
		else if (keypadnum == 1)
		{
			while(1)
			{
				LEDDown();
				if (INT_Flag)
					break;
			}
		}
		else if (keypadnum == 3)
		{
			LEDUp();
		}
		else if (keypadnum == 4)
		{
			LEDDown();
		}
	}	

	return 0;
}

