wemos request php page and displays data on lcd 1602
php page is running on apache2 and making request to my sql db

connection
  i2c shield is soldered to lcd 1602
  GND - G
  VCC - 5V
  SDA - D2 (pin 19, gpio4, sda)
  SCL - D1 (pin 20, gpio5, scl)

Arduino IDE settings
  File->Preferences->Additional Boards Manager URLs = https://arduino.esp8266.com/stable/package_esp8266com_index.json
  Tools->Board->Boards Manager = ESP8266 and install
  Tools->Board->Generic ESP8266 Module

arduino code uses two libraries: 
  ESP8266WiFi - connecting to WiFi (install through IDE menu Sketch->Include Library)
  ESP8266HTTPClient - making http request to website (https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient)
  LiquidCrystal_I2C - working with lcd over i2c (https://github.com/marcoschwartz/LiquidCrystal_I2C)

apache
  install packages
    apache2 
    libapache2-mod-php
    php-mysql
    php-dom # for parsing html
    php-intl # for transliteration
  verify that php mode was enabled in /etc/apache2/mods-enabled/
  verify port 80 is being listened in /etc/apache2/ports.conf
  restart apache
  check for errors in /var/log/apache2/error.log

php page
  uses simple_html_dom.php library (https://sourceforge.net/projects/simplehtmldom/files/latest/download) 
   uncomment extension=intl in the php.ini # for transliteration  