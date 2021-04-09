#include <wiringPi.h>
#include <softPwm.h>

#define MOTOR_MT_N_PIN	17
#define MOTOR_MT_P_PIN	4
#define LEFT_ROTATE		1
#define RIGHT_ROTATE	2

const int LEFT_ROTATE_CYCLE[4] = {32, 64, 96, 128};		// 25, 50, 75, 100 %
const int RIGHT_ROTATE_CYCLE[4] = {128, 96, 64, 32};	// 100, 75, 50, 25 %

void MotorStop()
{
	softPwmWrite(MOTOR_MT_N_PIN, 0);
	softPwmWrite(MOTOR_MT_P_PIN, 0);
}

void MotorControl(int rotate, int speed)
{
	// LEFT_ROTATE
	if (rotate == LEFT_ROTATE)
	{
		digitalWrite(MOTOR_MT_P_PIN, LOW);
		softPwmWrite(MOTOR_MT_N_PIN, speed);
	}

	// RIGHT_ROTATE
	if (rotate == RIGHT_ROTATE)
	{
		digitalWrite(MOTOR_MT_N_PIN, LOW);
		softPwmWrite(MOTOR_MT_P_PIN, speed);
	}
}

void MotorCycle(int rotate)
{
	const int *rotate_cycle;
	int i;

	// select rotate cycle array
	if (rotate == LEFT_ROTATE)
		rotate_cycle = LEFT_ROTATE_CYCLE;
	else
		rotate_cycle = RIGHT_ROTATE_CYCLE;

	for (i = 0; i < 4; i++)
	{
		MotorControl(rotate, rotate_cycle[i]);
		delay(2000);
		MotorStop();
		delay(2000);
	}
}

int main(void)
{
	if (wiringPiSetupGpio() == -1)
		return 1;

	pinMode(MOTOR_MT_N_PIN, OUTPUT);
	pinMode(MOTOR_MT_P_PIN, OUTPUT);
	
	// pwmRange : 128
	softPwmCreate(MOTOR_MT_N_PIN, 0, 128);
	softPwmCreate(MOTOR_MT_P_PIN, 0, 128);

	while(1)
	{
		MotorCycle(LEFT_ROTATE);
		MotorCycle(RIGHT_ROTATE);
	}

	return 0;
}
