#include <Servo.h>

#define BLYNK_TEMPLATE_ID           "TMPL2oyfdxz_"

#define BLYNK_TEMPLATE_NAME         "Quickstart Template"

#define BLYNK_AUTH_TOKEN            "0k3C0hIczKXAu-ys_70tfSUFnFzHGmoJ"

 

/* Comment this out to disable prints and save space */

//#define BLYNK_PRINT Serial

#define DHTPIN 2
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
 

// This function is called every time the Virtual Pin 0 state changes

BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  if(value == 1){
    // open door
    doorServo.write(90);
    delay(500);
  }else{
    // close door
    doorServo.write(0);
    delay(500);
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

  Blynk.virtualWrite(V2, millis() / 1000);
  Blynk.virtualWrite(V4, bees);
  Blynk.virtualWrite(V5, humidity);
  Blynk.virtualWrite(V6, tempF);
  Blynk.virtualWrite(V7, noise);
  Serial.println("sent IoT data");

}

 

void setup()

{

  // Debug console

  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, myTimerEvent);

  pinMode(34, INPUT);

  dht.begin();

  doorServo.attach(35);
  doorServo.write(90);
  delay(500);

}
 

void loop()

{

  Blynk.run();

  timer.run();

  // Collect Sensor Data
  beeActivity += 1;
  humidity = dht.readHumidity();
  tempF = dht.readTemperature(true);
  noise = analogRead(34);

  Serial.print("bees: "); 
  Serial.print(bees);
  Serial.print(" humidity: ");
  Serial.print(humidity);
  Serial.print(" tempF: ");
  Serial.print(tempF);
  Serial.print(" noise: ");
  Serial.println(noise);


  // Actuate auto servos
  if(autoDoor && (bees > 20)){
    // open door
    doorServo.write(90);
    delay(500);
    Serial.println("auto door moved");
  }

  delay(500);
} 