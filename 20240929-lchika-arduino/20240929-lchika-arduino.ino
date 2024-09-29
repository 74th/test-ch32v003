void setup() {
  Serial.begin(115200);
  pinMode(PC1, OUTPUT);
}

void loop() {
  delay(500);
  Serial.printf("hello\n");
  digitalWrite(PC1, HIGH);
  delay(500);
  Serial.printf("hello\n");
  digitalWrite(PC1, LOW);
}
