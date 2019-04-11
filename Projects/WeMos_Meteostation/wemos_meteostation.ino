
#include <ESP8266WiFi.h>  // send over wifi data
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>  // mqtt
#include "RollingAverage.h"  // average value over the last 50
#include <ArduinoJson.h>  // (https://arduinojson.org/) to parse response from world time service

#define wifi_ssid "Alex"
#define wifi_password "12345678"

#define mqtt_server "192.168.0.7"
#define mqtt_user "guest"      // if exist
#define mqtt_password "guest"  //idem

const float INITIAL_VOLTAGE = 5.0;
const float CORRECTION_COEFFICIENT = 0;
const float TOLERANCE = 0.3; // In Celsius Degrees
const int DELAY_TIME = 5; // Delay time in seconds for loop

WiFiServer wifiServer(80);
WiFiClient espClient;
PubSubClient mqttClient(espClient);
RollingAverage rollingAverage;
int counter = 0;
float previous_average_temperature = -100;

void setup() {
  Serial.begin(9600);     
  pinMode(LED_BUILTIN, OUTPUT);  // For blinking LED

  IPAddress ip(192, 168, 0, 99);
  IPAddress dns(8, 8, 8, 8);
  IPAddress gateaway(192, 168, 0, 1);
  IPAddress subnet_mask(255, 255, 255, 0);
  WiFi.config(ip, dns, gateaway, subnet_mask);
  
  WiFi.begin(wifi_ssid, wifi_password);
  delay(10);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("=> ESP8266 IP address: ");
  Serial.println(WiFi.localIP());
  
  mqttClient.setServer(mqtt_server, 1883);    // Configure MQTT connexion
  // mqttClient.setCallback(callback);           // callback function to execute when a MQTT message   

  mqttClient.connect("ESP8266Client");
  wifiServer.begin();

//  server.on("/on", getInfo);
}

void loop() {

  blinkLed();
  
  checkConnectionAndReconnectIfNeeded();
  
  mqttClient.loop();

  float current_temperature = getTemperature((float) analogRead(A0));
  rollingAverage.add(current_temperature);
  float current_average_temperature = rollingAverage.getAverage();
  
  reportToSerial(current_temperature, current_average_temperature);

  if (isValueChanged(current_average_temperature, previous_average_temperature)){
    Serial.println("Value changed more than toleralnce level");
    previous_average_temperature = rollingAverage.getAverage();
    reportToMqttClient(current_average_temperature);
  }

  queryWorldTime();  
  reportToWifiClient(current_temperature, current_average_temperature);

  counter++;

  Serial.println("-----");
  delay(DELAY_TIME * 1000);  
}

bool isValueChanged(float value1, float value2){
  return abs(value1 - value2) > TOLERANCE;
}

String getWifiStatus(){
  switch(WiFi.status()){
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
  }
}

void reportToWifiClient(float current_temperature, float current_average_temperature){
 WiFiClient wifiClient = wifiServer.available();
  if (wifiClient.available()){
//     String httpRequest = client.readStringUntil('\r');
//     client.flush();

    wifiClient.println("HTTP/1.1 200 OK");
    wifiClient.println("Content-Type: text/html");
    wifiClient.println(""); //  do not forget this one
    wifiClient.println("<!DOCTYPE HTML>");
    wifiClient.println("<html>");
    
    wifiClient.println("Counter: " + String(counter) + "<br/>");
    wifiClient.println("Current Temperature: " + String(current_temperature) + " C" + "<br/>");   
    wifiClient.println("Average Temperature: " + String(current_average_temperature) + " C" + "<br/>");
    wifiClient.println("MQTT Client State: " + String(mqttClient.state()) + "<br/>"); 
    wifiClient.println("WiFi State: " + getWifiStatus() + "<br/>");
    wifiClient.println("<br/><br/>");

    String rollingAverageString = rollingAverage.getArrayString();
    rollingAverageString.replace(" ", "&emsp;");
    wifiClient.println("Rolling Average Array: " + rollingAverageString + "<br/>");
    
    wifiClient.println("</html>");    
  }
}

void blinkLed(){
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);      
}

void checkConnectionAndReconnectIfNeeded(){
    if (mqttClient.state() != 0){
  
    Serial.print("Trying to reconnect to mqtt server");
    while (mqttClient.state() != 0){
      Serial.print(".");
      mqttClient.connect("ESP8266Client");    
    }
    
    Serial.println();
    Serial.println("Reconnected");
  }  
}

float getTemperature(float analog_pin_raw_value){
  analog_pin_raw_value /= 1.65; // not sure why.
  float voltage = INITIAL_VOLTAGE * analog_pin_raw_value / 1024;
  float temperature = voltage / 0.01 - 273.14 + CORRECTION_COEFFICIENT;      
  
  return temperature;
}

void reportToSerial(float current_temperature, float current_average_temperature){
  Serial.println("Counter: " + String(counter++));
  Serial.println("Current Temperature: " + String(current_temperature));
  Serial.println("Average Temperature: " + String(current_average_temperature));
  Serial.println("MQTT State: " + String(mqttClient.state())); 
  Serial.println("WiFi Status: " + getWifiStatus());
  // Serial.println("RollingAverage Array: " + rollingAverage.getArrayString());
}

void reportToMqttClient(float current_average_temperature){
    String payload = "{ \"data\": { \"temperature\" : " + String(current_average_temperature) + "} }";
    mqttClient.publish("wemos", payload.c_str(), true);
}

String requestTimeFromHttp(){

    WiFiClient client;
    HTTPClient http;
    String payload = "";
    
    if (http.begin(client, "http://worldclockapi.com/api/json/utc/now")) {

      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          payload = http.getString();
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }

  return payload;
}

void parseJsonTime(String jsonTime){
  // capacity calculated by https://arduinojson.org/v6/assistant/
  const int capacity = JSON_OBJECT_SIZE(9) + 173;
  DynamicJsonDocument doc(capacity);

  DeserializationError err = deserializeJson(doc, jsonTime);
  if (err){
    Serial.print("Error during parsing JSON time: ");
    Serial.println(err.c_str());
  }
  
  const char* currentDateTime = doc["currentDateTime"];

  Serial.println(currentDateTime);
}

void queryWorldTime(){

  String jsonTime = requestTimeFromHttp();
  parseJsonTime(jsonTime);
}

//void callback(char* topic, byte* payload, unsigned int length) {
//
//  Serial.println(topic);
//}


// todo:
// + refactor - extract the rollingAverage into a class
// + display real time from: http://worldclockapi.com/ (JSON)
// - display last time sent values to MQTT broker
// - send to MQTT broker at least every hour
// + start wifi server with specific IP, so it will be possible to configure Port Forwarding
// - make loop delay less than 5 secs (so html page is more responsive), but read temperature values once every 5 or 10 secs.
// - add light sensor
// - user voltage regulator for accurate temperature measurements
