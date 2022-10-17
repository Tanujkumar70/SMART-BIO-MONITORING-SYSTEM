#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN 0 

const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
int inputPin = 13;
int sensor_pin = A0;
int output_value ;

const char* ssid     = "realme C21";
const char* password = "radha krishna";
const char* host = "127.0.0.1";
DHT dht(DHTPIN, DHT11);





void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(inputPin, INPUT);
  pinMode(sensor_pin, INPUT);
  Serial.begin(115200);
  delay(100);
  dht.begin();
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
}
void loop() {

  float t = dht.readTemperature();
  float h = dht.readHumidity();;
  output_value= analogRead(sensor_pin);
  output_value = map(output_value,550,0,0,100);
  int s=output_value;
  int i=digitalRead(inputPin);
  
  long duration;
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  int u= duration*0.034/2;
//-----------------------------------------ultrasonic----------------------------------

  
  
  Serial.print("connecting to ");
  Serial.println(host);

  WiFiClient client;
  const int httpPort = 8000;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
 String url = "?temp="+String(t)+"&hum="+String(h)+"&soil="+String(s)+"&ir="+String(i)+"&ultrasonic="+String(u);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  delay(3000);
}
