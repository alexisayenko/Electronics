#include <HTTPClient.h>

#define _LCD_TYPE 1
// #include <ESP8266WiFi.h>
#include <WiFi.h>
// #include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <LCD_1602_RUS_ALL.h>

LCD_1602_RUS <LiquidCrystal_I2C> lcd(0x27, 16, 2);
const char* ssid = "netis";
const char* password = "password";

HTTPClient http;

void scrollText(String text){
  lcd.setCursor(0,0);
  lcd.clear();
  
  lcd.print(text);

  if (text.length() <= 15){   
    return;
  }

  String line1 = text.substring(0, 15);
  String line2 = text.substring(15);
  if (text.length() > 31){
    line2 = text.substring(15, 30);
  }

  lcd.setCursor(0,1);
  lcd.print("line2");
}

void setup()
{
  lcd.init(); 
  lcd.backlight(); 

  lcd.print("абвгдеёжзийклмн");
  delay(15000);

  // scrollText("Последние новости");


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
  lcd.print("___");
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

void showStatistics(){
  printLog(">");
  String temperature = httpRequest("http://68.183.222.243/temperature.php");
  printLog(">>");
  String covid19 = httpRequest("http://68.183.222.243/covid-19.php");
  printLog(">>>");

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(temperature);
  lcd.setCursor(0,1);
  lcd.print(covid19);  
}



void showNews(){
   
  String korr = httpRequest("http://68.183.222.243/korr.php");
  scrollText(korr);
}

void loop() {
  printLog(">");
  
  while (WiFi.status() != WL_CONNECTED){
    lcd.clear();
    lcd.print("Not connected");
    delay5sec();
    WiFi.begin(ssid, password);
  }

  showStatistics();
  delay5sec();
  showNews();
  delay5sec();
}
