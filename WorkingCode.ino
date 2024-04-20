#define BLYNK_TEMPLATE_ID "TMPL6GN6o5DYj"
#define BLYNK_TEMPLATE_NAME "SmartPot"         
#define BLYNK_AUTH_TOKEN "PJjVGVFWSRBjpFETSEZrF564tRDzCrda"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define RELAY_PIN D4  

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Yehan's Pixel 6 Pro";  
char pass[] = "NoAccess";  

BlynkTimer timer;
#define DHTPIN 4 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

void sendSensor() {
  int value = analogRead(A0);
  value = map(value, 400, 1023, 100, 0);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V0, value);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  Serial.print("Soil Moisture : ");
  Serial.print(value);
  Serial.print(" Temperature : ");
  Serial.print(t);
  Serial.print(" Humidity : ");
  Serial.println(h);
}

BLYNK_WRITE(V3) {  // Virtual Pin for controlling the relay
  int relayValue = param.asInt();
  digitalWrite(RELAY_PIN, relayValue);
}

void setup() {   
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Ensure the relay is initially off

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(100L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
