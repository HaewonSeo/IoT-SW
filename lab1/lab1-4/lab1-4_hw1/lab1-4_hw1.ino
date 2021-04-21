enum { REST = 0, DO = 262, RE = 294, MI = 330, FA = 349, SOL = 392, LA = 440, SHI = 494, DO2 = 523 };

#define BUZZER 5 // Arduino PIN 정의

int Num = 26;
int Num2 = 49;

// 음계 정의
int Frequency[] = { SOL, SOL, LA, LA, SOL, SOL, MI, SOL, SOL, MI, MI, RE, REST,
                    SOL, SOL, LA, LA, SOL, SOL, MI, SOL, MI, RE, MI, DO, REST
                  };
int Frequency2[] = { DO, DO, DO, DO, DO, MI, SOL, SOL, MI, DO,
                     SOL, SOL, MI, SOL, SOL, MI, DO, DO, DO,
                     SOL, SOL, MI, DO, SOL, SOL, SOL,
                     SOL, SOL, MI, DO, SOL, SOL, SOL,
                     SOL, SOL, MI, DO, SOL, SOL, SOL, LA, SOL,
                     DO2, SOL, DO2, SOL, MI, RE, DO
                   };

// 유지시간 정의
int Delay[] = { 500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 2000, 500,
                500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 2000, 500
              };

int Delay2[] = { 500, 250, 250, 500, 500, 500, 250, 250, 500, 500,
                 250, 250, 500, 250, 250, 500, 500, 500, 1000,
                 500, 500, 500, 500, 500, 500, 1000,
                 500, 500, 500, 500, 500, 500, 1000,
                 500, 500, 500, 500, 250, 250, 250, 250, 1000,
                 500, 500, 500, 500, 500, 500, 1000
               };

void setup() {
  pinMode(BUZZER, OUTPUT); // 5번 PIN을 출력으로 설정
}

// 2가지 방법이 있는데,
// tone(pin, frequency, duration)을 사용하여 duration을 주고,
// 추가적으로 delay(30); 라인이 포함된 코드가 더 뚜렷한 음을 낸다.
void loop() {
  int i;

  for (i = 0; i < Num; i++) {
    tone(BUZZER, Frequency[i], Delay[i]); // 5번 PIN을 통해 출력
    delay(Delay[i] + 30);            // 유지 시간
    //tone(BUZZER, REST, 30);     // 음사이 delay 주어 구별하기 위함
  }
  delay(1500);
  for (i = 0; i < Num2; i++) {
    tone(BUZZER, Frequency2[i], Delay2[i]);  // 5번 PIN을 통해 출력
    delay(Delay2[i] + 30);             // 유지 시간
    //tone(BUZZER, REST, 30);       // 음사이 delay 주어 구별하기 위함
  }
  delay(1500);
}
