#include <DHT.h> //  DHT.h library
#include <ESP8266WiFi.h> // ESP8266WiFi.h library
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
int led = D0;
DHT dht(DHTPIN, DHTTYPE);
//above dor DHT11
//Below for LDR
int sensorPin = A0;//Pin For LDR
int sensorValue = 0;
//For ThingsSpeak
const char* ssid     = "iBall-Baton";// replace subscribe with your WiFi SSID(Name)
const char* password = "sagar1166";//replace with Your Wifi Password name
const char* host = "api.thingspeak.com";
const char* writeAPIKey = "LYV91JVB227ZPGVO"; //copy yout ThingSpeak channel API Key.

void setup() {
  pinMode(D0,OUTPUT);
  Serial.begin(115200);//Port fro Communication
  dht.begin();
  delay(1000);
 Serial.println("Connecting to ");
       Serial.println(ssid);
//  Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   Serial.println("");
   Serial.println("WiFi connected");

   //above for Thing Speak
//Below for DHT11
  Serial.println(F("DHTxx test!"));
  pinMode(D0,OUTPUT); 
  dht.begin();
}
  


void loop() {
 sensorValue = analogRead(sensorPin); // Reading Value for Sensor 
 Serial.println(sensorValue);//printing
//Below DHT11
delay(5000);
  float Humidity = dht.readHumidity();
  float Tempreture = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(Humidity) || isnan(Tempreture) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
 
if(sensorValue<400 or (Humidity)>78)
  {
   digitalWrite(D0,HIGH);
   Serial.println("LOW TEMPRATURE LOW LIGHT, TURNING LIGHT ON");
  }
  else{
    digitalWrite(D0,LOW);
    delay(100);
    Serial.print(F("Humidity: "));
    Serial.println(Humidity);
  }
//  // Compute heat index in Fahrenheit (the default)
//  float hif = dht.computeHeatIndex(f, h);
//  // Compute heat index in Celsius (isFahreheit = false)
//  float hic = dht.computeHeatIndex(t, h, false);

// make TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }

  String url = "/update?key=";
  url+="LYV91JVB227ZPGVO";
  url+="&field1=";
  url+=String(Tempreture);
  url+="&field2=";
  url+=String(Humidity);
  url+="&field3=";
  url+=String(sensorValue);
  url+="\r\n";
  
  // Request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  Serial.print("Tempreture:");
  Serial.print(Tempreture);
  Serial.print("\n");
  Serial.print("Light:");
  Serial.println(sensorValue);
  Serial.print("\n");
  Serial.print("Humidity:");
  Serial.println(Humidity);
  Serial.println("Send to ThingSpeak.\n");
client.stop();
  Serial.println("Wait for 15 sec to update next datapack in thingSpeak");
    delay(1000);
 
}
