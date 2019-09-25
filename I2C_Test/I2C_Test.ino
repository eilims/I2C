int period = 20;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //Setup start condition
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  delay(period);
  // Start condition
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  delay(period);
  for(int i = 0; i < 5; i++){
    uint8_t index = 0x80;
    Serial.println(i);
    Serial.println();
    for(int o = 0; o < 8; o++){
      digitalWrite(9, LOW);
      delay(period);
      digitalWrite(8, i & index);
      digitalWrite(9, HIGH);
      Serial.println(i & index);
      index = index >> 1;
      delay(period);
    }
    Serial.println();
    delay(1000);
  }
  // Setup for stop condition
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  delay(period);
  // Stop condition
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  delay(period);
  delay(5000);
}
