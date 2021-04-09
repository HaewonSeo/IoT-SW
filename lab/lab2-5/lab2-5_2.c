#include <wiringPi.h>
#include <softPwm.h>

#define MOTOR_MT_N_PIN	17
#define MOTOR_MT_P_PIN	4
#define LEFT_ROTATE		1
#define RIGHT_ROTATE	2

void MotorStop()
{
	softPwmWrite(MOTOR_MT_N_PIN, 0);
	softPwmWrite(MOTOR_MT_P_PIN, 0);
}

void MotorControl(int speed)
{
	// LEFT_ROTATE
	digitalWrite(MOTOR_MT_P_PIN, LOW);
	softPwmWrite(MOTOR_MT_N_PIN, speed);
}

int main(void)
{
	if (wiringPiSetupGpio() == -1)
		return 1;

	pinMode(MOTOR_MT_N_PIN, OUTPUT);
	pinMode(MOTOR_MT_P_PIN, OUTPUT);
	
	// pwmRange : 100
	softPwmCreate(MOTOR_MT_N_PIN, 0, 100);
	softPwmCreate(MOTOR_MT_P_PIN, 0, 100);

	while(1)
	{
		// duty cycle 25%
		MotorControl(25);
		delay(2000);
		MotorStop();
		delay(1000);

		// duty cycle 50%
		MotorControl(50);
		delay(2000);
		MotorStop();
		delay(1000);

		// duty cycle 75%
		MotorControl(75);
		delay(2000);
		MotorStop();
		delay(1000);
		
		// duty cycle 100%
		MotorControl(100);
		delay(2000);
		MotorStop();
		delay(1000);
	}
	
	return 0;
}
