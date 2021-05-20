#define LED 5
#define BUZZER 6 // Arduino PIN 정의
#define LED_DELAY 500
#define NUM 9
enum { REST = 0, DO = 262, RE = 294, MI = 330, FA = 349, SOL = 392, LA = 440, SHI = 494, DO2 = 523 };
int Frequency[] = { DO, RE, MI, REST, FA, SOL, LA, SHI, DO2 }; // 음계 정의
int Delay[] = { 500, 0, 500, 500, 500, 500, 1000, 0, 500 }; // 유지시간 정의
boolean led_state = HIGH;

// millis()를 이용하여 유지시간 계산, led와 buzzer의 유지시간을 각각 계산
unsigned int led_pre_time;
unsigned int buzzer_pre_time;
unsigned int cur_time;
int i;

void setup() {
    // led와 buzzer를 출력으로 설정
    pinMode(LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    // 변수 초기화
    led_pre_time = millis();
    buzzer_pre_time = millis();

    // 초기 상태 정의
    digitalWrite(LED, led_state);
    tone(BUZZER, Frequency[i]);
}

void loop() {

    // LED 출력을 위한 현재 시간 저장
    cur_time = millis();

    // 500ms마다 LED ON/OFF
    if (cur_time - led_pre_time > LED_DELAY)
    {
      led_state = !led_state;
      digitalWrite(LED, led_state);
      led_pre_time = cur_time;
    }
    
    // BUZZER 출력을 위한 현재 시간 저장
    cur_time = millis();
    
    // Delay[i] 마다 buzzer 출력
    if (cur_time - buzzer_pre_time > Delay[i])
    {
      i = (i + 1) % NUM;
      tone(BUZZER, Frequency[i]);
      buzzer_pre_time = cur_time;
    }
}
