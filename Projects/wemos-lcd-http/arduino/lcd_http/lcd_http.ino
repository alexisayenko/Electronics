
#include <ESP8266WiFi.h>
#include "ESP8266HTTPClient.h"
#include "LiquidCrystal_I2C.h"
 
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x3F for a 16 chars and 2 line display
const char* ssid = "netis";
const char* password = "password";

HTTPClient http;

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

void printLog(String text){
  lcd.setCursor(13, 1);
  lcd.print("   ");
  lcd.setCursor(13, 1);
  lcd.print(text);
}

void delay5sec(){
  printLog("  5"); 
  delay(1000);
  printLog("  4");
  delay(1000);
  printLog("  3");
  delay(1000);
  printLog("  2");
  delay(1000);
  printLog("  1");
  delay(1000);
  printLog("  0");    
}

void loop() {
  
  while (WiFi.status() != WL_CONNECTED){
    lcd.clear();
    lcd.print("Not connected");
    delay5sec();
    WiFi.begin(ssid, password);
  }

  printLog(">");
  String temperature = httpRequest("http://68.183.222.243/temperature.php");
  printLog(">>");
  String covid19 = httpRequest("http://68.183.222.243/covid-19.php");
  printLog(">>>");

  lcd.setCursor(0,0);
  lcd.print(temperature);
  lcd.setCursor(0,1);
  lcd.print(covid19);

  delay5sec();
}
