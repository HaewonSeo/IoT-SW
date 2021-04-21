int Led[4] = {5, 6, 7, 8}; // LED:1,2,3,4
int Led16 = 9;  // LED16을 9번 digital pin에 연결

void setup() {
  int i;
  for (i = 0; i < 4; i++)
    pinMode(Led[i], OUTPUT);
  pinMode(Led16, OUTPUT);
}

void loop() {
  int i;
  byte output = HIGH;
  
  for (i = 0; i < 4; i++) {
    digitalWrite(Led[i], HIGH);
    delay(500);
    digitalWrite(Led[i], LOW);
    delay(500);
    // Led16을 1초마다 점등
    digitalWrite(Led16, output);
    output = !output;
  }
}
