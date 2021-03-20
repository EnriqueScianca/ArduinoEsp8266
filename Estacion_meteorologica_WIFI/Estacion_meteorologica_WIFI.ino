#include <OneWire.h>
#include <DallasTemperature.h>
OneWire OurWire(2);
DallasTemperature sensors(&ourWire);




#include <ESP8266WiFi.h>
#include <SimpleDHT.h>
String apiKey = "VWQX00EV27JVY7VW";
const char* ssid = "Fibertel WiFi156 2.4GHz";
const char* password = "0041036553";
const char* server = "api.thingspeak.com";
int pinDHT11 = D0;
SimpleDHT11 dht11;
WiFiClient client;
void setup() {

  sensors.begin();
  
  Serial.begin(115200);
  delay(10);
  pinMode(A0, INPUT);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}
void loop() {


  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  
  
  float luminosidad = analogRead(A0);
  if (dht11.read(pinDHT11, &temperatura, &humedad, NULL)) {
    Serial.print("Read DHT11 failed.");
    return;
  }
  if (client.connect(server,80)) {
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(luminosidad);
    postStr +="&field2=";
    postStr += String((int)temp);
    postStr +="&field3=";
    postStr += String((int)humedad);
    postStr += "\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.println("% send to Thingspeak");
  }
  client.stop();
  Serial.println("Waiting…");
  delay(20000);
}
