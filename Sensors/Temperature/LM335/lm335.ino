void setup()
{
  Serial.begin(38400);
}

void loop()
{
  float initial_volts = 5.00;
  int pin = 0;
  float correction_coeficient = 2.2;

  float raw_val = (float) analogRead(pin);
  float volts = initial_volts * raw_val / 1024;
  float celcius = volts / 0.01 - 273.14;

  Serial.println(raw_val);
  Serial.println(volts);
  Serial.println(celcius + correction_coeficient);
  Serial.println("-----");
  
  delay(5000);
}
