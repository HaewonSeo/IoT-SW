#include <MsTimer2.h>


// Timer function
void LedControl(){
   static int counter = 0;
   static byte output = HIGH;

   digitalWrite(Led[counter++], output);    // LED(1, 9) ON/OFF 수행

   if (counter > 1){
      counter = 0;
      output = !output;
   }
}

// ISR
void ButtonLedControl(){
  digitalWrite(Led[2], Led16_output);     // LED 16 On
}


void setup()
{
	int i;

	pinMode(Led, OUTPUT);
	//DDRH |= 0x78; // DDRH |= 0b01111000; pin 6,7,8,9

	pinMode(Button, INPUT_PULLUP);

	pinMode(BUZZER, OUTPUT); // 5번 PIN을 출력으로 설정

	attachInterrupt(0, ButtonLedControl, RISING);

	MsTimer2::set(500, LedControl);
	MsTimer2::start();
}

void loop()
{
	digitalWrite(LED, HIGH);
	delay(500);

	// PORTH ^= (1 << pin);
	// delay(500);

	buttonState[i] = digitalRead(Button[i]);

	tone(BUZZER, 440, 500); // 5번 PIN을 통해 440Hz 출력(라 출력), 0.5초 지속 후 종료
	delay(1000);			// 1000ms 유지 시간, 0.5초 간 소리 출력 후 0.5초간 소리 미 출력

}
