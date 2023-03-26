void setup() {
  Serial.begin(9600);
  
  pinMode(4,INPUT);
}

void loop() {
  // Wait a few seconds between measurements.

  Serial.println(analogRead(4));
  delay(1000);
}
