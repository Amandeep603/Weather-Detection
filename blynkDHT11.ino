#define BLYNK_TEMPLATE_ID "TMPL3aJcb9mQV"
#define BLYNK_TEMPLATE_NAME "project"
#define BLYNK_AUTH_TOKEN "r1w0fMYJQbzQkakw1GMnsFElC5sKA9j2"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Servo.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "test";
char pass[] = "Test12345";

#define DHTPIN 2 //d4
#define DHTTYPE DHT11
#define MQ135_PIN A0  
#define SERVO_PIN 14 //d5 

DHT dht(DHTPIN, DHTTYPE);
Servo servo;

BlynkTimer timer;

void sendSensorData() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int airQuality = analogRead(MQ135_PIN);

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send temperature and humidity to Blynk
  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2, hum);
  Blynk.virtualWrite(V3, airQuality);

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C, Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Air Quality: ");
  Serial.println(airQuality);
}


BLYNK_WRITE(V4) {
  int servoPosition = param.asInt();  
  servo.write(servoPosition);  
  Serial.print("Servo position: ");
  Serial.println(servoPosition);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  dht.begin();
  servo.attach(SERVO_PIN);

  timer.setInterval(1000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
