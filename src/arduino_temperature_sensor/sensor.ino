// ***********************************************************************************
// 
// PROJECT     :  temperature sensor
// 
// FUNCTION    :  sensor
// 
// AUTHOR      :  Wei-Ting Chen
// 
// EMAIL       :  
//
// ***********************************************************************************

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

// define the display type
LiquidCrystal_I2C lcd (0x27, 20,  4);


// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
int index = 0 ;

void setup(void)
{
  // start serial port
  Serial.begin(2400);
  Serial.println("Temperature Sensor");

  // Start up sensor
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  
  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // Start up LCD
  lcd.init();
  lcd.backlight();
  
}

// print the temperature on screen
void printTemperature(int index)
{
  
  //float tempC = sensors.getTempC(deviceAddress);
  float tempC = sensors.getTempCByIndex(index);
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  
  // print temperature
  Serial.print("Temp C: ");
  Serial.println(tempC);
  //Serial.print(" Temp F: ");
  //Serial.println(DallasTemperature::toFahrenheit(tempC));

  //
  lcd.clear();
  lcd.print(" Temp C: ");
  lcd.setCursor(10,0);
  lcd.print(tempC);
  
}

// device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void loop(void)
{ 
  
  // request to all devices on the bus and to issue a global temperature 
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
 
  printTemperature(index);
  
}




