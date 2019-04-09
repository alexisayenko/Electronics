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
const float TOLERANCE = 0.5;

RollingAverage rollingAverage;

bool isValueChanged(float value1, float value2){
  return abs(value1 - value2) > TOLERANCE;
}


void setup() {
  Serial.begin(9600);     
  pinMode(LED_BUILTIN, OUTPUT);     //Pin 2 for LED
  // digitalWrite(D2, HIGH);
  
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
// bool isLedOn = false;

void loop() {

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);    

if (mqttClient.state() != 0){

  Serial.print("Trying to reconnect to mqtt server");
  while (mqttClient.state() != 0){
    Serial.print(".");
    mqttClient.connect("ESP8266Client");    
  }
  
  Serial.println();
  Serial.println("Reconnected");
  }

mqttClient.loop();


  float initial_volts = 5.00;
  int pin = A0;
  float correction_coeficient = 0; //2.2;

  float raw_val = (float) analogRead(pin) / 1.65;
  
  float volts = initial_volts * raw_val / 1024;
  float celcius = volts / 0.01 - 273.14;
  float current_temperature = celcius + correction_coeficient;
  rollingAverage.add(current_temperature);

  Serial.println("counter: " + String(counter++));
  Serial.println("raw value: " + String(raw_val));
  Serial.println("voltage: " + String(volts));
  Serial.println("cur temp: " + String(current_temperature));
  rollingAverage.serialPrint();
  float current_averageTemperature = rollingAverage.getAverage();
  
  Serial.println("rol avg: " + String(current_averageTemperature));
  Serial.println("mqtt state: " + String(mqttClient.state())); 
  Serial.println("wifi state: " + String(WiFi.status()));
  Serial.println("-----");

  

  if (isValueChanged(current_averageTemperature, previous_averageTemperature)){
    Serial.println("Value changed more than toleralnce level");
    previous_averageTemperature = rollingAverage.getAverage();
    String payload = "{ \"data\": { \"temperature\" : " + String(current_averageTemperature) + "} }";
    mqttClient.publish("wemos", payload.c_str(), true);
  }
  
  delay(5000);  
}


//void callback(char* topic, byte* payload, unsigned int length) {
//
//  Serial.println(topic);
//}
