// digital pin 6,7,8,9 을 출력으로 사용하는 경우
void setup()
{
  DDRH |= 0x78; // DDRH |= 0b01111000; pin 6,7,8,9
}

void loop()
{
  int pin, i;

  // LED를 변경
  for (pin = 3; pin <= 6; pin++)
  {
    // LEN ON/OFF
    for (i = 0; i <= 1; i++)
    {
      PORTH ^= (1 << pin);
      delay(500);
    }
  }
}

//// digital pin 5,6,7,8 을 출력으로 사용하는 경우
//void setup()
//{
//  DDRE |= 0x08; // DDRE |= 0b00001000; pin5
//  DDRH |= 0x38; // DDRH |= 0b00111000; pin6,7,8
//}
//
//void loop()
//{
//  int pin, i;
//
//  // pin 5에 연결된 LED ON/OFF
//  for (i = 0; i <= 1; i++)
//  {
//    PORTE ^= (1 << 3);
//    delay(500);
//  }
//  // pin 6,7,8에 연결된 LED ON/OFF
//  for (pin = 3; pin <= 5; pin++)
//  {
//    for (i = 0; i <= 1; i++)
//    {
//      PORTH ^= (1 << pin);
//      delay(500);
//    }
//  }
//}
