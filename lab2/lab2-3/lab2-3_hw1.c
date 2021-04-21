#include <wiringPi.h>
#include <stdio.h>

const int LedRed[8] = {4, 17, 18, 27, 22, 23, 24, 25};
const int Keypad[8] = {16, 13, 12, 6, 21, 26, 20, 19};

int	LedState[8];
int	LedChanged[8];			// to react only on the first toggle every debounce
int	LastButtonState[8];
unsigned long LastDebounceTime[8];
const unsigned long DebounceDelay = 50;

// Consider Debounce
void KeypadControlLED()
{
	int i;
	int reading;

	for (i = 0; i < 8; i++)
	{
		reading = digitalRead(Keypad[i]);

		// when button state is changed
		if (reading != LastButtonState[i])
		{
			LastDebounceTime[i] = millis();
			LedChanged[i] = 0;
		}

		// debounce
		if (millis() - LastDebounceTime[i] > DebounceDelay)
		{
			// if button is pressed
			if (!reading && !LedChanged[i]) 
			{ 
				LedState[i] = !LedState[i];
				LedChanged[i] = 1;
			}
		}			
		// Led control
		digitalWrite(LedRed[i], LedState[i]);

		// keep buttion state
		LastButtonState[i] = reading;
	}
}

int main()
{
	int i, keypadnum = -1;

	if (wiringPiSetupGpio() == -1)
		return 1;
	
	// Setup
	for (i = 0; i < 8; i++)
	{
		pinMode(LedRed[i], OUTPUT);
		digitalWrite(LedRed[i], LOW);
	}

	for (i = 0; i < 8; i++)
	{
		pinMode(Keypad[i], INPUT);
	}

	// Loop
	while (1)
	{
		KeypadControlLED();
	}
	
	return 0;
}

