#include <Servo.h>

#define BLYNK_TEMPLATE_ID           "TMPL2oyfdxz_"

#define BLYNK_TEMPLATE_NAME         "Quickstart Template"

#define BLYNK_AUTH_TOKEN            "0k3C0hIczKXAu-ys_70tfSUFnFzHGmoJ"

 

/* Comment this out to disable prints and save space */

//#define BLYNK_PRINT Serial

#define DHTPIN 23
#define DHTTYPE DHT11

 

 

#include <WiFi.h>

#include <WiFiClient.h>

#include <BlynkSimpleEsp32.h>

#include "DHT.h"

Servo doorServo;
 

// Your WiFi credentials.

// Set password to "" for open networks.

char ssid[] = "I14";

char pass[] = "horwitz3";

 

BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);

int beeActivity = 0;
float humidity = 0;
float tempF = 0;
int noise = 0;

int ledPin = 13;      // LED pin on arduino
int detectorPin = 5;  // obstacle avoidance sensor interface
int greenPin = 12;      // LED pin on arduino
int redPin = 14;  // obstacle avoidance sensor interface
int val;              // variable to store result
bool counter = false;
int count;


float beeHappy = 0;
float humidityK = 1;
float tempK = 1;
float soundK = 0.5;
float beeK = 0.5;
 

// This function is called every time the Virtual Pin 0 state changes

BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  Serial.println("move door");
  int value = param.asInt();

  if(value == 1){
    // open door
    for(int posDegrees = 3; posDegrees <= 80; posDegrees++) {
      doorServo.write(posDegrees);
      delay(20);
    }
  }else{
    // close door
    for(int posDegrees = 80; posDegrees >= 3; posDegrees--) {
        doorServo.write(posDegrees);
        delay(20);
    }
  } 
}

int autoDoor = 0;
BLYNK_WRITE(V1)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  autoDoor = value;
}

 

// This function is called every time the device is connected to the Blynk.Cloud

BLYNK_CONNECTED()

{

  // Change Web Link Button message to "Congratulations!"

  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");

  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");

  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");

}

 

// This function sends Arduino's uptime every second to Virtual Pin 2.

void myTimerEvent()

{

  // You can send any value at any time.

  // Please don't send more that 10 values per second.
  humidity = dht.readHumidity();
  tempF = dht.readTemperature(true);

  beeHappy = humidityK*(100 - abs(humidity - 55)) + tempK*(100 - abs(tempF - 74.5)) + soundK * noise + beeK * beeActivity;
  if(beeHappy >= 200){
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
  }else{
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);    
  }

  Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V4, beeActivity);
  Blynk.virtualWrite(V5, humidity);
  Blynk.virtualWrite(V6, tempF);
  Blynk.virtualWrite(V7, noise);
  Blynk.virtualWrite(V8, beeHappy);
  Serial.println("sent IoT data");

}

 //int openedDoor = 0;

void setup()

{

  // Debug console

  Serial.begin(115200);

  Serial.println("starting setup");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, myTimerEvent);

  pinMode(34, INPUT);
  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);

  dht.begin();

  doorServo.attach(4);
    for(int posDegrees = 80; posDegrees >= 3; posDegrees--) {
      doorServo.write(posDegrees);
      delay(20);
  }
  delay(500);

  pinMode(ledPin, OUTPUT);  // Define LED as output interface
  pinMode(detectorPin, INPUT);  // Define obstacle avoidance sensor as input interface

  Serial.println("setup complete");
}
 

void loop()

{

  Blynk.run();

  timer.run();

  // Collect Sensor Data
  //beeActivity += 1;

  noise = analogRead(34);

  val = digitalRead(detectorPin); // Read value from sensor
  if(val == LOW) // When the sensor detects an obstacle, the LED on the Arduino lights up
  {
    digitalWrite(ledPin, HIGH);
    counter = true;
  }
  else
  {
    digitalWrite(ledPin, LOW);
    if(counter){
      beeActivity ++;
      counter = false;
    }
  }

  Serial.print("bees: "); 
  Serial.print(beeActivity);
  Serial.print(" humidity: ");
  Serial.print(humidity);
  Serial.print(" tempF: ");
  Serial.print(tempF);
  Serial.print(" noise: ");
  Serial.print(noise);
  Serial.print(" BS: ");
  Serial.println(beeHappy);


  // Actuate auto servos
  if(autoDoor && (beeActivity > 20)){
    // open door
    // for(int posDegrees = 3; posDegrees <= 80; posDegrees++) {
    //   doorServo.write(posDegrees);
    //   delay(20);
    // }
    doorServo.write(80);    
    delay(500);
    //Blynk.virtualWrite(V0, 1);
    //openedDoor = 1;
    Serial.println("auto door moved");
  }

  //delay(500);
} 