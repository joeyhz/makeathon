#include <Servo.h>
Servo s;

void setup() {
  Serial.begin(9600);
  
  pinMode(4,INPUT);
  s.attach(35);
  s.write(90);
}

void loop() {
  // Wait a few seconds between measurements.

  Serial.println(analogRead(4));
  delay(1000);
}
