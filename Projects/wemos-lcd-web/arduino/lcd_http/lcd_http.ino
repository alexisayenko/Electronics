#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x3F for a 16 chars and 2 line display
const char* ssid = "netis";
const char* password = "password";

HTTPClient http;

void cls(){
  lcd.setCursor(0,0);
}

void setup()
{
  lcd.init(); 
  lcd.backlight(); 

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    lcd.clear();
    lcd.print("Connecting...");
    delay(1000);
  }

  lcd.clear();
  lcd.print("On-line");
}

String httpRequest(String url){
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode == 0){
    http.end();
    return "HTTP Request failed";
  }
  
  String result = http.getString();
  http.end();

  return result;
}

void loop() {
  
  while (WiFi.status() != WL_CONNECTED){
    lcd.clear();
    lcd.print("Not connected");
    delay(5000);
  }

  String temperature = httpRequest("http://68.183.222.243/temperature.php");
  String covid19 = httpRequest("http://68.183.222.243/covid-19.php");

  lcd.setCursor(0,0);
  lcd.print(temperature);
  lcd.setCursor(0,1);
  lcd.print(covid19);

  
//  http.begin("http://68.183.222.243/test.php");
//  int httpCode = http.GET();                                                                  //Send the request
//  
//  if (httpCode > 0) { //Check the returning code     
//    String payload = http.getString();   //Get the request response payload
//    lcd.setCursor(0,0);
//    lcd.print(payload); //Print the response payload
//  }
//  else {
//    lcd.print("code 0");
//  }
   
//  http.end();   //Close connection
  

  delay(5000);

  
}
