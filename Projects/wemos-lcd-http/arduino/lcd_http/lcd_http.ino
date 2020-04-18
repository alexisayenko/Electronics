
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
  lcd.setCursor(0,0);
  lcd.print("On-line");
  delay(1000);
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

void delayLog(int sec){
  for (int i = sec; i >= 0; i--){
    printLog("  " + String(i));
    delay(1000);
  }    
}

void scrollText(String text){
  lcd.clear();
  
  lcd.setCursor(0,0);
  if (text.length()<=16){
    lcd.print(text);
    return;  
  }

  String line1 = text.substring(0,15);
  String line2 = text.substring(16);

  lcd.print(line1);

  lcd.setCursor(0,1);
  
  if (line2.length()<=16){
    lcd.print(line2);
    return;
  }

  line2 = line2.substring(0,15);
  lcd.print(line2);
  
  delay(750);

  scrollText(text.substring(1));
}

String previousNews = "";

void loop() {
  
  while (WiFi.status() != WL_CONNECTED){
    lcd.clear();
    lcd.print("Not connected");
    delayLog(5);
  }

  printLog(">");
  String news = httpRequest("http://68.183.222.243/korr.php");
  printLog(">>");
  String covid19 = httpRequest("http://68.183.222.243/covid-19.php");
  printLog(">>>");
  String temperature = httpRequest("http://68.183.222.243/temperature.php");

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(temperature);
  lcd.setCursor(0,1);
  lcd.print(covid19);

  delayLog(5);

  if (previousNews != news){
    previousNews = news;
    scrollText("     " + news + "   ");
    delayLog(5); 
  }
}
