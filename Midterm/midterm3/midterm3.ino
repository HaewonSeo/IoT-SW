
#define BUZZER 13 // Arduino PIN 정의

const int Button[8] = {2,3,4,5,6,7,8,9};
static int buttonState[8];

enum
{
  REST = 0,
  DO = 262,
  RE = 294,
  MI = 330,
  FA = 349,
  SOL = 392,
  LA = 440,
  SHI = 494,
  DO2 = 523
};

const int Frequency[8] = {DO, RE, MI, FA, SOL, LA, SHI, DO2};

void setup()
{
  int i;

  for (i = 0; i < 8; i++)
    pinMode(Button[i], INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);
}

void loop()
{
  int i;

  for (i = 0; i < 8; i++)
  {
    buttonState[i] = digitalRead(Button[i]);
    while (buttonState[i] == 0)
    {
      tone(BUZZER, Frequency[i], 100); // 13번 PIN을 통해 출력
      delay(100);
      buttonState[i] = digitalRead(Button[i]);
      if (digitalRead(Button[i]) == 1)
      {
        noTone(BUZZER);
        break;
      }
    }
  }
}
