void setup() {
  Serial.begin(115200);
}
// just trying staff
void loop() {
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  delay(100);
}
