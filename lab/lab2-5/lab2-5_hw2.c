#include <wiringPi.h>
#include <softPwm.h>

#define LED_PIN	 6			// GPIO6

void LEDPwmControl()
{
	int i;

	// LED duty cycle control : 0% -> 100% -> 0%
	for (i = 0; i < 128; i++)
	{
		softPwmWrite(LED_PIN, i);
		delay(10);
	}
	for (i = 128; i >= 0; i--)
	{
		softPwmWrite(LED_PIN, i);
		delay(10);
	}
}

int main(void)
{
	if (wiringPiSetupGpio() == -1)
		return 1;

	pinMode(LED_PIN, OUTPUT);
	
	// initial pulse width : 0, pwmRange : 128
	softPwmCreate(LED_PIN, 0, 128);

	while(1)
	{
		LEDPwmControl();
	}

	return 0;
}
