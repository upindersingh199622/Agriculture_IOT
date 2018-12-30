#include <DHT.h> //  DHT.h library from adafruit
#include<SFE_BMP180.h>
#include<Wire.h>
#include <ESP8266WiFi.h> // ESP8266WiFi.h library
#define DHTPIN 4 //GPIO4 = D2
#define DHTTYPE  DHT11

SFE_BMP180 bmp;
const char* ssid     = "Ujjwal Kumar";
const char* password = "ujjwalk450";
const char* host = "api.thingspeak.com";
const char* writeAPIKey = "05JJUPCWNQKDWVVW";

//DHT dht(DHTPIN, DHTTYPE, 15);
DHT dht(DHTPIN, DHTTYPE);
double T,P,temperature;
void setup() {
  Serial.begin(9600);
//   Initialize sensor
 dht.begin();
  delay(1000);
  Wire.begin(14,12);
  
//  Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  char status = bmp.startTemperature();
  if(status != 0)
  { delay(status);
  status = bmp.getTemperature(T);
    if (status != 0)
    {  
       temperature = T;
  

       status = bmp.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        
        status = bmp.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print ("Pressure  ");
          Serial.println(P);}}}}
      
      
            
     
    
 float humidity = dht.readHumidity();
 float temperature = dht.readTemperature();
     Serial.print("temperature ");
Serial.println(temperature);
 
 float water = (1024-analogRead(A0))/102;
 String motor;
 if(water<80)
 {digitalWrite(3,HIGH);
 motor="1";}
 else{
 motor="0";digitalWrite(3,LOW);}
Serial.print("humidity ");
Serial.println(humidity);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("no dht22 data");
    return;
  }

// make TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }

  String url = "/update?key=";
  url+=writeAPIKey;
  url+="&field1=";
  url+=String(temperature);
  url+="&field2=";
  url+=String(humidity);
  url+="&field3=";
  url+=String(water);
  url+="&field4=";
  url+=motor;
  url+="\r\n";
  
  // Request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    delay(100);
}
