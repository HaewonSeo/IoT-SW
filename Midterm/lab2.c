#include <wiringPi.h>

#define LED_OFF		0
#define LED_ON		1

const int Led[16] = {4, 17, 18, 27, 22, 23, 24, 25,
					 6, 12, 13, 16, 19, 20, 26, 21};
const int LedRed[8] = {4, 17, 18, 27, 22, 23, 24, 25};
const int LedGreen[8] = {6, 12, 13, 16, 19, 20, 26, 21};

const int Keypad[8] = {16, 13, 12, 6, 21, 26, 20, 19};

const int FndSelectPin[6] = {4, 17, 18, 27, 22, 23};	//S0~S5
const int FndPin[8] = {6, 12, 13, 16, 19, 20, 26, 21};	//A~H
const int FndFont[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,	//0~9
						0x6D, 0x7D, 0x07, 0x7F, 0x67};

#define MOTOR_MT_N_PIN	17
#define MOTOR_MT_P_PIN	4
#define LEFT_ROTATE		1
#define RIGHT_ROTATE	2

const int LEFT_ROTATE_CYCLE[4] = {32, 64, 96, 128};		// 25, 50, 75, 100 %
const int RIGHT_ROTATE_CYCLE[4] = {128, 96, 64, 32};	// 100, 75, 50, 25 %


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

	for (i = 0; i < 8; i++)
	{
		if (!digitalRead(Keypad[i]))
		{
			keypadnum = i;
			//printf("%d ", i);
			break;
		}
	}
	return keypadnum;
}

void LEDControl(int pinnum)
{
	int i;

	for (i = 0; i < 16; i++)
	{
		if (i == pinnum)
			digitalWrite(Led[i], LED_ON);
		else
			digitalWrite(Led[i], LED_OFF);
	}
}


void FndSelect(int position)
{
	int i;

	for (i = 0; i < 6; i++)
	{
		if (i == position)
			digitalWrite(FndSelectPin[i], LOW);		// select pin on
		else
			digitalWrite(FndSelectPin[i], HIGH);	// select pin off
	}
}


void FndDisplay(int position, int num)
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

	//digitalWrite(FndSelectPin[position], LOW);
	FndSelect(position);
}

void MotorStop()
{
	softPwmWrite(MOTOR_MT_N_PIN, 0);
	softPwmWrite(MOTOR_MT_P_PIN, 0);
}

void MotorControl(int rotate)
{
	if (rotate == LEFT_ROTATE)
	{
		digitalWrite(MOTOR_MT_P_PIN, LOW);
		softPwmWrite(MOTOR_MT_N_PIN, 50);
	}
	else if (rotate == RIGHT_ROTATE)
	{
		digitalWrite(MOTOR_MT_N_PIN, LOW);
		softPwmWrite(MOTOR_MT_P_PIN, 50);
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

void Init()
{
	int i;

	if (wiringPiSetupGpio() == -1)
	{
		printf("wiringPiSetupGpio() error\n");
		exit(-1);
	}
	//int wiringPiISR (int pin, int edgeType,  void (*function)(void))
	//-edgeType:
	// INT_EDGE_FALLING
	// INT_EDGE_RISING
	// INT_EDGE_BOTH
	// INT_EDGE_SETUP
	if (wiringPiISR(Keypad[2], INT_EDGE_FALLING, &LEDStop) < 0)
		return 1;

	for(i=0; i<16; i++)
	{
		pinMode(Led[i], OUTPUT);
		digitalWrite(Led[i], LED_OFF);
	}

	for (i = 0; i < 8; i++)
	{
		pinMode(Keypad[i], INPUT);
	}



	for (i = 0; i < 6; i++)
	{
		pinMode(FndSelectPin[i], OUTPUT);
		digitalWrite(FndSelectPin[i], HIGH);
	}

	for (i = 0; i < 8; i++)
	{
		pinMode(FndPin[i], OUTPUT);
		digitalWrite(FndPin[i], LOW);
	}


	pinMode(MOTOR_MT_N_PIN, OUTPUT);
	pinMode(MOTOR_MT_P_PIN, OUTPUT);

	softPwmCreate(MOTOR_MT_N_PIN, 0, 100);
	softPwmCreate(MOTOR_MT_P_PIN, 0, 100);

}

int main()
{

	if (argc != 3)
	{
		printf("Usage: %s [position] [number]\n", argv[0]);
		exit(-1);
	}

	Init();

	while(1)
	{
		digitalWrite(Led[0], LED_ON);
		delay(500);
		digitalWrite(Led[0], LED_OFF);
		delay(500);
	}


	while (1)
	{
		keypadnum = KeypadRead();
		LEDControl(keypadnum);
	}

	int pos;
	int data[6] = {0, 1, 2, 3, 4, 5};

	while(1)
	{
		for (pos = 0; pos < 6; pos++)
		{
			FndDisplay(pos, data[pos]);
			delay(2);
		}
	}

	while(1)
	{
		MotorControl(LEFT_ROTATE);
		delay(2000);
		MotorStop();
		delay(1000);

		MotorControl(RIGHT_ROTATE);
		delay(2000);
		MotorStop();
		delay(1000);
	}


	return 0;
}


