void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 20; i++){
    uint8_t index = 0x80;
    Serial.println(i);
    Serial.println();
    for(int o = 0; o < 8; o++){
      digitalWrite(9, HIGH);
      digitalWrite(8, i & index);
      Serial.println(i & index);
      index = index >> 1;
      delay(20);
      digitalWrite(9, LOW);
      delay(20);
    }
    Serial.println();
    delay(1000);
  }
}
