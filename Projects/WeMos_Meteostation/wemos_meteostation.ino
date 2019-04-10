#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "Alex"
#define wifi_password "12345678"

#define mqtt_server "192.168.0.7"
#define mqtt_user "guest"      // if exist
#define mqtt_password "guest"  //idem

class RollingAverage{
  private:
    const int size = 50;
    const int empty_value = -100;
    float values[50];
  
  public:
  RollingAverage(){
    for(int i = 0; i<size; i++){
      values[i] = empty_value;
    }
  }

  void add(float value){
    for (int i=size-1;i>0;i--){
      values[i] = values[i-1];
    }

    values[0] = value;
  }

  float getAverage(){
    float sum = 0;
    int count = 0;

    for(int i=0;i<size;i++){

      if (values[i] == empty_value){

        if (i == 0){
          return empty_value;
        }
        
        break;
      }

      sum += values[i];      
      count = i+1;
    }
    
    return sum/count;
  }

  void serialPrint(){
        
    Serial.print("rollingAverage: [");

    for(int i = 0; i < size; i++){
      Serial.print((String(values[i]) + " ").c_str());
    }
    
    Serial.println("]");
  }


};

WiFiClient espClient;
PubSubClient mqttClient(espClient);
RollingAverage rollingAverage;

const float TOLERANCE = 0.3; // In Volts
const int DELAY_TIME = 5; // Delay time in seconds for loop

bool isValueChanged(float value1, float value2){
  return abs(value1 - value2) > TOLERANCE;
}

void setup() {
  Serial.begin(9600);     
  pinMode(LED_BUILTIN, OUTPUT);     //Pin 2 for LED
  
  WiFi.begin(wifi_ssid, wifi_password);
  delay(10);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("=> ESP8266 IP address: ");
  Serial.print(WiFi.localIP());
  
  mqttClient.setServer(mqtt_server, 1883);    // Configure MQTT connexion
  // mqttClient.setCallback(callback);           // callback function to execute when a MQTT message   

  mqttClient.connect("ESP8266Client");
}

int counter = 0;
float previous_averageTemperature = -100;

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
//
//class Measurement{
//
//  private:
//    const float INITIAL_VOLTAGE = 5.0;
//    const float CORRECTION_COEFFICIENT = 0;
//    
//  public:
//    float voltage;
//    float temperature;
//    
//    Measurement(float analog_pin_raw_value){
//      analog_pin_raw_value /= 1.65; // not sure why.
//      voltage = INITIAL_VOLTAGE * analog_pin_raw_value / 1024;
//      temperature = volgage / 0.01 - 273.14 + CORRECTION_COEFFICIENT;      
//    }
//};

const float INITIAL_VOLTAGE = 5.0;
const float CORRECTION_COEFFICIENT = 0;

float getTemperature(float analog_pin_raw_value){
  analog_pin_raw_value /= 1.65; // not sure why.
  float voltage = INITIAL_VOLTAGE * analog_pin_raw_value / 1024;
  float temperature = voltage / 0.01 - 273.14 + CORRECTION_COEFFICIENT;      
  
  return temperature;
}

void loop() {

  blinkLed();
  
  checkConnectionAndReconnectIfNeeded();
  
  mqttClient.loop();

  float current_temperature = getTemperature ((float) analogRead(A0));
  rollingAverage.add(current_temperature);

  Serial.println("counter: " + String(counter++));
  Serial.println("cur temp: " + String(current_temperature));
  rollingAverage.serialPrint();
  float current_averageTemperature = rollingAverage.getAverage();
  
  Serial.println("rol avg: " + String(current_averageTemperature));
  Serial.println("mqtt state: " + String(mqttClient.state())); 
  Serial.println("wifi state: " + String(WiFi.status()));

  if (isValueChanged(current_averageTemperature, previous_averageTemperature)){
    Serial.println("Value changed more than toleralnce level");
    previous_averageTemperature = rollingAverage.getAverage();
    String payload = "{ \"data\": { \"temperature\" : " + String(current_averageTemperature) + "} }";
    mqttClient.publish("wemos", payload.c_str(), true);
  }
  
  Serial.println("-----");
  delay(DELAY_TIME*1000);  
}


//void callback(char* topic, byte* payload, unsigned int length) {
//
//  Serial.println(topic);
//}
